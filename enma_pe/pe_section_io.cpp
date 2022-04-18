#include "stdafx.h"
#include "pe_section_io.h"


extern bool view_data(
    size_t  required_offset, size_t required_size, size_t& real_offset,
    size_t& available_size, size_t& down_oversize, size_t& up_oversize,
    size_t present_offset, size_t present_size);

pe_section_io::pe_section_io(pe_section & _section,
    enma_io_mode mode,
    enma_io_addressing_type type,
    uint32_t raw_aligment,
    uint32_t virtual_aligment) :
    section(&_section), section_offset(0),
    raw_aligment(raw_aligment), virtual_aligment(virtual_aligment),
    last_code(enma_io_code::enma_io_success), mode(mode), addressing_type(type){}

pe_section_io::pe_section_io(
    pe_section & _section,
    pe_image& image,
    enma_io_mode mode,
    enma_io_addressing_type type
):  section(&_section), section_offset(0),
    raw_aligment(image.get_file_align()), virtual_aligment(image.get_section_align()),
    last_code(enma_io_code::enma_io_success), mode(mode), addressing_type(type){}


pe_section_io::pe_section_io(const pe_section_io& io_section) {
    this->operator=(io_section);
}

pe_section_io::~pe_section_io() {

}

pe_section_io& pe_section_io::operator=(const pe_section_io& io_section) {
    this->section           = io_section.section;
    this->section_offset    = io_section.section_offset;
    this->last_code         = io_section.last_code;
    this->mode              = io_section.mode;
    this->addressing_type   = io_section.addressing_type;
    this->raw_aligment      = io_section.raw_aligment;
    this->virtual_aligment  = io_section.virtual_aligment;

    return *this;
}



bool pe_section_io::view_section(
    size_t required_offset, size_t required_size, size_t& real_offset,
    size_t& available_size, size_t& down_oversize, size_t& up_oversize) {

 
    switch (addressing_type) {
        case enma_io_addressing_type::enma_io_address_raw: {
            return view_data(
                required_offset, required_size,
                real_offset, available_size, down_oversize, up_oversize,
                this->section->get_pointer_to_raw(), ALIGN_UP(section->get_size_of_raw_data(), this->raw_aligment)
            );
        }

        case enma_io_addressing_type::enma_io_address_rva: {
            return view_data(
                required_offset, required_size,
                real_offset, available_size, down_oversize, up_oversize,
                this->section->get_virtual_address(), ALIGN_UP(this->section->get_virtual_size(), this->virtual_aligment)
            );
        }

        default: {return false; }
    }
    
    return false;
}

uint32_t pe_section_io::get_present_size(uint32_t required_offset) {

    if (required_offset < section->get_size_of_raw_data()) {
        return section->get_size_of_raw_data() - required_offset;
    }

    return 0;
}

enma_io_code pe_section_io::internal_read(
    size_t data_offset,
    void * buffer, size_t size,
    size_t& readed_size, size_t& down_oversize, size_t& up_oversize) {

    size_t real_offset    = 0;

    bool b_view = view_section(data_offset, size,
        real_offset,
        readed_size, down_oversize, up_oversize);


    if (b_view && readed_size) {
        size_t present_size = get_present_size((uint32_t)real_offset);

        if (present_size) {

            if (present_size >= readed_size) {
                memcpy(&((uint8_t*)buffer)[down_oversize], &section->get_section_data().data()[real_offset], readed_size);
            }
            else {               
                memcpy(&((uint8_t*)buffer)[down_oversize], &section->get_section_data().data()[real_offset], present_size);
                memset(&((uint8_t*)buffer)[down_oversize + present_size], 0, readed_size - present_size);
            }
        }
        else {
            memset(&((uint8_t*)buffer)[down_oversize], 0, readed_size);
        }

        if (down_oversize || up_oversize) {

            last_code = enma_io_code::enma_io_incomplete;
            return enma_io_code::enma_io_incomplete;
        }

        last_code = enma_io_code::enma_io_success;
        return enma_io_code::enma_io_success;
    }

    last_code = enma_io_code::enma_io_data_not_present;
    return enma_io_code::enma_io_data_not_present;
}

enma_io_code pe_section_io::internal_write(
    size_t data_offset,
    const void * buffer, size_t size,
    size_t& wrote_size, size_t& down_oversize, size_t& up_oversize) {


    size_t real_offset = 0;

    bool b_view = view_section(data_offset, size,
        real_offset,
        wrote_size, down_oversize, up_oversize);


    if (b_view &&
        (wrote_size || (up_oversize && mode == enma_io_mode::enma_io_mode_allow_expand) )) {

        if ((up_oversize && mode == enma_io_mode::enma_io_mode_allow_expand)) { 
            if (addressing_type == enma_io_addressing_type::enma_io_address_raw) {

                add_size(
                    (ALIGN_UP(section->get_size_of_raw_data(), this->raw_aligment) - section->get_size_of_raw_data()) + up_oversize            
                    , false);

                wrote_size += min(up_oversize, size);
                up_oversize = 0;
            }
            else if (addressing_type == enma_io_addressing_type::enma_io_address_rva) {

                add_size(
                    (ALIGN_UP(section->get_virtual_size(), this->virtual_aligment) - section->get_virtual_size()) + up_oversize
                    , false);

                wrote_size += min(up_oversize, size);
                up_oversize = 0;
            }
            else {
                return enma_io_code::enma_io_data_not_present;//unk addressing_type
            }        
        }
    
        if (real_offset + wrote_size > section->get_size_of_raw_data()) { //aligned by raw align

            add_size((real_offset + wrote_size) - section->get_size_of_raw_data(), false);
        }

        memcpy(&section->get_section_data().data()[real_offset], &((uint8_t*)buffer)[down_oversize], wrote_size);

        if (down_oversize || up_oversize) {

            last_code = enma_io_code::enma_io_incomplete;
            return enma_io_code::enma_io_incomplete;
        }

        last_code = enma_io_code::enma_io_success;
        return enma_io_code::enma_io_success;
    }

    last_code = enma_io_code::enma_io_data_not_present;
    return enma_io_code::enma_io_data_not_present;
}

