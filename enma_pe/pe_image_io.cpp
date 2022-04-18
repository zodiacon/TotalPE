#include "stdafx.h"
#include "pe_image_io.h"

bool view_data(
    size_t  required_offset, size_t required_size, size_t& real_offset,
    size_t& available_size, size_t& down_oversize, size_t& up_oversize,
    size_t present_offset, size_t present_size);

pe_image_io::pe_image_io(
    pe_image& image,
    enma_io_mode mode,
    enma_io_addressing_type type
):image(&image), image_offset(0), last_code(enma_io_success), mode(mode), addressing_type(type){}


pe_image_io::pe_image_io(
    const pe_image& image,
    enma_io_addressing_type type
): image((pe_image*)&image), image_offset(0), last_code(enma_io_success), mode(enma_io_mode_default), addressing_type(type){}

pe_image_io::pe_image_io(const pe_image_io& image_io) {
    operator=(image_io);
}

pe_image_io::~pe_image_io() {

}

pe_image_io& pe_image_io::operator=(const pe_image_io& image_io) {
    this->image         = image_io.image;
    this->image_offset  = image_io.image_offset;
    this->last_code     = image_io.last_code;
    this->mode          = image_io.mode;
    this->addressing_type = image_io.addressing_type;

    return *this;
}





bool pe_image_io::view_image( //-> return like in view_data
    size_t required_offset, size_t required_size, size_t& real_offset,
    size_t& available_size, size_t& down_oversize, size_t& up_oversize) {


    if (image->get_sections_number()) {
        switch (addressing_type) {
        case enma_io_addressing_type::enma_io_address_raw: {

            size_t top_raw = image->get_headers_data().size();
            
            for (auto& section : image->get_sections()) {
                size_t current_section_raw = section->get_pointer_to_raw() + section->get_size_of_raw_data();
                
                if (current_section_raw > top_raw) {
                    top_raw = current_section_raw;
                }
            }

            return view_data(
                required_offset, required_size,
                real_offset, available_size, down_oversize, up_oversize,
                0, ALIGN_UP(
                    top_raw,
                    image->get_file_align())
                + uint32_t(image->get_overlay_data().size()));
        }

        case enma_io_addressing_type::enma_io_address_rva: {

            size_t top_rva = image->get_headers_data().size();

            for (auto& section : image->get_sections()) {
                size_t current_section_rva = section->get_virtual_address() + section->get_virtual_size();

                if (current_section_rva > top_rva) {
                    top_rva = current_section_rva;
                }
            }

            return view_data(
                required_offset, required_size,
                real_offset, available_size, down_oversize, up_oversize,
                0, ALIGN_UP(
                    top_rva,
                    image->get_section_align())
            );
        }

        default: {return false; }
        }
    }

    return false;
}


