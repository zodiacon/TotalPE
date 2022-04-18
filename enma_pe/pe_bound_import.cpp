#include "stdafx.h"
#include "pe_bound_import.h"


pe_bound_imported_ref::pe_bound_imported_ref() {
    timestamp = 0;
}

pe_bound_imported_ref::pe_bound_imported_ref(const std::string& ref_name, uint32_t  timestamp):
    ref_name(ref_name), timestamp(timestamp){

}

void pe_bound_imported_ref::set_ref_name(const std::string& ref_name) {
    this->ref_name = ref_name;
}
void pe_bound_imported_ref::set_timestamp(uint32_t  timestamp) {
    this->timestamp = timestamp;
}

std::string pe_bound_imported_ref::get_ref_name() const {
    return this->ref_name;
}
uint32_t  pe_bound_imported_ref::get_timestamp() const {
    return this->timestamp;
}


pe_bound_library::pe_bound_library() {
    timestamp = 0;
}

void pe_bound_library::set_library_name(const std::string& library_name) {
    this->library_name = library_name;
}
void pe_bound_library::set_timestamp(uint32_t  timestamp) {
    this->timestamp = timestamp;
}

void pe_bound_library::add_ref(const pe_bound_imported_ref& ref) {
    this->refs.push_back(ref);
}

std::string pe_bound_library::get_library_name() const{
    return this->library_name;
}
uint32_t  pe_bound_library::get_timestamp() const {
    return this->timestamp;
}
size_t pe_bound_library::get_number_of_forwarder_refs() const {
    return this->refs.size();
}
const std::vector<pe_bound_imported_ref>& pe_bound_library::get_refs() const {
    return this->refs;
}
std::vector<pe_bound_imported_ref>& pe_bound_library::get_refs() {
    return this->refs;
}

void pe_bound_import_directory::add_library(const pe_bound_library& lib) {
    libraries.push_back(lib);
}
void pe_bound_import_directory::clear() {
    this->libraries.clear();
}

size_t pe_bound_import_directory::size() const {
    return this->libraries.size();
}

bool pe_bound_import_directory::has_library(const std::string& library_name, uint32_t timestamp) const {

    for (auto& library : this->libraries) {
        if (library.get_library_name() == library_name &&
            library.get_timestamp() == timestamp) {

            return true;
        }
    }

    return false;
}

bool pe_bound_import_directory::has_library(const std::string& library_name) const {

    for (auto& library : this->libraries) {
        if (library.get_library_name() == library_name) {

            return true;
        }
    }

    return false;
}

const std::vector<pe_bound_library>& pe_bound_import_directory::get_libraries() const {
    return libraries;
}


std::vector<pe_bound_library>& pe_bound_import_directory::get_libraries() {
    return libraries;
}


