#pragma once


class pe_image_full {
    uint32_t code;

    pe_export_directory        exports;
    pe_import_directory        imports;
    pe_resource_directory      resources;
    pe_exceptions_directory    exceptions;
    pe_security_directory      security;
    pe_relocations_directory   relocations;
    pe_debug_directory         debug;
    pe_tls_directory           tls;
    pe_load_config_directory   load_config;
    pe_delay_import_directory  delay_imports;
    pe_bound_import_directory  bound_imports;

    pe_image image;

public:
    pe_image_full();
    pe_image_full(const pe_image& image);
    pe_image_full(const pe_image_full& image);
    explicit pe_image_full(bool _pe32);
    pe_image_full(const uint8_t* raw_image, uint32_t size);
    pe_image_full(const std::string& file_path);

    pe_image_full& operator=(const pe_image_full& image);
    pe_image_full& operator=(const pe_image& image);
public:
    void set_directory_code(pe_directory_code code);

    void set_exports(pe_export_directory& export_directory);
    void set_imports(pe_import_directory& import_directory);
    void set_resources(pe_resource_directory& resources_directory);
    void set_exceptions(pe_exceptions_directory& exceptions_directory);
    void set_security(pe_security_directory& security_directory);
    void set_relocations(pe_relocations_directory& relocations_directory);
    void set_debug(pe_debug_directory& debug_directory);
    void set_tls(pe_tls_directory& tls_directory);
    void set_load_config(pe_load_config_directory& load_config_directory);
    void set_delay_imports(pe_delay_import_directory& delay_import_directory);
    void set_bound_imports(pe_bound_import_directory& bound_import_directory);
    void set_image(pe_image& image);

public:
    pe_directory_code get_directory_code();

    pe_export_directory& get_exports();
    pe_import_directory& get_imports();
    pe_resource_directory& get_resources();
    pe_exceptions_directory& get_exceptions();
    pe_security_directory& get_security();
    pe_relocations_directory& get_relocations();
    pe_debug_directory& get_debug();
    pe_tls_directory& get_tls();
    pe_load_config_directory& get_load_config();
    pe_delay_import_directory& get_delay_imports();
    pe_bound_import_directory& get_bound_imports();
    pe_image& get_image();


    const pe_export_directory& get_exports() const;
    const pe_import_directory& get_imports() const;
    const pe_resource_directory& get_resources() const;
    const pe_exceptions_directory& get_exceptions() const;
    const pe_security_directory& get_security() const;
    const pe_relocations_directory& get_relocations() const;
    const pe_debug_directory& get_debug() const;
    const pe_tls_directory& get_tls() const;
    const pe_load_config_directory& get_load_config() const;
    const pe_delay_import_directory& get_delay_imports() const;
    const pe_bound_import_directory& get_bound_imports() const;
    const pe_image& get_image() const;
};


pe_directory_code get_directories_placement(_Inout_ pe_image &image, _Out_ pe_placement& placement,
    _In_opt_ const pe_bound_import_directory* bound_imports = 0);