#include "stdafx.h"
#include "pe_string_helper.h"

namespace pe_string_extractor {

    bool default_string_ascii_validator(uint8_t c) {

        return (
            (c >= '0' && c <= '9') ||                                                           //dig
            (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ') ||      //space
            (c >= ' ' && c <= '~') ||                                                           //printable
            ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ||                               //en
            (((c >= 0xE0 && c <= 0xFF) || c == 0xB8) || ((c >= 0xC0 && c <= 0xDF) || c == 0xA8)) //ru
            );
    }

    bool default_string_unicode_validator(uint16_t c) {

        return iswprint(c) || iswblank(c);
    }



    void get_strings_from_image(const pe_image& image, string_base_table& string_table,
        bool find_in_clean_image,
        bool find_in_execute_sections,
        uint32_t min_string_size,
        uint32_t max_string_size,
        string_ascii_validator ascii_validator,
        string_unicode_validator unicode_validator) {

        if (!ascii_validator || !unicode_validator) {
            return;
        }

        pe_image current_image = image;

        if (find_in_clean_image) {
            pe_placement placement;
            get_directories_placement(current_image, placement);
            pe_erase_placement(current_image, placement, 0, true);
        }

        string_table.ansi_base = ansi_string_base_table();
        string_table.wide_base = wide_string_base_table();


        //find ansi strings

        {
            pe_image_io ansi_string_io(current_image);
            ansi_string_io.seek_to_start();

            char image_byte;
            uint32_t pre_string_rva = 0;
            std::string pre_string;
            while (ansi_string_io.read(&image_byte, sizeof(image_byte)) == enma_io_success) {

                if ((find_in_execute_sections || !ansi_string_io.is_executable_rva(ansi_string_io.get_image_offset() - 1)) &&
                    ascii_validator(image_byte)) {

                    if (!pre_string.size()) {
                        pre_string_rva = ansi_string_io.get_image_offset() - 1;
                    }

                    pre_string += image_byte;
                }
                else {
                    if (image_byte == 0 &&
                        pre_string.size() >= MIN_STRING_SIZE && pre_string.size() <= MAX_STRING_SIZE) {

                        string_table.ansi_base.push_back({ pre_string_rva , pre_string });
                        pre_string.clear();
                    }
                    else {
                        pre_string.clear();
                    }
                }
            }

        }

        //find wide strings
        {
            pe_image_io wide_string_io(current_image);
            wide_string_io.seek_to_start();

            wchar_t image_byte;
            uint32_t pre_string_rva = 0;
            std::wstring pre_string;

            while (wide_string_io.read(&image_byte, sizeof(image_byte)) == enma_io_success) {
                if ((find_in_execute_sections || !wide_string_io.is_executable_rva(wide_string_io.get_image_offset() - 1)) &&
                    (unicode_validator(image_byte))) {

                    if (!pre_string.size()) {
                        pre_string_rva = wide_string_io.get_image_offset() - 1;
                    }

                    pre_string += image_byte;
                }
                else {
                    if (image_byte == 0 &&
                        pre_string.size() >= MIN_STRING_SIZE && pre_string.size() <= MAX_STRING_SIZE) {

                        string_table.wide_base.push_back({ pre_string_rva , pre_string });
                        pre_string.clear();
                    }
                    else {
                        pre_string.clear();
                    }
                }
            }

        }
    }

};