#include "stdafx.h"
#include "pe_image.h"


unsigned char th_dos_stub[64] = {
    0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD, 0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21, 0x54, 0x68,
    0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F,
    0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6E, 0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
    0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

pe_image::pe_image() {
    clear_image();
}
pe_image::pe_image(const pe_image& image) {
    this->operator=(image);
}

pe_image::pe_image(bool _pe32, bool init_dos_thunk) {
    clear_image();

    this->headers_data.resize(sizeof(image_dos_header) + (init_dos_thunk ? sizeof(th_dos_stub) : 0));
    memset(this->headers_data.data(), 0, this->headers_data.size());

    if (init_dos_thunk) {
        memcpy(&this->headers_data[sizeof(image_dos_header)], th_dos_stub, sizeof(th_dos_stub));
    }

    image_dos_header * dos_header = (image_dos_header*)this->headers_data.data();
    dos_header->e_magic = IMAGE_DOS_SIGNATURE;
    dos_header->e_cblp = 0x90;
    dos_header->e_cp = 0x03;
    dos_header->e_cparhdr = 0x04;
    dos_header->e_maxalloc = 0xFFFF;
    dos_header->e_sp = 0xB8;
    dos_header->e_lfanew = sizeof(image_dos_header) + (init_dos_thunk ? sizeof(th_dos_stub) : 0);


    if (_pe32) {
        machine = IMAGE_FILE_MACHINE_I386;
        characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_32BIT_MACHINE;
        magic = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
        image_base = 0x400000;
    }
    else {
        machine = IMAGE_FILE_MACHINE_AMD64;
        characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE;
        magic = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
        image_base = 0x0000000140000000;
        
    }

    {
        image_size = 0x1000;
        headers_size = 0x200;
        sub_system = IMAGE_SUBSYSTEM_WINDOWS_GUI;
        characteristics_dll =
            IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE | 
            IMAGE_DLLCHARACTERISTICS_NX_COMPAT | 
            IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE;
    }

    {
        section_align = 0x1000;
        file_align = 0x200;
    }

    {
        os_ver_major = 5;
        os_ver_minor = 1;

    }

    {
        subsystem_ver_major = 5;
        subsystem_ver_minor = 1;
    }

    stack_reserve_size = 0x00100000;
    stack_commit_size  = 0x1000;
    heap_reserve_size  = 0x00100000;
    heap_commit_size   = 0x1000;

    image_status = pe_image_status::pe_image_status_ok;
}

pe_image::pe_image(const uint8_t* raw_image, uint32_t size) {
    clear_image();
    init_from_file(raw_image, size);
}

pe_image::pe_image(const std::string& file_path) {
    clear_image();

    FILE* hfile = 0;
    fopen_s(&hfile, file_path.c_str(), "rb");

    if (hfile != nullptr) {
        fseek(hfile, 0, SEEK_END);
        size_t file_size = ftell(hfile);
        fseek(hfile, 0, SEEK_SET);

        uint8_t * file_buffer = new uint8_t[file_size];


        if (fread(file_buffer, file_size, 1, hfile)) {

            init_from_file(file_buffer, file_size);
        }
        else {
            image_status = pe_image_status_bad_format;
        }

        delete[] file_buffer;

        fclose(hfile);
    }
    else {
        image_status = pe_image_status_unknown;
    }
}

pe_image::pe_image(const std::wstring& file_path) {
    clear_image();

    FILE* hfile = 0;
    _wfopen_s(&hfile, file_path.c_str(), L"rb");


    if (hfile != nullptr) {
        fseek(hfile, 0, SEEK_END);
        size_t file_size = ftell(hfile);
        fseek(hfile, 0, SEEK_SET);

        uint8_t* file_buffer = new uint8_t[file_size];


        if (fread(file_buffer, file_size, 1, hfile)) {

            init_from_file(file_buffer, file_size);
        }
        else {
            image_status = pe_image_status_bad_format;
        }

        delete[] file_buffer;

        fclose(hfile);
    }
    else {
        image_status = pe_image_status_unknown;
    }
}

pe_image::~pe_image() {
    clear_image();
}

pe_image& pe_image::operator=(const pe_image& image) {
    clear_image();

    headers_data = image.headers_data;
    rich_header  = image.rich_header;
    image_status        = image.image_status;
    machine                = image.machine;
    timestamp           = image.timestamp;
    pointer_to_symbol_table = image.pointer_to_symbol_table;
    number_of_symbols   = image.number_of_symbols;
    size_of_optional_header = image.size_of_optional_header;
    characteristics     = image.characteristics;
    magic               = image.magic;
    major_linker        = image.major_linker;
    minor_linker        = image.minor_linker;
    size_of_code        = image.size_of_code;
    size_of_init_data   = image.size_of_init_data;
    size_of_uninit_data = image.size_of_uninit_data;
    entry_point         = image.entry_point;
    base_of_code        = image.base_of_code;
    base_of_data        = image.base_of_data;
    image_base          = image.image_base;
    section_align       = image.section_align;
    file_align          = image.file_align;
    os_ver_major        = image.os_ver_major;
    os_ver_minor        = image.os_ver_minor;
    image_ver_major     = image.image_ver_major;
    image_ver_minor     = image.image_ver_minor;
    subsystem_ver_major = image.subsystem_ver_major;
    subsystem_ver_minor = image.subsystem_ver_minor;
    win32_version_value = image.win32_version_value;
    image_size          = image.image_size;
    headers_size        = image.headers_size;
    checksum            = image.checksum;
    sub_system          = image.sub_system;
    characteristics_dll = image.characteristics_dll;
    stack_reserve_size  = image.stack_reserve_size;
    stack_commit_size   = image.stack_commit_size;
    heap_reserve_size   = image.heap_reserve_size;
    heap_commit_size    = image.heap_commit_size;
    loader_flags        = image.loader_flags;

    for (size_t i = 0; i < 16; i++) {
        directories[i].virtual_address = image.directories[i].virtual_address;
        directories[i].size           = image.directories[i].size;
    }

    for (size_t i = 0; i < image.get_sections_number(); i++) {
        this->add_section(*image.sections[i]);
    }


    overlay_data = image.overlay_data;

    return *this;
}

template <typename image_format>
bool init_nt_header(pe_image& image,void * nt_header,uint32_t& sections_offset,uint32_t& number_of_sections) {

    typename image_format::image_nt_headers * header = (typename image_format::image_nt_headers *)nt_header;

    if (header->optional_header.magic == image_format::image_magic) {

        image.set_machine(header->file_header.machine)
            .set_timestamp(header->file_header.time_date_stamp)
            .set_pointer_to_symbol_table(header->file_header.pointer_to_symbol_table)
            .set_number_of_symbols(header->file_header.number_of_symbols)
            .set_characteristics(header->file_header.characteristics)
            .set_magic(header->optional_header.magic)
            .set_major_linker(header->optional_header.major_linker_version)
            .set_minor_linker(header->optional_header.minor_linker_version)
            .set_size_of_code(header->optional_header.size_of_code)
            .set_size_of_init_data(header->optional_header.size_of_initialized_data)
            .set_size_of_uninit_data(header->optional_header.size_of_uninitialized_data)
            .set_entry_point(header->optional_header.address_of_entry_point)
            .set_base_of_code(header->optional_header.base_of_code)
            .set_image_base(header->optional_header.image_base)
            .set_section_align(header->optional_header.section_alignment)
            .set_file_align(header->optional_header.file_alignment)
            .set_os_ver_major(header->optional_header.major_operating_system_version)
            .set_os_ver_minor(header->optional_header.minor_operating_system_version)
            .set_image_ver_major(header->optional_header.major_image_version)
            .set_image_ver_minor(header->optional_header.minor_image_version)
            .set_subsystem_ver_major(header->optional_header.major_subsystem_version)
            .set_subsystem_ver_minor(header->optional_header.minor_subsystem_version)
            .set_win32_version_value(header->optional_header.win32_version_value)
            .set_image_size(header->optional_header.size_of_image)
            .set_headers_size(header->optional_header.size_of_headers)
            .set_checksum(header->optional_header.checksum)
            .set_sub_system(header->optional_header.subsystem)
            .set_characteristics_dll(header->optional_header.dll_characteristics)
            .set_stack_reserve_size(header->optional_header.size_of_stack_reserve)
            .set_stack_commit_size(header->optional_header.size_of_stack_commit)
            .set_heap_reserve_size(header->optional_header.size_of_heap_reserve)
            .set_heap_commit_size(header->optional_header.size_of_heap_commit)
            .set_loader_flags(header->optional_header.loader_flags);

        for (uint32_t i = 0; i < 16; i++) {
            image.set_directory_virtual_address(i, header->optional_header.data_directory[i].virtual_address)
                .set_directory_virtual_size(i, header->optional_header.data_directory[i].size);
        }

        sections_offset += header->file_header.size_of_optional_header;
        number_of_sections = header->file_header.number_of_sections;
        return true;
    }

    return false;
}


void pe_image::init_from_file(const uint8_t * image, size_t size) {

    if (size < sizeof(image_dos_header)) {this->image_status = pe_image_status_bad_format; return;};

    image_dos_header dos_header;

    memcpy(&dos_header, image, sizeof(image_dos_header));

    if (dos_header.e_magic == IMAGE_DOS_SIGNATURE || //check MZ sign
        dos_header.e_magic == 'MZ') {  //check ZM sign

        if (size <= ((size_t)dos_header.e_lfanew + INT32_SIZE + sizeof(image_file_header))) {
            this->image_status = pe_image_status_bad_format; return; 
        };

        if (*(uint32_t*)(&image[dos_header.e_lfanew]) == IMAGE_NT_SIGNATURE) { //check PE00 sign
            uint32_t section_offset = dos_header.e_lfanew + INT32_SIZE + (uint32_t)sizeof(image_file_header);
            uint32_t number_of_sections = 0;
            
            if (size < section_offset + sizeof(image_nt_headers32)) { this->image_status = pe_image_status_bad_format; return; };

            if (init_nt_header<pe_image_32>(*this, (void*)&image[dos_header.e_lfanew], section_offset, number_of_sections)) {
                set_base_of_data(pimage_nt_headers32(&image[dos_header.e_lfanew])->optional_header.base_of_data);
            }
            else {
                if (size < section_offset + sizeof(image_nt_headers64)) { this->image_status = pe_image_status_bad_format; return; };

                if (!init_nt_header<pe_image_64>(*this, (void*)&image[dos_header.e_lfanew], section_offset, number_of_sections)) {
                    this->image_status = pe_image_status_bad_format;
                    return;
                }

                set_base_of_data(0);
            }

            headers_data.resize(min(this->headers_size, size));
            memset(headers_data.data(), 0, headers_data.size());

            memcpy(headers_data.data(), image, min(this->headers_size, size));

            get_image_rich_header(this->headers_data, this->rich_header);

            {
                size_t image_top_size = 0;
                for (size_t i = 0; i < number_of_sections; i++) {
                    image_section_header* section_image = (image_section_header*)(&image[section_offset]);

                    if (size < section_offset + sizeof(image_section_header) ||
                        size < (size_t)ALIGN_DOWN(section_image->pointer_to_raw_data + section_image->size_of_raw_data, this->get_file_align())
                        ) {

                        this->image_status = pe_image_status_bad_format; return;
                    };

                    std::vector<uint8_t> section_data;

                    if (section_image->pointer_to_raw_data < size) {
                        
                        section_data.resize(
                            (size_t)ALIGN_DOWN(min(section_image->size_of_raw_data, (uint32_t)(size - section_image->pointer_to_raw_data)), this->get_file_align())
                        );
                    }
                    else {
                        this->image_status = pe_image_status_bad_format; return;
                    }


                    memcpy(section_data.data(), &image[section_image->pointer_to_raw_data], section_data.size());

                    add_section(pe_section(*section_image, section_data));
                    section_offset += (uint16_t)sizeof(image_section_header);

                    size_t current_size = ALIGN_DOWN( (uint32_t)(section_image->pointer_to_raw_data + section_data.size()), this->get_file_align());

                    if (image_top_size < current_size) {
                        image_top_size = current_size;
                    }
                }



                if (image_top_size < size) {
                    overlay_data.resize(size - image_top_size);
                    memcpy(overlay_data.data(), &image[image_top_size], size - image_top_size);
                }
            }

            this->image_status = pe_image_status_ok;
            return;
        }

    }

    this->image_status = pe_image_status_bad_format;
}


bool    pe_image::is_x32_image() const{
    if (this->magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return true;
    }
    return false;
}
bool pe_image::is_dll_image()  const {
    if (this->characteristics&IMAGE_FILE_DLL) {
        return true;
    }
    return false;
}
bool pe_image::is_dotnet_image() const {
    return get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR) != 0;
}

