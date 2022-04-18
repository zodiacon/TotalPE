#include "stdafx.h"
#include "pe_builder.h"


std::vector<uint8_t> get_dos_headers(pe_image& image, uint32_t build_flags, size_t& headers_size) {
#define GET_RICH_HASH(x,i) (((x) << (i)) | ((x) >> (32 - (i))))

    std::vector<uint8_t> image_start;


    {   //add dos header

        image_dos_header dos_header;
        image.get_dos_header(dos_header);

        if (dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
            dos_header.e_magic = IMAGE_DOS_SIGNATURE;
        }

        image_start.resize(sizeof(image_dos_header));


        memcpy(image_start.data(), &dos_header, sizeof(image_dos_header));  //write dos header
    }


    {   //add dos stub

        pe_dos_stub dos_header;

        if (get_image_dos_stub(image.get_headers_data(), dos_header)) {
            auto& dos_stub = dos_header.get_stub();

            image_start.resize(sizeof(image_dos_header) + dos_stub.size());

            memcpy(&image_start.data()[sizeof(image_dos_header)], dos_stub.data(), dos_stub.size()); //write dos stub
        }

    }


    if(build_flags & PE_IMAGE_BUILD_UPD_RICH_DATA) {   //add rich data

        pe_rich_header &rich_header = image.get_rich_header();

        if (rich_header.get_entries().size()) {

            std::vector<uint32_t> rich_stub;

            size_t image_start_size = image_start.size();

            rich_stub.resize(4 + (rich_header.get_entries().size() * 2) + 4); /* DanS sign size + size of item + Rich sign size */

            uint32_t * rich_dw = rich_stub.data();

            rich_dw[0] = 0x536E6144; //start DanS sign

            for (size_t entry_idx = 0; entry_idx < rich_header.get_entries().size(); entry_idx++) {      //init rich items    
                rich_dw[4 + (entry_idx * 2)] = (rich_header.get_entries()[entry_idx].get_compiler_build() & 0xFFFF) |
                    ((rich_header.get_entries()[entry_idx].get_type() & 0xFFFF) << 16);
                rich_dw[4 + (entry_idx * 2) + 1] = rich_header.get_entries()[entry_idx].get_count();
            }

            rich_dw[4 + (rich_header.get_entries().size() * 2)] = 0x68636952;//end Rich sign

            uint32_t rich_hash = uint32_t(image_start.size());

            for (size_t i = 0; i < image_start.size(); i++) {  //calc rich hash (dos header + stub) 
                if (i >= 0x3C && i < 0x40) { continue; }//skip e_lfanew

                rich_hash += GET_RICH_HASH((uint32_t)image_start.data()[i], i);
            }

            for (size_t i = 0; i < rich_header.get_entries().size(); i++) { //calc rich hash (Rich struct)
                rich_hash += GET_RICH_HASH(rich_dw[4 + (i * 2)], rich_dw[4 + (i * 2) + 1]);
            }

            for (size_t i = 0; i < 4 + (rich_header.get_entries().size() * 2); i++) { //crypt rich data by hash
                rich_dw[i] ^= rich_hash;
            }

            rich_dw[4 + (rich_header.get_entries().size() * 2) + 1] = rich_hash; //set Rich hash


            image_start.resize(image_start.size() + rich_stub.size() * sizeof(uint32_t));
            memcpy(&image_start.data()[image_start_size], rich_stub.data(), rich_stub.size() * sizeof(uint32_t));   //write rich data
        }
    }

    image_start.resize(ALIGN_UP(image_start.size(), 0x10)); //align start header size
    pimage_dos_header(image_start.data())->e_lfanew = uint32_t(image_start.size());

    headers_size = image_start.size();
    return image_start;
}


