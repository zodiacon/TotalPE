#include "stdafx.h"
#include "pe_file_version_info.h"

using namespace pe_resource_irnl;


pe_file_version_info::pe_file_version_info()
    : valid_info(false), valid_file_info(false), file_info({ 0 }) {

}

pe_file_version_info::pe_file_version_info(const uint8_t* version_info, size_t version_info_size) 
    : valid_info(false), valid_file_info(false), file_info({ 0 }) {

    parse(version_info, version_info_size);
}

pe_file_version_info::~pe_file_version_info() {

}

uint16_t translate_info_cvt(const std::wstring& value) {

    uint16_t val = 0;

    for (size_t idx = 0; idx < value.size() ||
        idx < 4; idx++) {
        
        if (value[idx] >= L'0' && value[idx] <= L'9') {
            val = (val << 4) | (value[idx] - L'0');
        }
        else if (value[idx] >= L'a' && value[idx] <= L'f') {
            val = (val << 4) | (value[idx] - L'a' + 10);
        }
        else if (value[idx] >= L'A' && value[idx] <= L'F') {
            val = (val << 4) | (value[idx] - L'A' + 10);
        }
    }
    
    return val;
}

bool pe_file_version_info::parse(const uint8_t* version_info, size_t version_info_size) {

    this->supported_translation.clear();
    this->vs_string_table.clear();

    this->valid_info = true;
    this->valid_file_info = false;
    memset(&this->file_info, 0, sizeof(this->file_info));

    size_t ver_position = 0;

    vs_version_info * ver_root = (vs_version_info*)&version_info[ver_position];

    if (ver_root->value_length) {

        ver_position += ALIGN_UP((offsetof(vs_version_info, sz_key) + sizeof(VERSION_INFO_MAGIC)), sizeof(uint32_t));

        vs_fixed_file_info* fixed_info = (vs_fixed_file_info*)&version_info[ver_position];

        if (fixed_info->signature == FIXED_FILE_INFO_MAGIC) {

            this->valid_file_info = true;

            this->file_info.struct_version = fixed_info->struct_version;
            this->file_info.file_version_ms = fixed_info->file_version_ms;
            this->file_info.file_version_ls = fixed_info->file_version_ls;
            this->file_info.product_version_ms = fixed_info->product_version_ms;
            this->file_info.product_version_ls = fixed_info->product_version_ls;
            this->file_info.file_flags_mask = fixed_info->file_flags_mask;
            this->file_info.file_flags = fixed_info->file_flags;
            this->file_info.file_os = fixed_info->file_os;
            this->file_info.file_type = fixed_info->file_type;
            this->file_info.file_sub_type = fixed_info->file_sub_type;
            this->file_info.file_date_ms = fixed_info->file_date_ms;
            this->file_info.file_date_ls = fixed_info->file_date_ls;

        }

        ver_position = ALIGN_UP((ver_position + ver_root->value_length), sizeof(uint32_t));
    }

    for (; (ver_position + sizeof(uint16_t) * 3) < version_info_size;) {

        size_t hi_entry_position = 0;

        uint16_t hi_entry_length = *(uint16_t*)&version_info[ver_position + hi_entry_position];

        if (version_info_size - ver_position < hi_entry_length) {

            this->valid_info = false;

            break;
        }

        hi_entry_position += sizeof(uint16_t);

        uint16_t hi_entry_value_length = *(uint16_t*)&version_info[ver_position + hi_entry_position]; hi_entry_position += sizeof(uint16_t);
        uint16_t hi_entry_type = *(uint16_t*)&version_info[ver_position + hi_entry_position]; hi_entry_position += sizeof(uint16_t);
  
        if (hi_entry_value_length == 0 &&
            !memcmp(&version_info[ver_position + hi_entry_position], STRING_FILE_INFO_MAGIC, sizeof(STRING_FILE_INFO_MAGIC))) {

            if (!parse_string_file_info(&version_info[ver_position + hi_entry_position - sizeof(uint16_t) * 3], hi_entry_length)) {

                this->valid_info = false;
            }
        }
        else if (hi_entry_value_length == 0 &&
            !memcmp(&version_info[ver_position + hi_entry_position], VAR_FILE_INFO_MAGIC, sizeof(VAR_FILE_INFO_MAGIC))) {

            if (!parse_var_file_info(&version_info[ver_position + hi_entry_position - sizeof(uint16_t) * 3], hi_entry_length)) {

                this->valid_info = false;
            }
        }


        ver_position = ALIGN_UP( (ver_position + hi_entry_length), sizeof(uint32_t));
    }


    return this->valid_info;
}