void pe_image::clear_image() {
    image_status = pe_image_status_unknown;

    machine = 0;
    timestamp = 0;
    pointer_to_symbol_table = 0;
    number_of_symbols = 0;
    size_of_optional_header = 0;
    characteristics = 0;
    magic = 0;
    major_linker = 0;
    minor_linker = 0;
    size_of_code = 0;
    size_of_init_data = 0;
    size_of_uninit_data = 0;
    entry_point = 0;
    base_of_code = 0;
    base_of_data = 0;
    image_base = 0;
    section_align = 0;
    file_align = 0;
    os_ver_major = 0;
    os_ver_minor = 0;
    image_ver_major = 0;
    image_ver_minor = 0;
    subsystem_ver_major = 0;
    subsystem_ver_minor = 0;
    win32_version_value = 0;
    image_size = 0;
    headers_size = 0;
    checksum = 0;
    sub_system = 0;
    characteristics_dll = 0;
    stack_reserve_size = 0;
    stack_commit_size = 0;
    heap_reserve_size = 0;
    heap_commit_size = 0;
    loader_flags = 0;

    for (size_t i = 0; i < 16; i++) {
        directories[i].virtual_address = 0;
        directories[i].size            = 0;
    }

    while (sections.size()) { delete sections[sections.size() - 1]; sections.pop_back(); }

    rich_header.get_entries().clear();
    headers_data.clear();
}




