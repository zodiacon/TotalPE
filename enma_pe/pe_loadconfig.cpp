#include "stdafx.h"
#include "pe_loadconfig.h"


pe_load_config_directory::pe_load_config_directory() 
    :size(0),
        timestamp(0),
        major_version(0),
        minor_version(0),
        global_flagsclear(0),
        global_flagsset(0),
        criticalsection_default_timeout(0),
        decommit_freeblock_threshold(0),
        decommit_totalfree_threshold(0),
        lock_prefix_table(0),
        maximum_allocation_size(0),
        virtual_memory_threshold(0),
        process_heap_flags(0),
        process_affinity_mask(0),
        csd_version(0),
        dependent_load_flags(0),
        editlist(0),
        security_cookie(0),
        se_handler_table(0),
        se_handler_count(0),
        guard_cf_check_function_pointer(0),
        guard_cf_dispatch_function_pointer(0),
        guard_cf_function_table(0),
        guard_cf_function_count(0),
        guard_flags(0),
        code_integrity({ 0 }),
        guard_address_taken_iat_entry_table(0),
        guard_address_taken_iat_entry_count(0),
        guard_long_jump_target_table(0),
        guard_long_jump_target_count(0),
        dynamic_value_reloc_table(0),
        chpe_meta_data_pointer(0),
        guard_rf_failure_routine(0),
        guard_rf_failure_routine_function_pointer(0),
        dynamic_value_reloc_table_offset(0),
        dynamic_value_reloc_table_section(0),
        guard_rf_verify_stack_pointer_function_pointer(0),
        hot_patch_table_offset(0),
        enclave_configuration_pointer(0){}

void        pe_load_config_directory::set_size(uint32_t  size) {
    this->size = size;
}
void        pe_load_config_directory::set_timestamp(uint32_t timestamp) {
    this->timestamp = timestamp;
}
void        pe_load_config_directory::set_major_version(uint16_t major_version) {
    this->major_version = major_version;
}
void        pe_load_config_directory::set_minor_version(uint16_t minor_version) {
    this->minor_version = minor_version;
}
void        pe_load_config_directory::set_global_flagsclear(uint32_t global_flagsclear) {
    this->global_flagsclear = global_flagsclear;
}
void        pe_load_config_directory::set_global_flagsset(uint32_t global_flagsset) {
    this->global_flagsset = global_flagsset;
}
void        pe_load_config_directory::set_criticalsection_default_timeout(uint32_t criticalsection_default_timeout) {
    this->criticalsection_default_timeout = criticalsection_default_timeout;
}
void        pe_load_config_directory::set_decommit_freeblock_threshold(uint64_t  decommit_freeblock_threshold) {
    this->decommit_freeblock_threshold = decommit_freeblock_threshold;
}
void        pe_load_config_directory::set_decommit_totalfree_threshold(uint64_t  decommit_totalfree_threshold) {
    this->decommit_totalfree_threshold = decommit_totalfree_threshold;
}
void        pe_load_config_directory::set_lock_prefix_table(uint32_t lock_prefix_table) {
    this->lock_prefix_table = lock_prefix_table;
}
void        pe_load_config_directory::set_maximum_allocation_size(uint64_t  maximum_allocation_size) {
    this->maximum_allocation_size = maximum_allocation_size;
}
void        pe_load_config_directory::set_virtual_memory_threshold(uint64_t  virtual_memory_threshold) {
    this->virtual_memory_threshold = virtual_memory_threshold;
}
void        pe_load_config_directory::set_process_heap_flags(uint32_t process_heap_flags) {
    this->process_heap_flags = process_heap_flags;
}
void        pe_load_config_directory::set_process_affinity_mask(uint64_t  process_affinity_mask) {
    this->process_affinity_mask = process_affinity_mask;
}
void        pe_load_config_directory::set_csd_version(uint16_t csd_version) {
    this->csd_version = csd_version;
}
void        pe_load_config_directory::set_dependent_load_flags(uint16_t dependent_load_flags) {
    this->dependent_load_flags = dependent_load_flags;
}
void        pe_load_config_directory::set_editlist(uint32_t editlist) {
    this->editlist = editlist;
}
void        pe_load_config_directory::set_security_cookie(uint32_t security_cookie) {
    this->security_cookie = security_cookie;
}
void        pe_load_config_directory::set_se_handler_table(uint32_t se_handler_table) {
    this->se_handler_table = se_handler_table;
}
void        pe_load_config_directory::set_se_handler_count(uint32_t  se_handler_count) {
    this->se_handler_count = se_handler_count;
}
void        pe_load_config_directory::set_guard_cf_check_function_pointer(uint32_t guard_cf_check_function_pointer) {
    this->guard_cf_check_function_pointer = guard_cf_check_function_pointer;
}
void        pe_load_config_directory::set_guard_cf_dispatch_function_pointer(uint32_t  guard_cf_dispatch_function_pointer) {
    this->guard_cf_dispatch_function_pointer = guard_cf_dispatch_function_pointer;
}
void        pe_load_config_directory::set_guard_cf_function_table(uint32_t guard_cf_function_table) {
    this->guard_cf_function_table = guard_cf_function_table;
}
void        pe_load_config_directory::set_guard_cf_function_count(uint32_t  guard_cf_function_count) {
    this->guard_cf_function_count = guard_cf_function_count;
}
void        pe_load_config_directory::set_guard_flags(uint32_t guard_flags) {
    this->guard_flags = guard_flags;
}
void        pe_load_config_directory::set_code_integrity(image_load_config_code_integrity& code_integrity) {
    this->code_integrity = code_integrity;
}
void        pe_load_config_directory::set_guard_address_taken_iat_entry_table(uint32_t guard_address_taken_iat_entry_table) {
    this->guard_address_taken_iat_entry_table = guard_address_taken_iat_entry_table;
}
void        pe_load_config_directory::set_guard_address_taken_iat_entry_count(uint32_t guard_address_taken_iat_entry_count) {
    this->guard_address_taken_iat_entry_count = guard_address_taken_iat_entry_count;
}
void        pe_load_config_directory::set_guard_long_jump_target_table(uint32_t guard_long_jump_target_table) {
    this->guard_long_jump_target_table = guard_long_jump_target_table;
}
void        pe_load_config_directory::set_guard_long_jump_target_count(uint32_t guard_long_jump_target_count) {
    this->guard_long_jump_target_count = guard_long_jump_target_count;
}
void        pe_load_config_directory::set_dynamic_value_reloc_table(uint32_t dynamic_value_reloc_table) {
    this->dynamic_value_reloc_table = dynamic_value_reloc_table;
}
void        pe_load_config_directory::set_chpe_meta_data_pointer(uint32_t chpe_meta_data_pointer) {
    this->chpe_meta_data_pointer = chpe_meta_data_pointer;
}
void        pe_load_config_directory::set_guard_rf_failure_routine(uint32_t guard_rf_failure_routine) {
    this->guard_rf_failure_routine = guard_rf_failure_routine;
}
void        pe_load_config_directory::set_guard_rf_failure_routine_function_pointer(uint32_t guard_rf_failure_routine_function_pointer) {
    this->guard_rf_failure_routine_function_pointer = guard_rf_failure_routine_function_pointer;
}
void        pe_load_config_directory::set_dynamic_value_reloc_table_offset(uint32_t dynamic_value_reloc_table_offset) {
    this->dynamic_value_reloc_table_offset = dynamic_value_reloc_table_offset;
}
void        pe_load_config_directory::set_dynamic_value_reloc_table_section(uint16_t dynamic_value_reloc_table_section) {
    this->dynamic_value_reloc_table_section = dynamic_value_reloc_table_section;
}
void        pe_load_config_directory::set_guard_rf_verify_stack_pointer_function_pointer(uint32_t guard_rf_verify_stack_pointer_function_pointer) {
    this->guard_rf_verify_stack_pointer_function_pointer = guard_rf_verify_stack_pointer_function_pointer;
}
void        pe_load_config_directory::set_hot_patch_table_offset(uint32_t hot_patch_table_offset) {
    this->hot_patch_table_offset = hot_patch_table_offset;
}
void        pe_load_config_directory::set_enclave_configuration_pointer(uint32_t enclave_configuration_pointer) {
    this->enclave_configuration_pointer = enclave_configuration_pointer;
}

