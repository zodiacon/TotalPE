#pragma once

#include "pe_section.h"


class pe_image{
    pe_image_status image_status;

    std::vector<uint8_t> headers_data;
    pe_rich_header rich_header;

    uint16_t machine;
    uint32_t timestamp;
    uint32_t pointer_to_symbol_table;
    uint32_t number_of_symbols;
    uint32_t size_of_optional_header;
    uint16_t characteristics;

    uint16_t magic;
    uint8_t  major_linker;
    uint8_t  minor_linker;
    uint32_t size_of_code;
    uint32_t size_of_init_data;
    uint32_t size_of_uninit_data;
    uint32_t entry_point;
    uint32_t base_of_code;
    uint32_t base_of_data;
    uint64_t image_base;
    uint32_t section_align;
    uint32_t file_align;
    uint16_t os_ver_major;
    uint16_t os_ver_minor;
    uint16_t image_ver_major;
    uint16_t image_ver_minor;
    uint16_t subsystem_ver_major;
    uint16_t subsystem_ver_minor;
    uint32_t win32_version_value;
    uint32_t image_size;
    uint32_t headers_size;
    uint32_t checksum;
    uint16_t sub_system;
    uint16_t characteristics_dll;
    uint64_t stack_reserve_size;
    uint64_t stack_commit_size;
    uint64_t heap_reserve_size;
    uint64_t heap_commit_size;
    uint32_t loader_flags;
    image_data_directory directories[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];

    std::vector<pe_section*> sections;

    std::vector<uint8_t> overlay_data;

    void init_from_file(const uint8_t * image, size_t size);
public:
    pe_image();
    pe_image(const pe_image& image);
    explicit pe_image(bool _pe32, bool init_dos_thunk = true);
    pe_image(const uint8_t* raw_image,uint32_t size);
    pe_image(const std::string& file_path);
    pe_image(const std::wstring& file_path);
    ~pe_image();

    pe_image& operator=(const pe_image& image);
    

    bool is_x32_image() const;
    bool is_dll_image() const;
    bool is_dotnet_image() const;
public:// data/sections helpers
    pe_section& add_section();
    pe_section& add_section(const pe_section& section);
    pe_section& add_section(const image_section_header& header);
    pe_section& add_section(const image_section_header& header, const std::vector<uint8_t>& data);

    std::vector<pe_section*>& get_sections();
    const std::vector<pe_section*>& get_sections() const;

    size_t get_sections_number() const;
    pe_section* get_section_by_rva(uint32_t rva) const;/*0 - failed, else - success*/
    pe_section* get_section_by_va(uint64_t va) const;
    pe_section* get_section_by_raw(uint32_t raw) const;
    pe_section* get_section_by_idx(uint32_t idx) const;
    pe_section* get_last_section() const;
    
    pe_section* get_section_top_raw() const;
    pe_section* get_section_top_rva() const;

    uint32_t va_to_rva(uint64_t va) const;
    uint32_t va_to_raw(uint64_t va) const;
    uint64_t rva_to_va(uint32_t rva) const;
    uint32_t rva_to_raw(uint32_t rva) const;
    uint64_t raw_to_va(uint32_t raw) const;
    uint32_t raw_to_rva(uint32_t raw) const;

public:

