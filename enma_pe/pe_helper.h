#pragma once


double   get_data_entropy(_In_ const std::vector<uint8_t> &data);
double   get_section_entropy(_In_ const pe_section& section);

uint32_t calculate_checksum(_In_ const std::vector<uint8_t> &pe_image);

void pe_erase_placement(_Inout_ pe_image &image, _Inout_ pe_placement& placement,
    _Inout_opt_ pe_relocations_directory* relocs = 0, _In_opt_ bool delete_empty_sections = false);

#include "pe_string_helper.h"
#include "pe_rtti_helper.h"
#include "pe_resources_helper.h"