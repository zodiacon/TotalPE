#include "stdafx.h"
#include "pe_image_full.h"


pe_image_full::pe_image_full()
    :code(pe_directory_code_currupted) {}

pe_image_full::pe_image_full(const pe_image& image) {
    this->operator=(image);
}
pe_image_full::pe_image_full(const pe_image_full& image) {
    this->operator=(image);
}
pe_image_full::pe_image_full(bool _pe32) {
    this->code = pe_directory_code_success;
    this->image = pe_image(_pe32);
}

pe_image_full::pe_image_full(const uint8_t* raw_image, uint32_t size) {
    this->image = pe_image(raw_image, size);

    this->code = 0;
    this->code |= get_export_directory(this->image, this->exports);
    this->code |= get_bound_import_directory(this->image, this->bound_imports);
    this->code |= get_import_directory(this->image, this->imports, this->bound_imports);
    this->code |= get_delay_import_directory(this->image, this->delay_imports, this->bound_imports);
    this->code |= get_resources_directory(this->image, this->resources);
    this->code |= get_exception_directory(this->image, this->exceptions);
    this->code |= get_security_directory(this->image, this->security);
    this->code |= get_relocation_directory(this->image, this->relocations);
    this->code |= get_debug_directory(this->image, this->debug);
    this->code |= get_tls_directory(this->image, this->tls);
    this->code |= get_load_config_directory(this->image, this->load_config);

    this->code &= ~(pe_directory_code_not_present);
}

pe_image_full::pe_image_full(const std::string& file_path) {
    this->image = pe_image(file_path);

    this->code = 0;
    this->code |= get_export_directory(this->image, this->exports);
    this->code |= get_bound_import_directory(this->image, this->bound_imports);
    this->code |= get_import_directory(this->image, this->imports, this->bound_imports);
    this->code |= get_delay_import_directory(this->image, this->delay_imports, this->bound_imports);
    this->code |= get_resources_directory(this->image, this->resources);
    this->code |= get_exception_directory(this->image, this->exceptions);
    this->code |= get_security_directory(this->image, this->security);
    this->code |= get_relocation_directory(this->image, this->relocations);
    this->code |= get_debug_directory(this->image, this->debug);
    this->code |= get_tls_directory(this->image, this->tls);
    this->code |= get_load_config_directory(this->image, this->load_config);

    this->code &= ~(pe_directory_code_not_present);
}

pe_image_full& pe_image_full::operator=(const pe_image_full& image) {

    this->code = image.code;
    this->exports = image.exports;
    this->imports = image.imports;
    this->resources = image.resources;
    this->exceptions = image.exceptions;
    this->security = image.security;
    this->relocations = image.relocations;
    this->debug = image.debug;
    this->tls = image.tls;
    this->load_config = image.load_config;
    this->delay_imports = image.delay_imports;
    this->bound_imports = image.bound_imports;
    this->image = image.image;

    this->code &= ~(pe_directory_code_not_present);

    return *this;
}

pe_image_full& pe_image_full::operator=(const pe_image& image) {

    this->image = image;

    this->code = 0;
    this->code |= get_export_directory(this->image, this->exports);
    this->code |= get_bound_import_directory(this->image, this->bound_imports);
    this->code |= get_import_directory(this->image, this->imports, this->bound_imports);
    this->code |= get_delay_import_directory(this->image, this->delay_imports, this->bound_imports);
    this->code |= get_resources_directory(this->image, this->resources);
    this->code |= get_exception_directory(this->image, this->exceptions);
    this->code |= get_security_directory(this->image, this->security);
    this->code |= get_relocation_directory(this->image, this->relocations);
    this->code |= get_debug_directory(this->image, this->debug);
    this->code |= get_tls_directory(this->image, this->tls);
    this->code |= get_load_config_directory(this->image, this->load_config);

    this->code &= ~(pe_directory_code_not_present);

    return *this;
}


void pe_image_full::set_directory_code(pe_directory_code code) {
    this->code = code;
}

void pe_image_full::set_exports(pe_export_directory& export_directory) {
    this->exports = export_directory;
}

void pe_image_full::set_imports(pe_import_directory& import_directory) {
    this->imports = import_directory;
}