template<typename pe_image_format>
void _get_nt_header(pe_image& image, uint32_t header_size, std::vector<uint8_t>& header) {

    typename pe_image_format::image_nt_headers nt_header;

    memset(&nt_header, 0, sizeof(typename pe_image_format::image_nt_headers));

    nt_header.signature = IMAGE_NT_SIGNATURE;

    nt_header.file_header.machine = image.get_machine();
    nt_header.file_header.number_of_sections = (uint16_t)image.get_sections_number();
    nt_header.file_header.time_date_stamp = image.get_timestamp();
    nt_header.file_header.pointer_to_symbol_table = image.get_pointer_to_symbol_table();
    nt_header.file_header.number_of_symbols = image.get_number_of_symbols();
    nt_header.file_header.size_of_optional_header = sizeof(nt_header.optional_header);
    nt_header.file_header.characteristics = image.get_characteristics();

    nt_header.optional_header.magic = pe_image_format::image_magic;

    nt_header.optional_header.major_linker_version = image.get_major_linker();
    nt_header.optional_header.minor_linker_version = image.get_minor_linker();

    nt_header.optional_header.size_of_code = image.get_size_of_code();
    nt_header.optional_header.size_of_initialized_data = image.get_size_of_init_data();
    nt_header.optional_header.size_of_uninitialized_data = image.get_size_of_uninit_data();

    nt_header.optional_header.address_of_entry_point = image.get_entry_point();

    nt_header.optional_header.base_of_code = image.get_base_of_code();

    nt_header.optional_header.image_base = (typename pe_image_format::ptr_size)image.get_image_base();

    nt_header.optional_header.section_alignment = image.get_section_align();
    nt_header.optional_header.file_alignment = image.get_file_align();

    nt_header.optional_header.major_operating_system_version = image.get_os_ver_major();
    nt_header.optional_header.minor_operating_system_version = image.get_os_ver_minor();
    nt_header.optional_header.major_image_version = image.get_image_ver_major();
    nt_header.optional_header.minor_image_version = image.get_image_ver_minor();
    nt_header.optional_header.major_subsystem_version = image.get_subsystem_ver_major();
    nt_header.optional_header.minor_subsystem_version = image.get_subsystem_ver_minor();

    nt_header.optional_header.win32_version_value = image.get_win32_version_value();

    nt_header.optional_header.size_of_image = ALIGN_UP(
        (image.get_sections()[image.get_sections_number() - 1]->get_virtual_address() +
            image.get_sections()[image.get_sections_number() - 1]->get_virtual_size()),
        image.get_section_align());

    nt_header.optional_header.size_of_headers = ALIGN_UP(header_size, image.get_file_align());

    nt_header.optional_header.checksum = 0;

    nt_header.optional_header.subsystem = image.get_sub_system();
    nt_header.optional_header.dll_characteristics = image.get_characteristics_dll();

    nt_header.optional_header.size_of_stack_reserve = (typename pe_image_format::ptr_size)image.get_stack_reserve_size();
    nt_header.optional_header.size_of_stack_commit = (typename pe_image_format::ptr_size)image.get_stack_commit_size();
    nt_header.optional_header.size_of_heap_reserve = (typename pe_image_format::ptr_size)image.get_heap_reserve_size();
    nt_header.optional_header.size_of_heap_commit = (typename pe_image_format::ptr_size)image.get_heap_commit_size();

    nt_header.optional_header.loader_flags = image.get_loader_flags();
    nt_header.optional_header.number_of_rva_and_sizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;

    for (unsigned int dir_idx = 0; dir_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; dir_idx++) {
        nt_header.optional_header.data_directory[dir_idx].virtual_address = image.get_directory_virtual_address(dir_idx);
        nt_header.optional_header.data_directory[dir_idx].size = image.get_directory_virtual_size(dir_idx);
    }

    header.resize(sizeof(typename pe_image_format::image_nt_headers));
    memcpy(header.data(), &nt_header, sizeof(nt_header));
}

void get_nt_header(pe_image& image, uint32_t header_size, std::vector<uint8_t>& header) {

    if (image.is_x32_image()) {
        _get_nt_header<pe_image_32>(image, header_size, header);
    }
    else {
        _get_nt_header<pe_image_64>(image, header_size, header);
    }
}

size_t calculate_headers_size(pe_image& image, size_t dos_headers_size, uint32_t new_sections) {

    uint32_t total_sections = (uint32_t)image.get_sections_number() + new_sections;

    return ALIGN_UP(
        dos_headers_size +  //start headers size
        (image.is_x32_image() ? sizeof(image_nt_headers32) : sizeof(image_nt_headers64)) + //nt header size
        total_sections * sizeof(image_section_header) //sections desc size

        , image.get_file_align() //alignment
    );
}