pe_section& pe_image::add_section() {

    uint32_t virtual_address = section_align;
    uint32_t raw_pointer = file_align;

    if (sections.size()) {
        virtual_address = ALIGN_UP(sections[sections.size() - 1]->get_virtual_address() +
            sections[sections.size() - 1]->get_virtual_size(), section_align);

        raw_pointer = ALIGN_UP(sections[sections.size() - 1]->get_pointer_to_raw() +
            sections[sections.size() - 1]->get_size_of_raw_data(), file_align);

        if (!sections[sections.size() - 1]->get_virtual_size())        { virtual_address += section_align; }
    }

    sections.push_back(new pe_section());
    pe_section& section =  *sections[sections.size() - 1];

    section.set_virtual_address(virtual_address);
    section.set_pointer_to_raw(raw_pointer);

    return section;
}
pe_section& pe_image::add_section(const pe_section& section) {
    return (add_section() = section);
}
pe_section& pe_image::add_section(const image_section_header& header) {
    sections.push_back(new pe_section(header));
    return *sections[sections.size() - 1];
}
pe_section& pe_image::add_section(const image_section_header& header, const std::vector<uint8_t>& data) {
    sections.push_back(new pe_section(header, data));
    return *sections[sections.size() - 1];
}

std::vector<pe_section*>& pe_image::get_sections() {
    return this->sections;
}

