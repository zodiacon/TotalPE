#pragma once

class pe_export_entry {
    uint32_t rva;
    uint16_t ordinal;
    uint16_t name_ordinal;
    bool b_name;
    bool b_forward; 
    std::string forward_name;
    std::string func_name;
public:
    pe_export_entry();
    ~pe_export_entry() = default;

    pe_export_entry(const pe_export_entry&) = default;
    pe_export_entry& operator=(const pe_export_entry&) = default;
    pe_export_entry(pe_export_entry&&) = default;
    pe_export_entry& operator=(pe_export_entry&&) = default;

    bool operator==(const pe_export_entry& item);
public:
    pe_export_entry& set_rva(uint32_t rva);
    pe_export_entry& set_ordinal(uint16_t ordinal);
    pe_export_entry& set_name_ordinal(uint16_t ordinal);
    pe_export_entry& set_has_name(bool b);
    pe_export_entry& set_forward(bool b);
    pe_export_entry& set_forward_name(const std::string& forward_name);
    pe_export_entry& set_func_name(const std::string& func_name);
public:
    uint32_t  get_rva() const;
    uint16_t  get_ordinal() const;
    uint16_t  get_name_ordinal() const;
    bool  has_name() const;
    bool  is_forward() const;
    std::string  get_forward_name() const;
    std::string  get_func_name() const;

};

class pe_export_directory {
    uint32_t characteristics;
    uint32_t time_stamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t ordinal_base;
    uint32_t number_of_functions;
    uint32_t number_of_names;

    std::string library_name;
    std::vector<pe_export_entry> functions;
public:
    pe_export_directory();
    ~pe_export_directory() = default;

    pe_export_directory(const pe_export_directory&) = default;
    pe_export_directory& operator=(const pe_export_directory&) = default;
    pe_export_directory(pe_export_directory&&) = default;
    pe_export_directory& operator=(pe_export_directory&&) = default;
public:
    pe_export_directory& set_characteristics(uint32_t characteristics);
    pe_export_directory& set_time_stamp(uint32_t time_stamp);
    pe_export_directory& set_major_version(uint16_t major_version);
    pe_export_directory& set_minor_version(uint16_t minor_version);
    pe_export_directory& set_ordinal_base(uint32_t ordinal_base);
    pe_export_directory& set_number_of_functions(uint32_t number_of_functions);
    pe_export_directory& set_number_of_names(uint32_t number_of_names);
    pe_export_directory& set_library_name(const std::string& library_name);
    pe_export_directory& add_function(const pe_export_entry& function);

public:
    size_t size() const;

    uint32_t get_characteristics() const;
    uint32_t get_time_stamp() const;
    uint16_t get_major_version() const;
    uint16_t get_minor_version() const;
    uint32_t get_ordinal_base() const;
    uint32_t get_number_of_functions() const;
    uint32_t get_number_of_names() const;
    std::string get_library_name() const;
    const std::vector<pe_export_entry>& get_functions() const;

    std::vector<pe_export_entry>& get_functions();

    bool get_exported_function(const std::string& func_name, pe_export_entry * & function);
    bool get_exported_function(uint16_t ordinal, pe_export_entry * & function);
};


pe_directory_code get_export_directory(_In_ const pe_image &image,
    _Out_ pe_export_directory& exports);
bool build_export_directory(_Inout_ pe_image &image,
    _Inout_ pe_section& section, _In_ const pe_export_directory& exports);
pe_directory_code get_placement_export_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);
