#include "stdafx.h"
#include "pe_rtti_helper.h"


rtti_msvc_type_descriptor::rtti_msvc_type_descriptor()
    : vtable_addr_rva(0), spare_rva(0) {}

rtti_msvc_type_descriptor::rtti_msvc_type_descriptor(const rtti_msvc_type_descriptor& type_desc) {
    this->operator=(type_desc);
}

rtti_msvc_type_descriptor::~rtti_msvc_type_descriptor() {}

rtti_msvc_type_descriptor& rtti_msvc_type_descriptor::operator=(const rtti_msvc_type_descriptor& type_desc) {

    this->vtable_addr_rva = type_desc.vtable_addr_rva;
    this->spare_rva = type_desc.spare_rva;
    this->name = type_desc.name;

    return *this;
}


void rtti_msvc_type_descriptor::set_vtable_addr_rva(uint32_t rva) {
    this->vtable_addr_rva = rva;
}
void rtti_msvc_type_descriptor::set_spare_rva(uint32_t rva) {
    this->spare_rva = rva;
}
void rtti_msvc_type_descriptor::set_name(const std::string& name) {
    this->name = name;
}

uint32_t rtti_msvc_type_descriptor::get_vtable_addr_rva() const {
    return this->vtable_addr_rva;
}
uint32_t rtti_msvc_type_descriptor::get_spare_rva() const {
    return this->spare_rva;
}

std::string& rtti_msvc_type_descriptor::get_name() {
    return this->name;
}
const std::string& rtti_msvc_type_descriptor::get_name() const {
    return this->name;
}


rtti_msvc_base_class_descriptor::rtti_msvc_base_class_descriptor()
    : type_descriptor_addr_rva(0), num_contained_bases(0), mdisp(0), pdisp(0),
    vdisp(0), attributes(0) , hierarchy_descriptor_ref(0) {}

rtti_msvc_base_class_descriptor::rtti_msvc_base_class_descriptor(const rtti_msvc_base_class_descriptor& base_class_desc) {
    this->operator=(base_class_desc);
}
rtti_msvc_base_class_descriptor::~rtti_msvc_base_class_descriptor() {}

rtti_msvc_base_class_descriptor& rtti_msvc_base_class_descriptor::operator=(const rtti_msvc_base_class_descriptor& base_class_desc) {

    this->type_descriptor_addr_rva = base_class_desc.type_descriptor_addr_rva;
    this->num_contained_bases = base_class_desc.num_contained_bases;
    this->mdisp = base_class_desc.mdisp;
    this->pdisp = base_class_desc.pdisp;
    this->vdisp = base_class_desc.vdisp;
    this->attributes = base_class_desc.attributes;
    this->hierarchy_descriptor_ref = base_class_desc.hierarchy_descriptor_ref;

    return *this;
}

void rtti_msvc_base_class_descriptor::set_type_descriptor_addr_rva(uint32_t rva) {
    this->type_descriptor_addr_rva = rva;
}
void rtti_msvc_base_class_descriptor::set_num_contained_bases(uint32_t rva) {
    this->num_contained_bases = rva;
}
void rtti_msvc_base_class_descriptor::set_mdisp(uint32_t disp) {
    this->mdisp = disp;
}
void rtti_msvc_base_class_descriptor::set_pdisp(uint32_t disp) {
    this->pdisp = disp;
}
void rtti_msvc_base_class_descriptor::set_vdisp(uint32_t disp) {
    this->vdisp = disp;
}
void rtti_msvc_base_class_descriptor::set_attributes(uint32_t attributes) {
    this->attributes = attributes;
}
void rtti_msvc_base_class_descriptor::set_hierarchy_descriptor_ref(uint32_t ref) {
    this->hierarchy_descriptor_ref = ref;
}