uint32_t        pe_load_config_directory::get_size() const {
    return this->size;
}
uint32_t        pe_load_config_directory::get_timestamp() const {
    return this->timestamp;
}
uint16_t        pe_load_config_directory::get_major_version() const {
    return this->major_version;
}
uint16_t        pe_load_config_directory::get_minor_version() const {
    return this->minor_version;
}
uint32_t        pe_load_config_directory::get_global_flagsclear() const {
    return this->global_flagsclear;
}
uint32_t        pe_load_config_directory::get_global_flagsset() const {
    return this->global_flagsset;
}
uint32_t        pe_load_config_directory::get_criticalsection_default_timeout() const {
    return this->criticalsection_default_timeout;
}
uint64_t     pe_load_config_directory::get_decommit_freeblock_threshold() const {
    return this->decommit_freeblock_threshold;
}
uint64_t     pe_load_config_directory::get_decommit_totalfree_threshold() const {
    return this->decommit_totalfree_threshold;
}
uint32_t    pe_load_config_directory::get_lock_prefix_table() const {
    return this->lock_prefix_table;
}
uint64_t     pe_load_config_directory::get_maximum_allocation_size() const {
    return this->maximum_allocation_size;
}
uint64_t     pe_load_config_directory::get_virtual_memory_threshold() const {
    return this->virtual_memory_threshold;
}
uint32_t        pe_load_config_directory::get_process_heap_flags() const {
    return this->process_heap_flags;
}
uint64_t     pe_load_config_directory::get_process_affinity_mask() const {
    return this->process_affinity_mask;
}
uint16_t        pe_load_config_directory::get_csd_version() const {
    return this->csd_version;
}
uint16_t        pe_load_config_directory::get_dependent_load_flags() const {
    return this->dependent_load_flags;
}
uint32_t    pe_load_config_directory::get_editlist() const {
    return this->editlist;
}
uint32_t    pe_load_config_directory::get_security_cookie() const {
    return this->security_cookie;
}
uint32_t    pe_load_config_directory::get_se_handler_table() const {
    return this->se_handler_table;
}
uint32_t    pe_load_config_directory::get_se_handler_count() const {
    return this->se_handler_count;
}
uint32_t    pe_load_config_directory::get_guard_cf_check_function_pointer() const {
    return this->guard_cf_check_function_pointer;
}
uint32_t        pe_load_config_directory::get_guard_cf_dispatch_function_pointer() const {
    return this->guard_cf_dispatch_function_pointer;
}
uint32_t    pe_load_config_directory::get_guard_cf_function_table() const {
    return this->guard_cf_function_table;
}
uint32_t        pe_load_config_directory::get_guard_cf_function_count() const {
    return this->guard_cf_function_count;
}
uint32_t        pe_load_config_directory::get_guard_flags() const {
    return this->guard_flags;
}