const std::vector<pe_section*>& pe_image::get_sections() const {
    return this->sections;
}

size_t pe_image::get_sections_number() const {
    return this->sections.size();
}

pe_section* pe_image::get_section_by_rva(uint32_t rva) const {
    for (size_t section_idx = 0; section_idx < sections.size(); section_idx++) {
        if (
            sections[section_idx]->get_virtual_address() <= rva &&
            sections[section_idx]->get_virtual_address() + sections[section_idx]->get_virtual_size() > rva
            ) {
            return sections[section_idx];
        }
    }
    return 0;
}
pe_section* pe_image::get_section_by_va(uint64_t va) const {
    return get_section_by_rva(va_to_rva(va));
}
pe_section* pe_image::get_section_by_raw(uint32_t raw) const {
    for (size_t section_idx = 0; section_idx < sections.size(); section_idx++) {
        if (
            sections[section_idx]->get_pointer_to_raw() <= raw &&
            sections[section_idx]->get_pointer_to_raw() + sections[section_idx]->get_size_of_raw_data() > raw
            ) {
            return sections[section_idx];
        }
    }
    return 0;
}
pe_section* pe_image::get_section_by_idx(uint32_t idx) const {
    if (this->sections.size() > idx) {
        return this->sections[idx];
    }

    return 0;
}
pe_section* pe_image::get_last_section() const {
    if (this->sections.size()) {
        return this->sections[this->sections.size() - 1];
    }
    
    return 0;
}

