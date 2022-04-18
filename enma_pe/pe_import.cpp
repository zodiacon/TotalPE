#include "stdafx.h"
#include "pe_import.h"


pe_import_function::pe_import_function() 
 :hint(0), ordinal(0), b_import_by_name(0), 
    iat_rva(0), iat_item(0), oft_rva(0), oft_item(0) {
}

pe_import_function::pe_import_function(uint32_t iat_rva, uint32_t oft_rva, const std::string& func_name, uint16_t hint, uint64_t iat_item, uint64_t oft_item):
    iat_rva(iat_rva), oft_rva(oft_rva),  hint(hint), func_name(func_name), ordinal(0), iat_item(iat_item), oft_item(oft_item), b_import_by_name(true){

}
pe_import_function::pe_import_function(uint32_t iat_rva, uint32_t oft_rva, uint16_t ordinal, uint64_t iat_item, uint64_t oft_item):
    iat_rva(iat_rva), oft_rva(oft_rva), hint(0), func_name(""), ordinal(ordinal), iat_item(iat_item), oft_item(oft_item), b_import_by_name(false){
}

pe_import_function& pe_import_function::set_hint(uint16_t hint) {
    this->hint = hint;

    return *this;
}
pe_import_function& pe_import_function::set_ordinal(uint16_t ordinal) {
    this->b_import_by_name = false;
    this->ordinal = ordinal;

    return *this;
}
pe_import_function& pe_import_function::set_func_name(const std::string& func_name) {
    this->b_import_by_name = true;
    this->func_name = func_name;

    return *this;
}
pe_import_function& pe_import_function::set_import_by_name(bool b) {
    this->b_import_by_name = b;

    return *this;
}
pe_import_function& pe_import_function::set_iat_rva(uint32_t rva) {
    this->iat_rva = rva;

    return *this;
}
pe_import_function& pe_import_function::set_iat_item(uint64_t iat_item) {
    this->iat_item = iat_item;

    return *this;
}
pe_import_function& pe_import_function::set_oft_rva(uint32_t rva) {
    this->oft_rva = rva;

    return *this;
}
pe_import_function& pe_import_function::set_oft_item(uint64_t oft_item) {
    this->oft_item = oft_item;

    return *this;
}
uint16_t pe_import_function::get_hint() const {
    return this->hint;
}
uint16_t pe_import_function::get_ordinal() const {
    return this->ordinal;
}
std::string pe_import_function::get_func_name() const {
    return this->func_name;
}
bool  pe_import_function::is_import_by_name() const {
    return this->b_import_by_name;
}
uint32_t pe_import_function::get_iat_rva() const {
    return this->iat_rva;
}
uint64_t pe_import_function::get_iat_item()const {
    return this->iat_item;
}
uint32_t pe_import_function::get_oft_rva() const {
    return this->oft_rva;
}
uint64_t pe_import_function::get_oft_item()const {
    return this->oft_item;
}
pe_import_library::pe_import_library() 
 : timestamp(0), is_bound(0), iat_rva(-1), oft_rva(-1), library_name_rva(0){
}

pe_import_library::pe_import_library(const std::string& library_name) 
  : timestamp(0), is_bound(0), iat_rva(-1), oft_rva(-1), library_name_rva(0), library_name(library_name){
}

pe_import_library& pe_import_library::set_library_name(const std::string& library_name){
    this->library_name = library_name;

    return *this;
}
pe_import_library& pe_import_library::set_timestamp(uint32_t timestamp) {
    this->timestamp = timestamp;

    return *this;
}
pe_import_library& pe_import_library::set_rva_iat(uint32_t rva) {
    this->iat_rva = rva;

    return *this;
}
pe_import_library& pe_import_library::set_rva_oft(uint32_t rva) {
    this->oft_rva = rva;

    return *this;
}
pe_import_library& pe_import_library::set_rva_library_name(uint32_t rva) {
    this->library_name_rva = rva;

    return *this;
}
pe_import_library& pe_import_library::set_bound_library(bool is_bound) {
    this->is_bound = is_bound;

    return *this;
}
pe_import_library& pe_import_library::add_function(const pe_import_function& function) {
    this->functions.push_back(function);

    return *this;
}
void pe_import_library::clear() {
    this->functions.clear();
}

size_t pe_import_library::size() {
    return this->functions.size();
}