const std::vector<uint32_t >& pe_load_config_directory::get_se_handlers() const {
    return this->se_handlers;
}
const std::vector<uint32_t >& pe_load_config_directory::get_lock_prefixes() const {
    return this->lock_prefixes_rva;
}
const std::vector<uint32_t >& pe_load_config_directory::get_guard_cf_functions() const {
    return this->guard_cf_functions_rva;
}
const std::vector<uint32_t >& pe_load_config_directory::get_guard_iat_entries() const {
    return this->guard_iat_entries_rva;
}
const std::vector<uint32_t >& pe_load_config_directory::get_guard_long_jump_targets() const {
    return this->guard_long_jump_targets_rva;
}

std::vector<uint32_t>& pe_load_config_directory::get_se_handlers() {
    return this->se_handlers;
}
std::vector<uint32_t>& pe_load_config_directory::get_lock_prefixes() {
    return this->lock_prefixes_rva;
}
std::vector<uint32_t>& pe_load_config_directory::get_guard_cf_functions() {
    return this->guard_cf_functions_rva;
}
std::vector<uint32_t >& pe_load_config_directory::get_guard_iat_entries() {
    return this->guard_iat_entries_rva;
}
std::vector<uint32_t >& pe_load_config_directory::get_guard_long_jump_targets() {
    return this->guard_long_jump_targets_rva;
}
image_load_config_code_integrity pe_load_config_directory::get_code_integrity() const {
    return this->code_integrity;
}
uint32_t        pe_load_config_directory::get_guard_address_taken_iat_entry_table() const {
    return this->guard_address_taken_iat_entry_table;
}
uint32_t        pe_load_config_directory::get_guard_address_taken_iat_entry_count() const {
    return this->guard_address_taken_iat_entry_count;
}
uint32_t        pe_load_config_directory::get_guard_long_jump_target_table() const {
    return this->guard_long_jump_target_table;
}
uint32_t        pe_load_config_directory::get_guard_long_jump_target_count() const {
    return this->guard_long_jump_target_count;
}
uint32_t        pe_load_config_directory::get_dynamic_value_reloc_table() const {
    return this->dynamic_value_reloc_table;
}
uint32_t        pe_load_config_directory::get_chpe_meta_data_pointer() const {
    return this->chpe_meta_data_pointer;
}
uint32_t        pe_load_config_directory::get_guard_rf_failure_routine() const {
    return this->guard_rf_failure_routine;
}
uint32_t        pe_load_config_directory::get_guard_rf_failure_routine_function_pointer() const {
    return this->guard_rf_failure_routine_function_pointer;
}
uint32_t        pe_load_config_directory::get_dynamic_value_reloc_table_offset() const {
    return this->dynamic_value_reloc_table_offset;
}
uint16_t        pe_load_config_directory::get_dynamic_value_reloc_table_section() const {
    return this->dynamic_value_reloc_table_section;
}
uint32_t        pe_load_config_directory::get_guard_rf_verify_stack_pointer_function_pointer() const {
    return this->guard_rf_verify_stack_pointer_function_pointer;
}
uint32_t        pe_load_config_directory::get_hot_patch_table_offset() const {
    return this->hot_patch_table_offset;
}
uint32_t        pe_load_config_directory::get_enclave_configuration_pointer() const {
    return this->enclave_configuration_pointer;
}