uint32_t rtti_msvc_base_class_descriptor::get_type_descriptor_addr_rva() const {
    return this->type_descriptor_addr_rva;
}
uint32_t rtti_msvc_base_class_descriptor::get_num_contained_bases() const {
    return this->num_contained_bases;
}
uint32_t rtti_msvc_base_class_descriptor::get_mdisp() const {
    return this->mdisp;
}
uint32_t rtti_msvc_base_class_descriptor::get_pdisp() const {
    return this->pdisp;
}
uint32_t rtti_msvc_base_class_descriptor::get_vdisp() const {
    return this->vdisp;
}
uint32_t rtti_msvc_base_class_descriptor::get_attributes() const {
    return this->attributes;
}
uint32_t rtti_msvc_base_class_descriptor::get_hierarchy_descriptor_ref() const {
    return this->hierarchy_descriptor_ref;
}

rtti_msvc_class_hierarchy_descriptor::rtti_msvc_class_hierarchy_descriptor()
    : signature(0), attributes(0), num_base_classes(0), base_class_array_addr_rva(0) {}

rtti_msvc_class_hierarchy_descriptor::rtti_msvc_class_hierarchy_descriptor(const rtti_msvc_class_hierarchy_descriptor& class_hier_desc) {
    this->operator=(class_hier_desc);
}

rtti_msvc_class_hierarchy_descriptor::~rtti_msvc_class_hierarchy_descriptor() {}

rtti_msvc_class_hierarchy_descriptor& rtti_msvc_class_hierarchy_descriptor::operator=(const rtti_msvc_class_hierarchy_descriptor& class_hier_desc) {

    this->signature = class_hier_desc.signature;
    this->attributes = class_hier_desc.attributes;
    this->num_base_classes = class_hier_desc.num_base_classes;
    this->base_class_array_addr_rva = class_hier_desc.base_class_array_addr_rva;
    this->base_class_entries = class_hier_desc.base_class_entries;

    return *this;
}

void rtti_msvc_class_hierarchy_descriptor::set_signature(uint32_t sign) {
    this->signature = sign;
}
void rtti_msvc_class_hierarchy_descriptor::set_attributes(uint32_t attributes) {
    this->attributes = attributes;
}
void rtti_msvc_class_hierarchy_descriptor::set_num_base_classes(uint32_t num) {
    this->num_base_classes = num;
}
void rtti_msvc_class_hierarchy_descriptor::set_base_class_array_addr_rva(uint32_t base_class_rva) {
    this->base_class_array_addr_rva = base_class_rva;
}


uint32_t rtti_msvc_class_hierarchy_descriptor::get_signature() const {
    return this->signature;
}
uint32_t rtti_msvc_class_hierarchy_descriptor::get_attributes() const {
    return this->attributes;
}
uint32_t rtti_msvc_class_hierarchy_descriptor::get_num_base_classes() const {
    return this->num_base_classes;
}
uint32_t rtti_msvc_class_hierarchy_descriptor::get_base_class_array_addr_rva() const {
    return this->base_class_array_addr_rva;
}

std::vector<uint32_t>& rtti_msvc_class_hierarchy_descriptor::get_base_class_entries() {
    return this->base_class_entries;
}

const std::vector<uint32_t>& rtti_msvc_class_hierarchy_descriptor::get_base_class_entries() const {
    return this->base_class_entries;
}


rtti_msvc_complete_object_locator::rtti_msvc_complete_object_locator()
    :signature(0), vtable_offset(0), cd_offset(0), type_descriptor_addr_rva(0), class_descriptor_addr_rva(0), object_base_rva(0) {}

rtti_msvc_complete_object_locator::rtti_msvc_complete_object_locator(const rtti_msvc_complete_object_locator& obj_loc) {
    this->operator=(obj_loc);
}

rtti_msvc_complete_object_locator::~rtti_msvc_complete_object_locator() {}

rtti_msvc_complete_object_locator& rtti_msvc_complete_object_locator::operator=(const rtti_msvc_complete_object_locator& obj_loc) {
    this->signature = obj_loc.signature;
    this->vtable_offset = obj_loc.vtable_offset;
    this->cd_offset = obj_loc.cd_offset;
    this->type_descriptor_addr_rva = obj_loc.type_descriptor_addr_rva;
    this->class_descriptor_addr_rva = obj_loc.class_descriptor_addr_rva;
    this->object_base_rva = obj_loc.object_base_rva;

    return *this;
}

