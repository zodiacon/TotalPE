#include "stdafx.h"
#include "pe_security.h"


pe_security_entry::pe_security_entry() {
    revision = 0;
    certificate_type = 0;
}

pe_security_entry::pe_security_entry(uint16_t revision, uint16_t certificate_type, std::vector<uint8_t>& certificate_data)
:revision(revision), certificate_type(certificate_type), certificate_data(certificate_data){}

void   pe_security_entry::set_revision(uint16_t revision) {
    this->revision = revision;
}
void   pe_security_entry::set_certificate_type(uint16_t type) {
    this->certificate_type = type;
}

void pe_security_entry::set_certificate_data(const std::vector<uint8_t>& data) {
    this->certificate_data = data;
}

uint16_t   pe_security_entry::get_revision() const {
    return this->revision;
}

uint16_t   pe_security_entry::get_certificate_type() const {
    return this->certificate_type;
}

const std::vector<uint8_t>& pe_security_entry::get_certificate_data() const {
    return this->certificate_data;
}

std::vector<uint8_t>& pe_security_entry::get_certificate_data() {
    return this->certificate_data;
}

void pe_security_directory::add_certificate(const pe_security_entry& item) {
    this->certificates.push_back(item);
}

void pe_security_directory::clear() {
    this->certificates.clear();
}

size_t pe_security_directory::get_certificates_count() const {
    return this->certificates.size();
}

const std::vector<pe_security_entry>& pe_security_directory::get_certificates() const {
    return this->certificates;
}

std::vector<pe_security_entry>& pe_security_directory::get_certificates() {
    return this->certificates;
}

pe_directory_code get_security_directory(const pe_image &image, pe_security_directory& security) {
    security.clear();

    uint32_t raw_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_SECURITY);
    uint32_t virtual_size = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_SECURITY);

    if (raw_address && virtual_size) {
        pe_image_io security_io(image, enma_io_address_raw);
        
        if (!security_io.is_present_raw(raw_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        uint32_t total_size = 0;
        security_io.set_image_offset(raw_address);

        do {
            win_certificate win_cert;
            std::vector<uint8_t> data;

            if (security_io.read(&win_cert, sizeof(win_cert)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }
            
            if (!win_cert.length) {
                break;
            }

            if (security_io.read(data, win_cert.length - (uint32_t)sizeof(win_certificate)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }

            security.add_certificate(pe_security_entry(win_cert.w_revision, win_cert.w_certificate_type, data));

            total_size += win_cert.length;
        } while (total_size < virtual_size);


        return pe_directory_code::pe_directory_code_success;
    }

    return pe_directory_code::pe_directory_code_not_present;
}


pe_directory_code get_placement_security_directory(const pe_image &image, pe_placement& placement) {


    uint32_t raw_address = image.get_directory_virtual_address(IMAGE_DIRECTORY_ENTRY_SECURITY);
    uint32_t virtual_size = image.get_directory_virtual_size(IMAGE_DIRECTORY_ENTRY_SECURITY);

    if (raw_address && virtual_size) {
        pe_image_io security_io(image, enma_io_address_raw);

        if (!security_io.is_present_raw(raw_address)) {
            return pe_directory_code::pe_directory_code_not_present;
        }

        uint32_t total_size = 0;
        security_io.set_image_offset(raw_address);

        do {
            win_certificate win_cert;
            std::vector<uint8_t> data;
      
            if (security_io.read(&win_cert, sizeof(win_certificate)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }

            placement[
                image.raw_to_rva(security_io.get_image_offset() - (uint32_t)sizeof(win_certificate)) //TODO: FIXIT because it might not be in virtual memory
            ] = pe_placement_entry(sizeof(win_certificate), id_pe_placement::id_pe_security_descriptor, "");


            if (security_io.read(data, win_cert.length - (uint32_t)sizeof(win_certificate)) != enma_io_success) {
                return pe_directory_code::pe_directory_code_currupted;
            }

            placement[
                image.raw_to_rva(security_io.get_image_offset() - (win_cert.length - (uint32_t)sizeof(win_certificate))) //TODO: FIXIT because it might not be in virtual memory
            ] = pe_placement_entry(sizeof(win_certificate), id_pe_placement::id_pe_security_certificate, "");


            total_size += win_cert.length;
        } while (total_size < virtual_size);


        return pe_directory_code::pe_directory_code_success;
    }

    return pe_directory_code::pe_directory_code_not_present;
}