enma_io_code pe_section_io::read(void * data, size_t size) {

    size_t readed_size;
    size_t down_oversize;
    size_t up_oversize;


    enma_io_code code = internal_read(section_offset, data, size,
        readed_size, down_oversize, up_oversize);

    section_offset += (uint32_t)readed_size;

    return code;
}

enma_io_code pe_section_io::write(const void * data, size_t size) {

    size_t wrote_size;
    size_t down_oversize;
    size_t up_oversize;

    enma_io_code code = internal_write(section_offset, data, size,
        wrote_size, down_oversize, up_oversize);

    section_offset += (uint32_t)wrote_size;

    return code;
}

enma_io_code pe_section_io::read(std::vector<uint8_t>& buffer, size_t size) {

    if (buffer.size() < size) { buffer.resize(size); }

    return read(buffer.data(), uint32_t(buffer.size()));
}

enma_io_code pe_section_io::write(const std::vector<uint8_t>& buffer) {

    return write(buffer.data(), uint32_t(buffer.size()));
}

enma_io_code pe_section_io::memory_set(size_t size, uint8_t data) {

    std::vector<uint8_t> set_buffer;
    set_buffer.resize(size);
    memset(set_buffer.data(), data, size);

    return write(set_buffer);
}

enma_io_code pe_section_io::read_string(std::string& _string) {
    
    _string.clear();
    char _char = 0;

    do {
        enma_io_code code = read(&_char, sizeof(_char));

        if (code != enma_io_success) {
            return code;
        }

        if (_char) {
            _string += _char;
        }

    } while (_char);

    return enma_io_code::enma_io_success;
}

enma_io_code pe_section_io::read_wstring(std::wstring& _wstring) {

    _wstring.clear();
    wchar_t _char = 0;

    do {
        enma_io_code code = read(&_char, sizeof(_char));

        if (code != enma_io_success) {
            return code;
        }

        if (_char) {
            _wstring += _char;
        }

    } while (_char);

    return enma_io_code::enma_io_success;
}

pe_section_io& pe_section_io::align_up(size_t factor, bool offset_to_end) {
    add_size(ALIGN_UP(section->get_size_of_raw_data(), factor) - section->get_size_of_raw_data(), offset_to_end);

    return *this;
}
pe_section_io& pe_section_io::align_up_offset(size_t factor) {
    this->section_offset = (uint32_t)ALIGN_UP(this->section_offset, factor);

    return *this;
}

pe_section_io& pe_section_io::add_size(size_t size, bool offset_to_end) {
    if (size) {
        section->get_section_data().resize(section->get_size_of_raw_data() + size);
    }

    if (section->get_size_of_raw_data() > section->get_virtual_size()) {
        section->set_virtual_size(section->get_size_of_raw_data());
    }

    if (offset_to_end) { seek_to_end(); }

    return *this;
}

pe_section_io& pe_section_io::set_mode(enma_io_mode mode) {
    this->mode = mode;

    return *this;
}
pe_section_io& pe_section_io::set_addressing_type(enma_io_addressing_type type) {
    this->addressing_type = type;

    return *this;
}
pe_section_io& pe_section_io::set_section_offset(uint32_t offset) {
    this->section_offset = offset;

    return *this;
}
pe_section_io& pe_section_io::set_raw_aligment(uint32_t aligment) {
    this->raw_aligment = aligment;

    return *this;
}
pe_section_io& pe_section_io::set_virtual_aligment(uint32_t aligment) {
    this->virtual_aligment = aligment;

    return *this;
}
pe_section_io& pe_section_io::seek_to_start() {

    switch (addressing_type) {
        case enma_io_addressing_type::enma_io_address_raw: {
            this->section_offset = this->section->get_pointer_to_raw();
            break;
        }

        case enma_io_addressing_type::enma_io_address_rva: {
            this->section_offset = this->section->get_virtual_address();
            break;
        }

        default: {this->section_offset = 0; break; }
    }

    return *this;
}
pe_section_io& pe_section_io::seek_to_end() {

    switch (addressing_type) {
    case enma_io_addressing_type::enma_io_address_raw: {
        this->section_offset = this->section->get_pointer_to_raw() + this->section->get_size_of_raw_data();
        break;
    }

    case enma_io_addressing_type::enma_io_address_rva: {
        this->section_offset = this->section->get_virtual_address() + this->section->get_virtual_size();
        break;
    }

    default: {this->section_offset = 0; break; }
    }


    return *this;
}

enma_io_mode pe_section_io::get_mode() const {
    return this->mode;
}
enma_io_code pe_section_io::get_last_code() const {
    return this->last_code;
}
enma_io_addressing_type pe_section_io::get_addressing_type() const {
    return this->addressing_type;
}
uint32_t pe_section_io::get_section_offset() const {
    return this->section_offset;
}
uint32_t pe_section_io::get_raw_aligment() const {
    return this->raw_aligment;
}
uint32_t pe_section_io::get_virtual_aligment() const {
    return this->virtual_aligment;
}
pe_section* pe_section_io::get_section() {
    return this->section;
}