template <typename image_format>
pe_directory_code _get_load_config_directory(const pe_image &image, pe_load_config_directory& load_config) {
    load_config.set_size(0);
    load_config.set_timestamp(0);
    load_config.set_major_version(0);
    load_config.set_minor_version(0);
    load_config.set_global_flagsclear(0);
    load_config.set_global_flagsset(0);
    load_config.set_criticalsection_default_timeout(0);
    load_config.set_decommit_freeblock_threshold(0);
    load_config.set_decommit_totalfree_threshold(0);
    load_config.set_lock_prefix_table(0);
    load_config.set_maximum_allocation_size(0);
    load_config.set_virtual_memory_threshold(0);
    load_config.set_process_heap_flags(0);
    load_config.set_process_affinity_mask(0);
    load_config.set_csd_version(0);
    load_config.set_dependent_load_flags(0);
    load_config.set_editlist(0);
    load_config.set_security_cookie(0);
    load_config.set_se_handler_table(0);
    load_config.set_se_handler_count(0);
    load_config.set_guard_cf_check_function_pointer(0);
    load_config.set_guard_cf_function_table(0);
    load_config.set_guard_cf_function_count(0);
    load_config.set_guard_flags(0);
    image_load_config_code_integrity _code_integrity = { 0 };
    load_config.set_code_integrity(_code_integrity);
    load_config.set_guard_address_taken_iat_entry_table(0);
    load_config.set_guard_address_taken_iat_entry_count(0);
    load_config.set_guard_long_jump_target_table(0);
    load_config.set_guard_long_jump_target_count(0);
    load_config.set_dynamic_value_reloc_table(0);
    load_config.set_chpe_meta_data_pointer(0);
    load_config.set_guard_rf_failure_routine(0);
    load_config.set_guard_rf_failure_routine_function_pointer(0);
    load_config.set_dynamic_value_reloc_table_offset(0);
    load_config.set_dynamic_value_reloc_table_section(0);
    load_config.set_guard_rf_verify_stack_pointer_function_pointer(0);
    load_config.set_hot_patch_table_offset(0);
    load_config.set_enclave_configuration_pointer(0);
    load_config.get_se_handlers().clear();
    load_config.get_lock_prefixes().clear();
    load_config.get_guard_cf_functions().clear();
    
    uint32_t virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
    uint32_t virtual_size = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);

    if (virtual_address && virtual_size) {

        if (!pe_image_io(image).is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        typename image_format::image_load_config_directory load_config_desc;
        uint32_t desc_size;

        if (pe_image_io(image).set_image_offset(virtual_address).read(&desc_size,sizeof(desc_size)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        if (!desc_size) { return pe_directory_code::pe_directory_code_success; }

        desc_size = min(desc_size, sizeof(typename image_format::image_load_config_directory));

        if (pe_image_io(image).set_image_offset(virtual_address).read(&load_config_desc, desc_size) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        load_config.set_size(desc_size);
        load_config.set_timestamp(load_config_desc.time_date_stamp);
        load_config.set_major_version(load_config_desc.major_version);
        load_config.set_minor_version(load_config_desc.major_version);
        load_config.set_global_flagsclear(load_config_desc.global_flags_clear);
        load_config.set_global_flagsset(load_config_desc.global_flags_set);
        load_config.set_criticalsection_default_timeout(load_config_desc.critical_section_default_timeout);
        load_config.set_decommit_freeblock_threshold(load_config_desc.decommit_free_block_threshold);
        load_config.set_decommit_totalfree_threshold(load_config_desc.decommit_total_free_threshold);

        if (load_config_desc.lock_prefix_table) {
            load_config.set_lock_prefix_table(image.va_to_rva(load_config_desc.lock_prefix_table));

            if (image.is_x32_image()) {
                pe_image_io loadcfg_lock_prefix_io((pe_image &)image);
                loadcfg_lock_prefix_io.set_image_offset(image.va_to_rva(load_config_desc.lock_prefix_table));

                typename image_format::ptr_size lock_prefix_va = 0;

                do {

                    if (loadcfg_lock_prefix_io.read(&lock_prefix_va, sizeof(lock_prefix_va)) != enma_io_success) {
                        return pe_directory_code::pe_directory_code_currupted;
                    }

                    load_config.get_lock_prefixes().push_back(image.va_to_rva(lock_prefix_va));

                } while (lock_prefix_va);
            }
        }

        load_config.set_maximum_allocation_size(load_config_desc.maximum_allocation_size);
        load_config.set_virtual_memory_threshold(load_config_desc.virtual_memory_threshold);
        load_config.set_process_heap_flags(load_config_desc.process_heap_flags);
        load_config.set_process_affinity_mask(load_config_desc.process_affinity_mask);
        load_config.set_csd_version(load_config_desc.csd_version);
        load_config.set_dependent_load_flags(load_config_desc.dependent_load_flags);

        if (load_config_desc.edit_list) {
            load_config.set_editlist(image.va_to_rva(load_config_desc.edit_list));
        }
        if (load_config_desc.security_cookie) {
            load_config.set_security_cookie(image.va_to_rva(load_config_desc.security_cookie));
        }

        if (offsetof(typename image_format::image_load_config_directory, se_handler_table) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }


        if (load_config_desc.se_handler_table) {
            load_config.set_se_handler_table(image.va_to_rva(load_config_desc.se_handler_table));
            
            if (image.is_x32_image()) {
                pe_image_io loadcfg_se_handlers_io((pe_image &)image);
                loadcfg_se_handlers_io.set_image_offset(image.va_to_rva(load_config_desc.se_handler_table));

                for (uint32_t i = 0; i < load_config_desc.se_handler_count; i++) {
                    uint32_t se_handler_rva;

                    if (loadcfg_se_handlers_io.read(&se_handler_rva, sizeof(se_handler_rva)) != enma_io_success) {
                        return pe_directory_code::pe_directory_code_currupted;
                    }

                    load_config.get_se_handlers().push_back(se_handler_rva);
                }
            }
        }
        load_config.set_se_handler_count(uint32_t(load_config_desc.se_handler_count));

        if (offsetof(typename image_format::image_load_config_directory, guard_cf_check_function_pointer) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        if (load_config_desc.guard_cf_check_function_pointer) {
            load_config.set_guard_cf_check_function_pointer(image.va_to_rva(load_config_desc.guard_cf_check_function_pointer));
        }
        if (load_config_desc.guard_cf_dispatch_function_pointer) {
            load_config.set_guard_cf_dispatch_function_pointer(image.va_to_rva(load_config_desc.guard_cf_dispatch_function_pointer));
        }

        if (load_config_desc.guard_cf_function_table) {
            load_config.set_guard_cf_function_table(image.va_to_rva(load_config_desc.guard_cf_function_table));

            pe_image_io loadcfg_guard_cf_function_io((pe_image &)image);
            loadcfg_guard_cf_function_io.set_image_offset(image.va_to_rva(load_config_desc.guard_cf_function_table));
            for (uint32_t i = 0; i < load_config_desc.guard_cf_function_count; i++) {
                uint32_t cf_function_va;

                if (loadcfg_guard_cf_function_io.read(&cf_function_va, sizeof(cf_function_va)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                load_config.get_guard_cf_functions().push_back(cf_function_va);
            }
        }
        load_config.set_guard_cf_function_count(uint32_t(load_config_desc.guard_cf_function_count));

        load_config.set_guard_flags(load_config_desc.guard_flags);
        
        if (offsetof(typename image_format::image_load_config_directory, code_integrity) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        load_config.set_code_integrity(load_config_desc.code_integrity);

        if (offsetof(typename image_format::image_load_config_directory, guard_address_taken_iat_entry_table) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }


        if (load_config_desc.guard_address_taken_iat_entry_table) {
            load_config.set_guard_address_taken_iat_entry_table(image.va_to_rva(load_config_desc.guard_address_taken_iat_entry_table));

            pe_image_io loadcfg_guard_address_taken_iat_io((pe_image &)image);
            loadcfg_guard_address_taken_iat_io.set_image_offset(image.va_to_rva(load_config_desc.guard_address_taken_iat_entry_table));
            for (uint32_t i = 0; i < load_config_desc.guard_address_taken_iat_entry_count; i++) {
                uint32_t address_taken_iat;

                if (loadcfg_guard_address_taken_iat_io.read(&address_taken_iat, sizeof(address_taken_iat)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                load_config.get_guard_iat_entries().push_back(address_taken_iat);
            }
        }
        load_config.set_guard_address_taken_iat_entry_count(uint32_t(load_config_desc.guard_address_taken_iat_entry_count));


        if (load_config_desc.guard_long_jump_target_table) {
            load_config.set_guard_address_taken_iat_entry_table(image.va_to_rva(load_config_desc.guard_long_jump_target_table));

            pe_image_io guard_long_jump_target_io((pe_image &)image);
            guard_long_jump_target_io.set_image_offset(image.va_to_rva(load_config_desc.guard_long_jump_target_table));
            for (uint32_t i = 0; i < load_config_desc.guard_long_jump_target_count; i++) {
                uint32_t guard_long_jump_target;

                if (guard_long_jump_target_io.read(&guard_long_jump_target, sizeof(guard_long_jump_target)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                load_config.get_guard_long_jump_targets().push_back(guard_long_jump_target);
            }
        }
        load_config.set_guard_long_jump_target_count(uint32_t(load_config_desc.guard_long_jump_target_count));


        if (offsetof(typename image_format::image_load_config_directory, dynamic_value_reloc_table) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        if (load_config_desc.dynamic_value_reloc_table) {
            load_config.set_dynamic_value_reloc_table(image.va_to_rva(load_config_desc.dynamic_value_reloc_table));
        }

        if (load_config_desc.chpe_meta_data_pointer) {
            load_config.set_chpe_meta_data_pointer(image.va_to_rva(load_config_desc.chpe_meta_data_pointer));
        }

        if (offsetof(typename image_format::image_load_config_directory, guard_rf_failure_routine) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        if (load_config_desc.guard_rf_failure_routine) {
            load_config.set_guard_rf_failure_routine(image.va_to_rva(load_config_desc.guard_rf_failure_routine));
        }

        if (load_config_desc.guard_rf_failure_routine_function_pointer) {
            load_config.set_guard_rf_failure_routine_function_pointer(image.va_to_rva(load_config_desc.guard_rf_failure_routine_function_pointer));
        }

        load_config.set_dynamic_value_reloc_table_offset(load_config_desc.dynamic_value_reloc_table_offset);
        load_config.set_dynamic_value_reloc_table_section(load_config_desc.dynamic_value_reloc_table_section);

        if (offsetof(typename image_format::image_load_config_directory, guard_rf_verify_stack_pointer_function_pointer) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        if (load_config_desc.guard_rf_verify_stack_pointer_function_pointer) {
            load_config.set_guard_rf_verify_stack_pointer_function_pointer(image.va_to_rva(load_config_desc.guard_rf_verify_stack_pointer_function_pointer));
        }

        load_config.set_hot_patch_table_offset(load_config_desc.hot_patch_table_offset);

        if (offsetof(typename image_format::image_load_config_directory, enclave_configuration_pointer) >= desc_size) {
            return pe_directory_code::pe_directory_code_success;
        }

        if (load_config_desc.enclave_configuration_pointer) {
            load_config.set_enclave_configuration_pointer(image.va_to_rva(load_config_desc.enclave_configuration_pointer));
        }

        return pe_directory_code::pe_directory_code_success;
    }
    
    return pe_directory_code::pe_directory_code_not_present;
}


template<typename image_format>
bool _build_load_config_directory_only(pe_image &image, pe_section& section, pe_load_config_directory& load_config, pe_relocations_directory& relocs) {

    if (load_config.get_size()) {
        pe_section_io load_config_io(section, image, enma_io_mode_allow_expand);
        load_config_io.align_up(0x10).seek_to_end(); //important !!!! if it directory not aligned file is incorrect for windows loader

        typename image_format::image_load_config_directory load_config_desc;
        memset(&load_config_desc,0,sizeof(load_config_desc));

        load_config_desc.size                           = load_config.get_size();
        load_config_desc.time_date_stamp                = load_config.get_timestamp();
        load_config_desc.major_version                  = load_config.get_major_version();
        load_config_desc.minor_version                  = load_config.get_minor_version();
        load_config_desc.global_flags_clear             = load_config.get_global_flagsclear();
        load_config_desc.global_flags_set               = load_config.get_global_flagsset();
        load_config_desc.critical_section_default_timeout = load_config.get_criticalsection_default_timeout();
        load_config_desc.decommit_free_block_threshold  = typename image_format::ptr_size(load_config.get_decommit_freeblock_threshold());
        load_config_desc.decommit_total_free_threshold  = typename image_format::ptr_size(load_config.get_decommit_totalfree_threshold());

        if (load_config.get_lock_prefix_table()) {
            load_config_desc.lock_prefix_table = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_lock_prefix_table()));
        }
        
        load_config_desc.maximum_allocation_size        = typename image_format::ptr_size(load_config.get_maximum_allocation_size());
        load_config_desc.virtual_memory_threshold       = typename image_format::ptr_size(load_config.get_virtual_memory_threshold());
        load_config_desc.process_affinity_mask          = typename image_format::ptr_size(load_config.get_process_affinity_mask());
        load_config_desc.process_heap_flags             = load_config.get_process_heap_flags();
        load_config_desc.csd_version                    = load_config.get_csd_version();
        load_config_desc.dependent_load_flags           = load_config.get_dependent_load_flags();

        if (load_config.get_editlist()) {
            load_config_desc.edit_list = typename image_format::ptr_size(image.rva_to_va(load_config.get_editlist()));
        }
        if (load_config.get_security_cookie()) {
            load_config_desc.security_cookie = typename image_format::ptr_size(image.rva_to_va(load_config.get_security_cookie()));
        }
        if (load_config.get_se_handler_table()) {
            load_config_desc.se_handler_table = typename image_format::ptr_size(image.rva_to_va(load_config.get_se_handler_table()));
        }
        load_config_desc.se_handler_count               = load_config.get_se_handler_count();
        if (load_config.get_guard_cf_check_function_pointer()) {
            load_config_desc.guard_cf_check_function_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_cf_check_function_pointer()));
        }

        if (load_config.get_guard_cf_dispatch_function_pointer()) {
            load_config_desc.guard_cf_dispatch_function_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_cf_dispatch_function_pointer()));
        }

        if (load_config.get_guard_cf_function_table()) {
            load_config_desc.guard_cf_function_table = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_cf_function_table()));
        }

        load_config_desc.guard_cf_function_count        = load_config.get_guard_cf_function_count();
        load_config_desc.guard_flags                    = load_config.get_guard_flags();
        load_config_desc.code_integrity                 = load_config.get_code_integrity();

        if (load_config.get_guard_address_taken_iat_entry_table()) {
            load_config_desc.guard_address_taken_iat_entry_table = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_address_taken_iat_entry_table()));
        }

        load_config_desc.guard_address_taken_iat_entry_count = load_config.get_guard_address_taken_iat_entry_count();

        if (load_config.get_guard_long_jump_target_table()) {
            load_config_desc.guard_long_jump_target_table = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_long_jump_target_table()));
        }

        load_config_desc.guard_long_jump_target_count   = 
            typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_long_jump_target_count()));

        if (load_config.get_dynamic_value_reloc_table()) {
            load_config_desc.dynamic_value_reloc_table = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_dynamic_value_reloc_table()));
        }

        if (load_config.get_chpe_meta_data_pointer()) {
            load_config_desc.chpe_meta_data_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_chpe_meta_data_pointer()));
        }

        if (load_config.get_guard_rf_failure_routine()) {
            load_config_desc.guard_rf_failure_routine = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_rf_failure_routine()));
        }

        if (load_config.get_guard_rf_failure_routine_function_pointer()) {
            load_config_desc.guard_rf_failure_routine_function_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_rf_failure_routine_function_pointer()));
        }
        load_config_desc.dynamic_value_reloc_table_offset  = load_config.get_dynamic_value_reloc_table_offset();
        load_config_desc.dynamic_value_reloc_table_section = load_config.get_dynamic_value_reloc_table_section();

        if (load_config.get_guard_rf_verify_stack_pointer_function_pointer()) {
            load_config_desc.guard_rf_verify_stack_pointer_function_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_guard_rf_verify_stack_pointer_function_pointer()));
        }
        load_config_desc.hot_patch_table_offset         = load_config.get_hot_patch_table_offset();

        if (load_config.get_enclave_configuration_pointer()) {
            load_config_desc.enclave_configuration_pointer = 
                typename image_format::ptr_size(image.rva_to_va(load_config.get_enclave_configuration_pointer()));
        }

        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, load_config_io.get_section_offset());
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, load_config.get_size());

        if (load_config_io.write(&load_config_desc, load_config.get_size()) != enma_io_success) {
            return false;
        }
    }
    else {
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, 0);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, 0);
    }

    return true;
}