enma_io_code pe_image_io::internal_read(size_t data_offset,
    void * buffer, size_t size,
    size_t& readed_size, size_t& down_oversize, size_t& up_oversize
) {

    if (addressing_type == enma_io_addressing_type::enma_io_address_va) {

        return enma_io_code::enma_io_type_not_supported;
    }

    size_t real_offset = 0;

    bool b_view = view_image(data_offset, size,
        real_offset,
        readed_size, down_oversize, up_oversize);


    if (b_view && readed_size) {
        size_t total_readed_size    = 0;
        size_t total_up_oversize    = 0;

        size_t available_headers_size = image->get_headers_data().size();
        size_t view_headers_size = addressing_type == enma_io_addressing_type::enma_io_address_raw ?
            ALIGN_UP(available_headers_size, image->get_file_align()) : ALIGN_UP(available_headers_size, image->get_section_align());


        if (data_offset < view_headers_size) {
            
            size_t header_readed_size = 0;
            size_t header_down_oversize = 0;
            size_t header_up_oversize = 0;

            
            b_view = view_data(
                data_offset, size,
                real_offset, header_readed_size, header_down_oversize, header_up_oversize,
                0, view_headers_size);

            if (b_view) {
                
                if (available_headers_size) {

                    if (available_headers_size >= (header_readed_size + real_offset) ) {
                        memcpy(&((uint8_t*)buffer)[header_down_oversize], &image->get_headers_data().data()[real_offset], header_readed_size);
                    }
                    else {
                        if (available_headers_size > real_offset) {

                            // available_headers_size - real_offset = is available data in vector for read
                            // header_readed_size = must be readed, includes virtual aligment

                            memcpy(&((uint8_t*)buffer)[header_down_oversize], &image->get_headers_data().data()[real_offset], 
                                available_headers_size - real_offset
                            );

                            memset(&((uint8_t*)buffer)[header_down_oversize + (available_headers_size - real_offset)], 0,
                                header_readed_size - ((available_headers_size - real_offset)));
                        }
                        else {
                            memset(&((uint8_t*)buffer)[header_down_oversize], 0, header_readed_size);
                        }                  
                    }
                }
                else {
                    memset(&((uint8_t*)buffer)[header_down_oversize], 0, header_readed_size);
                }

                total_readed_size += header_readed_size;
                total_up_oversize = header_up_oversize;
            }
        }

        for (auto &section : image->get_sections()) {

            if (total_readed_size == readed_size) { break; }

            size_t sec_readed_size   = 0;
            size_t sec_down_oversize = 0;
            size_t sec_up_oversize   = 0;

            pe_section_io(*section, *image, mode, addressing_type).internal_read(
                data_offset, buffer, size, sec_readed_size, sec_down_oversize, sec_up_oversize
            );

            total_readed_size += sec_readed_size;
            total_up_oversize = sec_up_oversize;

            if (total_readed_size == readed_size) { break; }
        }

        if (addressing_type == enma_io_addressing_type::enma_io_address_raw &&
            total_up_oversize && image->get_overlay_data().size()) { //take up size from overlay

            size_t top_section_raw = 0;

            size_t overlay_readed_size = 0;
            size_t overlay_down_oversize = 0;
            size_t overlay_up_oversize = 0;

            if (image->get_sections_number()) {
                top_section_raw = 
                    ALIGN_UP(image->get_last_section()->get_pointer_to_raw() + image->get_last_section()->get_size_of_raw_data(), image->get_file_align());
            }

            b_view = view_data(
                data_offset, size,
                real_offset, overlay_readed_size, overlay_down_oversize, overlay_up_oversize,
                top_section_raw, image->get_overlay_data().size());
            
            if (b_view) {
                memcpy(&((uint8_t*)buffer)[down_oversize], &image->get_overlay_data().data()[real_offset],overlay_readed_size);


                total_readed_size += overlay_readed_size;
                total_up_oversize = overlay_up_oversize;
            }
        }

        readed_size = total_readed_size;
        up_oversize = total_up_oversize;

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

enma_io_code pe_image_io::internal_write(size_t data_offset,
    const void * buffer, size_t size,
    size_t& wrote_size, size_t& down_oversize, size_t& up_oversize
) {

    if (addressing_type == enma_io_addressing_type::enma_io_address_va) {

        return enma_io_code::enma_io_type_not_supported;
    }

    size_t real_offset = 0;

    bool b_view = view_image(data_offset, size,
        real_offset,
        wrote_size, down_oversize, up_oversize);


    if (b_view &&
        (wrote_size || (up_oversize && mode == enma_io_mode::enma_io_mode_allow_expand))) {

        size_t total_wroted_size   = 0;
        size_t total_up_oversize   = 0;

        for (size_t section_idx = 0; section_idx < image->get_sections().size(); section_idx++) {

            size_t sec_wroted_size   = 0;
            size_t sec_down_oversize = 0;
            size_t sec_up_oversize   = 0;

            pe_section_io section_io(*image->get_sections()[section_idx], *image,
                (section_idx == (image->get_sections().size()-1) &&
                    mode == enma_io_mode::enma_io_mode_allow_expand) ? enma_io_mode_allow_expand : enma_io_mode_default,addressing_type);


            section_io.internal_write(data_offset, buffer, size,
                sec_wroted_size, sec_down_oversize, sec_up_oversize);


            total_wroted_size += sec_wroted_size;
            total_up_oversize  = sec_up_oversize;

            if (!total_up_oversize) { break; }
        }
        up_oversize = total_up_oversize;
        wrote_size  = total_wroted_size;

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

enma_io_code pe_image_io::read(void * data, size_t size) {

    size_t readed_size;
    size_t down_oversize;
    size_t up_oversize;


    enma_io_code code = internal_read(image_offset, data, size,
        readed_size, down_oversize, up_oversize);

    image_offset += (uint32_t)readed_size;

    return code;
}

enma_io_code pe_image_io::write(const void * data, size_t size) {

    size_t wrote_size;
    size_t down_oversize;
    size_t up_oversize;

    enma_io_code code = internal_write(image_offset, data, size,
        wrote_size, down_oversize, up_oversize);

    image_offset += (uint32_t)wrote_size;

    return code;
}

enma_io_code pe_image_io::read(std::vector<uint8_t>& buffer, size_t size) {

    if (buffer.size() < size) { buffer.resize(size); }

    return read(buffer.data(), buffer.size());
}

enma_io_code pe_image_io::write(std::vector<uint8_t>& buffer) {

    return write(buffer.data(), buffer.size());
}

enma_io_code pe_image_io::memory_set(size_t size, uint8_t data) {

    std::vector<uint8_t> set_buffer;
    set_buffer.resize(size);
    memset(set_buffer.data(), data, size);

    return write(set_buffer);
}

enma_io_code pe_image_io::read_string(std::string& _string) {

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

enma_io_code pe_image_io::read_wstring(std::wstring& _wstring) {

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


pe_image_io& pe_image_io::set_mode(enma_io_mode mode) {

    this->mode = mode;

    return *this;
}
pe_image_io& pe_image_io::set_addressing_type(enma_io_addressing_type type) {

    this->addressing_type = type;

    return *this;
}
pe_image_io& pe_image_io::set_image_offset(uint32_t offset) {

    this->image_offset = offset;

    return *this;
}

pe_image_io& pe_image_io::seek_to_start() {

    this->image_offset = 0;

    return *this;
}

pe_image_io& pe_image_io::seek_to_end() {

    if (image->get_sections_number()) {
        switch (addressing_type) {
        case enma_io_addressing_type::enma_io_address_raw: {

            size_t top_raw = image->get_headers_data().size();

            for (auto& section : image->get_sections()) {
                size_t current_section_raw = section->get_pointer_to_raw() + section->get_size_of_raw_data();

                if (current_section_raw > top_raw) {
                    top_raw = current_section_raw;
                }
            }
            this->image_offset = (uint32_t)ALIGN_UP(
                top_raw,
                image->get_file_align()
            );
            break;
        }

        case enma_io_addressing_type::enma_io_address_rva: {

            size_t top_rva = image->get_headers_data().size();

            for (auto& section : image->get_sections()) {
                size_t current_section_rva = section->get_virtual_address() + section->get_virtual_size();

                if (current_section_rva > top_rva) {
                    top_rva = current_section_rva;
                }
            }

            this->image_offset = (uint32_t)ALIGN_UP(
                top_rva,
                image->get_section_align()
            );
            break;
        }

        default: {this->image_offset = 0; break; }
        }
    }
    else {
        this->image_offset = 0;
    }

    return *this;
}

enma_io_mode  pe_image_io::get_mode() const {
    return this->mode;
}
enma_io_code  pe_image_io::get_last_code() const {
    return this->last_code;
}
enma_io_addressing_type pe_image_io::get_addressing_type() const {
    return this->addressing_type;
}
uint32_t  pe_image_io::get_image_offset() const {
    return this->image_offset;
}

bool  pe_image_io::is_executable_rva(uint32_t rva) const {

    pe_section * rva_section = image->get_section_by_rva(rva);

    if (rva_section) {
        return rva_section->is_executable();
    }

    return false;
}
bool  pe_image_io::is_writeable_rva(uint32_t rva) const {
    pe_section * rva_section = image->get_section_by_rva(rva);

    if (rva_section) {
        return rva_section->is_writeable();
    }

    return false;
}
bool  pe_image_io::is_readable_rva(uint32_t rva) const {
    pe_section * rva_section = image->get_section_by_rva(rva);

    if (rva_section) {
        return rva_section->is_readable();
    }

    return false;
}

bool  pe_image_io::is_present_rva(uint32_t rva) {

    uint8_t test_byte;

    uint32_t current_offset = this->image_offset;
    enma_io_addressing_type addressing_type = this->addressing_type;

    this->addressing_type = enma_io_address_rva;
    this->image_offset = rva;
    
    enma_io_code code = this->read((void*)&test_byte, 1);

    this->image_offset = current_offset;
    this->addressing_type = addressing_type;

    return code == enma_io_success;
}


bool pe_image_io::is_present_raw(uint32_t raw) {

    uint8_t test_byte;

    uint32_t current_offset = this->image_offset;
    enma_io_addressing_type addressing_type = this->addressing_type;

    this->addressing_type = enma_io_address_raw;
    this->image_offset = raw;

    enma_io_code code = this->read((void*)&test_byte, 1);

    this->image_offset = current_offset;
    this->addressing_type = addressing_type;

    return code == enma_io_success;
}

pe_image*  pe_image_io::get_image() {
    return this->image;
}



bool view_data(
    size_t  required_offset, size_t required_size, size_t& real_offset,
    size_t& available_size, size_t& down_oversize, size_t& up_oversize,
    size_t present_offset, size_t present_size) {


    //         ...............
    //  .............................
    //  |    | |             |      |
    //  v    v |             |      |
    // (down_oversize)       |      |
    //         |             |      |
    //         v             v      |
    //         (available_size)     |
    //                       |      |
    //                       v      v
    //                       (up_oversize)

    real_offset = 0;
    available_size = 0;
    down_oversize = 0;
    up_oversize = 0;

    if (required_offset < present_offset) {
        down_oversize = (present_offset - required_offset);

        if (down_oversize >= required_size) {

            return false; //not in bounds
        }
        else {
            available_size = (required_size - down_oversize);

            if (available_size > present_size) {
                up_oversize = (available_size - present_size);
                available_size = present_size;

                return true;//partially in bounds
            }

            return true;//partially in bounds
        }
    }
    else {//if(required_offset >= present_offset)

        if (required_offset <= (present_offset + present_size)) {
            real_offset = (required_offset - present_offset);

            if (required_size + required_offset > (present_offset + present_size)) {
                up_oversize = (required_size + required_offset) - (present_offset + present_size);
                available_size = required_size - up_oversize;

                return true; //partially in bounds
            }
            else {
                available_size = required_size;

                return true; //full in bounds
            }
        }
        else {
            real_offset = (required_offset - present_offset);
            up_oversize = (required_size + required_offset) - (present_offset + present_size);
            available_size = 0;

            return true; //trough full adding size 
        }
    }

    return true;
}