pe_section* pe_image::get_section_top_raw() const {

    pe_section* current_top = 0;

    for (auto& section : this->sections) {

        if (!current_top ||
            current_top->get_pointer_to_raw() < section->get_pointer_to_raw()) {

            current_top = section;
        }
    }

    return current_top;
}

pe_section* pe_image::get_section_top_rva() const {

    pe_section* current_top = 0;

    for (auto& section : this->sections) {

        if (!current_top ||
            current_top->get_virtual_address() < section->get_virtual_address()) {

            current_top = section;
        }
    }

    return current_top;
}


uint32_t pe_image::va_to_rva(uint64_t va) const {
    if (va) {
        return uint32_t(va - this->image_base);
    }
    else {
        return 0;
    }
}
uint32_t pe_image::va_to_raw(uint64_t va) const {

    pe_section * section = get_section_by_va(va);
    if (section)
        return (va_to_rva(va) - section->get_virtual_address()) + section->get_pointer_to_raw();
    else
        return 0;
}
uint64_t pe_image::rva_to_va(uint32_t rva) const {
    return (rva + this->image_base);
}
uint32_t pe_image::rva_to_raw(uint32_t rva) const {

    pe_section * section = get_section_by_rva(rva);
    if (section)
        return (rva - section->get_virtual_address()) + section->get_pointer_to_raw();
    else
        return 0;

}
uint64_t pe_image::raw_to_va(uint32_t raw) const {
    return rva_to_va(raw_to_rva(raw));
}
uint32_t pe_image::raw_to_rva(uint32_t raw) const {
    pe_section * section = get_section_by_raw(raw);
    if (section)
        return  (raw - section->get_pointer_to_raw()) + section->get_virtual_address();
    else
        return 0;
}