void rtti_msvc_complete_object_locator::set_signature(uint32_t sign) {
    this->signature = sign;
}

void rtti_msvc_complete_object_locator::set_vtable_offset(uint32_t vtable_offs) {
    this->vtable_offset = vtable_offs;
}

void rtti_msvc_complete_object_locator::set_cd_offset(uint32_t offset) {
    this->cd_offset = offset;
}

void rtti_msvc_complete_object_locator::set_type_descriptor_addr_rva(uint32_t type_descriptor_rva) {
    this->type_descriptor_addr_rva = type_descriptor_rva;
}

void rtti_msvc_complete_object_locator::set_class_descriptor_addr_rva(uint32_t class_descriptor_rva) {
    this->class_descriptor_addr_rva = class_descriptor_rva;
}

void rtti_msvc_complete_object_locator::set_object_base_rva(uint32_t object_base) {
    this->object_base_rva = object_base;
}


uint32_t rtti_msvc_complete_object_locator::get_signature() {
    return this->signature;
}
uint32_t rtti_msvc_complete_object_locator::get_vtable_offset() {
    return this->vtable_offset;
}
uint32_t rtti_msvc_complete_object_locator::get_cd_offset() {
    return this->cd_offset;
}
uint32_t rtti_msvc_complete_object_locator::get_type_descriptor_addr_rva() {
    return this->type_descriptor_addr_rva;
}
uint32_t rtti_msvc_complete_object_locator::get_class_descriptor_addr_rva() {
    return this->class_descriptor_addr_rva;
}
uint32_t rtti_msvc_complete_object_locator::get_object_base_rva() {
    return this->object_base_rva;
}


bool check_msvc_x32_rtti(const pe_image& image, uint32_t rtti_complete_object_locators_rva) {

    pe_image_io image_io(image);

    msvc_rtti_complete_object_locator obj_loc;

    if (image_io.set_image_offset(rtti_complete_object_locators_rva)
        .read(&obj_loc, sizeof(obj_loc)) != enma_io_success) {

        return false;
    }

    uint32_t image_base = (uint32_t)image.get_image_base();

    uint32_t hi_address = image_io.seek_to_end().get_image_offset();

    if (!obj_loc.type_descriptor_addr ||
        (obj_loc.type_descriptor_addr < image_base ||
        (obj_loc.type_descriptor_addr - image_base) >= hi_address)) {

        return false;
    }


    if (!obj_loc.class_descriptor_addr ||
        (obj_loc.class_descriptor_addr < image_base ||
        (obj_loc.class_descriptor_addr - image_base) >= hi_address)) {

        return false;
    }

    std::vector<uint8_t> type_desc_raw;


    if (image_io.set_image_offset(obj_loc.type_descriptor_addr - image_base)
        .read(type_desc_raw, sizeof(msvc_rtti_32_type_descriptor) + 3) != enma_io_success) {

        return false;
    }

    msvc_rtti_32_type_descriptor *type_desc = (msvc_rtti_32_type_descriptor*)type_desc_raw.data();

    if (!type_desc->vtable_addr ||
        (type_desc->vtable_addr < image_base ||
        (type_desc->vtable_addr - image_base) >= hi_address)) {

        return false;
    }

    if (type_desc->spare &&
        (type_desc->spare < image_base ||
        (type_desc->spare - image_base) >= hi_address)) {

        return false;
    }

    if (type_desc->name[0] != '.' &&
        type_desc->name[1] != '?' &&
        type_desc->name[2] != 'A') {

        return false;
    }


    return true;
}

