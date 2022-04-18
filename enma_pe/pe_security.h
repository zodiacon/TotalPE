#pragma once

class pe_security_entry {
    uint16_t revision;
    uint16_t certificate_type;

    std::vector<uint8_t> certificate_data;
public:
    pe_security_entry();
    pe_security_entry(uint16_t revision, uint16_t certificate_type, std::vector<uint8_t>& certificate_data);

    ~pe_security_entry() = default;

    pe_security_entry(const pe_security_entry&) = default;
    pe_security_entry& operator=(const pe_security_entry&) = default;
    pe_security_entry(pe_security_entry&&) = default;
    pe_security_entry& operator=(pe_security_entry&&) = default;

public:
    void set_revision(uint16_t revision);
    void set_certificate_type(uint16_t type);

    void set_certificate_data(const std::vector<uint8_t>& data);

public:
    uint16_t get_revision() const;
    uint16_t get_certificate_type() const;
    const std::vector<uint8_t>& get_certificate_data() const;
    std::vector<uint8_t>& get_certificate_data();
};


class pe_security_directory {
    std::vector<pe_security_entry> certificates;
public:
    pe_security_directory() = default;
    ~pe_security_directory() = default;

    pe_security_directory(const pe_security_directory&) = default;
    pe_security_directory& operator=(const pe_security_directory&) = default;
    pe_security_directory(pe_security_directory&&) = default;
    pe_security_directory& operator=(pe_security_directory&&) = default;

public:
    void add_certificate(const pe_security_entry& item);
    void clear();
public:
    size_t get_certificates_count() const;
    const std::vector<pe_security_entry>& get_certificates() const;
    std::vector<pe_security_entry>& get_certificates();
};

pe_directory_code get_security_directory(_In_ const pe_image &image,_Out_ pe_security_directory& security);
pe_directory_code get_placement_security_directory(_Inout_ const pe_image &image,_Inout_ pe_placement& placement);
