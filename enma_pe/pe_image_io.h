#pragma once


#ifndef ENMA_IO_DEFINES 
#define ENMA_IO_DEFINES

enum enma_io_mode {
    enma_io_mode_default,
    enma_io_mode_allow_expand,
};

enum enma_io_addressing_type {
    enma_io_address_raw,
    enma_io_address_rva,
    enma_io_address_va,
};

enum enma_io_code {
    enma_io_success,
    enma_io_incomplete, //part of read\write
    enma_io_data_not_present,

    enma_io_type_not_supported,
};

#endif


#include "pe_section_io.h"

class pe_image_io{
    pe_image* image;
    uint32_t image_offset;

    enma_io_code last_code;
    enma_io_mode mode;
    enma_io_addressing_type addressing_type;

public:
    pe_image_io(
        pe_image& image,
        enma_io_mode mode = enma_io_mode_default,
        enma_io_addressing_type type = enma_io_address_rva
    );
    pe_image_io(
        const pe_image& image,
        enma_io_addressing_type type = enma_io_address_rva
    );

    pe_image_io(const pe_image_io& image_io);
    
    ~pe_image_io();

    pe_image_io& operator=(const pe_image_io& image_io);
public:
    template<typename T>
    pe_image_io& operator >> (const T& data) {//read from image

        read((void*)&data, sizeof(T));

        return *this;
    }
    template<typename T>
    pe_image_io& operator << (const T& data) {//write to image

        write((void*)&data, sizeof(T));

        return *this;
    }




    enma_io_code read(void * data, size_t size);
    enma_io_code write(const void * data, size_t size);

    enma_io_code read(std::vector<uint8_t>& buffer, size_t size);
    enma_io_code write(std::vector<uint8_t>& buffer);

    enma_io_code memory_set(size_t size,uint8_t data);

    enma_io_code read_string(std::string& _string);
    enma_io_code read_wstring(std::wstring& _wstring);

    enma_io_code internal_read(size_t data_offset,
        void * buffer, size_t size,
        size_t& readed_size, size_t& down_oversize, size_t& up_oversize
    );
    enma_io_code internal_write(size_t data_offset,
        const void * buffer, size_t size,
        size_t& wrote_size, size_t& down_oversize, size_t& up_oversize
    );

    bool view_image( //-> return like in view_data
        size_t required_offset, size_t required_size, size_t& real_offset,
        size_t& available_size, size_t& down_oversize, size_t& up_oversize);
public:
    pe_image_io&   set_mode(enma_io_mode mode);
    pe_image_io&   set_addressing_type(enma_io_addressing_type type);
    pe_image_io&   set_image_offset(uint32_t offset);

    pe_image_io& seek_to_start();
    pe_image_io& seek_to_end();
public:

    enma_io_mode            get_mode() const;
    enma_io_code            get_last_code() const;
    enma_io_addressing_type get_addressing_type() const;
    uint32_t                get_image_offset() const;

    bool                    is_executable_rva(uint32_t rva) const;
    bool                    is_writeable_rva(uint32_t rva) const;
    bool                    is_readable_rva(uint32_t rva) const;

    bool                    is_present_rva(uint32_t rva);
    bool                    is_present_raw(uint32_t raw);

    pe_image*               get_image();
};