pe_image& pe_image::set_rich_header(const pe_rich_header& header) {
    this->rich_header = header;

    return *this;
}
pe_image& pe_image::set_image_status(pe_image_status status) {
    this->image_status = status;

    return *this;
}
pe_image& pe_image::set_headers_data(const std::vector<uint8_t>& headers_data) {
    this->headers_data = headers_data;

    return *this;
}
pe_image& pe_image::set_dos_header(const image_dos_header& header) {

    if (headers_data.size() < sizeof(image_dos_header)) {
        headers_data.resize(sizeof(image_dos_header));
    }

    memcpy(headers_data.data(), &header, sizeof(image_dos_header));

    return *this;
}
pe_image& pe_image::set_machine(uint16_t machine) {
    this->machine = machine;

    return *this;
}
pe_image& pe_image::set_timestamp(uint32_t timestamp) {
    this->timestamp = timestamp;

    return *this;
}
pe_image& pe_image::set_pointer_to_symbol_table(uint32_t pointer_to_symbol_table) {
    this->pointer_to_symbol_table = pointer_to_symbol_table;

    return *this;
}
pe_image& pe_image::set_number_of_symbols(uint32_t number_of_symbols) {
    this->number_of_symbols = number_of_symbols;

    return *this;
}
pe_image& pe_image::set_size_of_optional_header(uint32_t size_of_optional_header) {
    this->size_of_optional_header = size_of_optional_header;

    return *this;
}
pe_image& pe_image::set_characteristics(uint16_t characteristics) {
    this->characteristics = characteristics;

    return *this;
}
pe_image& pe_image::set_magic(uint16_t magic) {
    this->magic = magic;

    return *this;
}
pe_image& pe_image::set_major_linker(uint8_t major_linker) {
    this->major_linker = major_linker;

    return *this;
}
pe_image& pe_image::set_minor_linker(uint8_t minor_linker){
    this->minor_linker = minor_linker;

    return *this;
}
pe_image& pe_image::set_size_of_code(uint32_t size_of_code) {
    this->size_of_code = size_of_code;

    return *this;
}
pe_image& pe_image::set_size_of_init_data(uint32_t size_of_init_data) {
    this->size_of_init_data = size_of_init_data;

    return *this;
}
pe_image& pe_image::set_size_of_uninit_data(uint32_t size_of_uninit_data) {
    this->size_of_uninit_data = size_of_uninit_data;

    return *this;
}
pe_image& pe_image::set_entry_point(uint32_t    entry_point) {
    this->entry_point = entry_point;

    return *this;
}
pe_image& pe_image::set_base_of_code(uint32_t base_of_code) {
    this->base_of_code = base_of_code;

    return *this;
}
pe_image& pe_image::set_base_of_data(uint32_t base_of_data) {
    this->base_of_data = base_of_data;

    return *this;
}
pe_image& pe_image::set_image_base(uint64_t image_base) {
    this->image_base = image_base;

    return *this;
}
pe_image& pe_image::set_section_align(uint32_t section_align) {
    this->section_align = section_align;

    return *this;
}
pe_image& pe_image::set_file_align(uint32_t file_align) {
    this->file_align = file_align;

    return *this;
}
pe_image& pe_image::set_os_ver_major(uint16_t os_ver_major) {
    this->os_ver_major = os_ver_major;

    return *this;
}
pe_image& pe_image::set_os_ver_minor(uint16_t os_ver_minor) {
    this->os_ver_minor = os_ver_minor;

    return *this;
}
pe_image& pe_image::set_image_ver_major(uint16_t image_ver_major) {
    this->image_ver_major = image_ver_major;

    return *this;
}
pe_image& pe_image::set_image_ver_minor(uint16_t image_ver_minor) {
    this->image_ver_minor = image_ver_minor;

    return *this;
}
pe_image& pe_image::set_subsystem_ver_major(uint16_t subsystem_ver_major) {
    this->subsystem_ver_major = subsystem_ver_major;

    return *this;
}
pe_image& pe_image::set_subsystem_ver_minor(uint16_t subsystem_ver_minor) {
    this->subsystem_ver_minor = subsystem_ver_minor;

    return *this;
}
pe_image& pe_image::set_win32_version_value(uint32_t win32_version_value) {
    this->win32_version_value = win32_version_value;

    return *this;
}
pe_image& pe_image::set_image_size(uint32_t image_size) {
    this->image_size = image_size;

    return *this;
}
pe_image& pe_image::set_headers_size(uint32_t headers_size) {
    this->headers_size = headers_size;

    return *this;
}
pe_image& pe_image::set_checksum(uint32_t checksum) {
    this->checksum = checksum;

    return *this;
}
pe_image& pe_image::set_sub_system(uint16_t sub_system) {
    this->sub_system = sub_system;

    return *this;
}
pe_image& pe_image::set_characteristics_dll(uint16_t characteristics_dll) {
    this->characteristics_dll = characteristics_dll;

    return *this;
}
pe_image& pe_image::set_stack_reserve_size(uint64_t stack_reserve_size) {
    this->stack_reserve_size = stack_reserve_size;

    return *this;
}
pe_image& pe_image::set_stack_commit_size(uint64_t stack_commit_size) {
    this->stack_commit_size = stack_commit_size;

    return *this;
}
pe_image& pe_image::set_heap_reserve_size(uint64_t heap_reserve_size) {
    this->heap_reserve_size = heap_reserve_size;

    return *this;
}
pe_image& pe_image::set_heap_commit_size(uint64_t heap_commit_size) {
    this->heap_commit_size = heap_commit_size;

    return *this;
}
pe_image& pe_image::set_loader_flags(uint32_t loader_flags) {
    this->loader_flags = loader_flags;

    return *this;
}
pe_image& pe_image::set_overlay_data(std::vector<uint8_t>& data) {
    this->overlay_data = data;

    return *this;
}
pe_image& pe_image::set_directory_virtual_address(uint32_t directory_idx, uint32_t virtual_address) {
    if (directory_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
        this->directories[directory_idx].virtual_address = virtual_address;
    }

    return *this;
}
pe_image& pe_image::set_directory_virtual_size(uint32_t directory_idx, uint32_t virtual_size) {
    if (directory_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
        this->directories[directory_idx].size = virtual_size;
    }

    return *this;
}