    pe_image& set_image_status(pe_image_status status);
    pe_image& set_rich_header(const pe_rich_header& header);
    pe_image& set_headers_data(const std::vector<uint8_t>& headers_data);
    pe_image& set_dos_header(const image_dos_header& header);
    pe_image& set_machine(uint16_t machine);
    pe_image& set_timestamp(uint32_t timestamp);
    pe_image& set_pointer_to_symbol_table(uint32_t pointer_to_symbol_table);
    pe_image& set_number_of_symbols(uint32_t number_of_symbols);
    pe_image& set_size_of_optional_header(uint32_t size_of_optional_header);
    pe_image& set_characteristics(uint16_t characteristics);
    pe_image& set_magic(uint16_t magic);
    pe_image& set_major_linker(uint8_t major_linker);
    pe_image& set_minor_linker(uint8_t minor_linker);
    pe_image& set_size_of_code(uint32_t size_of_code);
    pe_image& set_size_of_init_data(uint32_t size_of_init_data);
    pe_image& set_size_of_uninit_data(uint32_t size_of_uninit_data);
    pe_image& set_entry_point(uint32_t    entry_point);
    pe_image& set_base_of_code(uint32_t base_of_code);
    pe_image& set_base_of_data(uint32_t base_of_data);
    pe_image& set_image_base(uint64_t image_base);
    pe_image& set_section_align(uint32_t section_align);
    pe_image& set_file_align(uint32_t file_align);
    pe_image& set_os_ver_major(uint16_t os_ver_major);
    pe_image& set_os_ver_minor(uint16_t os_ver_minor);
    pe_image& set_image_ver_major(uint16_t image_ver_major);
    pe_image& set_image_ver_minor(uint16_t image_ver_minor);
    pe_image& set_subsystem_ver_major(uint16_t subsystem_ver_major);
    pe_image& set_subsystem_ver_minor(uint16_t subsystem_ver_minor);
    pe_image& set_win32_version_value(uint32_t win32_version_value);
    pe_image& set_image_size(uint32_t image_size);
    pe_image& set_headers_size(uint32_t headers_size);
    pe_image& set_checksum(uint32_t checksum);
    pe_image& set_sub_system(uint16_t sub_system);
    pe_image& set_characteristics_dll(uint16_t characteristics_dll);
    pe_image& set_stack_reserve_size(uint64_t stack_reserve_size);
    pe_image& set_stack_commit_size(uint64_t stack_commit_size);
    pe_image& set_heap_reserve_size(uint64_t heap_reserve_size);
    pe_image& set_heap_commit_size(uint64_t heap_commit_size);
    pe_image& set_loader_flags(uint32_t loader_flags);
    pe_image& set_overlay_data(std::vector<uint8_t>& data);
    //directories
    pe_image& set_directory_virtual_address(uint32_t directory_idx,uint32_t virtual_address);
    pe_image& set_directory_virtual_size(uint32_t directory_idx, uint32_t virtual_size);

    
public://getter
    pe_image_status get_image_status() const;

    pe_rich_header &get_rich_header();
    const pe_rich_header &get_rich_header() const;

    std::vector<uint8_t>& get_headers_data();
    const std::vector<uint8_t>& get_headers_data() const;
    void  get_dos_header(image_dos_header &header) const;


    uint16_t get_machine() const;
    uint32_t get_timestamp() const;
    uint32_t get_pointer_to_symbol_table() const;
    uint32_t get_number_of_symbols() const;
    uint32_t get_size_of_optional_header() const;
    uint16_t get_characteristics() const;
    uint16_t get_magic() const;
    uint8_t  get_major_linker() const;
    uint8_t  get_minor_linker() const;
    uint32_t get_size_of_code() const;
    uint32_t get_size_of_init_data() const;
    uint32_t get_size_of_uninit_data() const;
    uint32_t get_entry_point() const;
    uint32_t get_base_of_code() const;
    uint32_t get_base_of_data() const;
    uint64_t get_image_base() const;
    uint32_t get_section_align() const;
    uint32_t get_file_align() const;
    uint16_t get_os_ver_major() const;
    uint16_t get_os_ver_minor() const;
    uint16_t get_image_ver_major() const;
    uint16_t get_image_ver_minor() const;
    uint16_t get_subsystem_ver_major() const;
    uint16_t get_subsystem_ver_minor() const;
    uint32_t get_win32_version_value() const;
    uint32_t get_image_size() const;
    uint32_t get_headers_size() const;
    uint32_t get_checksum() const;
    uint16_t get_sub_system() const;
    uint16_t get_characteristics_dll() const;
    uint64_t get_stack_reserve_size() const;
    uint64_t get_stack_commit_size() const;
    uint64_t get_heap_reserve_size() const;
    uint64_t get_heap_commit_size() const;
    uint32_t get_loader_flags() const;

    std::vector<uint8_t>& get_overlay_data();
    const std::vector<uint8_t>& get_overlay_data() const;
    //directories
    uint32_t get_directory_virtual_address(uint32_t directory_idx) const;
    uint32_t get_directory_virtual_size(uint32_t directory_idx) const;
    bool     has_directory(uint32_t directory_idx) const;

    
public://util
    void clear_image();
};

pe_image_status load_virtual_pe_image(_In_ const uint8_t* hmodule, _Out_ pe_image& image, _In_ bool check_signs = true,
    _In_ bool (*readmem)(void* dst, const uint8_t* src, size_t size) = ([](void* dst, const uint8_t* src, size_t size) -> bool {
        
        try {
            memcpy(dst, src, size);
        }
        catch (std::exception&) {
            return false;
        }

        return true;
    }));