uint32_t get_new_sections_count(pe_image_full& image_full, uint32_t build_flags) {

    uint32_t new_sections = 0;

    if (!image_full.get_load_config().get_size() && (build_flags & PE_IMAGE_BUILD_DIR_LOAD_CONFIG)) {
        
        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RDATA) {
            new_sections++;
        }
    }

    if ((!image_full.get_image().is_x32_image() && image_full.get_exceptions().size()) && (build_flags & PE_IMAGE_BUILD_DIR_EXCEPTIONS)) {
        
        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_PDATA) {
            new_sections++;
        }
    }

    if ((image_full.get_imports().size() && (build_flags & PE_IMAGE_BUILD_DIR_IMPORT)) ||
        (image_full.get_exports().get_number_of_functions() && (build_flags & PE_IMAGE_BUILD_DIR_EXPORT)) ||
        ((image_full.get_tls().get_address_of_index() || image_full.get_tls().get_callbacks().size()) && (build_flags & PE_IMAGE_BUILD_DIR_TLS))
        ) {

        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_EDATA) {
            new_sections++;
        }
    }

    if (image_full.get_relocations().size() && (build_flags & PE_IMAGE_BUILD_DIR_RELOCATIONS)) {     //build relocations

        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RSRC) {
            new_sections++;
        }
    }

    if (image_full.get_resources().get_entry_list().size() && (build_flags & PE_IMAGE_BUILD_DIR_RESOURCES)) {   //build resources

        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RSRC) {
            new_sections++;
        }
    }

    return new_sections;
}

void align_sections(pe_image& image, uint32_t start_header_size) {

    uint32_t current_section_raw = start_header_size;

    for (auto & section_ : image.get_sections()) {
        if (section_->get_size_of_raw_data() > section_->get_virtual_size()) {
            section_->set_virtual_size(section_->get_size_of_raw_data());
        }
        section_->set_pointer_to_raw(current_section_raw);

        if (section_->get_size_of_raw_data()) {
            section_->set_size_of_raw_data(ALIGN_UP(section_->get_size_of_raw_data(), image.get_file_align()));
        }

        current_section_raw += section_->get_size_of_raw_data();
    }
}