std::vector<uint8_t>& pe_image::get_headers_data() {
    return headers_data;
}

pe_image_status pe_image::get_image_status() const {
    return image_status;
}
pe_rich_header &pe_image::get_rich_header() {
    return this->rich_header;
}
const pe_rich_header &pe_image::get_rich_header() const {
    return this->rich_header;
}
const std::vector<uint8_t>& pe_image::get_headers_data() const {
    return headers_data;
}
uint16_t    pe_image::get_machine() const {
    return machine;
}
uint32_t    pe_image::get_timestamp() const {
    return timestamp;
}
uint32_t    pe_image::get_pointer_to_symbol_table() const {
    return pointer_to_symbol_table;
}
uint32_t    pe_image::get_number_of_symbols() const {
    return number_of_symbols;
}
uint32_t    pe_image::get_size_of_optional_header() const {
    return size_of_optional_header;
}
uint16_t    pe_image::get_characteristics() const {
    return characteristics;
}
uint16_t    pe_image::get_magic() const {
    return magic;
}
uint8_t     pe_image::get_major_linker() const {
    return major_linker;
}
uint8_t     pe_image::get_minor_linker() const {
    return minor_linker;
}
uint32_t    pe_image::get_size_of_code() const {
    return size_of_code;
}
uint32_t    pe_image::get_size_of_init_data() const {
    return size_of_init_data;
}
uint32_t    pe_image::get_size_of_uninit_data() const {
    return size_of_uninit_data;
}
uint32_t    pe_image::get_entry_point() const {
    return entry_point;
}
uint32_t    pe_image::get_base_of_code() const {
    return base_of_code;
}
uint32_t    pe_image::get_base_of_data() const {
    return base_of_data;
}
uint64_t   pe_image::get_image_base() const {
    return image_base;
}
uint32_t    pe_image::get_section_align() const {
    return section_align;
}
uint32_t    pe_image::get_file_align() const {
    return file_align;
}
uint16_t    pe_image::get_os_ver_major() const {
    return os_ver_major;
}
uint16_t    pe_image::get_os_ver_minor() const {
    return os_ver_minor;
}
uint16_t    pe_image::get_image_ver_major() const {
    return image_ver_major;
}
uint16_t    pe_image::get_image_ver_minor() const {
    return image_ver_minor;
}
uint16_t    pe_image::get_subsystem_ver_major() const {
    return subsystem_ver_major;
}
uint16_t    pe_image::get_subsystem_ver_minor() const {
    return subsystem_ver_minor;
}
uint32_t    pe_image::get_win32_version_value() const {
    return win32_version_value;
}
uint32_t    pe_image::get_image_size() const {
    return image_size;
}
uint32_t    pe_image::get_headers_size() const {
    return headers_size;
}
uint32_t    pe_image::get_checksum() const {
    return checksum;
}
uint16_t    pe_image::get_sub_system() const {
    return sub_system;
}
uint16_t    pe_image::get_characteristics_dll() const {
    return characteristics_dll;
}
uint64_t   pe_image::get_stack_reserve_size() const {
    return stack_reserve_size;
}
uint64_t   pe_image::get_stack_commit_size() const {
    return stack_commit_size;
}
uint64_t   pe_image::get_heap_reserve_size() const {
    return heap_reserve_size;
}
uint64_t   pe_image::get_heap_commit_size() const {
    return heap_commit_size;
}
uint32_t   pe_image::get_loader_flags() const {
    return loader_flags;
}

