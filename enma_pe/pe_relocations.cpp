#include "stdafx.h"
#include "pe_relocations.h"

void pe_relocations_directory::add_relocation(uint32_t rva, uint32_t relocation_id, uint8_t type) {
    relocations.push_back({ rva , relocation_id, 0 , type });
}

bool pe_relocations_directory::erase_item(uint32_t rva) {

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {
        
        if (relocations[item_idx].relative_virtual_address == rva) {
            relocations.erase(relocations.begin() + item_idx);
            return true;
        }
    }

    return false;
}

bool pe_relocations_directory::erase_first_item_by_id(uint32_t relocation_id) {

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {

        if (relocations[item_idx].relocation_id == relocation_id) {
            relocations.erase(relocations.begin() + item_idx);
            return true;
        }
    }

    return false;
}

bool pe_relocations_directory::erase_all_items_by_id(uint32_t relocation_id) {

    bool ret = false;

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {

        if (relocations[item_idx].relocation_id == relocation_id) {
            relocations.erase(relocations.begin() + item_idx);
            item_idx--;
            ret = true;
        }
    }

    return ret;
}

bool pe_relocations_directory::erase_items_in_segment(uint32_t segment_rva, size_t segment_size) {

    bool ret = false;

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {
        auto& relitem = relocations[item_idx];

        if (relitem.relative_virtual_address >= segment_rva &&
            relitem.relative_virtual_address < segment_rva + segment_size) {

            relocations.erase(relocations.begin() + item_idx);
            item_idx--;
            ret = true;
        }
    }

    return ret;
}


void pe_relocations_directory::get_items_by_relocation_id(std::vector<pe_relocation_entry*>& found_relocs, uint32_t relocation_id) {

    found_relocs.clear();

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {
        if (relocations[item_idx].relocation_id == relocation_id) {
            found_relocs.push_back(&relocations[item_idx]);
        }
    }
}

void pe_relocations_directory::get_items_in_segment(std::vector<pe_relocation_entry>& relocs, uint32_t segment_rva, size_t segment_size) const {

    relocs.clear();

    for (auto& item : relocations) {
        if (item.relative_virtual_address >= segment_rva &&
            item.relative_virtual_address < segment_rva + segment_size) {
            relocs.push_back(item);
        }
    }
}

void pe_relocations_directory::clear() {
    relocations.clear();
}
void pe_relocations_directory::sort() {
    std::sort(relocations.begin(), relocations.end(), [](pe_relocation_entry& lhs, pe_relocation_entry& rhs) {
        return lhs.relative_virtual_address < rhs.relative_virtual_address;
    });
}
size_t pe_relocations_directory::size() const {
    return relocations.size();
}

bool pe_relocations_directory::has_item(uint32_t rva) const {

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {

        if (relocations[item_idx].relative_virtual_address == rva) {
            return true;
        }
    }

    return false;
}

bool pe_relocations_directory::has_item_id(uint32_t relocation_id) const {

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {

        if (relocations[item_idx].relocation_id == relocation_id) {
            return true;
        }
    }

    return false;
}

bool pe_relocations_directory::get_item_id(uint32_t rva, uint32_t& relocation_id) const {

    for (size_t item_idx = 0; item_idx < relocations.size(); item_idx++) {

        if (relocations[item_idx].relative_virtual_address == rva) {
            relocation_id = relocations[item_idx].relocation_id;
            return true;
        }
    }
    return false;
}

const std::vector<pe_relocation_entry>& pe_relocations_directory::get_entries() const {
    return relocations;
}

std::vector<pe_relocation_entry>& pe_relocations_directory::get_entries() {
    return relocations;
}


