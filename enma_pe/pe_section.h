#pragma once

class pe_section{
    std::string section_name;
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t pointer_to_raw;
    uint32_t characteristics;

    std::vector<uint8_t> section_data;
public:
    pe_section();
    pe_section(const image_section_header& header);
    pe_section(const image_section_header& header, const std::vector<uint8_t>& data);
    ~pe_section() = default;
    
    pe_section(const pe_section&) = default;
    pe_section& operator=(const pe_section&) = default;
    pe_section(pe_section&&) = default;
    pe_section& operator=(pe_section&&) = default;

public:
    pe_section& set_section_name(const std::string& name);
    pe_section& set_virtual_size(uint32_t virtual_size);
    pe_section& set_virtual_address(uint32_t virtual_address);
    pe_section& set_size_of_raw_data(uint32_t size_of_raw_data);
    pe_section& set_pointer_to_raw(uint32_t pointer_to_raw);
    pe_section& set_characteristics(uint32_t characteristics);

    pe_section& set_shared(bool flag);
    pe_section& set_readable(bool flag);
    pe_section& set_writeable(bool flag);
    pe_section& set_executable(bool flag);

    void add_data(const uint8_t * data, size_t data_size);
public:
    std::string get_section_name() const;
    uint32_t get_virtual_size() const;
    uint32_t get_virtual_address() const;
    uint32_t get_size_of_raw_data() const;
    uint32_t get_pointer_to_raw() const;
    uint32_t get_characteristics() const;

    bool is_shared() const;
    bool is_readable() const;
    bool is_writeable() const;
    bool is_executable() const;


    const std::vector<uint8_t>& get_section_data() const;
    std::vector<uint8_t>& get_section_data();
};


