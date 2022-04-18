#include "stdafx.h"
#include "pe_headers_helper.h"


pe_dos_stub::pe_dos_stub() {}
pe_dos_stub::~pe_dos_stub() {}

void pe_dos_stub::set_stub(const std::vector<uint8_t>& stub) {
    this->dos_stub = stub;
}
std::vector<uint8_t>& pe_dos_stub::get_stub() {
    return this->dos_stub;
}
const std::vector<uint8_t>& pe_dos_stub::get_stub() const {
    return this->dos_stub;
}

pe_rich_item::pe_rich_item() :
    type((e_rich_type)0), compiler_build(0), count(0) {
}
pe_rich_item::pe_rich_item(const pe_rich_item& item) {
    this->operator=(item);
}

pe_rich_item::pe_rich_item(e_rich_type type, uint16_t compiler_build, uint32_t count) :
    type(type), compiler_build(compiler_build), count(count) {

}

pe_rich_item::~pe_rich_item() { }


pe_rich_item& pe_rich_item::operator=(const pe_rich_item& item) {

    this->type = item.type;
    this->compiler_build = item.compiler_build;
    this->count = item.count;

    return *this;
}

void pe_rich_item::set_type(e_rich_type type) {
    this->type = type;
}
void pe_rich_item::set_compiler_build(uint16_t compiler_build) {
    this->compiler_build = compiler_build;
}
void pe_rich_item::set_count(uint32_t count) {
    this->count = count;
}

e_rich_type pe_rich_item::get_type() const {
    return type;
}
uint16_t pe_rich_item::get_compiler_build() const {
    return compiler_build;
}
uint32_t pe_rich_item::get_count() const {
    return count;
}


pe_rich_header::pe_rich_header() {
    rich_offset = 0;
    rich_size = 0;
    rich_xorkey = 0;
    rich_correct_xorkey = 0;
}

pe_rich_header::pe_rich_header(const pe_rich_header& data) {
    operator=(data);
}

pe_rich_header::~pe_rich_header() {

}

pe_rich_header& pe_rich_header::operator=(const pe_rich_header& data) {
    rich_offset = data.rich_offset;
    rich_size = data.rich_size;
    rich_xorkey = data.rich_xorkey;
    rich_correct_xorkey = data.rich_correct_xorkey;
    entries = data.entries;

    return *this;
}

void pe_rich_header::set_rich_offset(uint32_t offset) {
    rich_offset = offset;
}

void pe_rich_header::set_rich_size(uint32_t size) {
    rich_size = size;
}

void pe_rich_header::set_rich_xorkey(uint32_t xorkey) {
    rich_xorkey = xorkey;
}

void pe_rich_header::set_rich_correct_xorkey(uint32_t correct_xorkey) {
    rich_correct_xorkey = correct_xorkey;
}

uint32_t pe_rich_header::get_rich_offset() const {
    return rich_offset;
}

uint32_t pe_rich_header::get_rich_size() const {
    return rich_size;
}

uint32_t pe_rich_header::get_rich_xorkey() const {
    return rich_xorkey;
}

uint32_t pe_rich_header::get_rich_correct_xorkey() const {
    return rich_correct_xorkey;
}

bool     pe_rich_header::is_valid_rich() const {
    return (rich_xorkey == rich_correct_xorkey);
}

bool     pe_rich_header::is_present() const {
    return (rich_offset && rich_size);
}

const std::vector<pe_rich_item>& pe_rich_header::get_entries() const {
    return entries;
}

std::vector<pe_rich_item>& pe_rich_header::get_entries() {
    return entries;
}




struct rich_data_entry {
    uint16_t compiler_build;
    uint16_t type;
    uint32_t count;
};

bool has_image_rich_header(const std::vector<uint8_t>& image_headers, uint32_t * rich_header_offset,
    uint32_t * rich_header_size, uint32_t * rich_xor_key) {

    if (rich_header_offset) { *rich_header_offset = 0; }
    if (rich_header_size) { *rich_header_size = 0; }
    if (rich_xor_key) { *rich_xor_key = 0; }

    if (image_headers.size() < sizeof(image_dos_header)) { return false; }

    image_dos_header* dos_header = (image_dos_header*)image_headers.data();

    if (image_headers.size() < dos_header->e_lfanew) { return false; }

    if (dos_header->e_lfanew < 8) {
        return false;
    }

    for (size_t rich_end = sizeof(image_dos_header); rich_end < (dos_header->e_lfanew - 8); rich_end += 4) {//check for rich data

        if (*(uint32_t*)(&image_headers.data()[rich_end]) == 0x68636952) { //'Rich'
            uint32_t xor_key = *(uint32_t*)(&image_headers.data()[rich_end + 4]);

            for (size_t rich_start = sizeof(image_dos_header);
                rich_start < (dos_header->e_lfanew - 8); rich_start += 4) {//get rich offset

                if ((*(uint32_t*)(&image_headers.data()[rich_start]) ^ xor_key) == 0x536E6144) { //'DanS'

                    if (rich_header_offset) {
                        *rich_header_offset = uint32_t(rich_start);
                    }
                    if (rich_header_size) {
                        *rich_header_size = uint32_t(rich_end - rich_start);
                    }
                    if (rich_xor_key) {
                        *rich_xor_key = xor_key;
                    }
                    return true;
                }
            }
        }
    }

    return false;
}