std::string pe_import_library::get_library_name() const {
    return this->library_name;
}
uint32_t pe_import_library::get_timestamp() const {
    return this->timestamp;
}
uint32_t pe_import_library::get_rva_iat() const {
    return this->iat_rva;
}
uint32_t pe_import_library::get_rva_oft() const {
    return this->oft_rva;
}
uint32_t pe_import_library::get_rva_library_name() const {
    return this->library_name_rva;
}
bool pe_import_library::is_bound_library() const {
    return this->is_bound;
}
const std::vector<pe_import_function>& pe_import_library::get_functions() const {
    return this->functions;
}

std::vector<pe_import_function>& pe_import_library::get_functions() {
    return this->functions;
}

void pe_import_directory::add_library(const pe_import_library& lib) {
    this->libraries.push_back(lib);
}

void pe_import_directory::clear() {
    this->libraries.clear();
}

size_t pe_import_directory::size() {
    return this->libraries.size();
}

std::vector<pe_import_library>& pe_import_directory::get_libraries() {
    return this->libraries;
}

const std::vector<pe_import_library>& pe_import_directory::get_libraries() const {
    return this->libraries;
}

bool pe_import_directory::get_imported_lib(const std::string& lib_name, pe_import_library * &lib) {

    for (auto& library : libraries) {
        if (library.get_library_name() == lib_name) {

            lib = &library;
            return true;
        }
    }

    return false;
}
bool pe_import_directory::get_imported_func(const std::string& lib_name, const std::string& func_name, pe_import_library * &lib, pe_import_function * &_func) {

    for (auto& library : libraries) {
        if (library.get_library_name() == lib_name) {
            for (auto& func : library.get_functions()) {
                if (func.is_import_by_name() && func.get_func_name() == func_name) {

                    lib = &library;
                    _func = &func;

                    return true;
                }
            }
        }
    }

    return false;
}
bool pe_import_directory::get_imported_func(const std::string& lib_name, uint16_t ordinal, pe_import_library * &lib, pe_import_function * &_func) {

    for (auto& library : libraries) {
        if (library.get_library_name() == lib_name) {
            for (auto& func : library.get_functions()) {
                if (!func.is_import_by_name() && func.get_ordinal() == ordinal) {

                    lib = &library;
                    _func = &func;

                    return true;
                }
            }
        }
    }

    return false;
}

