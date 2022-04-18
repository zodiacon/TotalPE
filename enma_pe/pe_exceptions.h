#pragma once

#include "pe_exceptions_helper.h"

class pe_exception_entry;

enum class unwind_parameter_type {
    unwind_parameter_raw,
    unwind_parameter_rva,
    unwind_parameter_va
};

struct unwind_parameter {
    unwind_parameter_type type;
    std::vector<uint8_t> param_data;
};

class pe_exception_unwind_info {
    uint32_t unwind_info_rva;
    
    uint8_t version;
    uint8_t flags;
    uint8_t size_of_prolog;
    uint8_t count_of_codes;
    uint8_t frame_register;
    uint8_t frame_offset;

    uint32_t handler_rva;
    pe_exception_entry * chained_entry;

    std::vector<unwind_code> codes;

    std::vector<unwind_parameter> params;

    exceptions_handler_specific_data custom_parameter;
public:
    pe_exception_unwind_info();
    pe_exception_unwind_info(uint32_t unwind_info_rva, uint8_t version,
    uint8_t flags,uint8_t size_of_prolog,uint8_t count_of_codes,uint8_t frame_register,uint8_t frame_offset);

    pe_exception_unwind_info(const pe_exception_unwind_info& item);
    ~pe_exception_unwind_info();

    pe_exception_unwind_info& operator=(const pe_exception_unwind_info& item);
public:
    void add_unwind_code(const unwind_code& code);

    void set_version(uint8_t version);
    void set_flags(uint8_t flags);
    void set_size_of_prolog(uint8_t size_of_prolog);
    void set_count_of_codes(uint8_t count_of_codes);
    void set_frame_register(uint8_t frame_register);
    void set_frame_offset(uint8_t frame_offset);

    void set_chained_entry(pe_exception_entry * chained_entry);
    void set_handler_rva(uint32_t rva);
    void set_unwind_info_rva(uint32_t rva);

    void set_codes(std::vector<unwind_code> &codes);
    void set_params(std::vector<unwind_parameter> &params);
    void set_custom_parameter(const exceptions_handler_specific_data& custom_parameter);
public: 

    uint8_t get_version() const;
    uint8_t get_flags() const;
    uint8_t get_size_of_prolog() const;
    uint8_t get_count_of_codes() const;
    uint8_t get_frame_register() const;
    uint8_t get_frame_offset() const;

    std::vector<unwind_code>& get_codes();
    const std::vector<unwind_code>& get_codes() const;

    std::vector<unwind_parameter>& get_params();
    const std::vector<unwind_parameter>& get_params() const;

    exceptions_handler_specific_data& get_custom_parameter();
    const exceptions_handler_specific_data& get_custom_parameter() const;

    pe_exception_entry * get_chained_entry();
    const pe_exception_entry * get_chained_entry() const;
    uint32_t get_handler_rva() const;
    uint32_t get_unwind_info_rva() const;
};

class pe_exception_entry {
    uint32_t address_begin;
    uint32_t address_end;
    uint32_t address_unwind_data;
public:
    pe_exception_entry();
    pe_exception_entry(const pe_exception_entry& entry);
    pe_exception_entry(uint32_t address_begin, uint32_t address_end, uint32_t address_unwind_data);
    ~pe_exception_entry();

    pe_exception_entry& operator=(const pe_exception_entry& entry);
public:
    void set_begin_address(uint32_t rva_address);
    void set_end_address(uint32_t rva_address);
    void set_unwind_data_address(uint32_t rva_address);

public:
    uint32_t get_begin_address() const;
    uint32_t get_end_address() const;
    uint32_t get_unwind_data_address() const;
};

class pe_exceptions_directory {
    std::vector<pe_exception_entry> exception_entries;
    std::vector<pe_exception_unwind_info> unwind_entries;
public:
    pe_exceptions_directory();
    pe_exceptions_directory(const pe_exceptions_directory& exceptions);
    ~pe_exceptions_directory();

    pe_exceptions_directory& operator=(const pe_exceptions_directory& exceptions);
public:
    void add_exception_entry(uint32_t address_begin, uint32_t address_end, uint32_t address_unwind_data);
    void add_exception_entry(const pe_exception_entry& entry);
    void add_unwind_entry(const pe_exception_unwind_info& entry);
    void add_item(const runtime_function_entry& exc_entry);
    void clear();
public:
    size_t size() const;

    const std::vector<pe_exception_unwind_info>& get_unwind_entries() const;
    std::vector<pe_exception_unwind_info>& get_unwind_entries();

    const std::vector<pe_exception_entry>& get_exception_entries() const;
    std::vector<pe_exception_entry>& get_exception_entries();
};



pe_directory_code get_exception_directory(_In_ const pe_image &image, _Out_ pe_exceptions_directory& exceptions);
bool build_exceptions_directory(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_exceptions_directory& exceptions, _Inout_ pe_relocations_directory& relocs, _In_ bool build_unwindinfo = false);
pe_directory_code get_placement_exceptions_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);