std::vector<uint8_t>&   pe_image::get_overlay_data() {
    return overlay_data;
}

const std::vector<uint8_t>&   pe_image::get_overlay_data() const {
    return overlay_data;
}

uint32_t    pe_image::get_directory_virtual_address(uint32_t directory_idx) const {
    if (directory_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
        return this->directories[directory_idx].virtual_address;
    }

    return 0;
}
uint32_t    pe_image::get_directory_virtual_size(uint32_t directory_idx) const {
    if (directory_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
        return this->directories[directory_idx].size;
    }

    return 0;
}

bool        pe_image::has_directory(uint32_t directory_idx) const {
    if (directory_idx < IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
        return (this->directories[directory_idx].virtual_address != 0);
    }
    return false;
}

void  pe_image::get_dos_header(image_dos_header &header) const {

    memset(&header, 0, sizeof(image_dos_header));

    if (headers_data.size() < sizeof(image_dos_header)) {
        memcpy(&header, headers_data.data(), headers_data.size());
    }
    else {
        memcpy(&header, headers_data.data(), sizeof(image_dos_header));
    }
}


pe_image_status load_virtual_pe_image(const uint8_t* hmodule, pe_image& image, bool check_signs,
    bool(*readmem)(void* dst, const uint8_t* src, size_t size)) {

    image.clear_image();

    if (!readmem) {
        image.set_image_status(pe_image_status_unknown);

        return image.get_image_status();
    }


    image.set_image_status(pe_image_status_unknown);

    image_dos_header dos_header;

    readmem(&dos_header, hmodule, sizeof(image_dos_header));

    if ( !check_signs || (dos_header.e_magic == IMAGE_DOS_SIGNATURE || //check MZ sign
        dos_header.e_magic == 'MZ')) {  //check ZM sign

        image_nt_headers32 nt32_header;
        readmem(&nt32_header, &hmodule[dos_header.e_lfanew], sizeof(image_nt_headers32));

        if ( !check_signs  || nt32_header.signature == IMAGE_NT_SIGNATURE) { //check PE00 sign

            uint32_t section_offset = dos_header.e_lfanew + INT32_SIZE + (uint32_t)sizeof(image_file_header);
            uint32_t number_of_sections = 0;

            if (init_nt_header<pe_image_32>(image, (void*)& nt32_header, section_offset, number_of_sections)) {
                image.set_base_of_data(nt32_header.optional_header.base_of_data);
            }
            else {

                image_nt_headers64 nt64_header;
                readmem(&nt64_header, &hmodule[dos_header.e_lfanew], sizeof(image_nt_headers64));

                if (!init_nt_header<pe_image_64>(image, (void*)&nt64_header, section_offset, number_of_sections)) {

                    image.set_image_status(pe_image_status_bad_format);

                    return image.get_image_status();
                }

                image.set_base_of_data(0);
            }

            image.get_headers_data().resize(image.get_headers_size());
            memset(image.get_headers_data().data(), 0, image.get_headers_size());
            readmem(image.get_headers_data().data(), hmodule, image.get_headers_size());

            get_image_rich_header(image.get_headers_data(), image.get_rich_header());

            {
                for (size_t i = 0; i < number_of_sections; i++) {

                    image_section_header section_image;
                    readmem(&section_image, &hmodule[section_offset], sizeof(image_section_header));

                    std::vector<uint8_t> section_data;

                    section_data.resize(ALIGN_UP((size_t)section_image.virtual_size, (size_t)image.get_section_align()));

                    readmem(section_data.data(), &hmodule[section_image.virtual_address], section_data.size());

                    image.add_section(pe_section(section_image, section_data));
                    section_offset += (uint16_t)sizeof(image_section_header);
                }
            }

            image.set_image_status(pe_image_status_ok);
        }

    }


    return image.get_image_status();
}