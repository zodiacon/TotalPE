#pragma once

#pragma pack(push, 1)

struct msvc_rtti_complete_object_locator {
    uint32_t signature;
    uint32_t vtable_offset;         // offset of the vtable within class
    uint32_t cd_offset;             // constructor displacement offset
    uint32_t type_descriptor_addr;  // only a relative offset for 64bit
    uint32_t class_descriptor_addr; // only a relative offset for 64bit
    uint32_t object_base;           // only for 64bit
};


struct msvc_rtti_class_hierarchy_descriptor {
    uint32_t signature;
    uint32_t attributes;            // bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
    uint32_t num_base_classes;
    uint32_t base_class_array_addr; // only a relative offset for 64bit
};

struct msvc_rtti_base_class_descriptor {
    uint32_t type_descriptor_addr;  // only a relative offset for 64bit
    uint32_t num_contained_bases;
    struct {
        uint32_t mdisp;             // member displacement
        uint32_t pdisp;             // vbtable displacement
        uint32_t vdisp;             // displacement inside vbtable
    } where;
    uint32_t attributes;
    uint32_t hierarchy_descriptor_ref;
};


#pragma warning(push)

#pragma warning(disable : 4200)

struct msvc_rtti_32_type_descriptor {
    uint32_t vtable_addr;
    uint32_t spare;
    char name[0];
};
struct msvc_rtti_64_type_descriptor {
    uint64_t vtable_addr;
    uint64_t spare;
    char name[0];
};

#pragma warning(pop)

#pragma pack(pop)


class rtti_msvc_type_descriptor {
    uint32_t vtable_addr_rva;
    uint32_t spare_rva;

    std::string name;
public:
    rtti_msvc_type_descriptor();
    rtti_msvc_type_descriptor(const rtti_msvc_type_descriptor& type_desc);
    ~rtti_msvc_type_descriptor();

    rtti_msvc_type_descriptor& operator=(const rtti_msvc_type_descriptor& type_desc);
public:
    void set_vtable_addr_rva(uint32_t rva);
    void set_spare_rva(uint32_t rva);

    void set_name(const std::string& name);

public:
    uint32_t get_vtable_addr_rva() const;
    uint32_t get_spare_rva() const;

    std::string& get_name();
    const std::string& get_name() const;
};

class rtti_msvc_base_class_descriptor {
    uint32_t type_descriptor_addr_rva;
    uint32_t num_contained_bases;
    uint32_t mdisp;
    uint32_t pdisp;
    uint32_t vdisp;
    uint32_t attributes;
    uint32_t hierarchy_descriptor_ref;

public:
    rtti_msvc_base_class_descriptor();
    rtti_msvc_base_class_descriptor(const rtti_msvc_base_class_descriptor& base_class_desc);
    ~rtti_msvc_base_class_descriptor();

    rtti_msvc_base_class_descriptor& operator=(const rtti_msvc_base_class_descriptor& base_class_desc);
public:
    void set_type_descriptor_addr_rva(uint32_t rva);
    void set_num_contained_bases(uint32_t rva);
    void set_mdisp(uint32_t disp);
    void set_pdisp(uint32_t disp);
    void set_vdisp(uint32_t disp);
    void set_attributes(uint32_t attributes);
    void set_hierarchy_descriptor_ref(uint32_t ref);
public:

    uint32_t get_type_descriptor_addr_rva() const;
    uint32_t get_num_contained_bases() const;
    uint32_t get_mdisp() const;
    uint32_t get_pdisp() const;
    uint32_t get_vdisp() const;
    uint32_t get_attributes() const;
    uint32_t get_hierarchy_descriptor_ref() const;
};

class rtti_msvc_class_hierarchy_descriptor {
    uint32_t signature;
    uint32_t attributes;
    uint32_t num_base_classes;
    uint32_t base_class_array_addr_rva;

    std::vector<uint32_t> base_class_entries;
public:
    rtti_msvc_class_hierarchy_descriptor();
    rtti_msvc_class_hierarchy_descriptor(const rtti_msvc_class_hierarchy_descriptor& class_hier_desc);
    ~rtti_msvc_class_hierarchy_descriptor();

    rtti_msvc_class_hierarchy_descriptor& operator=(const rtti_msvc_class_hierarchy_descriptor& class_hier_desc);
public:
    void set_signature(uint32_t sign);
    void set_attributes(uint32_t attributes);
    void set_num_base_classes(uint32_t num);
    void set_base_class_array_addr_rva(uint32_t base_class_rva);

public:

    uint32_t get_signature() const;
    uint32_t get_attributes() const;
    uint32_t get_num_base_classes() const;
    uint32_t get_base_class_array_addr_rva() const;

    std::vector<uint32_t>& get_base_class_entries();
    const std::vector<uint32_t>& get_base_class_entries() const;
};

class rtti_msvc_complete_object_locator {
    uint32_t signature;
    uint32_t vtable_offset;
    uint32_t cd_offset;
    uint32_t type_descriptor_addr_rva;
    uint32_t class_descriptor_addr_rva;
    uint32_t object_base_rva;

public:
    rtti_msvc_complete_object_locator();
    rtti_msvc_complete_object_locator(const rtti_msvc_complete_object_locator& obj_loc);
    ~rtti_msvc_complete_object_locator();

    rtti_msvc_complete_object_locator& operator=(const rtti_msvc_complete_object_locator& obj_loc);
public:
    void set_signature(uint32_t sign);
    void set_vtable_offset(uint32_t vtable_offs);
    void set_cd_offset(uint32_t offset);
    void set_type_descriptor_addr_rva(uint32_t type_descriptor_rva);
    void set_class_descriptor_addr_rva(uint32_t class_descriptor_rva);
    void set_object_base_rva(uint32_t object_base);

public:

    uint32_t get_signature();
    uint32_t get_vtable_offset();
    uint32_t get_cd_offset();
    uint32_t get_type_descriptor_addr_rva();
    uint32_t get_class_descriptor_addr_rva();
    uint32_t get_object_base_rva();
};


struct msvc_rtti_desc {
    // <desc_rva, desc_type>
    std::map<uint32_t, rtti_msvc_complete_object_locator> complete_object_locator_entries;
    std::map<uint32_t, rtti_msvc_class_hierarchy_descriptor> class_hierarchy_descriptor_entries;
    std::map<uint32_t, rtti_msvc_base_class_descriptor> base_class_descriptor_entries;

    std::map<uint32_t, rtti_msvc_type_descriptor> type_descriptor_entries;
};

void get_runtime_type_information(_In_ const pe_image_full& image_full, _Out_ msvc_rtti_desc& msvc_rtti);