bool pe_file_version_info::parse_string(const uint8_t* _string, size_t _string_size, vs_strings& strings) {


    size_t string_position = 0;

    for (; string_position < _string_size;) {

        pe_resource_irnl::string* s_string = (pe_resource_irnl::string*)&_string[string_position];

        if (s_string->length + string_position > _string_size) {

            return false;
        }

        size_t start_st_position = string_position;

        uint16_t string_entry_length = *(uint16_t*)&_string[string_position]; string_position += sizeof(uint16_t);
        uint16_t string_entry_value_length = *(uint16_t*)&_string[string_position]; string_position += sizeof(uint16_t);
        uint16_t string_entry_type = *(uint16_t*)&_string[string_position]; string_position += sizeof(uint16_t);

        std::wstring key;
        std::wstring value;

        {
            size_t key_size = 0;
            wchar_t* w_key = (wchar_t*)&_string[string_position];

            for (; key_size < string_entry_length; key_size++) {

                if (w_key[key_size] == 0) {

                    break;
                }
            }

            key.insert(key.begin(), &w_key[0], &w_key[key_size]);

            string_position = ALIGN_UP((string_position + sizeof(wchar_t) * (key_size + 1)), sizeof(uint32_t));
        }

        if ( ( string_position - start_st_position) + string_entry_value_length * sizeof(uint16_t) <= string_entry_length) {

            wchar_t* w_value = (wchar_t*)&_string[string_position];

            value.insert(value.begin(), &w_value[0], &w_value[string_entry_value_length]);

            string_position = ALIGN_UP((string_position + string_entry_value_length * sizeof(uint16_t)), sizeof(uint32_t));
        }

        strings.push_back({ key , value });
    }

    return true;
}

bool pe_file_version_info::parse_string_table(const uint8_t* _string_table, size_t _string_table_size) {

    string_table* st_info = (string_table*)_string_table;

    if (st_info->length > _string_table_size) {

        return false;
    }

    for (size_t st_position = 0; st_position < st_info->length; ) {

        uint16_t st_entry_length = *(uint16_t*)&_string_table[st_position]; st_position += sizeof(uint16_t);
        uint16_t st_entry_value_length = *(uint16_t*)&_string_table[st_position]; st_position += sizeof(uint16_t);
        uint16_t st_entry_type = *(uint16_t*)&_string_table[st_position]; st_position += sizeof(uint16_t);
        
#pragma pack(push, 1)
        struct {
            wchar_t lang_id[5];
            wchar_t code_page[5];
        }string_id;
#pragma pack(pop)

        memset(&string_id, 0, sizeof(string_id));
        memcpy(&string_id.lang_id, &_string_table[st_position], sizeof(wchar_t) * 4); st_position += sizeof(wchar_t) * 4;
        memcpy(&string_id.code_page, &_string_table[st_position], sizeof(wchar_t) * 4); st_position += sizeof(wchar_t) * 4;

        st_position = ALIGN_UP(st_position, sizeof(uint32_t));

        vs_strings strings;

        if (!parse_string(&_string_table[st_position], st_entry_length - st_position, 
            this->vs_string_table[{translate_info_cvt(string_id.lang_id), translate_info_cvt(string_id.code_page)}])) {

            return false;
        }

        st_position = ALIGN_UP((st_position + st_entry_length), sizeof(uint32_t));
    }

    return true;
}

bool pe_file_version_info::parse_string_file_info(const uint8_t* _string_file_info, size_t _string_file_info_size) {

    string_file_info* sf_info = (string_file_info*)_string_file_info;

    if (sf_info->length > _string_file_info_size) {

        return false;
    }

    for (size_t st_position = 0; st_position + sizeof(uint16_t) * 3 + sizeof(STRING_FILE_INFO_MAGIC) < sf_info->length; ) {

        uint16_t st_entry_length = *(uint16_t*)&_string_file_info[st_position]; st_position += sizeof(uint16_t);
        uint16_t st_entry_value_length = *(uint16_t*)&_string_file_info[st_position]; st_position += sizeof(uint16_t);
        uint16_t st_entry_type = *(uint16_t*)&_string_file_info[st_position]; st_position += sizeof(uint16_t);

        if (st_position + st_entry_length < sf_info->length) {

            return false;
        }

        if (st_entry_value_length == 0 &&
            !memcmp(&_string_file_info[st_position], STRING_FILE_INFO_MAGIC, sizeof(STRING_FILE_INFO_MAGIC))) {

            st_position = ALIGN_UP((st_position + sizeof(STRING_FILE_INFO_MAGIC)), sizeof(uint32_t));

            if (!parse_string_table(&_string_file_info[st_position], st_entry_length - st_position)) {

                return false;
            }
        }
        else {
            // unk
        }

        st_position = ALIGN_UP((st_position + st_entry_length), sizeof(uint32_t));
    }

    return true;
}

