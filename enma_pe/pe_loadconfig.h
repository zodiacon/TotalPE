#pragma once


class pe_load_config_directory{
    uint32_t size;
    uint32_t timestamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t global_flagsclear;
    uint32_t global_flagsset;
    uint32_t criticalsection_default_timeout;
    uint64_t decommit_freeblock_threshold;
    uint64_t decommit_totalfree_threshold;
    uint32_t lock_prefix_table;
    uint64_t maximum_allocation_size;
    uint64_t virtual_memory_threshold;
    uint32_t process_heap_flags;
    uint64_t process_affinity_mask;
    uint16_t csd_version;
    uint16_t dependent_load_flags;
    uint32_t editlist;
    uint32_t security_cookie;
    uint32_t se_handler_table;
    uint32_t se_handler_count;
    uint32_t guard_cf_check_function_pointer;
    uint32_t guard_cf_dispatch_function_pointer; // VA
    uint32_t guard_cf_function_table;
    uint32_t guard_cf_function_count;
    uint32_t guard_flags;
    image_load_config_code_integrity code_integrity;
    uint32_t guard_address_taken_iat_entry_table;
    uint32_t guard_address_taken_iat_entry_count;
    uint32_t guard_long_jump_target_table;
    uint32_t guard_long_jump_target_count;
    uint32_t dynamic_value_reloc_table;
    uint32_t chpe_meta_data_pointer;
    uint32_t guard_rf_failure_routine;
    uint32_t guard_rf_failure_routine_function_pointer;
    uint32_t dynamic_value_reloc_table_offset;
    uint16_t dynamic_value_reloc_table_section;
    uint32_t guard_rf_verify_stack_pointer_function_pointer;
    uint32_t hot_patch_table_offset;
    uint32_t enclave_configuration_pointer;

    std::vector<uint32_t> se_handlers;
    std::vector<uint32_t> lock_prefixes_rva;
    std::vector<uint32_t> guard_cf_functions_rva;
    std::vector<uint32_t> guard_iat_entries_rva;
    std::vector<uint32_t> guard_long_jump_targets_rva;
public:
    pe_load_config_directory();
    ~pe_load_config_directory() = default;