bool check_msvc_x64_rtti(const pe_image& image, uint32_t rtti_complete_object_locators_rva) {

    pe_image_io image_io(image);

    msvc_rtti_complete_object_locator obj_loc;

    if (image_io.set_image_offset(rtti_complete_object_locators_rva)
        .read(&obj_loc, sizeof(obj_loc)) != enma_io_success) {

        return false;
    }

    uint64_t image_base = image.get_image_base();
    uint32_t hi_address = image_io.seek_to_end().get_image_offset();

    if (!obj_loc.type_descriptor_addr ||
        obj_loc.type_descriptor_addr >= hi_address) {

        return false;
    }

    if (!obj_loc.class_descriptor_addr
        || obj_loc.class_descriptor_addr >= hi_address) {

        return false;
    }

    if (!obj_loc.object_base
        || obj_loc.object_base >= hi_address) {

        return false;
    }


    std::vector<uint8_t> type_desc_raw;


    if (image_io.set_image_offset(obj_loc.type_descriptor_addr)
        .read(type_desc_raw, sizeof(msvc_rtti_64_type_descriptor) + 3) != enma_io_success) {

        return false;
    }

    msvc_rtti_64_type_descriptor *type_desc = (msvc_rtti_64_type_descriptor*)type_desc_raw.data();


    if (!type_desc->vtable_addr
        || (type_desc->vtable_addr - image_base) >= hi_address) {

        return false;
    }

    if (type_desc->spare &&
        (type_desc->spare - image_base) >= hi_address) {

        return false;
    }

    if (type_desc->name[0] != '.' &&
        type_desc->name[1] != '?' &&
        type_desc->name[2] != 'A') {

        return false;
    }


    return true;
}

std::set<uint32_t> find_msvc_x32_rtti_complete_object_locators(const pe_image_full& image_full) {

    std::set<uint32_t> result_;
    pe_image_io image_io(image_full.get_image());



    for (auto& rel : image_full.get_relocations().get_entries()) {
        uint32_t reloc_data;

        if (image_io.set_image_offset(rel.relative_virtual_address).read(&reloc_data, sizeof(reloc_data)) != enma_io_success) {
            continue;
        }


        if (result_.find(uint32_t(reloc_data - image_full.get_image().get_image_base())) != result_.end()) {
            continue;
        }

        pe_section * section = image_full.get_image().get_section_by_va(reloc_data);
        if (!section || !section->get_size_of_raw_data()) { continue; }

        if (check_msvc_x32_rtti(image_full.get_image(), uint32_t(reloc_data - image_full.get_image().get_image_base()))) {
            result_.insert(uint32_t(reloc_data - image_full.get_image().get_image_base()));
        }
    }

    return result_;
}

std::set<uint32_t> find_msvc_x64_rtti_complete_object_locators(const pe_image_full& image_full) {

    std::set<uint32_t> result_;
    pe_image_io image_io(image_full.get_image());

    for (auto& rel : image_full.get_relocations().get_entries()) {
        uint64_t reloc_data;

        if (image_io.set_image_offset(rel.relative_virtual_address).read(&reloc_data, sizeof(reloc_data)) != enma_io_success) {
            continue;
        }

        if (result_.find(uint32_t(reloc_data - image_full.get_image().get_image_base())) != result_.end()) {
            continue;
        }

        pe_section * section = image_full.get_image().get_section_by_va(reloc_data);
        if (!section || !section->get_size_of_raw_data()) { continue; }

        if (check_msvc_x64_rtti(image_full.get_image(), uint32_t(reloc_data - image_full.get_image().get_image_base()))) {
            result_.insert(uint32_t(reloc_data - image_full.get_image().get_image_base()));
        }
    }

    return result_;
}



bool msvc_parse_type_descriptor_32(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint32_t image_base) {

    rtti_msvc_type_descriptor type_desc;
    msvc_rtti_32_type_descriptor type_;

    if (image_io.set_image_offset(rva)
        .read(&type_, sizeof(type_)) != enma_io_success) {

        return false;
    }

    if (type_.vtable_addr && type_.vtable_addr < image_base) {
        return false;
    }

    if (type_.vtable_addr) {
        type_desc.set_vtable_addr_rva(type_.vtable_addr - image_base);
    }
    else {
        type_desc.set_vtable_addr_rva(0);
    }

    if (type_.spare && type_.spare < image_base) {
        return false;
    }

    if (type_.spare) {
        type_desc.set_spare_rva(type_.spare - image_base);
    }
    else {
        type_desc.set_spare_rva(0);
    }

    std::string name;

    if (image_io.read_string(name) != enma_io_success) {

        return false;
    }
    else {
        type_desc.set_name(name);
    }

    msvc_rtti.type_descriptor_entries[rva] = type_desc;

    return true;
}


