#pragma once

enum exception_handler_type {
    unknown_handler,
    __c_specific_handler,      // Scope table scope_table_entry
    __delphi_specific_handler, // Scope table delphi_scope_table_entry
    __llvm_specific_handler,   // relocatable ptr
    __gs_handler_check,       // GS data
    __gs_handler_check_seh,   // Scope table + GS data scope_table_entry
    __cxx_frame_handler3,     // RVA to FuncInfo
    __gs_handler_check_eh,    // RVA to FuncInfo + GS data
    handler_type_max
};

#define CXX_FRAME_MAGIC_VC6 0x19930520
#define CXX_FRAME_MAGIC_VC7 0x19930521
#define CXX_FRAME_MAGIC_VC8 0x19930522
#define CXX_EXCEPTION       0xe06d7363

#pragma pack(push, 1)

struct cxx_scope_table_entry {
    uint32_t begin_address;
    uint32_t end_address;
    uint32_t handler_address;
    uint32_t jump_target;
};

struct delphi_scope_table_entry {
    uint32_t begin_address;
    uint32_t end_address;
    uint32_t handle_type;
    uint32_t jump_target;
};

struct cxx_scope_table {
    uint32_t count;
    //scope_table_entry entries[count]
};

struct cxx_handler_type {
    uint32_t adjectives; // 0x01: const, 0x02: volatile, 0x08: reference
    uint32_t p_type; //rva type_descriptor RTTI descriptor of the exception type. 0=any (ellipsis)
    uint32_t disp_catch_obj; // ebp-based offset of the exception object in the function stack.  0 = no object (catch by type)
    uint32_t p_address_of_handler; // address of the catch handler code. returns address where to continues execution (i.e. code after the try block)
    uint32_t gs_handler_value;
};
struct cxx_es_type_list {
    uint32_t count; // number of entries in the list
    uint32_t type_array; //rva handler_type list of exceptions; it seems only pType field in HandlerType is used
};

struct cxx_function_desc {
    uint32_t magic_number;
    uint32_t max_state;
    uint32_t p_unwind_map;
    uint32_t try_blocks;
    uint32_t p_try_block_map;
    uint32_t ip_map_entries;
    uint32_t p_ip_to_state_map;
    uint32_t unwind_help;
    uint32_t p_es_type_list; // VC7+ only
    uint32_t eh_flags;       // VC8+ only bit 0 set if function was compiled with /EHs
};

struct cxx_unwind_map_entry {
    uint32_t to_state;   // target state
    uint32_t p_action;   // action to perform (unwind funclet address)
};

struct cxx_try_block_map_entry {
    uint32_t try_low;
    uint32_t try_high;      // this try {} covers states ranging from tryLow to tryHigh
    uint32_t catch_high;    // highest state inside catch handlers of this try
    uint32_t catches;       // number of catch handlers
    uint32_t p_handler_array; // rva handler_type catch handlers table
};

struct cxx_ip_to_state_map_entry {
    uint32_t ip; // Image relative offset of IP
    uint32_t state;
};

#pragma pack(pop)


class cxx_try_block_map_info {
    uint32_t try_low;
    uint32_t try_high;
    uint32_t catch_high;
    uint32_t catches;
    uint32_t p_handler_array;

    std::vector<cxx_handler_type> handler_entries;
public:
    cxx_try_block_map_info();
    cxx_try_block_map_info(const cxx_try_block_map_info& info);
    cxx_try_block_map_info(const cxx_try_block_map_entry& info);
    ~cxx_try_block_map_info();

    cxx_try_block_map_info& operator=(const cxx_try_block_map_info& info);
public:
    void set_try_low(uint32_t try_low);
    void set_try_high(uint32_t try_high);
    void set_catch_high(uint32_t catch_high);
    void set_catches(uint32_t catches);
    void set_p_handler_array(uint32_t p_handler_array);

public:

    uint32_t get_try_low() const;
    uint32_t get_try_high() const;
    uint32_t get_catch_high() const;
    uint32_t get_catches() const;
    uint32_t get_p_handler_array() const;

    std::vector<cxx_handler_type>& get_handler_entries();
    const std::vector<cxx_handler_type>& get_handler_entries() const;
};

class cxx_exception_func_info {

    uint32_t magic_number;
    uint32_t max_state;
    uint32_t p_unwind_map;
    uint32_t try_blocks;
    uint32_t p_try_block_map;
    uint32_t ip_map_entries;
    uint32_t p_ip_to_state_map;
    uint32_t unwind_help;
    uint32_t p_es_type_list;
    uint32_t eh_flags;