bool pe_file_version_info::parse_var_info(const uint8_t* _var_info, size_t _var_info_size) {

    var* v_info = (var*)_var_info;

    if (v_info->length > _var_info_size) {

        return false;
    }

    size_t var_position = 0;
    uint16_t var_entry_length = *(uint16_t*)&_var_info[var_position]; var_position += sizeof(uint16_t);
    uint16_t var_entry_value_length = *(uint16_t*)&_var_info[var_position]; var_position += sizeof(uint16_t);
    uint16_t var_entry_type = *(uint16_t*)&_var_info[var_position]; var_position += sizeof(uint16_t);

    if (!memcmp(&_var_info[var_position], VAR_MAGIC, sizeof(VAR_MAGIC))) {

        var_position = ALIGN_UP((var_position + sizeof(VAR_FILE_INFO_MAGIC)), sizeof(uint32_t));

        size_t values_count = (var_entry_length - var_position) / sizeof(uint32_t);

        for (size_t value_idx = 0; value_idx < values_count; value_idx++) {

            uint32_t val = *(uint32_t*)&(_var_info[var_position + sizeof(uint32_t) * value_idx]);

            supported_translation.push_back({ (uint16_t)(val & 0xFFFF), (uint16_t)( (val & 0xFFFF0000) >> 16)});
        }
    }
    else {
        // unk
    }


    return true;
}

bool pe_file_version_info::parse_var_file_info(const uint8_t* _var_file_info, size_t _var_file_info_size) {

    var_file_info* vf_info = (var_file_info*)_var_file_info;
   
    if (vf_info->length > _var_file_info_size) {

        return false;
    }

    for (size_t var_position = 0; var_position + sizeof(uint16_t) * 3 + sizeof(VAR_FILE_INFO_MAGIC) < vf_info->length; ) {

        uint16_t var_entry_length = *(uint16_t*)&_var_file_info[var_position]; var_position += sizeof(uint16_t);
        uint16_t var_entry_value_length = *(uint16_t*)&_var_file_info[var_position]; var_position += sizeof(uint16_t);
        uint16_t var_entry_type = *(uint16_t*)&_var_file_info[var_position]; var_position += sizeof(uint16_t);
        
        if (var_position + var_entry_length < vf_info->length) {

            return false;
        }

        if (var_entry_value_length == 0 &&
            !memcmp(&_var_file_info[var_position], VAR_FILE_INFO_MAGIC, sizeof(VAR_FILE_INFO_MAGIC))) {

            var_position = ALIGN_UP((var_position + sizeof(VAR_FILE_INFO_MAGIC)), sizeof(uint32_t));

            if (!parse_var_info(&_var_file_info[var_position], var_entry_length - var_position)) {

                return false;
            }
        }
        else {
            // unk
        }

        var_position = ALIGN_UP((var_position + var_entry_length), sizeof(uint32_t));
    }

    return true;
}

std::vector<uint8_t> pe_file_version_info::compile() {
    return std::vector<uint8_t>();
}

void pe_file_version_info::set_valid_info(bool valid) {
    this->valid_info = valid;
}

void pe_file_version_info::set_valid_file_info(bool valid) {
    this->valid_file_info = valid;
}

bool pe_file_version_info::is_valid_info() const {
    return this->valid_info;
}

bool pe_file_version_info::has_file_info() const {
    return this->valid_file_info;
}

const std::map<vs_string_translation, vs_strings>& pe_file_version_info::get_string_table() const {
    return this->vs_string_table;
}

std::map<vs_string_translation, vs_strings>& pe_file_version_info::get_string_table() {
    return this->vs_string_table;
}

const std::vector<vs_string_translation>& pe_file_version_info::get_supported_translation() const {
    return this->supported_translation;
}

std::vector<vs_string_translation>& pe_file_version_info::get_supported_translation() {
    return this->supported_translation;
}

const pe_file_version_info::_file_info& pe_file_version_info::get_file_info() const {
    return this->file_info;
}

pe_file_version_info::_file_info& pe_file_version_info::get_file_info() {
    return this->file_info;
}