pe_directory_code get_relocation_directory(const pe_image &image, 
    pe_relocations_directory& relocs, bool ignore_absolute) {

    relocs.clear();
    
    uint32_t virtual_address    = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BASERELOC);
    uint32_t virtual_size        = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_BASERELOC);

    if (virtual_address && virtual_size) {
        pe_image_io reloc_io(image);

        if (!reloc_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }


        reloc_io.set_image_offset(virtual_address);


        while (reloc_io.get_image_offset() < virtual_address + virtual_size) {
            image_base_relocation reloc_base;

            if (reloc_io.read(&reloc_base,sizeof(reloc_base)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }

            uint32_t current_block_size = sizeof(image_base_relocation);

            while ( (reloc_io.get_image_offset() < virtual_address + virtual_size) &&
                (current_block_size < reloc_base.size_of_block)) {
                uint16_t rel;

                if (reloc_io.read(&rel, sizeof(rel)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                uint8_t type = rel >> 12;

                if (type == IMAGE_REL_BASED_ABSOLUTE && ignore_absolute) {
                    current_block_size += INT16_SIZE;
                    continue; 
                }

                relocs.add_relocation((rel & 0x0FFF) | reloc_base.virtual_address, 0, type);

                current_block_size += INT16_SIZE;
            }
        }
        return pe_directory_code::pe_directory_code_success;
    }

    return pe_directory_code::pe_directory_code_not_present;
}

bool build_relocation_directory(pe_image &image, pe_section& section, const pe_relocations_directory& relocs) {

    auto reloc_items = relocs.get_entries();

    if (reloc_items.size()) {
        pe_section_io reloc_section = pe_section_io(section, image, enma_io_mode::enma_io_mode_allow_expand);
        reloc_section.align_up(0x10).seek_to_end();

        uint32_t virtual_address = reloc_section.get_section_offset();
        uint16_t reloc_type;
        
        if (image.is_x32_image()) {
            reloc_type = IMAGE_REL_BASED_HIGHLOW << 12;
        }
        else {
            reloc_type = IMAGE_REL_BASED_DIR64 << 12;
        }

        std::sort(reloc_items.begin(), reloc_items.end(), [](pe_relocation_entry& lhs, pe_relocation_entry& rhs) {
            return lhs.relative_virtual_address < rhs.relative_virtual_address;
        });

        std::vector<std::vector<uint32_t>> reloc_tables;

        uint32_t reloc_last_hi = 0;
        for (auto& reloc : reloc_items) {
            if (!reloc_tables.size() ||
                reloc_last_hi != (reloc.relative_virtual_address & 0xFFFFF000)) {

                reloc_tables.push_back(std::vector<uint32_t>());
                reloc_last_hi = reloc.relative_virtual_address & 0xFFFFF000;
            }

            reloc_tables[reloc_tables.size() - 1].push_back(reloc.relative_virtual_address);
        }

        for (auto& reloc_table : reloc_tables) {
            image_base_relocation reloc_base = { 
                reloc_table[0] & 0xFFFFF000 ,
                uint32_t(sizeof(image_base_relocation) + reloc_table.size()*sizeof(uint16_t) + (reloc_table.size() & 1 ? sizeof(uint16_t) : 0))
            };

            if (reloc_section.write(&reloc_base, sizeof(reloc_base)) != enma_io_success) { return false; }

            for (auto& reloc : reloc_table) {
                uint16_t reloc_ = uint16_t(reloc & 0xFFF) | reloc_type;
                if (reloc_section.write(&reloc_, sizeof(reloc_)) != enma_io_success) { return false; }
            }

            if (reloc_table.size()&1) { //parity align
                uint16_t reloc_ = 0;
                if (reloc_section.write(&reloc_, sizeof(reloc_)) != enma_io_success) { return false; }
            }

        }

        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BASERELOC, virtual_address);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_BASERELOC, reloc_section.get_section_offset() - virtual_address);
    }
    else {
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BASERELOC, 0);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_BASERELOC, 0);
    }

    return true;
}


pe_directory_code get_placement_relocation_directory(const pe_image &image, pe_placement& placement) {


    uint32_t virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BASERELOC);
    uint32_t virtual_size = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_BASERELOC);

    if (virtual_address && virtual_size) {
        pe_image_io reloc_io(image);

        if (!reloc_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        reloc_io.set_image_offset(virtual_address);


        while (reloc_io.get_image_offset() < virtual_address + virtual_size) {
            image_base_relocation reloc_base;

            if (reloc_io.read(&reloc_base, sizeof(image_base_relocation)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }

            placement[reloc_io.get_image_offset() - sizeof(image_base_relocation)]
                = pe_placement_entry(sizeof(image_base_relocation), id_pe_placement::id_pe_relocations_descriptor, "");


            size_t _offset_real = 0;
            size_t available_size = 0;
            size_t down_oversize = 0;
            size_t up_oversize = 0;

            reloc_io.view_image(
                reloc_io.get_image_offset(), reloc_base.size_of_block - sizeof(image_base_relocation),
                _offset_real,
                available_size, down_oversize, up_oversize
            );

            available_size = min(available_size, virtual_size);

            placement[reloc_io.get_image_offset()] = pe_placement_entry(available_size, id_pe_placement::id_pe_relocations_block, "");
        }

        return pe_directory_code::pe_directory_code_success;
    }

    return pe_directory_code::pe_directory_code_not_present;
}