    pe_load_config_directory(const pe_load_config_directory&) = default;
    pe_load_config_directory& operator=(const pe_load_config_directory&) = default;
    pe_load_config_directory(pe_load_config_directory&&) = default;
    pe_load_config_directory& operator=(pe_load_config_directory&&) = default;
public:
    void set_size(uint32_t  size);
    void set_timestamp(uint32_t  timestamp);
    void set_major_version(uint16_t  major_version);
    void set_minor_version(uint16_t  minor_version);
    void set_global_flagsclear(uint32_t  global_flagsclear);
    void set_global_flagsset(uint32_t  global_flagsset);
    void set_criticalsection_default_timeout(uint32_t  criticalsection_default_timeout);
    void set_decommit_freeblock_threshold(uint64_t  decommit_freeblock_threshold);
    void set_decommit_totalfree_threshold(uint64_t  decommit_totalfree_threshold);
    void set_lock_prefix_table(uint32_t  lock_prefix_table);
    void set_maximum_allocation_size(uint64_t  maximum_allocation_size);
    void set_virtual_memory_threshold(uint64_t  virtual_memory_threshold);
    void set_process_heap_flags(uint32_t  process_heap_flags);
    void set_process_affinity_mask(uint64_t  process_affinity_mask);
    void set_csd_version(uint16_t  csd_version);
    void set_dependent_load_flags(uint16_t dependent_load_flags);
    void set_editlist(uint32_t  editlist);   
    void set_security_cookie(uint32_t  security_cookie);
    void set_se_handler_table(uint32_t  se_handler_table);
    void set_se_handler_count(uint32_t  se_handler_count);
    void set_guard_cf_check_function_pointer(uint32_t  guard_cf_check_function_pointer);
    void set_guard_cf_dispatch_function_pointer(uint32_t  guard_cf_dispatch_function_pointer);
    void set_guard_cf_function_table(uint32_t  guard_cf_function_table);
    void set_guard_cf_function_count(uint32_t  guard_cf_function_count);
    void set_guard_flags(uint32_t  guard_flags);
    void set_code_integrity(image_load_config_code_integrity& code_integrity);
    void set_guard_address_taken_iat_entry_table(uint32_t guard_address_taken_iat_entry_table);
    void set_guard_address_taken_iat_entry_count(uint32_t guard_address_taken_iat_entry_count);
    void set_guard_long_jump_target_table(uint32_t guard_long_jump_target_table);
    void set_guard_long_jump_target_count(uint32_t guard_long_jump_target_count);
    void set_dynamic_value_reloc_table(uint32_t dynamic_value_reloc_table);
    void set_chpe_meta_data_pointer(uint32_t chpe_meta_data_pointer);
    void set_guard_rf_failure_routine(uint32_t guard_rf_failure_routine);
    void set_guard_rf_failure_routine_function_pointer(uint32_t guard_rf_failure_routine_function_pointer);
    void set_dynamic_value_reloc_table_offset(uint32_t dynamic_value_reloc_table_offset);
    void set_dynamic_value_reloc_table_section(uint16_t dynamic_value_reloc_table_section);
    void set_guard_rf_verify_stack_pointer_function_pointer(uint32_t guard_rf_verify_stack_pointer_function_pointer);
    void set_hot_patch_table_offset(uint32_t hot_patch_table_offset);
    void set_enclave_configuration_pointer(uint32_t enclave_configuration_pointer);

public:
    uint32_t get_size() const;
    uint32_t get_timestamp() const;
    uint16_t get_major_version() const;
    uint16_t get_minor_version() const;
    uint32_t get_global_flagsclear() const;
    uint32_t get_global_flagsset() const;
    uint32_t get_criticalsection_default_timeout() const;
    uint64_t get_decommit_freeblock_threshold() const;
    uint64_t get_decommit_totalfree_threshold() const;
    uint32_t get_lock_prefix_table() const; 
    uint64_t get_maximum_allocation_size() const;
    uint64_t get_virtual_memory_threshold() const;
    uint32_t get_process_heap_flags() const;
    uint64_t get_process_affinity_mask() const;
    uint16_t get_csd_version() const;
    uint16_t get_dependent_load_flags() const;
    uint32_t get_editlist() const;  
    uint32_t get_security_cookie() const;
    uint32_t get_se_handler_table() const;
    uint32_t get_se_handler_count() const;
    uint32_t get_guard_cf_check_function_pointer() const;
    uint32_t get_guard_cf_dispatch_function_pointer() const;
    uint32_t get_guard_cf_function_table() const;
    uint32_t get_guard_cf_function_count() const;
    uint32_t get_guard_flags() const;
    image_load_config_code_integrity get_code_integrity() const;
    uint32_t get_guard_address_taken_iat_entry_table() const;
    uint32_t get_guard_address_taken_iat_entry_count() const;
    uint32_t get_guard_long_jump_target_table() const;
    uint32_t get_guard_long_jump_target_count() const;
    uint32_t get_dynamic_value_reloc_table() const;
    uint32_t get_chpe_meta_data_pointer() const;
    uint32_t get_guard_rf_failure_routine() const;
    uint32_t get_guard_rf_failure_routine_function_pointer() const;
    uint32_t get_dynamic_value_reloc_table_offset() const;
    uint16_t get_dynamic_value_reloc_table_section() const;
    uint32_t get_guard_rf_verify_stack_pointer_function_pointer() const;
    uint32_t get_hot_patch_table_offset() const;
    uint32_t get_enclave_configuration_pointer() const;

    const std::vector<uint32_t >& get_se_handlers() const;   //x86 only
    const std::vector<uint32_t >& get_lock_prefixes() const; //x86 only
    const std::vector<uint32_t >& get_guard_cf_functions() const;
    const std::vector<uint32_t >& get_guard_iat_entries() const;
    const std::vector<uint32_t >& get_guard_long_jump_targets() const;

    std::vector<uint32_t >& get_se_handlers();   //x86 only
    std::vector<uint32_t >& get_lock_prefixes(); //x86 only
    std::vector<uint32_t >& get_guard_cf_functions();
    std::vector<uint32_t >& get_guard_iat_entries();
    std::vector<uint32_t >& get_guard_long_jump_targets();
};


enum load_config_table_build_id {
    load_config_table_build_se_handlers             = 1 << 1,
    load_config_table_build_lock_prefixes           = 1 << 2,
    load_config_table_build_guard_cf_functions      = 1 << 3,
    load_config_table_build_guard_iat_entrys        = 1 << 4,
    load_config_table_build_guard_long_jump_targets = 1 << 5,
};

pe_directory_code get_load_config_directory(_In_ const pe_image &image, _Out_ pe_load_config_directory& load_config);

bool build_internal_load_config_directory_data(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_load_config_directory& load_config, _In_ uint32_t build_items_ids/*import_table_build_id*/,
    _Inout_ pe_relocations_directory& relocs);

bool build_load_config_directory_only(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_load_config_directory& load_config, _Inout_ pe_relocations_directory& relocs);
bool build_load_config_directory_full(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_load_config_directory& load_config, _Inout_ pe_relocations_directory& relocs);


pe_directory_code get_placement_load_config_directory(_Inout_ const pe_image &image, _Inout_ pe_placement& placement);