void build_directories(pe_image_full& image_full, uint32_t build_flags) {

    if ((image_full.get_imports().size() && (build_flags & PE_IMAGE_BUILD_DIR_IMPORT)) ||
        (image_full.get_exports().size() && (build_flags & PE_IMAGE_BUILD_DIR_EXPORT)) ||
        ((image_full.get_tls().get_address_of_index() || image_full.get_tls().get_callbacks().size()) && (build_flags & PE_IMAGE_BUILD_DIR_TLS)) ||
        ((!image_full.get_image().is_x32_image() && image_full.get_exceptions().get_exception_entries().size()) && (build_flags & PE_IMAGE_BUILD_DIR_EXCEPTIONS)) ||
        ((image_full.get_load_config().get_size()) && (build_flags & PE_IMAGE_BUILD_DIR_LOAD_CONFIG))
        ) {


        if ((image_full.get_imports().size() && (build_flags & PE_IMAGE_BUILD_DIR_IMPORT)) ||
            (image_full.get_load_config().get_size() && (build_flags & PE_IMAGE_BUILD_DIR_LOAD_CONFIG)) ) {

            pe_section* rdata_section = 0;

            if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RDATA) {
                rdata_section = &image_full.get_image().add_section();
                rdata_section->set_section_name(std::string(".rdata"));
                rdata_section->set_readable(true).set_writeable(false).set_executable(false);
            }
            else {
                rdata_section = image_full.get_image().get_last_section();
                rdata_section->set_readable(true);
            }


            if ((image_full.get_imports().size() && (build_flags & PE_IMAGE_BUILD_DIR_IMPORT))) { //build import
                build_import_directory_full(image_full.get_image(), *rdata_section, image_full.get_imports());
            }

            if ((image_full.get_load_config().get_size() && (build_flags & PE_IMAGE_BUILD_DIR_LOAD_CONFIG))) { //build load config
                build_load_config_directory_full(image_full.get_image(), *rdata_section, image_full.get_load_config(), image_full.get_relocations()); 
            }
        }

        if(image_full.get_exports().size() && (build_flags & PE_IMAGE_BUILD_DIR_EXPORT)) {

            pe_section* edata_section = 0;

            if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_EDATA) {
                edata_section = &image_full.get_image().add_section();
                edata_section->set_section_name(std::string(".edata"));
                edata_section->set_readable(true).set_writeable(false).set_executable(false);
            }
            else {
                edata_section = image_full.get_image().get_last_section();
                edata_section->set_readable(true);
            }


            if ((build_flags & PE_IMAGE_BUILD_DIR_EXPORT) && image_full.get_exports().size()) { //build export
                build_export_directory(image_full.get_image(), *edata_section, image_full.get_exports());
            }
        }

        if ((image_full.get_tls().get_address_of_index() || image_full.get_tls().get_callbacks().size()) && (build_flags & PE_IMAGE_BUILD_DIR_TLS)) {

            pe_section* tls_section = 0;

            if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_TLS) {
                tls_section = &image_full.get_image().add_section();
                tls_section->set_section_name(std::string(".edata"));
                tls_section->set_readable(true).set_writeable(false).set_executable(false);
            }
            else {
                tls_section = image_full.get_image().get_last_section();
                tls_section->set_readable(true);
            }

            if ((build_flags & PE_IMAGE_BUILD_DIR_TLS) &&   //build tls
                image_full.get_tls().get_address_of_index() || image_full.get_tls().get_callbacks().size()) {

                build_tls_directory_full(image_full.get_image(), *tls_section, image_full.get_tls(), image_full.get_relocations());
                tls_section->set_writeable(true);
            }
        }

        if ((!image_full.get_image().is_x32_image() && image_full.get_exceptions().size()) && (build_flags & PE_IMAGE_BUILD_DIR_EXCEPTIONS)) {
            pe_section* pdata_section = 0;

            if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_PDATA) {
                pdata_section = &image_full.get_image().add_section();
                pdata_section->set_section_name(std::string(".pdata"));
                pdata_section->set_readable(true).set_writeable(false).set_executable(false);
            }
            else {
                pdata_section = image_full.get_image().get_last_section();
                pdata_section->set_readable(true);
            }

            if ((!image_full.get_image().is_x32_image() && image_full.get_exceptions().size())) { //build exceptions
                build_extended_exceptions_info(image_full);
                build_exceptions_directory(image_full.get_image(), *pdata_section, image_full.get_exceptions(), image_full.get_relocations(), true);
            }
        }
    }


    if (image_full.get_relocations().size() && (build_flags & PE_IMAGE_BUILD_DIR_RELOCATIONS)) {     //build relocations

        pe_section* reloc_section = 0;
        
        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RELOC) {
            reloc_section = &image_full.get_image().add_section();
            reloc_section->set_section_name(std::string(".reloc"));
            reloc_section->set_readable(true).set_writeable(false).set_executable(false);
        }
        else {
            reloc_section = image_full.get_image().get_last_section();
            reloc_section->set_readable(true);
        }

        build_relocation_directory(image_full.get_image(), *reloc_section, image_full.get_relocations());

    }

    if (image_full.get_resources().get_entry_list().size() && (build_flags & PE_IMAGE_BUILD_DIR_RESOURCES)) {   //build resources

        pe_section* rsrc_section = 0;
        
        if (build_flags & PE_IMAGE_BUILD_SEP_SECTION_RSRC) {
            rsrc_section = &image_full.get_image().add_section();
            rsrc_section->set_section_name(std::string(".rsrc"));
            rsrc_section->set_readable(true).set_writeable(false).set_executable(false);
        }
        else {
            rsrc_section = image_full.get_image().get_last_section();
            rsrc_section->set_readable(true);
        }


        build_resources_directory(image_full.get_image(), *rsrc_section, image_full.get_resources());
    }
}