pe_directory_code get_bound_import_directory(const pe_image &image, pe_bound_import_directory& imports) {

    imports.clear();

    uint32_t  virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);

    if (virtual_address) {
        pe_image_io bnd_import_desc_io(image);
        bnd_import_desc_io.set_image_offset(virtual_address);

        if (!bnd_import_desc_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        image_bound_import_descriptor bound_imp_description;

        if (bnd_import_desc_io.read(&bound_imp_description, sizeof(bound_imp_description)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        if (bound_imp_description.time_date_stamp && bound_imp_description.offset_module_name) {

            do {
                pe_bound_library bound_lib;
                bound_lib.set_timestamp(bound_imp_description.time_date_stamp);

                std::string lib_name;

                if (pe_image_io(image).set_image_offset(virtual_address + bound_imp_description.offset_module_name).read_string(
                    lib_name) != enma_io_success) {

                    return pe_directory_code::pe_directory_code_currupted;
                }

                bound_lib.set_library_name(lib_name);
                
                for (size_t ref_idx = 0; ref_idx < bound_imp_description.number_of_module_forwarder_refs; ref_idx++) {
                    image_bound_forwarded_ref ref_description;

                    if (bnd_import_desc_io.read(&ref_description, sizeof(ref_description)) != enma_io_success) {
                        return pe_directory_code::pe_directory_code_currupted;
                    }

                    std::string ref_name;

                    if (pe_image_io(image).set_image_offset(virtual_address + ref_description.offset_module_name).read_string(
                        ref_name) != enma_io_success) {

                        return pe_directory_code::pe_directory_code_currupted;
                    }

                    bound_lib.add_ref(pe_bound_imported_ref(ref_name, ref_description.time_date_stamp));
                }

                imports.add_library(bound_lib);

                if (bnd_import_desc_io.read(&bound_imp_description, sizeof(bound_imp_description)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }
            } while (bound_imp_description.time_date_stamp && bound_imp_description.offset_module_name);
        }

        return pe_directory_code::pe_directory_code_success;
    }


    return pe_directory_code::pe_directory_code_not_present;
}

bool build_bound_import_directory(pe_image &image, pe_section& section,
    pe_bound_import_directory& imports) {

    if (imports.size()) { 
        pe_section_io bnd_import_desc_io(section, image,enma_io_mode_allow_expand);
        pe_section_io bnd_import_names_io(section, image, enma_io_mode_allow_expand);

        bnd_import_desc_io.align_up(0x10).seek_to_end();

        size_t  descs_size = 0;
        size_t  names_size = 0;

        for (auto& bound_desc : imports.get_libraries()) {
            descs_size += sizeof(image_bound_import_descriptor);
            names_size += bound_desc.get_library_name().length() + 1;

            for (auto& bound_ref : bound_desc.get_refs()) {
                descs_size += sizeof(image_bound_forwarded_ref);
                names_size += ALIGN_UP( (bound_ref.get_ref_name().length() + 1),0x2);
            }
        }
        descs_size += sizeof(image_bound_import_descriptor);
        descs_size = ALIGN_UP(descs_size, 0x10);

        uint32_t virtual_address = bnd_import_desc_io.get_section_offset();
        bnd_import_names_io.set_section_offset( uint32_t(bnd_import_desc_io.get_section_offset() + descs_size));

        
        for (auto& bound_desc : imports.get_libraries()) {
            image_bound_import_descriptor bound_lib;
            bound_lib.time_date_stamp = bound_desc.get_timestamp();
            bound_lib.number_of_module_forwarder_refs = (uint16_t)bound_desc.get_number_of_forwarder_refs();
            bound_lib.offset_module_name = (uint16_t)(bnd_import_names_io.get_section_offset() - virtual_address);

            if (bnd_import_desc_io.write(&bound_lib,sizeof(bound_lib)) != enma_io_success) {
                return false;
            }

            if (bnd_import_names_io.write(
                (void*)bound_desc.get_library_name().c_str(), uint32_t(bound_desc.get_library_name().length() + 1)) != enma_io_success) {

                return false;
            }
            bnd_import_names_io.align_up(0x2);

            for (auto& bound_ref : bound_desc.get_refs()) {
                image_bound_forwarded_ref ref_lib;
                ref_lib.time_date_stamp = bound_ref.get_timestamp();
                ref_lib.offset_module_name = (uint16_t)(bnd_import_names_io.get_section_offset() - virtual_address);

                if (bnd_import_desc_io.write(&ref_lib, sizeof(ref_lib)) != enma_io_success) {
                    return false;
                }

                if (bnd_import_names_io.write(
                    (void*)bound_ref.get_ref_name().c_str(), uint32_t(bound_ref.get_ref_name().length() + 1)) != enma_io_success) {

                    return false;
                }
                bnd_import_names_io.align_up(0x2);
            }
        }

        return true;
    }
    else {
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT, 0);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT, 0);
        return true;
    }
}


pe_directory_code get_placement_bound_import_directory(const pe_image &image, pe_placement& placement) {

    uint32_t  virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);

    if (virtual_address) {
        pe_image_io bnd_import_desc_io(image);
        bnd_import_desc_io.set_image_offset(virtual_address);

        if (!bnd_import_desc_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        image_bound_import_descriptor bound_imp_description;

        if (bnd_import_desc_io.read(&bound_imp_description, sizeof(bound_imp_description)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        if (bound_imp_description.time_date_stamp && bound_imp_description.offset_module_name) {

            do {
                pe_bound_library bound_lib;
                bound_lib.set_timestamp(bound_imp_description.time_date_stamp);

                std::string lib_name;

                if (pe_image_io(image).set_image_offset(virtual_address + bound_imp_description.offset_module_name).read_string(
                    lib_name) != enma_io_success) {

                    return pe_directory_code::pe_directory_code_currupted;
                }

                placement[virtual_address + bound_imp_description.offset_module_name] = 
                    pe_placement_entry(ALIGN_UP((lib_name.length() + 1), 0x2), id_pe_placement::id_pe_bound_import_library_name, lib_name);

                for (size_t ref_idx = 0; ref_idx < bound_imp_description.number_of_module_forwarder_refs; ref_idx++) {
                    image_bound_forwarded_ref ref_description;

                    if (bnd_import_desc_io.read(&ref_description, sizeof(ref_description)) != enma_io_success) {
                        return pe_directory_code::pe_directory_code_currupted;
                    }

                    std::string ref_name;

                    if (pe_image_io(image).set_image_offset(virtual_address + ref_description.offset_module_name).read_string(
                        ref_name) != enma_io_success) {

                        return pe_directory_code::pe_directory_code_currupted;
                    }


                    placement[virtual_address + bound_imp_description.offset_module_name] =
                        pe_placement_entry(ALIGN_UP((lib_name.length() + 1), 0x2), id_pe_placement::id_pe_bound_import_library_name, ref_name);
                }

                if (bnd_import_desc_io.read(&bound_imp_description, sizeof(bound_imp_description)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }
            } while (bound_imp_description.time_date_stamp && bound_imp_description.offset_module_name);


            placement[virtual_address] =
                pe_placement_entry((size_t)ALIGN_UP((bnd_import_desc_io.get_image_offset() - virtual_address), 0x10), id_pe_placement::id_pe_bound_import_descriptor, "");
        }

        return pe_directory_code::pe_directory_code_success;
    }


    return pe_directory_code::pe_directory_code_not_present;
}