template <typename image_format>
pe_directory_code _get_placement_load_config_directory(const pe_image &image, pe_placement& placement) {

    uint32_t virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
    uint32_t virtual_size = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);

    if (virtual_address && virtual_size) {

        if (!pe_image_io(image).is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }


        typename image_format::image_load_config_directory load_config_desc;
        uint32_t desc_size;

        if (pe_image_io(image).set_image_offset(virtual_address).read(&desc_size, sizeof(desc_size)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        if (!desc_size) { return pe_directory_code::pe_directory_code_success; }

        desc_size = min(desc_size, sizeof(typename image_format::image_load_config_directory));

        if (pe_image_io(image).set_image_offset(virtual_address).read(&load_config_desc, desc_size) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        placement[virtual_address] = pe_placement_entry(desc_size, id_pe_placement::id_pe_loadconfig_descriptor, "");

        if (image.is_x32_image()) {
            if (offsetof(typename image_format::image_load_config_directory, se_handler_table) < desc_size &&
                load_config_desc.se_handler_count &&
                load_config_desc.se_handler_table) {

                placement[(uint32_t)image.va_to_rva(load_config_desc.se_handler_table)] = 
                    pe_placement_entry((uint32_t)(load_config_desc.se_handler_count * sizeof(uint32_t)), id_pe_placement::id_pe_loadconfig_se_table, "");
            }

            if (offsetof(typename image_format::image_load_config_directory, lock_prefix_table) < desc_size &&
                load_config_desc.lock_prefix_table) {

                size_t lock_prefix_count = 0;

                pe_image_io lock_pref_io(image);
                lock_pref_io.set_image_offset(image.va_to_rva(load_config_desc.lock_prefix_table));

                uint32_t lock_item = 1;

                for (; lock_item; lock_prefix_count++) {
                    if (lock_pref_io.read(&lock_item, sizeof(lock_item)) != enma_io_success) {
                        return pe_directory_code::pe_directory_code_currupted;
                    }
                }

                placement[image.va_to_rva(load_config_desc.lock_prefix_table)] = 
                    pe_placement_entry(((lock_prefix_count + 1) * sizeof(uint32_t)), id_pe_placement::id_pe_loadconfig_lock_table, "");
            }
        }

        if (offsetof(typename image_format::image_load_config_directory, guard_cf_function_table) < desc_size &&
            load_config_desc.guard_cf_function_count &&
            load_config_desc.guard_cf_function_table) {

            placement[image.va_to_rva(load_config_desc.guard_cf_function_table)] =
                pe_placement_entry((uint32_t)(load_config_desc.guard_cf_function_count * sizeof(uint32_t)), id_pe_placement::id_pe_loadconfig_cf_table, "");
        }

        if (offsetof(typename image_format::image_load_config_directory, guard_address_taken_iat_entry_table) < desc_size &&
            load_config_desc.guard_address_taken_iat_entry_count &&
            load_config_desc.guard_address_taken_iat_entry_table) {

            placement[image.va_to_rva(load_config_desc.guard_address_taken_iat_entry_table)] =
                pe_placement_entry((uint32_t)(load_config_desc.guard_address_taken_iat_entry_count * sizeof(uint32_t)), id_pe_placement::id_pe_loadconfig_iat_table, "");
        }

        if (offsetof(typename image_format::image_load_config_directory, guard_long_jump_target_table) < desc_size &&
            load_config_desc.guard_long_jump_target_count &&
            load_config_desc.guard_long_jump_target_table) {

            placement[image.va_to_rva(load_config_desc.guard_long_jump_target_table)] =
                pe_placement_entry((uint32_t)(load_config_desc.guard_long_jump_target_count * sizeof(uint32_t)), id_pe_placement::id_pe_loadconfig_long_jump_table, "");
        }

        
    }

    return pe_directory_code::pe_directory_code_not_present;
}

pe_directory_code get_load_config_directory(const pe_image &image, pe_load_config_directory& load_config) {
   
    if (image.is_x32_image()) {
        return _get_load_config_directory<pe_image_32>(image, load_config);
    }
    else {
        return _get_load_config_directory<pe_image_64>(image, load_config);
    }
}


bool build_internal_load_config_directory_data(pe_image &image, pe_section& section,
    pe_load_config_directory& load_config, uint32_t build_items_ids/*import_table_build_id*/,
    pe_relocations_directory& relocs) {

    if (image.is_x32_image() &&
        (build_items_ids & load_config_table_build_se_handlers || build_items_ids & load_config_table_build_lock_prefixes)) {

        if (build_items_ids & load_config_table_build_se_handlers) {
            if (load_config.get_se_handlers().size()) {
                pe_section_io se_handlers_io(section, image, enma_io_mode_allow_expand);
                se_handlers_io.align_up(0x4).seek_to_end();

                load_config.set_se_handler_table(se_handlers_io.get_section_offset());
                load_config.set_se_handler_count(uint32_t(load_config.get_se_handlers().size()));

                for (auto& item : load_config.get_se_handlers()) {

                    if (se_handlers_io.write(&item,sizeof(item)) != enma_io_success) {
                        return false;
                    }
                }

            }
            else {
                load_config.set_se_handler_table(0);
                load_config.set_se_handler_count(0);
            }
        }

        if (build_items_ids & load_config_table_build_lock_prefixes) {
            if (load_config.get_lock_prefixes().size()) {
                pe_section_io lock_prefixes_io(section, image, enma_io_mode_allow_expand);
                lock_prefixes_io.align_up(0x4).seek_to_end();

                load_config.set_se_handler_table(lock_prefixes_io.get_section_offset());

                for (auto& item : load_config.get_lock_prefixes()) {
                    uint32_t lock_pref_va = uint32_t(image.rva_to_va(item));//uint32 bacause used only in x86 

                    if (lock_prefixes_io.write(&lock_pref_va, sizeof(lock_pref_va)) != enma_io_success) {
                        return false;
                    }
                }
                uint32_t lock_pref_null = 0;
                if (lock_prefixes_io.write(&lock_pref_null, sizeof(lock_pref_null)) != enma_io_success) {
                    return false;
                }
            }
            else {
                load_config.set_lock_prefix_table(0);
            }
        }
    }

    if (build_items_ids & load_config_table_build_guard_cf_functions) {
        if (load_config.get_guard_cf_functions().size()) {
            pe_section_io guard_cf_functions_io(section, image, enma_io_mode_allow_expand);
            guard_cf_functions_io.align_up(0x4).seek_to_end();

            load_config.set_guard_cf_function_table(guard_cf_functions_io.get_section_offset());
            load_config.set_guard_cf_function_count(uint32_t(load_config.get_guard_cf_functions().size()));

            for (auto& item : load_config.get_guard_cf_functions()) {

                if (guard_cf_functions_io.write(&item, sizeof(item)) != enma_io_success) {
                    return false;
                }
            }
        }
        else {
            load_config.set_guard_cf_function_table(0);
            load_config.set_guard_cf_function_count(0);
        }
    }

    if (build_items_ids & load_config_table_build_guard_iat_entrys) {
        if (load_config.get_guard_iat_entries().size()) {
            pe_section_io guard_iat_io(section, image, enma_io_mode_allow_expand);
            guard_iat_io.align_up(0x4).seek_to_end();

            load_config.set_guard_address_taken_iat_entry_table(guard_iat_io.get_section_offset());
            load_config.set_guard_address_taken_iat_entry_count(uint32_t(load_config.get_guard_iat_entries().size()));

            for (auto& item : load_config.get_guard_iat_entries()) {

                if (guard_iat_io.write(&item, sizeof(item)) != enma_io_success) {
                    return false;
                }
            }
        }
        else {
            load_config.set_guard_address_taken_iat_entry_table(0);
            load_config.set_guard_address_taken_iat_entry_count(0);
        }
    }

    if (build_items_ids & load_config_table_build_guard_long_jump_targets) {
        if (load_config.get_guard_long_jump_targets().size()) {
            pe_section_io guard_long_jump_io(section, image, enma_io_mode_allow_expand);
            guard_long_jump_io.align_up(0x4).seek_to_end();

            load_config.set_guard_long_jump_target_table(guard_long_jump_io.get_section_offset());
            load_config.set_guard_long_jump_target_count(uint32_t(load_config.get_guard_long_jump_targets().size()));

            for (auto& item : load_config.get_guard_long_jump_targets()) {

                if (guard_long_jump_io.write(&item, sizeof(item)) != enma_io_success) {
                    return false;
                }
            }
        }
        else {
            load_config.set_guard_long_jump_target_table(0);
            load_config.set_guard_long_jump_target_count(0);
        }
    }

    return true;
}

bool build_load_config_directory_only( pe_image &image, pe_section& section,
     pe_load_config_directory& load_config, pe_relocations_directory& relocs) {


    if (image.is_x32_image()) {
        return _build_load_config_directory_only<pe_image_32>(image, section, load_config, relocs);
    }
    else {
        return _build_load_config_directory_only<pe_image_64>(image, section, load_config, relocs);
    }
}

bool build_load_config_directory_full(pe_image &image, pe_section& section,
     pe_load_config_directory& load_config, pe_relocations_directory& relocs) {

    return build_internal_load_config_directory_data(image, section, load_config,
        load_config_table_build_se_handlers | load_config_table_build_lock_prefixes | load_config_table_build_guard_cf_functions |
        load_config_table_build_guard_iat_entrys | load_config_table_build_guard_long_jump_targets, relocs) && 
        build_load_config_directory_only(image, section, load_config, relocs);
}

pe_directory_code get_placement_load_config_directory(const pe_image &image, pe_placement& placement) {

    if (image.is_x32_image()) {
        return _get_placement_load_config_directory<pe_image_32>(image,placement);
    }
    else {
        return _get_placement_load_config_directory<pe_image_64>(image, placement);
    }
}