bool msvc_parse_base_class_descriptor_32(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint32_t image_base) {

    msvc_rtti_base_class_descriptor base_class_desc_;

    if (image_io.set_image_offset(rva)
        .read(&base_class_desc_, sizeof(base_class_desc_)) != enma_io_success) {

        return false;
    }

    if ((base_class_desc_.type_descriptor_addr && (base_class_desc_.type_descriptor_addr < image_base)) ||
        (base_class_desc_.type_descriptor_addr && !msvc_parse_type_descriptor_32(image_io, msvc_rtti,
            base_class_desc_.type_descriptor_addr - image_base, image_base))) {

        return false;
    }

    rtti_msvc_base_class_descriptor base_class_desc;

    if (base_class_desc_.type_descriptor_addr) {
        base_class_desc.set_type_descriptor_addr_rva(base_class_desc_.type_descriptor_addr - image_base);
    }
    else {
        base_class_desc.set_type_descriptor_addr_rva(0);
    }

    base_class_desc.set_num_contained_bases(base_class_desc_.num_contained_bases);
    base_class_desc.set_mdisp(base_class_desc_.where.mdisp);
    base_class_desc.set_pdisp(base_class_desc_.where.pdisp);
    base_class_desc.set_vdisp(base_class_desc_.where.vdisp);
    base_class_desc.set_attributes(base_class_desc_.attributes);
    base_class_desc.set_hierarchy_descriptor_ref(base_class_desc_.hierarchy_descriptor_ref);

    msvc_rtti.base_class_descriptor_entries[rva] = base_class_desc;

    return true;
}


bool msvc_parse_class_hierarchy_descriptor_32(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint32_t image_base) {

    msvc_rtti_class_hierarchy_descriptor hierarchy_desc_;

    if (image_io.set_image_offset(rva)
        .read(&hierarchy_desc_, sizeof(hierarchy_desc_)) != enma_io_success) {

        return false;
    }

    std::vector<uint32_t> base_addresses;
    base_addresses.resize(hierarchy_desc_.num_base_classes);

    if ((hierarchy_desc_.base_class_array_addr && (hierarchy_desc_.base_class_array_addr < image_base)) ||
        (hierarchy_desc_.base_class_array_addr && image_io.set_image_offset(hierarchy_desc_.base_class_array_addr - image_base)
            .read(base_addresses.data(), hierarchy_desc_.num_base_classes * sizeof(uint32_t)) != enma_io_success)) {

        return false;
    }

    for (uint32_t base_address : base_addresses) {
        if ((base_address && (base_address < image_base)) ||
            (base_address && !msvc_parse_base_class_descriptor_32(image_io, msvc_rtti, (base_address - image_base), image_base))) {
            return false;
        }
    }

    rtti_msvc_class_hierarchy_descriptor hierarchy_desc;
    hierarchy_desc.set_signature(hierarchy_desc_.signature);
    hierarchy_desc.set_attributes(hierarchy_desc_.attributes);
    hierarchy_desc.set_num_base_classes(hierarchy_desc_.num_base_classes);

    if (hierarchy_desc_.base_class_array_addr) {
        hierarchy_desc.set_base_class_array_addr_rva(hierarchy_desc_.base_class_array_addr - image_base);
    }
    else {
        hierarchy_desc.set_base_class_array_addr_rva(0);
    }

    for (uint32_t base_address : base_addresses) {
        if (base_address) {
            hierarchy_desc.get_base_class_entries().push_back(base_address - image_base);
        }
    }



    msvc_rtti.class_hierarchy_descriptor_entries[rva] = hierarchy_desc;

    return true;
}