bool get_image_dos_stub(const std::vector<uint8_t>& image_headers, pe_dos_stub& dos_stub) {

    dos_stub.get_stub().clear();


    if (image_headers.size() < sizeof(image_dos_header)) { return false; }

    image_dos_header* dos_header = (image_dos_header*)image_headers.data();

    if (image_headers.size() < dos_header->e_lfanew) { return false; }

    uint32_t dos_stub_size = 0;
    uint32_t rich_offset = 0;

    if (has_image_rich_header(image_headers, &rich_offset)) {
        dos_stub_size = rich_offset;
    }
    else {
        dos_stub_size = dos_header->e_lfanew;
    }

    if (dos_stub_size > sizeof(image_dos_header)) {
        std::vector<uint8_t> dos_stub_holder;
        dos_stub_holder.resize(dos_stub_size - sizeof(image_dos_header));
        memcpy(dos_stub_holder.data(), image_headers.data() + sizeof(image_dos_header), dos_stub_size - sizeof(image_dos_header));
        dos_stub.set_stub(dos_stub_holder);
        return true;
    }

    return false;
}



bool get_image_rich_header(const std::vector<uint8_t>& image_headers, pe_rich_header& rich_header) {

    rich_header.set_rich_offset(0);
    rich_header.set_rich_size(0);
    rich_header.set_rich_xorkey(0);
    rich_header.set_rich_correct_xorkey(0);
    rich_header.get_entries().clear();

    uint32_t rich_offset = 0;
    uint32_t rich_size = 0;
    uint32_t rich_xor_key = 0;
    uint32_t rich_correct_xor_key = 0;

    if (has_image_rich_header(image_headers, &rich_offset, &rich_size, &rich_xor_key)) {
        checksum_rich(image_headers, &rich_correct_xor_key);

        rich_header.set_rich_offset(rich_offset);
        rich_header.set_rich_size(rich_size);
        rich_header.set_rich_xorkey(rich_xor_key);
        rich_header.set_rich_correct_xorkey(rich_correct_xor_key);


        rich_data_entry* rich_entries = (rich_data_entry*)(&image_headers.data()[rich_offset]);

        for (size_t item_idx = 2; item_idx < (rich_size / sizeof(rich_data_entry)); item_idx++) {
            rich_data_entry rich_entry = rich_entries[item_idx];
            *(uint32_t*)(&rich_entry) ^= rich_xor_key;
            *(uint32_t*)((uint8_t*)&rich_entry + 4) ^= rich_xor_key;

            rich_header.get_entries().push_back(
                pe_rich_item((e_rich_type)rich_entry.type, rich_entry.compiler_build, rich_entry.count)
            );
        }

        return true;
    }

    return false;
}

#define GET_RICH_HASH(x,i) (((x) << (i)) | ((x) >> (32 - (i))))
bool checksum_rich(const std::vector<uint8_t>& image_headers, uint32_t * correct_rich_xor_key) {

    if (correct_rich_xor_key) { *correct_rich_xor_key = 0; }
    uint32_t rich_offset = 0;
    uint32_t rich_size = 0;
    uint32_t rich_xor_key = 0;

    if (has_image_rich_header(image_headers, &rich_offset, &rich_size, &rich_xor_key)) {
        rich_data_entry* rich_entries = (rich_data_entry*)(&image_headers.data()[rich_offset]);

        uint32_t calc_hash = rich_offset;

        for (uint32_t i = 0; i < rich_offset; i++) { //dos header + stub
            if (i >= 0x3C && i < 0x40) { continue; }//skip e_lfanew

            calc_hash += GET_RICH_HASH(uint32_t(image_headers.data()[i]), i);
        }

        for (size_t entry_idx = 1; entry_idx < (rich_size / sizeof(rich_data_entry)); entry_idx++) {
            rich_data_entry rich_entry = rich_entries[entry_idx];
            *(uint32_t*)(&rich_entry) ^= rich_xor_key;
            *(uint32_t*)((uint8_t*)&rich_entry + 4) ^= rich_xor_key;

            calc_hash += GET_RICH_HASH(*(uint32_t*)(&rich_entry), *(uint32_t*)((uint8_t*)&rich_entry + 4));
        }

        if (correct_rich_xor_key) {
            *correct_rich_xor_key = calc_hash;
        }

        if (rich_xor_key == calc_hash) {
            return true;
        }
    }
    return false;
}