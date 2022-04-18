#include "stdafx.h"
#include "pe_section.h"


pe_section::pe_section() {
    section_name.clear();
    virtual_size        = 0;
    virtual_address        = 0;
    pointer_to_raw        = 0;
    characteristics        = 0;

    section_data.clear();
}

pe_section::pe_section(const image_section_header& header) {
    this->section_name.reserve(8);
    this->section_name.resize(strlen((char*)header.name));
    memcpy((void*)this->section_name.data(), header.name,8);

    this->virtual_size        = header.virtual_size;
    this->virtual_address    = header.virtual_address;
    this->pointer_to_raw    = header.pointer_to_raw_data;
    this->characteristics    = header.characteristics;

    this->section_data.clear();
}
pe_section::pe_section(const image_section_header& header, const std::vector<uint8_t>& data):
    section_data(data){

    this->section_name.reserve(8);
    this->section_name.resize(strlen((char*)header.name));
    memcpy((void*)this->section_name.data(), header.name, 8);

    this->virtual_size        = header.virtual_size;
    this->virtual_address    = header.virtual_address;
    this->pointer_to_raw    = header.pointer_to_raw_data;
    this->characteristics    = header.characteristics;

}

pe_section& pe_section::set_section_name(const std::string& section_name) {
    this->section_name = section_name;
    return *this;
}
pe_section& pe_section::set_virtual_size(uint32_t virtual_size) {
    this->virtual_size = virtual_size;
    return *this;
}
pe_section& pe_section::set_virtual_address(uint32_t virtual_address) {
    this->virtual_address = virtual_address;
    return *this;
}
pe_section& pe_section::set_size_of_raw_data(uint32_t size_of_raw_data) {
    section_data.resize(size_of_raw_data);
    return *this;
}
pe_section& pe_section::set_pointer_to_raw(uint32_t pointer_to_raw) {
    this->pointer_to_raw = pointer_to_raw;
    return *this;
}
pe_section& pe_section::set_characteristics(uint32_t characteristics) {
    this->characteristics = characteristics;
    return *this;
}

pe_section& pe_section::set_shared(bool flag) {
    if (flag) {
        this->characteristics |= IMAGE_SCN_MEM_SHARED;
    }
    else {
        this->characteristics &= ~IMAGE_SCN_MEM_SHARED;
    }
    return *this;
}
pe_section& pe_section::set_readable(bool flag) {
    if (flag) {
        this->characteristics |= IMAGE_SCN_MEM_READ;
    }
    else {
        this->characteristics &= ~IMAGE_SCN_MEM_READ;
    }
    return *this;
}
pe_section& pe_section::set_writeable(bool flag) {
    if (flag) {
        this->characteristics |= IMAGE_SCN_MEM_WRITE;
    }
    else {
        this->characteristics &= ~IMAGE_SCN_MEM_WRITE;
    }
    return *this;
}
pe_section& pe_section::set_executable(bool flag) {
    if (flag) {
        this->characteristics |= IMAGE_SCN_MEM_EXECUTE;
    }
    else {
        this->characteristics &= ~IMAGE_SCN_MEM_EXECUTE;
    }
    return *this;
}


void pe_section::add_data(const uint8_t * data, size_t data_size) {
    section_data.resize(section_data.size() + data_size);
    memcpy(section_data.data() + section_data.size() - data_size, data, data_size);
}

std::string pe_section::get_section_name() const {
    return section_name;
}
uint32_t pe_section::get_virtual_size() const {
    return virtual_size;
}
uint32_t pe_section::get_virtual_address() const {
    return virtual_address;
}
uint32_t pe_section::get_size_of_raw_data() const {
    return uint32_t(section_data.size());
}
uint32_t pe_section::get_pointer_to_raw() const {
    return pointer_to_raw;
}
uint32_t pe_section::get_characteristics() const {
    return characteristics;
}

bool pe_section::is_shared() const {
    return (characteristics&IMAGE_SCN_MEM_SHARED) != 0;
}
bool pe_section::is_readable() const {
    return (characteristics&IMAGE_SCN_MEM_READ) != 0;
}
bool pe_section::is_writeable() const {
    return (characteristics&IMAGE_SCN_MEM_WRITE) != 0;
}
bool pe_section::is_executable() const {
    return (characteristics&IMAGE_SCN_MEM_EXECUTE) != 0;
}

std::vector<uint8_t>& pe_section::get_section_data() {
    return section_data;
}

const std::vector<uint8_t>& pe_section::get_section_data() const {
    return section_data;
}