bool msvc_parse_complete_object_locator_32(const pe_image_full& image_full, msvc_rtti_desc& msvc_rtti, uint32_t rva) {

    pe_image_io image_io(image_full.get_image());

    uint32_t image_base = (uint32_t)image_full.get_image().get_image_base();

    msvc_rtti_complete_object_locator object_locator_;

    if (image_io.set_image_offset(rva)
        .read(&object_locator_, sizeof(object_locator_) - sizeof(uint32_t)) != enma_io_success) {

        return false;
    }

    if ((object_locator_.type_descriptor_addr && (object_locator_.type_descriptor_addr < image_base)) ||
        (object_locator_.type_descriptor_addr && !msvc_parse_type_descriptor_32(image_io, msvc_rtti, object_locator_.type_descriptor_addr - image_base, image_base))) {

        return false;
    }


    if ((object_locator_.class_descriptor_addr && (object_locator_.class_descriptor_addr < image_base)) ||
        (object_locator_.class_descriptor_addr && !msvc_parse_class_hierarchy_descriptor_32(image_io, msvc_rtti, object_locator_.class_descriptor_addr - image_base, image_base))) {

        return false;
    }

    rtti_msvc_complete_object_locator object_locator;
    object_locator.set_signature(object_locator_.signature);
    object_locator.set_vtable_offset(object_locator_.vtable_offset);
    object_locator.set_cd_offset(object_locator_.cd_offset);

    if (object_locator_.type_descriptor_addr) {
        object_locator.set_type_descriptor_addr_rva(object_locator_.type_descriptor_addr - image_base);
    }
    else {
        object_locator.set_type_descriptor_addr_rva(0);
    }

    if (object_locator_.class_descriptor_addr) {
        object_locator.set_class_descriptor_addr_rva(object_locator_.class_descriptor_addr - image_base);
    }
    else {
        object_locator.set_class_descriptor_addr_rva(0);
    }

    object_locator.set_object_base_rva(0);

    msvc_rtti.complete_object_locator_entries[rva] = object_locator;

    return true;
}



bool msvc_parse_type_descriptor_64(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint64_t image_base) {

    rtti_msvc_type_descriptor type_desc;
    msvc_rtti_64_type_descriptor type_;

    if (image_io.set_image_offset(rva)
        .read(&type_, sizeof(type_)) != enma_io_success) {

        return false;
    }

    if (type_.vtable_addr && type_.vtable_addr < image_base) {
        return false;
    }

    if (type_.vtable_addr) {
        type_desc.set_vtable_addr_rva(uint32_t(type_.vtable_addr - image_base));
    }
    else {
        type_desc.set_vtable_addr_rva(0);
    }

    if (type_.spare && type_.spare < image_base) {
        return false;
    }

    if (type_.spare) {
        type_desc.set_spare_rva(uint32_t(type_.spare - image_base));
    }
    else {
        type_desc.set_spare_rva(0);
    }

    std::string name;

    if (image_io.read_string(name) != enma_io_success) {

        return false;
    }
    else {
        type_desc.set_name(name);
    }

    msvc_rtti.type_descriptor_entries[rva] = type_desc;

    return true;
}