template<typename image_format>
pe_directory_code _get_import_directory(const pe_image &image, pe_import_directory& imports,const pe_bound_import_directory& bound_imports) {
    imports.clear();

    uint32_t virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IMPORT);


    if (virtual_address) {
        pe_image_io import_io(image);

        if (!import_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        import_io.set_image_offset(virtual_address);

        image_import_descriptor import_desc;

        if (import_io.read(&import_desc,sizeof(import_desc)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }

        if (import_desc.first_thunk) {

            do {
                pe_import_library library;

                std::string library_name;
                if (pe_image_io(image).set_image_offset(import_desc.name).read_string(library_name) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                bool is_bound_library = (import_desc.time_date_stamp &&
                    bound_imports.has_library(library_name, import_desc.time_date_stamp));


                library.set_library_name(library_name)
                    .set_rva_library_name(import_desc.name)
                    .set_rva_iat(import_desc.first_thunk)
                    .set_rva_oft(import_desc.original_first_thunk)
                    .set_timestamp(import_desc.time_date_stamp)
                    .set_bound_library(is_bound_library);

                pe_image_io iat_io(image);
                pe_image_io original_iat_io(image);
                iat_io.set_image_offset(import_desc.first_thunk);
                original_iat_io.set_image_offset(import_desc.original_first_thunk);

                bool original_first_thunk_present = import_io.is_present_rva(import_desc.original_first_thunk);

                do {
                    pe_import_function func;

                    if (original_first_thunk_present) {

                        typename image_format::ptr_size original_iat_item = 0;
                        typename image_format::ptr_size iat_item = 0;

                        if (iat_io.read(&iat_item, sizeof(iat_item)) != enma_io_success) {
                            return pe_directory_code::pe_directory_code_currupted;
                        }

                        if (original_iat_io.read(&original_iat_item, sizeof(original_iat_item)) != enma_io_success) {
                            return pe_directory_code::pe_directory_code_currupted;
                        }

                        if (!original_iat_item) {
                            break;
                        }

                        func.set_iat_rva(iat_io.get_image_offset() - (uint32_t)sizeof(typename image_format::ptr_size))
                            .set_oft_rva(original_iat_io.get_image_offset() - (uint32_t)sizeof(typename image_format::ptr_size))
                            .set_iat_item(iat_item)
                            .set_oft_item(original_iat_item);

                        if (original_iat_item&image_format::ordinal_flag) {

                            func.set_import_by_name(false)
                                .set_ordinal(uint16_t(original_iat_item ^ image_format::ordinal_flag));

                        }
                        else {
                            pe_image_io import_func_name_io(image);
                            import_func_name_io.set_image_offset(uint32_t(original_iat_item));

                            uint16_t hint;
                            std::string func_name;

                            if (import_func_name_io.read(&hint, sizeof(hint)) != enma_io_success) {
                                return pe_directory_code::pe_directory_code_currupted;
                            }

                            if (import_func_name_io.read_string(func_name) != enma_io_success) {
                                return pe_directory_code::pe_directory_code_currupted;
                            }

                            func.set_import_by_name(true)
                                .set_hint(hint)
                                .set_func_name(func_name);

                        }
                    }
                    else {

                        typename image_format::ptr_size iat_item = 0;

                        if (iat_io.read(&iat_item, sizeof(iat_item)) != enma_io_success) {
                            return pe_directory_code::pe_directory_code_currupted;
                        }

                        if (!iat_item) {
                            break;
                        }

                        func.set_iat_rva(iat_io.get_image_offset() - (uint32_t)sizeof(typename image_format::ptr_size))
                            .set_oft_rva(-1)
                            .set_iat_item(iat_item)
                            .set_oft_item(0);

                        if (is_bound_library) {

                            func.set_import_by_name(true)
                                .set_func_name("");
                        
                        }
                        else {

                            if (iat_item&image_format::ordinal_flag) {
                                
                                func.set_import_by_name(false)
                                    .set_ordinal(uint16_t(iat_item^ image_format::ordinal_flag));

                            }
                            else {
                                pe_image_io import_func_name_io(image);
                                import_func_name_io.set_image_offset(uint32_t(iat_item));

                                uint16_t hint;
                                std::string func_name;

                                if (import_func_name_io.read(&hint, sizeof(hint)) != enma_io_success) {
                                    return pe_directory_code::pe_directory_code_currupted;
                                }

                                if (import_func_name_io.read_string(func_name) != enma_io_success) {
                                    return pe_directory_code::pe_directory_code_currupted;
                                }

                                func.set_import_by_name(true)
                                    .set_hint(hint)
                                    .set_func_name(func_name);
                            }
                        }
                    }

                    library.add_function(func);

                } while (1);


                imports.add_library(library);

                if (import_io.read(&import_desc, sizeof(import_desc)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

            } while (import_desc.first_thunk);
        }

        return pe_directory_code::pe_directory_code_success;
    }


    return pe_directory_code::pe_directory_code_not_present;
}

template<typename image_format>
bool _build_internal_import_directory_data(pe_image &image, pe_section& section, pe_import_directory& imports,
    uint32_t build_items_ids/*import_table_build_id*/,
    uint32_t iat_rva, uint32_t oft_rva, uint32_t names_rva,
    const pe_bound_import_directory* bound_imports = 0) {

    if (imports.size()) {
        pe_section_io import_io(section, image, enma_io_mode_allow_expand);

        if (iat_rva == -1 || oft_rva == -1 || names_rva == -1) {
            import_io.align_up(0x10).seek_to_end();
        }

        uint32_t names_size = 0;
        uint32_t thunk_size = 0;
        uint32_t original_thunk_size = 0;

        for (auto& library : imports.get_libraries()) {
            if (!library.size()) { continue; }

            uint32_t current_lib_thunk_size = 0;

            if ((build_items_ids & import_table_build_iat) || (build_items_ids & import_table_build_oft)) {

                for (auto& func : library.get_functions()) {

                    if (func.is_import_by_name()) {
                        names_size += uint32_t(ALIGN_UP(func.get_func_name().length() + 1 + INT16_SIZE, 0x2));
                    }
                    current_lib_thunk_size += (uint32_t)sizeof(typename image_format::ptr_size);
                }
                current_lib_thunk_size += (uint32_t)sizeof(typename image_format::ptr_size);

                if (build_items_ids & import_table_build_iat) {
                    thunk_size += current_lib_thunk_size;
                }
                if (build_items_ids & import_table_build_oft) {
                    original_thunk_size += current_lib_thunk_size;
                }
            }

            if (build_items_ids & import_table_build_library_name) {
                names_size += uint32_t(ALIGN_UP(library.get_library_name().length() + 1, 0x2));
            }
        }

        uint32_t import_iat_virtual_address = iat_rva == -1 ? import_io.get_section_offset() : iat_rva;
        uint32_t import_iat_virtual_size    = thunk_size;

        pe_section_io import_thunks_io(section, image, enma_io_mode_allow_expand);
        pe_section_io import_original_thunks_io(section, image, enma_io_mode_allow_expand);
        pe_section_io import_names_io(section, image, enma_io_mode_allow_expand);

        import_thunks_io.set_section_offset(iat_rva == -1 ? import_io.get_section_offset() : iat_rva);
        import_original_thunks_io.set_section_offset(oft_rva == -1 ? (import_io.get_section_offset() + thunk_size) : oft_rva);
        import_names_io.set_section_offset(names_rva == -1 ? (import_io.get_section_offset() + thunk_size + original_thunk_size) : names_rva);      
  
        for (auto& library : imports.get_libraries()) {

            bool is_bound_library = (library.get_timestamp() == UINT32_MAX && bound_imports &&
                bound_imports->has_library(library.get_library_name()));

            if (library.size() && 
                ((build_items_ids & import_table_build_iat) || (build_items_ids & import_table_build_oft))) {

                std::vector<typename image_format::ptr_size> thunk_table;
                std::vector<typename image_format::ptr_size> original_thunk_table;

                uint32_t func_iat_rva = import_thunks_io.get_section_offset();
                uint32_t func_oft_rva = import_original_thunks_io.get_section_offset();

                for (auto& func : library.get_functions()) {
                    typename image_format::ptr_size thunk_item;

                    if (build_items_ids & import_table_build_iat) {
                        func.set_iat_rva(func_iat_rva);
                        func_iat_rva += (uint32_t)sizeof(typename image_format::ptr_size);
                    }

                    if (build_items_ids & import_table_build_oft) {
                        func.set_oft_rva(func_oft_rva);
                        func_oft_rva += (uint32_t)sizeof(typename image_format::ptr_size);
                    }

                    if (!is_bound_library || (build_items_ids & import_table_build_oft)) {

                        if (func.is_import_by_name()) {
                            thunk_item = import_names_io.get_section_offset();

                            uint16_t hint = func.get_hint();

                            if (import_names_io.write(&hint, sizeof(hint)) != enma_io_success) {
                                return false;
                            }

                            if (import_names_io.write(
                                (void*)func.get_func_name().c_str(), uint32_t(func.get_func_name().length() + 1)) != enma_io_success) {

                                return false;
                            }
                            import_names_io.align_up_offset(0x2);
                        }
                        else {
                            thunk_item = func.get_ordinal() | image_format::ordinal_flag;
                        }
                    }

                    if (is_bound_library) {
                        thunk_table.push_back(typename image_format::ptr_size(func.get_iat_item()));
                        original_thunk_table.push_back(thunk_item);

                        if (build_items_ids & import_table_build_iat) {
                            func.set_iat_item(typename image_format::ptr_size(func.get_iat_item()));
                        }

                        if (build_items_ids & import_table_build_oft) {
                            func.set_oft_item(thunk_item);
                        }

                    }
                    else {
                        thunk_table.push_back(thunk_item);
                        original_thunk_table.push_back(thunk_item);

                        if (build_items_ids & import_table_build_iat) {
                            func.set_iat_item(thunk_item);
                        }

                        if (build_items_ids & import_table_build_oft) {
                            func.set_oft_item(thunk_item);
                        }
                    }
                }


                if (build_items_ids & import_table_build_iat) {
                    library.set_rva_iat(import_thunks_io.get_section_offset());
                    typename image_format::ptr_size null_item = 0;

                    for (auto& item : thunk_table) {
                        if (import_thunks_io.write(&item, sizeof(item)) != enma_io_success) {
                            return false;
                        }
                    }
                    
                    if (import_thunks_io.write(&null_item, sizeof(null_item)) != enma_io_success) {
                        return false;
                    }
                }

                if (build_items_ids & import_table_build_oft) {
                    library.set_rva_oft(import_original_thunks_io.get_section_offset());
                    typename image_format::ptr_size null_item = 0;

                    for (auto& item : original_thunk_table) {
                        if (import_original_thunks_io.write(&item, sizeof(item)) != enma_io_success) {
                            return false;
                        }
                    }

                    if (import_original_thunks_io.write(&null_item, sizeof(null_item)) != enma_io_success) {
                        return false;
                    }
                }
            }

            if (build_items_ids & import_table_build_library_name) {
                library.set_rva_library_name(import_names_io.get_section_offset());

                if (import_names_io.write(
                    (void*)library.get_library_name().c_str(), uint32_t(library.get_library_name().length() + 1)) != enma_io_success) {

                    return false;
                }
                import_names_io.align_up_offset(0x2);
            }
        }
       
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IAT, import_iat_virtual_address);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_IAT, import_iat_virtual_size);
    }
    else {
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IAT, 0);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_IAT, 0);
    }

    return true;
}


