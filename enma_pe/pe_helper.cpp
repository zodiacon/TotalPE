#include "stdafx.h"
#include "pe_helper.h"


double get_data_entropy(_In_ const std::vector<uint8_t> &data) {

    uint32_t bytes_count[256];
    memset(bytes_count, 0, sizeof(bytes_count));

    for (size_t offset = 0; offset < data.size(); offset++) {
        bytes_count[data[offset]]++;
    }

    double total_entropy = 0.;

    for (size_t i = 0; i < 256; i++) {
        double temp = (double)bytes_count[i] / (double)data.size();
        if (temp > 0.) {
            total_entropy += fabs(temp * (log(temp) * 1.44269504088896340736));
        }
    }

    return total_entropy;
}

double   get_section_entropy(_In_ const pe_section& section) {
    return get_data_entropy(section.get_section_data());
}

uint32_t calculate_checksum(_In_ const std::vector<uint8_t> &pe_image) {
    image_dos_header* p_dos_header = (image_dos_header*)pe_image.data();
    if (p_dos_header->e_magic != IMAGE_DOS_SIGNATURE) { return 0; }

    uint32_t checksum_field_offset = p_dos_header->e_lfanew +
        (uint32_t)offsetof(image_nt_headers32, optional_header.checksum);

    uint64_t checksum = 0;

    for (size_t i = 0; i < pe_image.size(); i += 4) {
        if (i == checksum_field_offset) { continue; }

        checksum = (checksum & 0xffffffff) + *(uint32_t*)&pe_image.data()[i] + (checksum >> 32);

        if (checksum > 0xffffffff) {
            checksum = (checksum & 0xffffffff) + (checksum >> 32);
        }
    }

    checksum = (checksum & 0xffff) + (checksum >> 16);
    checksum = (checksum)+(checksum >> 16);
    checksum = checksum & 0xffff;
    checksum += pe_image.size();

    return uint32_t(checksum & 0xffffffff);
}


void pe_erase_placement(pe_image &image, pe_placement& placement, pe_relocations_directory* relocs, bool delete_empty_sections) {

    struct placement_item{
        uint32_t rva;
        size_t size;
    };

    std::vector<placement_item> placement_items;

    for (auto& place_item : placement) {
        placement_items.push_back({ place_item.first, place_item.second.size });
    }


    for (size_t parent_zone_idx = 0; parent_zone_idx + 1 < placement_items.size(); parent_zone_idx++) { //link zones
        
        if (placement_items[parent_zone_idx].rva <= placement_items[parent_zone_idx + 1].rva &&
            (placement_items[parent_zone_idx].rva + placement_items[parent_zone_idx].size) >= placement_items[parent_zone_idx + 1].rva
            ) {

            if ((placement_items[parent_zone_idx + 1].rva +
                placement_items[parent_zone_idx + 1].size) > (placement_items[parent_zone_idx].rva + placement_items[parent_zone_idx].size)) {

                placement_items[parent_zone_idx].size =
                    ((placement_items[parent_zone_idx + 1].rva + placement_items[parent_zone_idx + 1].size) - placement_items[parent_zone_idx].rva);
            }

            placement_items.erase(placement_items.begin() + parent_zone_idx + 1);
            parent_zone_idx--;
        }
    }

    pe_image_io image_io(image);

    for (auto& item : placement_items) {
        if (relocs) {
            relocs->erase_items_in_segment(item.rva, item.size);
        }

        image_io.set_image_offset(item.rva).memory_set(item.size, 0);
    }


    if (delete_empty_sections && image.get_sections().size()) {
        for (int32_t section_idx = int32_t(image.get_sections().size() - 1); section_idx >= 0; section_idx--) {
            auto _section = image.get_sections()[section_idx];

            for (size_t zone_idx = 0; zone_idx < placement_items.size(); zone_idx++) {
                
                if ((uint64_t)_section->get_virtual_address() >= placement_items[zone_idx].rva && //if it cover full section
                    ((uint64_t)_section->get_virtual_address() + (uint64_t)_section->get_virtual_size()) <= ((uint64_t)placement_items[zone_idx].rva + (uint64_t)placement_items[zone_idx].size)) {

                    if (((uint64_t)_section->get_virtual_address() + (uint64_t)_section->get_virtual_size()) <= ((uint64_t)placement_items[zone_idx].rva + (uint64_t)placement_items[zone_idx].size)) {
                        placement_items.erase(placement_items.begin() + zone_idx);
                    }
                    else {
                        placement_items[zone_idx].size = (placement_items[zone_idx].rva - _section->get_virtual_address());
                    }

                    image.get_sections().erase(image.get_sections().begin() + section_idx);
                    goto go_next_;
                }

                if (_section->get_virtual_address() < placement_items[zone_idx].rva && //if it cover up part of the section
                    ((uint64_t)_section->get_virtual_address() + (uint64_t)_section->get_virtual_size()) <= ((uint64_t)placement_items[zone_idx].rva + (uint64_t)placement_items[zone_idx].size)) {
                    size_t minus_size = ((uint64_t)_section->get_virtual_address() + (uint64_t)_section->get_virtual_size()) - (uint64_t)placement_items[zone_idx].rva;
                    
                    if (minus_size < placement_items[zone_idx].size) {
                        placement_items[zone_idx].size -= minus_size;
                    }
                    else {
                        placement_items.erase(placement_items.begin() + zone_idx);
                    }

                    _section->set_size_of_raw_data( uint32_t(_section->get_size_of_raw_data() - minus_size) );
                }
            }

            goto go_return;

        go_next_:;
        }
    }
go_return:;


    placement.clear();

    for (auto& place_item : placement_items) {
        placement[place_item.rva] = pe_placement_entry(place_item.size, id_pe_placement::id_pe_none, "");
    }
}