bool msvc_parse_base_class_descriptor_64(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint64_t image_base) {

    msvc_rtti_base_class_descriptor base_class_desc_;

    if (image_io.set_image_offset(rva)
        .read(&base_class_desc_, sizeof(base_class_desc_)) != enma_io_success) {

        return false;
    }

    if (base_class_desc_.type_descriptor_addr &&
        !msvc_parse_type_descriptor_64(image_io, msvc_rtti, base_class_desc_.type_descriptor_addr, image_base)) {

        return false;
    }

    rtti_msvc_base_class_descriptor base_class_desc;

    if (base_class_desc_.type_descriptor_addr) {
        base_class_desc.set_type_descriptor_addr_rva(base_class_desc_.type_descriptor_addr);
    }
    else {
        base_class_desc.set_type_descriptor_addr_rva(0);
    }

    base_class_desc.set_num_contained_bases(base_class_desc_.num_contained_bases);
    base_class_desc.set_mdisp(base_class_desc_.where.mdisp);
    base_class_desc.set_pdisp(base_class_desc_.where.pdisp);
    base_class_desc.set_vdisp(base_class_desc_.where.vdisp);
    base_class_desc.set_attributes(base_class_desc_.attributes);

    if (base_class_desc_.hierarchy_descriptor_ref) {
        base_class_desc.set_hierarchy_descriptor_ref(base_class_desc_.hierarchy_descriptor_ref);
    }
    else {
        base_class_desc.set_hierarchy_descriptor_ref(0);
    }

    msvc_rtti.base_class_descriptor_entries[rva] = base_class_desc;

    return true;
}


bool msvc_parse_class_hierarchy_descriptor_64(pe_image_io& image_io, msvc_rtti_desc& msvc_rtti, uint32_t rva, uint64_t image_base) {

    msvc_rtti_class_hierarchy_descriptor hierarchy_desc_;

    if (image_io.set_image_offset(rva)
        .read(&hierarchy_desc_, sizeof(hierarchy_desc_)) != enma_io_success) {

        return false;
    }

    std::vector<uint32_t> base_addresses;
    base_addresses.resize(hierarchy_desc_.num_base_classes);

    if (image_io.set_image_offset(hierarchy_desc_.base_class_array_addr)
        .read(base_addresses.data(), hierarchy_desc_.num_base_classes * sizeof(uint32_t)) != enma_io_success) {

        return false;
    }

    for (uint32_t base_address : base_addresses) {
        if (base_address &&
            !msvc_parse_base_class_descriptor_64(image_io, msvc_rtti, base_address, image_base)) {

            return false;
        }
    }

    rtti_msvc_class_hierarchy_descriptor hierarchy_desc;
    hierarchy_desc.set_signature(hierarchy_desc_.signature);
    hierarchy_desc.set_attributes(hierarchy_desc_.attributes);
    hierarchy_desc.set_num_base_classes(hierarchy_desc_.num_base_classes);

    if (hierarchy_desc_.base_class_array_addr) {
        hierarchy_desc.set_base_class_array_addr_rva(hierarchy_desc_.base_class_array_addr);
    }
    else {
        hierarchy_desc.set_base_class_array_addr_rva(0);
    }

    for (uint32_t base_address : base_addresses) {
        if (base_address) {
            hierarchy_desc.get_base_class_entries().push_back(base_address);
        }
    }

    msvc_rtti.class_hierarchy_descriptor_entries[rva] = hierarchy_desc;

    return true;
}

bool msvc_parse_complete_object_locator_64(const pe_image_full& image_full, msvc_rtti_desc& msvc_rtti, uint32_t rva) {

    pe_image_io image_io(image_full.get_image());

    uint64_t image_base = image_full.get_image().get_image_base();

    msvc_rtti_complete_object_locator object_locator_;

    if (image_io.set_image_offset(rva)
        .read(&object_locator_, sizeof(object_locator_)) != enma_io_success) {

        return false;
    }

    if (object_locator_.type_descriptor_addr &&
        !msvc_parse_type_descriptor_64(image_io, msvc_rtti, object_locator_.type_descriptor_addr, image_base)) {

        return false;
    }


    if (object_locator_.class_descriptor_addr &&
        !msvc_parse_class_hierarchy_descriptor_64(image_io, msvc_rtti, object_locator_.class_descriptor_addr, image_base)) {

        return false;
    }

    rtti_msvc_complete_object_locator object_locator;
    object_locator.set_signature(object_locator_.signature);
    object_locator.set_vtable_offset(object_locator_.vtable_offset);
    object_locator.set_cd_offset(object_locator_.cd_offset);

    if (object_locator_.type_descriptor_addr) {
        object_locator.set_type_descriptor_addr_rva(object_locator_.type_descriptor_addr);
    }
    else {
        object_locator.set_type_descriptor_addr_rva(0);
    }

    if (object_locator_.class_descriptor_addr) {
        object_locator.set_class_descriptor_addr_rva(object_locator_.class_descriptor_addr);
    }
    else {
        object_locator.set_class_descriptor_addr_rva(0);
    }

    if (object_locator_.object_base) {
        object_locator.set_object_base_rva(object_locator_.object_base);
    }
    else {
        object_locator.set_object_base_rva(0);
    }


    msvc_rtti.complete_object_locator_entries[rva] = object_locator;

    return true;
}