template<typename image_format>
pe_directory_code _get_placement_import_directory(const pe_image &image, pe_placement& placement,
    const pe_bound_import_directory& bound_imports) {

    uint32_t virtual_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IMPORT);


    if (virtual_address) {
        pe_image_io import_io(image);

        if (!import_io.is_present_rva(virtual_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        import_io.set_image_offset(virtual_address);

        image_import_descriptor import_desc;

        if (import_io.read(&import_desc, sizeof(import_desc)) != enma_io_success) {
            return pe_directory_code::pe_directory_code_currupted;
        }
        uint32_t import_desc_table_size = sizeof(image_import_descriptor);

        if (import_desc.first_thunk) {

            do {

                std::string library_name;
                if (pe_image_io(image).set_image_offset(import_desc.name).read_string(library_name) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

                placement[import_desc.name] =
                    pe_placement_entry(ALIGN_UP(library_name.length() + 1, 0x2), id_pe_placement::id_pe_import_library_name, library_name);

                bool is_bound_library = (import_desc.time_date_stamp == UINT32_MAX &&
                    bound_imports.has_library(library_name));

                pe_image_io iat_io(image);
                pe_image_io original_iat_io(image);
                iat_io.set_image_offset(import_desc.first_thunk);
                original_iat_io.set_image_offset(import_desc.original_first_thunk);

                uint32_t iat_table_size = sizeof(typename image_format::ptr_size);

                bool original_first_thunk_present = import_io.is_present_rva(import_desc.original_first_thunk);


                do {
                    pe_import_function func;

                    if (original_first_thunk_present) {

                        typename image_format::ptr_size original_iat_item = 0;

                        if (original_iat_io.read(&original_iat_item, sizeof(original_iat_item)) != enma_io_success) {
                            return pe_directory_code::pe_directory_code_currupted;
                        }

                        if (!original_iat_item) {
                            break;
                        }

                        if (!(original_iat_item&image_format::ordinal_flag)) {

                            pe_image_io import_func_name_io(image);
                            import_func_name_io.set_image_offset(uint32_t(original_iat_item + sizeof(uint16_t)));

                            std::string func_name;

                            if (import_func_name_io.read_string(func_name) != enma_io_success) {
                                return pe_directory_code::pe_directory_code_currupted;
                            }


                            placement[(uint32_t)original_iat_item] =
                                pe_placement_entry(ALIGN_UP(func_name.length() + 1 + sizeof(uint16_t), 0x2), id_pe_placement::id_pe_import_function_name, func_name);
                        }
                    }
                    else {

                        typename image_format::ptr_size iat_item = 0;

                        if (iat_io.read(&iat_item, sizeof(iat_item)) != enma_io_success) {
                            return pe_directory_code::pe_directory_code_currupted;
                        }

                        if (!iat_item) {
                            break;
                        }

                        if (!is_bound_library && !(iat_item&image_format::ordinal_flag)) {

                            pe_image_io import_func_name_io(image);
                            import_func_name_io.set_image_offset(uint32_t(iat_item + sizeof(uint16_t)));

                            std::string func_name;

                            if (import_func_name_io.read_string(func_name) == enma_io_success) {

                                placement[(uint32_t)(iat_item + sizeof(uint16_t))] =
                                    pe_placement_entry(ALIGN_UP(func_name.length() + 1 + sizeof(uint16_t), 0x2), id_pe_placement::id_pe_import_function_name, func_name);
                            }
                        }
                    }

                    iat_table_size += (uint32_t)sizeof(typename image_format::ptr_size);

                } while (1);


                if (original_first_thunk_present) {
                    placement[import_desc.original_first_thunk] = pe_placement_entry(iat_table_size, id_pe_placement::id_pe_import_original_first_think, "");
                }

                placement[import_desc.first_thunk] = pe_placement_entry(iat_table_size, id_pe_placement::id_pe_import_first_think, "");

                import_desc_table_size += (uint32_t)sizeof(image_import_descriptor);


                if (import_io.read(&import_desc, sizeof(import_desc)) != enma_io_success) {
                    return pe_directory_code::pe_directory_code_currupted;
                }

            } while (import_desc.first_thunk);
        }

        placement[virtual_address] =
            pe_placement_entry(import_desc_table_size, id_pe_placement::id_pe_import_descriptor, "");

        return pe_directory_code::pe_directory_code_success;
    }


    return pe_directory_code::pe_directory_code_not_present;
}


pe_directory_code get_import_directory(const pe_image &image, pe_import_directory& imports, const pe_bound_import_directory& bound_imports) {

    if (image.is_x32_image()) {
        return _get_import_directory<pe_image_32>(image, imports, bound_imports);
    }
    else {
        return _get_import_directory<pe_image_64>(image, imports, bound_imports);
    }
}


bool build_internal_import_directory_data(pe_image &image, pe_section& section, pe_import_directory& imports,
     uint32_t build_items_ids/*import_table_build_id*/, 
    uint32_t iat_rva, uint32_t oft_rva, uint32_t names_rva,

     const pe_bound_import_directory* bound_imports) {

    if (image.is_x32_image()) {
        return _build_internal_import_directory_data<pe_image_32>(image, section, imports, build_items_ids,
            iat_rva, oft_rva, names_rva,
            bound_imports);
    }
    else {
        return _build_internal_import_directory_data<pe_image_64>(image, section, imports, build_items_ids, 
            iat_rva, oft_rva, names_rva,
            bound_imports);
    }
}


bool build_import_directory_only(pe_image &image, pe_section& section, pe_import_directory& imports,
    uint32_t desc_rva) {

    if (imports.size()) {
        pe_section_io import_io(section, image);

        if (desc_rva == -1) {
            import_io.align_up(0x10).seek_to_end();
        }
        else {
            import_io.set_section_offset(desc_rva);
        }


        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IMPORT, import_io.get_section_offset());
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_IMPORT, uint32_t(imports.size()*sizeof(image_import_descriptor)));

        for (auto& library : imports.get_libraries()) {
            image_import_descriptor import_desc;
            import_desc.original_first_thunk = library.get_rva_oft();        
            import_desc.time_date_stamp      = library.get_timestamp();
            import_desc.forwarder_chain      = 0;
            import_desc.name                 = library.get_rva_library_name();
            import_desc.first_thunk          = library.get_rva_iat();


            if (import_io.write(&import_desc, sizeof(import_desc)) != enma_io_success) {
                return false;
            }
        }
        image_import_descriptor null_desc;
        memset(&null_desc,0,sizeof(null_desc));
        if (import_io.write(&null_desc, sizeof(null_desc)) != enma_io_success) {
            return false;
        }
    }
    else {
        image.set_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_IMPORT, 0);
        image.set_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_IMPORT, 0);
    }

    return true;
}

bool build_import_directory_full(pe_image &image,
    pe_section& section, pe_import_directory& imports,
    uint32_t iat_rva, uint32_t oft_rva, uint32_t names_rva, uint32_t desc_rva,
    const pe_bound_import_directory* bound_imports) {

    return build_internal_import_directory_data(image, section, imports,
        import_table_build_iat | import_table_build_oft | import_table_build_library_name,
        iat_rva, oft_rva, names_rva, bound_imports) &&
        build_import_directory_only(image,section,imports, desc_rva);
}

pe_directory_code get_placement_import_directory(const pe_image &image, pe_placement& placement,
    const pe_bound_import_directory& bound_imports) {

    if (image.is_x32_image()) {
        return _get_placement_import_directory<pe_image_32>(image, placement, bound_imports);
    }
    else {
        return _get_placement_import_directory<pe_image_64>(image, placement, bound_imports);
    }
}