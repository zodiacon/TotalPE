#pragma once


template<
    typename ptr_size_type,
    typename image_nt_headers_type,
    typename image_tls_directory_type,
    typename image_load_config_directory_type,
    typename image_dynamic_relocation_type,
    typename image_dynamic_relocation_v2_type,

    uint16_t _image_magic,
    ptr_size_type _ordinal_flag
>
struct pe_image_types{
    typedef ptr_size_type ptr_size; 
    typedef image_nt_headers_type image_nt_headers;
    typedef image_tls_directory_type image_tls_directory;

    typedef image_load_config_directory_type image_load_config_directory;
    typedef image_dynamic_relocation_type    image_dynamic_relocation;
    typedef image_dynamic_relocation_v2_type image_dynamic_relocation_v2;


    static const uint16_t image_magic  = _image_magic;
    static const ptr_size ordinal_flag = _ordinal_flag;
};


typedef pe_image_types <
    uint32_t,
    image_nt_headers32,
    image_tls_directory32,
    image_load_config_directory32,
    image_dynamic_relocation32,
    image_dynamic_relocation32_v2,

    IMAGE_NT_OPTIONAL_HDR32_MAGIC,
    IMAGE_ORDINAL_FLAG32> pe_image_32;

typedef pe_image_types <
    uint64_t,
    image_nt_headers64,
    image_tls_directory64,
    image_load_config_directory64,
    image_dynamic_relocation64,
    image_dynamic_relocation64_v2,

    IMAGE_NT_OPTIONAL_HDR64_MAGIC,
    IMAGE_ORDINAL_FLAG64> pe_image_64;


enum class id_pe_placement {
    id_pe_none,

    id_pe_export_descriptor,
    id_pe_export_functions_table,
    id_pe_export_names_table,
    id_pe_export_ordinals_table,
    id_pe_export_name,
    id_pe_export_function_name,
    id_pe_export_function_forwarded_name,

    id_pe_import_descriptor,
    id_pe_import_original_first_think,
    id_pe_import_first_think,
    id_pe_import_library_name,
    id_pe_import_function_name,

    id_pe_bound_import_descriptor,
    id_pe_bound_import_ref_desc,
    id_pe_bound_import_library_name,

    id_pe_delay_import_descriptor,
    id_pe_delay_import_name_table,
    id_pe_delay_import_bound_table,
    id_pe_delay_import_unload_table,
    id_pe_delay_import_library_name,
    id_pe_delay_import_function_name,

    id_pe_resources,

    id_pe_exception_descriptors,
    id_pe_exception_unwindinfo,

    id_pe_security_descriptor,
    id_pe_security_certificate,

    id_pe_relocations_descriptor,
    id_pe_relocations_block,

    id_pe_debug_descriptor,
    id_pe_debug_item_data,

    id_pe_tls_descriptor,
    id_pe_tls_raw_data,
    id_pe_tls_index,
    id_pe_tls_callbacks,

    id_pe_loadconfig_descriptor,
    id_pe_loadconfig_se_table,
    id_pe_loadconfig_lock_table,
    id_pe_loadconfig_cf_table,
    id_pe_loadconfig_iat_table,
    id_pe_loadconfig_long_jump_table,
};

struct pe_placement_entry {
    size_t size;
    id_pe_placement id;
    std::string name;

    pe_placement_entry()
        :size(0), id(id_pe_placement::id_pe_none), name() {};
    pe_placement_entry(size_t size, id_pe_placement id, std::string name)
        :size(size), id(id), name(name) {};
};

typedef std::map<uint32_t, pe_placement_entry> pe_placement;

enum pe_directory_code {
    pe_directory_code_success       = 0 << 1,
    pe_directory_code_not_present   = 1 << 1,
    pe_directory_code_currupted     = 1 << 2
};

enum pe_image_status {
    pe_image_status_ok,
    pe_image_status_bad_format,
    pe_image_status_unknown,
};