    std::vector<cxx_unwind_map_entry> unwind_map_entries;
    std::vector<cxx_try_block_map_info> try_block_map_entries;
    std::vector<cxx_ip_to_state_map_entry> ip_to_state_map_entries;
public:
    cxx_exception_func_info();
    cxx_exception_func_info(const cxx_exception_func_info& info);
    cxx_exception_func_info(const cxx_function_desc& desc);
    ~cxx_exception_func_info();

    cxx_exception_func_info& operator=(const cxx_exception_func_info& info);
public:
    void set_magic_number(uint32_t magic_number);
    void set_max_state(uint32_t max_state);
    void set_p_unwind_map(uint32_t p_unwind_map);
    void set_try_blocks(uint32_t try_blocks);
    void set_p_try_block_map(uint32_t p_try_block_map);
    void set_ip_map_entries(uint32_t ip_map_entries);
    void set_p_ip_to_state_map(uint32_t p_ip_to_state_map);
    void set_unwind_help(uint32_t unwind_help);
    void set_p_es_type_list(uint32_t p_es_type_list);
    void set_eh_flags(uint32_t eh_flags);

public:
    uint32_t get_magic_number() const;
    uint32_t get_max_state() const;
    uint32_t get_p_unwind_map() const;
    uint32_t get_try_blocks() const;
    uint32_t get_p_try_block_map() const;
    uint32_t get_ip_map_entries() const;
    uint32_t get_p_ip_to_state_map() const;
    uint32_t get_unwind_help() const;
    uint32_t get_p_es_type_list() const;
    uint32_t get_eh_flags() const;

    std::vector<cxx_unwind_map_entry> &get_unwind_map_entries();
    const std::vector<cxx_unwind_map_entry> &get_unwind_map_entries() const;
    std::vector<cxx_try_block_map_info> &get_try_block_map_entries();
    const std::vector<cxx_try_block_map_info> &get_try_block_map_entries() const;
    std::vector<cxx_ip_to_state_map_entry> &get_ip_to_state_map_entries();
    const std::vector<cxx_ip_to_state_map_entry> &get_ip_to_state_map_entries() const;
};

struct c_specific_handler_parameters_data {
    std::vector<cxx_scope_table_entry> table;

    c_specific_handler_parameters_data(){}
    c_specific_handler_parameters_data(const std::vector<cxx_scope_table_entry>& table)
        : table(table) {}
    c_specific_handler_parameters_data(const c_specific_handler_parameters_data& data)
        : table(data.table){}
};

struct delphi_specific_handler_parameters_data {
    std::vector<delphi_scope_table_entry> table;

    delphi_specific_handler_parameters_data() {}
    delphi_specific_handler_parameters_data(const std::vector<delphi_scope_table_entry>& table)
        : table(table) {}
    delphi_specific_handler_parameters_data(const delphi_specific_handler_parameters_data& data)
        : table(data.table) {}
};

struct llvm_specific_handler_parameters_data {
    uint32_t data_rva;

    llvm_specific_handler_parameters_data() 
        : data_rva(0){}
    llvm_specific_handler_parameters_data(uint32_t data_rva)
        : data_rva(data_rva) {}
    llvm_specific_handler_parameters_data(const llvm_specific_handler_parameters_data& data)
        : data_rva(data.data_rva) {}
};

struct gs_handler_check_parameters_data {
    uint32_t gs_data;

    gs_handler_check_parameters_data()
        : gs_data(0){}
    gs_handler_check_parameters_data(uint32_t gs_data)
        : gs_data(gs_data) {}
    gs_handler_check_parameters_data(const gs_handler_check_parameters_data& data)
        : gs_data(data.gs_data) {}
};

struct gs_handler_check_seh_parameters_data {
    std::vector<cxx_scope_table_entry> table;
    uint32_t gs_data;

    gs_handler_check_seh_parameters_data()
        : gs_data(0){}
    gs_handler_check_seh_parameters_data(const std::vector<cxx_scope_table_entry>& table, uint32_t gs_data)
        : table(table), gs_data(gs_data) {}
    gs_handler_check_seh_parameters_data(const gs_handler_check_seh_parameters_data& data)
        : table(data.table), gs_data(data.gs_data) {};
};

struct cxx_frame_handler3_parameters_data {
    cxx_exception_func_info func_info;