void pe_image_full::set_resources(pe_resource_directory& resources_directory) {
    this->resources = resources_directory;
}

void pe_image_full::set_exceptions(pe_exceptions_directory& exceptions_directory) {
    this->exceptions = exceptions_directory;
}

void pe_image_full::set_security(pe_security_directory& security_directory) {
    this->security = security_directory;
}

void pe_image_full::set_relocations(pe_relocations_directory& relocations_directory) {
    this->relocations = relocations_directory;
}

void pe_image_full::set_debug(pe_debug_directory& debug_directory) {
    this->debug = debug_directory;
}

void pe_image_full::set_tls(pe_tls_directory& tls_directory) {
    this->tls = tls_directory;
}

void pe_image_full::set_load_config(pe_load_config_directory& load_config_directory) {
    this->load_config = load_config_directory;
}

void pe_image_full::set_delay_imports(pe_delay_import_directory& delay_import_directory) {
    this->delay_imports = delay_import_directory;
}

void pe_image_full::set_bound_imports(pe_bound_import_directory& bound_import_directory) {
    this->bound_imports = bound_import_directory;
}

void pe_image_full::set_image(pe_image& image) {
    this->image = image;
}

pe_directory_code pe_image_full::get_directory_code() {
    return (pe_directory_code)this->code;
}

pe_export_directory& pe_image_full::get_exports() {
    return this->exports;
}

pe_import_directory& pe_image_full::get_imports() {
    return this->imports;
}

pe_resource_directory& pe_image_full::get_resources() {
    return this->resources;
}

pe_exceptions_directory& pe_image_full::get_exceptions() {
    return this->exceptions;
}

pe_security_directory& pe_image_full::get_security() {
    return this->security;
}

pe_relocations_directory& pe_image_full::get_relocations() {
    return this->relocations;
}

pe_debug_directory& pe_image_full::get_debug() {
    return this->debug;
}

pe_tls_directory& pe_image_full::get_tls() {
    return this->tls;
}

pe_load_config_directory& pe_image_full::get_load_config() {
    return this->load_config;
}

pe_delay_import_directory& pe_image_full::get_delay_imports() {
    return this->delay_imports;
}

pe_bound_import_directory& pe_image_full::get_bound_imports() {
    return this->bound_imports;
}

pe_image& pe_image_full::get_image() {
    return this->image;
}


const pe_export_directory& pe_image_full::get_exports() const {
    return this->exports;
}

const pe_import_directory& pe_image_full::get_imports() const {
    return this->imports;
}

const pe_resource_directory& pe_image_full::get_resources() const {
    return this->resources;
}

const pe_exceptions_directory& pe_image_full::get_exceptions() const {
    return this->exceptions;
}

const pe_security_directory& pe_image_full::get_security() const {
    return this->security;
}

const pe_relocations_directory& pe_image_full::get_relocations() const {
    return this->relocations;
}

const pe_debug_directory& pe_image_full::get_debug() const {
    return this->debug;
}

const pe_tls_directory& pe_image_full::get_tls() const {
    return this->tls;
}

const pe_load_config_directory& pe_image_full::get_load_config() const {
    return this->load_config;
}

const pe_delay_import_directory& pe_image_full::get_delay_imports() const {
    return this->delay_imports;
}

const pe_bound_import_directory& pe_image_full::get_bound_imports() const {
    return this->bound_imports;
}

const pe_image& pe_image_full::get_image() const {
    return this->image;
}

pe_directory_code get_directories_placement(pe_image &image, pe_placement& placement,
    const pe_bound_import_directory* bound_imports) {

    placement.clear();
    get_placement_export_directory(image, placement);
    get_placement_import_directory(image, placement, bound_imports ? *bound_imports : pe_bound_import_directory());
    get_placement_resources_directory(image, placement);
    get_placement_exceptions_directory(image, placement);
    get_placement_security_directory(image, placement);
    get_placement_relocation_directory(image, placement);
    get_placement_debug_directory(image, placement);
    get_placement_tls_directory(image, placement);
    get_placement_load_config_directory(image, placement);
    get_placement_bound_import_directory(image, placement);
    get_placement_delay_import_directory(image, placement, bound_imports ? *bound_imports : pe_bound_import_directory());

    return pe_directory_code::pe_directory_code_success;
}