void get_runtime_type_information(_In_ const pe_image_full& image_full, msvc_rtti_desc& msvc_rtti) {

    msvc_rtti.complete_object_locator_entries.clear();
    msvc_rtti.class_hierarchy_descriptor_entries.clear();
    msvc_rtti.base_class_descriptor_entries.clear();
    msvc_rtti.type_descriptor_entries.clear();

    if (image_full.get_image().is_x32_image()) {

        std::set<uint32_t> msvc_rtti_loc_rvas = find_msvc_x32_rtti_complete_object_locators(image_full);

        for (uint32_t loc_rva : msvc_rtti_loc_rvas) {

            msvc_rtti_desc pre_msvc_rtti;
            if (msvc_parse_complete_object_locator_32(image_full, pre_msvc_rtti, loc_rva)) {

                for (auto& entry : pre_msvc_rtti.complete_object_locator_entries) {
                    if (msvc_rtti.complete_object_locator_entries.find(entry.first) == msvc_rtti.complete_object_locator_entries.end()) {
                        msvc_rtti.complete_object_locator_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.class_hierarchy_descriptor_entries) {
                    if (msvc_rtti.class_hierarchy_descriptor_entries.find(entry.first) == msvc_rtti.class_hierarchy_descriptor_entries.end()) {
                        msvc_rtti.class_hierarchy_descriptor_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.base_class_descriptor_entries) {
                    if (msvc_rtti.base_class_descriptor_entries.find(entry.first) == msvc_rtti.base_class_descriptor_entries.end()) {
                        msvc_rtti.base_class_descriptor_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.type_descriptor_entries) {
                    if (msvc_rtti.type_descriptor_entries.find(entry.first) == msvc_rtti.type_descriptor_entries.end()) {
                        msvc_rtti.type_descriptor_entries[entry.first] = entry.second;
                    }
                }
            }
        }

    }
    else {
        std::set<uint32_t> msvc_rtti_loc_rvas = find_msvc_x64_rtti_complete_object_locators(image_full);

        for (uint32_t loc_rva : msvc_rtti_loc_rvas) {

            msvc_rtti_desc pre_msvc_rtti;
            if (msvc_parse_complete_object_locator_64(image_full, pre_msvc_rtti, loc_rva)) {

                for (auto& entry : pre_msvc_rtti.complete_object_locator_entries) {
                    if (msvc_rtti.complete_object_locator_entries.find(entry.first) == msvc_rtti.complete_object_locator_entries.end()) {
                        msvc_rtti.complete_object_locator_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.class_hierarchy_descriptor_entries) {
                    if (msvc_rtti.class_hierarchy_descriptor_entries.find(entry.first) == msvc_rtti.class_hierarchy_descriptor_entries.end()) {
                        msvc_rtti.class_hierarchy_descriptor_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.base_class_descriptor_entries) {
                    if (msvc_rtti.base_class_descriptor_entries.find(entry.first) == msvc_rtti.base_class_descriptor_entries.end()) {
                        msvc_rtti.base_class_descriptor_entries[entry.first] = entry.second;
                    }
                }

                for (auto& entry : pre_msvc_rtti.type_descriptor_entries) {
                    if (msvc_rtti.type_descriptor_entries.find(entry.first) == msvc_rtti.type_descriptor_entries.end()) {
                        msvc_rtti.type_descriptor_entries[entry.first] = entry.second;
                    }
                }
            }
        }
    }
}