    cxx_frame_handler3_parameters_data() {}
    cxx_frame_handler3_parameters_data(const cxx_exception_func_info& func_info)
        : func_info(func_info) {}
    cxx_frame_handler3_parameters_data(const cxx_frame_handler3_parameters_data& data) 
        : func_info(data.func_info) {}
};

struct gs_handler_check_eh_parameters_data {
    cxx_exception_func_info func_info;
    uint32_t gs_data;

    gs_handler_check_eh_parameters_data()
        : gs_data(0) {}
    gs_handler_check_eh_parameters_data(const cxx_exception_func_info& func_info, uint32_t gs_data)
        : func_info(func_info), gs_data(gs_data) {}
    gs_handler_check_eh_parameters_data(const gs_handler_check_eh_parameters_data& data)
        : func_info(data.func_info), gs_data(data.gs_data) {};
};


class exceptions_handler_specific_data {
    void * data;
    exception_handler_type data_type;
public:
    exceptions_handler_specific_data();
    exceptions_handler_specific_data(const exceptions_handler_specific_data& data);
    ~exceptions_handler_specific_data();

    exceptions_handler_specific_data& operator=(const exceptions_handler_specific_data& data);

    exceptions_handler_specific_data& operator=(const c_specific_handler_parameters_data& data);
    exceptions_handler_specific_data& operator=(const delphi_specific_handler_parameters_data& data);
    exceptions_handler_specific_data& operator=(const llvm_specific_handler_parameters_data& data);
    exceptions_handler_specific_data& operator=(const gs_handler_check_parameters_data& data);
    exceptions_handler_specific_data& operator=(const gs_handler_check_seh_parameters_data& data);
    exceptions_handler_specific_data& operator=(const cxx_frame_handler3_parameters_data& data);
    exceptions_handler_specific_data& operator=(const gs_handler_check_eh_parameters_data& data);
public:
    void set_c_specific_handler_parameters_data(const  c_specific_handler_parameters_data& data);
    void set_delphi_specific_handler_parameters_data(const  delphi_specific_handler_parameters_data& data);
    void set_llvm_specific_handler_parameters_data(const  llvm_specific_handler_parameters_data& data);
    void set_gs_handler_check_parameters_data(const  gs_handler_check_parameters_data& data);
    void set_gs_handler_check_seh_parameters_data(const  gs_handler_check_seh_parameters_data& data);
    void set_cxx_frame_handler3_parameters_data(const  cxx_frame_handler3_parameters_data& data);
    void set_gs_handler_check_eh_parameters_data(const  gs_handler_check_eh_parameters_data& data);

public:
    const c_specific_handler_parameters_data* get_c_specific_handler_parameters_data() const;
    const delphi_specific_handler_parameters_data* get_delphi_specific_handler_parameters_data() const;
    const llvm_specific_handler_parameters_data* get_llvm_specific_handler_parameters_data() const;
    const gs_handler_check_parameters_data* get_gs_handler_check_parameters_data() const;
    const gs_handler_check_seh_parameters_data* get_gs_handler_check_seh_parameters_data() const;
    const cxx_frame_handler3_parameters_data* get_cxx_frame_handler3_parameters_data() const;
    const gs_handler_check_eh_parameters_data* get_gs_handler_check_eh_parameters_data() const;

    c_specific_handler_parameters_data* get_c_specific_handler_parameters_data();
    delphi_specific_handler_parameters_data* get_delphi_specific_handler_parameters_data();
    llvm_specific_handler_parameters_data* get_llvm_specific_handler_parameters_data();
    gs_handler_check_parameters_data* get_gs_handler_check_parameters_data();
    gs_handler_check_seh_parameters_data* get_gs_handler_check_seh_parameters_data();
    cxx_frame_handler3_parameters_data* get_cxx_frame_handler3_parameters_data();
    gs_handler_check_eh_parameters_data* get_gs_handler_check_eh_parameters_data();

    exception_handler_type get_data_type() const;
};


enum ex_exceptions_info_result {
    ex_exceptions_info_ok,
    ex_exceptions_info_has_unknown,
    ex_exceptions_info_has_error,
};

ex_exceptions_info_result get_extended_exception_info(_Inout_ pe_image_full& image_full);
ex_exceptions_info_result get_extended_exception_info_placement(_In_ const pe_image_full& image_full, _Inout_ pe_placement& placement);

void build_extended_exceptions_info(_Inout_ pe_image_full& image_full);