bool build_pe_image(pe_image& image, uint32_t build_flags, std::vector<uint8_t>& out_image) {

    out_image.clear();

    size_t dos_headers_size;
    size_t nt_headers_size;
    size_t headers_size;

    out_image = get_dos_headers(image, build_flags, dos_headers_size);
    nt_headers_size = dos_headers_size + (image.is_x32_image() ? sizeof(image_nt_headers32) : sizeof(image_nt_headers64));
    headers_size = calculate_headers_size(image, dos_headers_size, 0);

    out_image.resize(headers_size);

    align_sections(image, (uint32_t)headers_size);

    {
        std::vector<uint8_t> nt_header;
        get_nt_header(image, (uint32_t)headers_size, nt_header);
        memcpy(&out_image[dos_headers_size], nt_header.data(), nt_header.size());
    }

    if (image.get_sections_number()) {

        uint32_t section_raw_top = 0;
        uint32_t section_raw_top_size = 0;


        {
            uint8_t * p_sections_header = &out_image[nt_headers_size];

            for (unsigned int section_idx = 0; section_idx < image.get_sections_number(); section_idx++) {

                if (section_raw_top < image.get_section_by_idx(section_idx)->get_pointer_to_raw()) {
                    section_raw_top = image.get_section_by_idx(section_idx)->get_pointer_to_raw();
                    section_raw_top_size = image.get_section_by_idx(section_idx)->get_size_of_raw_data();
                }

                image_section_header section_hdr = { 0 };

                memcpy(section_hdr.name, image.get_section_by_idx(section_idx)->get_section_name().c_str(),
                    min(image.get_section_by_idx(section_idx)->get_section_name().length(), 8));

                section_hdr.virtual_size = image.get_section_by_idx(section_idx)->get_virtual_size();
                section_hdr.virtual_address = image.get_section_by_idx(section_idx)->get_virtual_address();
                section_hdr.size_of_raw_data = image.get_section_by_idx(section_idx)->get_size_of_raw_data();
                section_hdr.pointer_to_raw_data = image.get_section_by_idx(section_idx)->get_pointer_to_raw();
                section_hdr.characteristics = image.get_section_by_idx(section_idx)->get_characteristics();

                memcpy(p_sections_header, &section_hdr, sizeof(image_section_header));

                p_sections_header += sizeof(image_section_header);
            }
        }


        if (image.get_sections().size()) {

            out_image.resize(image.get_section_top_raw()->get_pointer_to_raw() +
                (size_t)ALIGN_UP(
                    image.get_section_top_raw()->get_size_of_raw_data()
                    , image.get_file_align()));

        }
        else {
            out_image.resize(headers_size);
        }

        for (auto& section_ : image.get_sections()) { //write sections data
            memcpy(&out_image[section_->get_pointer_to_raw()], section_->get_section_data().data(), section_->get_size_of_raw_data());
        }
    }

    if (build_flags & PE_IMAGE_BUILD_UPD_CHECKSUM) {
        *(uint32_t*)&out_image[dos_headers_size + offsetof(image_nt_headers32, optional_header.checksum)] = calculate_checksum(out_image);
    }

    if (image.get_overlay_data().size() && build_flags & PE_IMAGE_BUILD_OVERLAY) {
        size_t overlay_offset = out_image.size();
        out_image.resize(overlay_offset + image.get_overlay_data().size());

        memcpy(&out_image[overlay_offset], image.get_overlay_data().data(), image.get_overlay_data().size());
    }

    return true;
}

bool build_pe_image_full(pe_image_full& image_full, uint32_t build_flags, std::vector<uint8_t>& out_image) {

    size_t dos_headers_size;
    size_t nt_headers_size;
    size_t headers_size;

    get_dos_headers(image_full.get_image(), build_flags, dos_headers_size);
    nt_headers_size = dos_headers_size + (image_full.get_image().is_x32_image() ? sizeof(image_nt_headers32) : sizeof(image_nt_headers64));
    headers_size = calculate_headers_size(image_full.get_image(), dos_headers_size, get_new_sections_count(image_full, build_flags));

    align_sections(image_full.get_image(), (uint32_t)headers_size);

    build_directories(image_full, build_flags);

    return build_pe_image(image_full.get_image(), build_flags, out_image);
}
