#pragma once

#pragma pack(push,1)

//
// Image Format
//

#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

typedef struct _image_dos_header {          // DOS .EXE header
    uint16_t   e_magic;                     // Magic number
    uint16_t   e_cblp;                      // Bytes on last page of file
    uint16_t   e_cp;                        // Pages in file
    uint16_t   e_crlc;                      // Relocations
    uint16_t   e_cparhdr;                   // Size of header in paragraphs
    uint16_t   e_minalloc;                  // Minimum extra paragraphs needed
    uint16_t   e_maxalloc;                  // Maximum extra paragraphs needed
    uint16_t   e_ss;                        // Initial (relative) SS value
    uint16_t   e_sp;                        // Initial SP value
    uint16_t   e_csum;                      // Checksum
    uint16_t   e_ip;                        // Initial IP value
    uint16_t   e_cs;                        // Initial (relative) CS value
    uint16_t   e_lfarlc;                    // File address of relocation table
    uint16_t   e_ovno;                      // Overlay number
    uint16_t   e_res[4];                    // Reserved uint16_ts
    uint16_t   e_oemid;                     // OEM identifier (for e_oeminfo)
    uint16_t   e_oeminfo;                   // OEM information; e_oemid specific
    uint16_t   e_res2[10];                  // Reserved uint16_ts
    uint32_t   e_lfanew;                    // File address of new exe header
} image_dos_header, *pimage_dos_header;

//
// File header format.
//

#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved external references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Aggressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_TARGET_HOST       0x0001  // Useful for indicating we want to interact with the host and not a WoW guest.
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2  // ARM Thumb/Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_ARMNT             0x01c4  // ARM Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_ARM64             0xAA64  // ARM64 Little-Endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE


typedef struct _image_file_header {
    uint16_t   machine;
    uint16_t   number_of_sections;
    uint32_t   time_date_stamp;
    uint32_t   pointer_to_symbol_table;
    uint32_t   number_of_symbols;
    uint16_t   size_of_optional_header;
    uint16_t   characteristics;
} image_file_header, *pimage_file_header;

//
// Directory format.
//

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor


typedef struct _image_data_directory {
    uint32_t   virtual_address;
    uint32_t   size;
} image_data_directory, *pimage_data_directory;

//
// Optional header format.
//

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC       0x107

#define IMAGE_SUBSYSTEM_UNKNOWN                  0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE                   1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI              2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI              3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI                  5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI                7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS           8   // image is a native Win9x driver.
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI           9   // Image runs in the Windows CE subsystem.
#define IMAGE_SUBSYSTEM_EFI_APPLICATION          10  //
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11  //
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER       12  //
#define IMAGE_SUBSYSTEM_EFI_ROM                  13
#define IMAGE_SUBSYSTEM_XBOX                     14
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16
#define IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG        17

#define IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA        0x0020  // Image can handle a high entropy 64-bit virtual address space.
#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE           0x0040  // DLL can move.
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY        0x0080  // Code Integrity Image
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT              0x0100  // Image is NX compatible
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION           0x0200  // Image understands isolation and doesn't want it
#define IMAGE_DLLCHARACTERISTICS_NO_SEH                 0x0400  // Image does not use SEH.  No SE handler may reside in this image
#define IMAGE_DLLCHARACTERISTICS_NO_BIND                0x0800  // Do not bind this image.
#define IMAGE_DLLCHARACTERISTICS_APPCONTAINER           0x1000  // Image should execute in an AppContainer
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER             0x2000  // Driver uses WDM model
#define IMAGE_DLLCHARACTERISTICS_GUARD_CF               0x4000  // Image supports Control Flow Guard.
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE  0x8000

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

typedef struct _image_optional_header {
    uint16_t   magic;
    uint8_t    major_linker_version;
    uint8_t    minor_linker_version;
    uint32_t   size_of_code;
    uint32_t   size_of_initialized_data;
    uint32_t   size_of_uninitialized_data;
    uint32_t   address_of_entry_point;
    uint32_t   base_of_code;
    uint32_t   base_of_data;
    uint32_t   image_base;
    uint32_t   section_alignment;
    uint32_t   file_alignment;
    uint16_t   major_operating_system_version;
    uint16_t   minor_operating_system_version;
    uint16_t   major_image_version;
    uint16_t   minor_image_version;
    uint16_t   major_subsystem_version;
    uint16_t   minor_subsystem_version;
    uint32_t   win32_version_value;
    uint32_t   size_of_image;
    uint32_t   size_of_headers;
    uint32_t   checksum;
    uint16_t   subsystem;
    uint16_t   dll_characteristics;
    uint32_t   size_of_stack_reserve;
    uint32_t   size_of_stack_commit;
    uint32_t   size_of_heap_reserve;
    uint32_t   size_of_heap_commit;
    uint32_t   loader_flags;
    uint32_t   number_of_rva_and_sizes;
    image_data_directory data_directory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} image_optional_header32, *pimage_optional_header32;

typedef struct _image_optional_header64 {
    uint16_t   magic;
    uint8_t    major_linker_version;
    uint8_t    minor_linker_version;
    uint32_t   size_of_code;
    uint32_t   size_of_initialized_data;
    uint32_t   size_of_uninitialized_data;
    uint32_t   address_of_entry_point;
    uint32_t   base_of_code;
    uint64_t   image_base;
    uint32_t   section_alignment;
    uint32_t   file_alignment;
    uint16_t   major_operating_system_version;
    uint16_t   minor_operating_system_version;
    uint16_t   major_image_version;
    uint16_t   minor_image_version;
    uint16_t   major_subsystem_version;
    uint16_t   minor_subsystem_version;
    uint32_t   win32_version_value;
    uint32_t   size_of_image;
    uint32_t   size_of_headers;
    uint32_t   checksum;
    uint16_t   subsystem;
    uint16_t   dll_characteristics;
    uint64_t   size_of_stack_reserve;
    uint64_t   size_of_stack_commit;
    uint64_t   size_of_heap_reserve;
    uint64_t   size_of_heap_commit;
    uint32_t   loader_flags;
    uint32_t   number_of_rva_and_sizes;
    image_data_directory data_directory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} image_optional_header64, *pimage_optional_header64;


typedef struct _image_nt_headers32 {
    uint32_t                signature;
    image_file_header       file_header;
    image_optional_header32 optional_header;
} image_nt_headers32, *pimage_nt_headers32;

typedef struct _image_nt_headers64 {
    uint32_t                signature;
    image_file_header       file_header;
    image_optional_header64 optional_header;
} image_nt_headers64, *pimage_nt_headers64;

//
// Section header format.
//
#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.

#define IMAGE_SCN_NO_DEFER_SPEC_EXC          0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define IMAGE_SCN_GPREL                      0x00008000  // Section content can be accessed relative to GP
#define IMAGE_SCN_MEM_FARDATA                0x00008000
#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000  //
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000  //
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000  //
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000  //
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000  //
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000  //
#define IMAGE_SCN_ALIGN_128BYTES             0x00800000  //
#define IMAGE_SCN_ALIGN_256BYTES             0x00900000  //
#define IMAGE_SCN_ALIGN_512BYTES             0x00A00000  //
#define IMAGE_SCN_ALIGN_1024BYTES            0x00B00000  //
#define IMAGE_SCN_ALIGN_2048BYTES            0x00C00000  //
#define IMAGE_SCN_ALIGN_4096BYTES            0x00D00000  //
#define IMAGE_SCN_ALIGN_8192BYTES            0x00E00000  //
#define IMAGE_SCN_ALIGN_MASK                 0x00F00000  // Unused

#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.

#define IMAGE_SIZEOF_SHORT_NAME              8


typedef struct _image_section_header {
    uint8_t    name[IMAGE_SIZEOF_SHORT_NAME];
    uint32_t   virtual_size;
    uint32_t   virtual_address;
    uint32_t   size_of_raw_data;
    uint32_t   pointer_to_raw_data;
    uint32_t   pointer_to_relocations;
    uint32_t   pointer_to_linenumbers;
    uint16_t   number_of_relocations;
    uint16_t   number_of_linenumbers;
    uint32_t   characteristics;
} image_section_header, *pimage_section_header;


//
// Export Format
//

typedef struct _image_export_directory {
    uint32_t   characteristics;
    uint32_t   time_date_stamp;
    uint16_t   major_version;
    uint16_t   minor_version;
    uint32_t   name;
    uint32_t   base;
    uint32_t   number_of_functions;
    uint32_t   number_of_names;
    uint32_t   address_of_functions;      // RVA from base of image
    uint32_t   address_of_names;          // RVA from base of image
    uint32_t   address_of_name_ordinals;  // RVA from base of image
}image_export_directory, *pimage_export_directory;

//
// Import Format
//


#ifndef IMAGE_ORDINAL_FLAG64
    #define IMAGE_ORDINAL_FLAG64 0x8000000000000000
#endif

#ifndef IMAGE_ORDINAL_FLAG32
    #define IMAGE_ORDINAL_FLAG32 0x80000000
#endif

#ifndef IMAGE_ORDINAL64
    #define IMAGE_ORDINAL64(Ordinal) (Ordinal & 0xffff)
#endif
#ifndef IMAGE_ORDINAL32
    #define IMAGE_ORDINAL32(Ordinal) (Ordinal & 0xffff)
#endif
#ifndef IMAGE_SNAP_BY_ORDINAL64
    #define IMAGE_SNAP_BY_ORDINAL64(Ordinal) ((Ordinal & IMAGE_ORDINAL_FLAG64) != 0)
#endif
#ifndef IMAGE_SNAP_BY_ORDINAL32
    #define IMAGE_SNAP_BY_ORDINAL32(Ordinal) ((Ordinal & IMAGE_ORDINAL_FLAG32) != 0)
#endif


typedef struct _image_import_by_name {
    uint16_t    hint;
    uint8_t     name[1];
} image_import_by_name, *pimage_import_by_name;

typedef struct _image_thunk_data64 {
    union {
        uint64_t forwarder_string; // uint8_t * 
        uint64_t function;         // uint32_t *
        uint64_t ordinal;
        uint64_t address_of_data;  // pimage_import_by_name
    } u1;
} image_thunk_data64,*pimage_thunk_data64;

typedef struct _image_thunk_data32 {
    union {
        uint32_t forwarder_string;     // uint8_t *  
        uint32_t function;             // uint32_t *
        uint32_t ordinal;
        uint32_t address_of_data;      // pimage_import_by_name
    } u1;
} image_thunk_data32,*pimage_thunk_data32;


typedef struct _image_import_descriptor {
    union {
        uint32_t   characteristics;         // 0 for terminating null import descriptor
        uint32_t   original_first_thunk;    // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
    };
    uint32_t   time_date_stamp;             // 0 if not bound,
                                            // -1 if bound, and real date\time stamp
                                            //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                                            // O.W. date/time stamp of DLL bound to (Old BIND)

    uint32_t   forwarder_chain;             // -1 if no forwarders
    uint32_t   name;
    uint32_t   first_thunk;                 // RVA to IAT (if bound this IAT has actual addresses)
} image_import_descriptor,*pimage_import_descriptor;

//
// New format import descriptors pointed to by DataDirectory[ IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT ]
//

typedef struct _image_bound_import_descriptor {
    uint32_t   time_date_stamp;
    uint16_t   offset_module_name;
    uint16_t   number_of_module_forwarder_refs;
    // Array of zero or more IMAGE_BOUND_FORWARDER_REF follows
} image_bound_import_descriptor, *pimage_bound_import_descriptor;

typedef struct _image_bound_forwarded_ref {
    uint32_t   time_date_stamp;
    uint16_t   offset_module_name;
    uint16_t   reserved;
} image_bound_forwarded_ref, *pimage_bound_forwarded_ref;

typedef struct _image_delayload_descriptor {
    union {
        uint32_t all_attributes;
        struct {
            uint32_t rva_based : 1;             // Delay load version 2
            uint32_t reserved_attributes : 31;
        };
    } attributes;

    uint32_t dll_name_rva;                    // RVA to the name of the target library (NULL-terminate ASCII string)
    uint32_t module_handle_rva;               // RVA to the HMODULE caching location (PHMODULE)
    uint32_t import_address_table_rva;        // RVA to the start of the IAT (PIMAGE_THUNK_DATA)
    uint32_t import_name_table_rva;           // RVA to the start of the name table (PIMAGE_THUNK_DATA::AddressOfData)
    uint32_t bound_import_address_table_rva;  // RVA to an optional bound IAT
    uint32_t unload_information_table_rva;    // RVA to an optional unload info table
    uint32_t time_date_stamp;                 // 0 if not bound,
                                              // Otherwise, date/time of the target DLL

} image_delayload_descriptor, *pimage_delayload_descriptor;


//
// Resource Format.
//

#define IMAGE_RESOURCE_NAME_IS_STRING        0x80000000
#define IMAGE_RESOURCE_DATA_IS_DIRECTORY     0x80000000


typedef struct _image_resource_directory {
    uint32_t   characteristics;
    uint32_t   time_date_stamp;
    uint16_t   major_version;
    uint16_t   minor_version;
    uint16_t   number_of_named_entries;
    uint16_t   number_of_id_entries;
    //  image_resource_directory_entry directory_entries[];
} image_resource_directory, *pimage_resource_directory;

typedef struct _image_resource_directory_entry {
    union {
        struct {
            uint32_t name_offset    : 31;
            uint32_t name_is_string : 1;
        };
        uint32_t   name;
        uint16_t   id;
    };
    union {
        uint32_t   offset_to_data;
        struct {
            uint32_t   offset_to_directory : 31;
            uint32_t   data_is_directory   : 1;
        };
    };
} image_resource_directory_entry, *pimage_resource_directory_entry;

typedef struct _image_resource_directory_string {
    uint16_t  length;
    uint8_t   name_string[1];
} image_resource_directory_string, *pimage_resource_directory_string;


typedef struct _image_resource_dir_string_u {
    uint16_t  length;
    uint16_t  name_string[1];
} image_resource_dir_string_u, *pimage_resource_dir_string_u;

typedef struct _image_resource_data_entry {
    uint32_t   offset_to_data;
    uint32_t   size;
    uint32_t   code_page;
    uint32_t   reserved;
} image_resource_data_entry, *pimage_resource_data_entry;


//
//  Exception table format
//

typedef enum _unwind_op_codes {
    UWOP_PUSH_NONVOL = 0, /* info == register number */
    UWOP_ALLOC_LARGE,     /* no info, alloc size in next 2 slots */
    UWOP_ALLOC_SMALL,     /* info == size of allocation / 8 - 1 */
    UWOP_SET_FPREG,       /* no info, FP = RSP + UNWIND_INFO.FPRegOffset*16 */
    UWOP_SAVE_NONVOL,     /* info == register number, offset in next slot */
    UWOP_SAVE_NONVOL_FAR, /* info == register number, offset in next 2 slots */
    UWOP_SAVE_XMM128 = 8, /* info == XMM reg number, offset in next slot */
    UWOP_SAVE_XMM128_FAR, /* info == XMM reg number, offset in next 2 slots */
    UWOP_PUSH_MACHFRAME   /* info == 0: no error-code, 1: error-code */
} unwind_code_ops;

typedef union _unwind_code {
    struct {
        uint8_t code_offset;
        uint8_t unwind_op : 4;
        uint8_t op_info : 4;
    };
    uint16_t frame_offset;
} unwind_code, *punwind_code;

#ifndef UNW_FLAG_EHANDLER
    #define UNW_FLAG_EHANDLER  0x01
#endif

#ifndef UNW_FLAG_UHANDLER
    #define UNW_FLAG_UHANDLER  0x02
#endif

#ifndef UNW_FLAG_CHAININFO
    #define UNW_FLAG_CHAININFO 0x04
#endif


typedef struct _unwind_info {
    uint8_t version : 3;
    uint8_t flags   : 5;
    uint8_t size_of_prolog;
    uint8_t count_of_codes;
    uint8_t frame_register : 4;
    uint8_t frame_offset   : 4;
    //unwind_code code_entries[]
} unwind_info, *punwind_info;

typedef struct _runtime_function_entry {
    uint32_t begin_address;
    uint32_t end_address;
    union {
        uint32_t unwind_info_address;
        uint32_t unwind_data;
    };
} runtime_function_entry, *pruntime_function_entry;


//
// Win certificate format.
//

#define WIN_CERT_REVISION_1_0               (0x0100)
#define WIN_CERT_REVISION_2_0               (0x0200)

#define WIN_CERT_TYPE_X509                  (0x0001)   // bCertificate contains an X.509 Certificate
#define WIN_CERT_TYPE_PKCS_SIGNED_DATA      (0x0002)   // bCertificate contains a PKCS SignedData structure
#define WIN_CERT_TYPE_RESERVED_1            (0x0003)   // Reserved
#define WIN_CERT_TYPE_TS_STACK_SIGNED       (0x0004)   // Terminal Server Protocol Stack Certificate signing
#define WIN_CERT_TYPE_PKCS1_SIGN            (0x0009)   // Certificate is PKCS1 signature.

typedef struct _win_certificate {
    uint32_t   length;
    uint16_t   w_revision;
    uint16_t   w_certificate_type;
   // uint8_t    b_certificate[1];
} win_certificate, *pwin_certificate;


//
// Based relocation format.
//

#define IMAGE_REL_BASED_ABSOLUTE              0
#define IMAGE_REL_BASED_HIGH                  1
#define IMAGE_REL_BASED_LOW                   2
#define IMAGE_REL_BASED_HIGHLOW               3
#define IMAGE_REL_BASED_HIGHADJ               4
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_5    5
#define IMAGE_REL_BASED_RESERVED              6
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_7    7
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_8    8
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_9    9
#define IMAGE_REL_BASED_DIR64                 10

//
// I386 relocation types.
//
#define IMAGE_REL_I386_ABSOLUTE         0x0000  // Reference is absolute, no relocation is necessary
#define IMAGE_REL_I386_DIR16            0x0001  // Direct 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_REL16            0x0002  // PC-relative 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32            0x0006  // Direct 32-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32NB          0x0007  // Direct 32-bit reference to the symbols virtual address, base not included
#define IMAGE_REL_I386_SEG12            0x0009  // Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
#define IMAGE_REL_I386_SECTION          0x000A
#define IMAGE_REL_I386_SECREL           0x000B
#define IMAGE_REL_I386_TOKEN            0x000C  // clr token
#define IMAGE_REL_I386_SECREL7          0x000D  // 7 bit offset from base of section containing target
#define IMAGE_REL_I386_REL32            0x0014  // PC-relative 32-bit reference to the symbols virtual address

//
// x64 relocations
//
#define IMAGE_REL_AMD64_ABSOLUTE        0x0000  // Reference is absolute, no relocation is necessary
#define IMAGE_REL_AMD64_ADDR64          0x0001  // 64-bit address (VA).
#define IMAGE_REL_AMD64_ADDR32          0x0002  // 32-bit address (VA).
#define IMAGE_REL_AMD64_ADDR32NB        0x0003  // 32-bit address w/o image base (RVA).
#define IMAGE_REL_AMD64_REL32           0x0004  // 32-bit relative address from byte following reloc
#define IMAGE_REL_AMD64_REL32_1         0x0005  // 32-bit relative address from byte distance 1 from reloc
#define IMAGE_REL_AMD64_REL32_2         0x0006  // 32-bit relative address from byte distance 2 from reloc
#define IMAGE_REL_AMD64_REL32_3         0x0007  // 32-bit relative address from byte distance 3 from reloc
#define IMAGE_REL_AMD64_REL32_4         0x0008  // 32-bit relative address from byte distance 4 from reloc
#define IMAGE_REL_AMD64_REL32_5         0x0009  // 32-bit relative address from byte distance 5 from reloc
#define IMAGE_REL_AMD64_SECTION         0x000A  // Section index
#define IMAGE_REL_AMD64_SECREL          0x000B  // 32 bit offset from base of section containing target
#define IMAGE_REL_AMD64_SECREL7         0x000C  // 7 bit unsigned offset from base of section containing target
#define IMAGE_REL_AMD64_TOKEN           0x000D  // 32 bit metadata token
#define IMAGE_REL_AMD64_SREL32          0x000E  // 32 bit signed span-dependent value emitted into object
#define IMAGE_REL_AMD64_PAIR            0x000F
#define IMAGE_REL_AMD64_SSPAN32         0x0010  // 32 bit signed span-dependent value applied at link time

typedef struct _image_base_relocation {
    uint32_t   virtual_address;
    uint32_t   size_of_block;
} image_base_relocation,*pimage_base_relocation;


//
// Debug Format
//

#define IMAGE_DEBUG_TYPE_UNKNOWN          0
#define IMAGE_DEBUG_TYPE_COFF             1
#define IMAGE_DEBUG_TYPE_CODEVIEW         2
#define IMAGE_DEBUG_TYPE_FPO              3
#define IMAGE_DEBUG_TYPE_MISC             4
#define IMAGE_DEBUG_TYPE_EXCEPTION        5
#define IMAGE_DEBUG_TYPE_FIXUP            6
#define IMAGE_DEBUG_TYPE_OMAP_TO_SRC      7
#define IMAGE_DEBUG_TYPE_OMAP_FROM_SRC    8
#define IMAGE_DEBUG_TYPE_BORLAND          9
#define IMAGE_DEBUG_TYPE_RESERVED10       10
#define IMAGE_DEBUG_TYPE_CLSID            11
#define IMAGE_DEBUG_TYPE_VC_FEATURE       12
#define IMAGE_DEBUG_TYPE_POGO             13
#define IMAGE_DEBUG_TYPE_ILTCG            14
#define IMAGE_DEBUG_TYPE_MPX              15
#define IMAGE_DEBUG_TYPE_REPRO            16

#define FRAME_FPO       0
#define FRAME_TRAP      1
#define FRAME_TSS       2
#define FRAME_NONFPO    3

#define IMAGE_DEBUG_MISC_EXENAME    1

#ifndef _MAC
    #define IMAGE_SEPARATE_DEBUG_SIGNATURE 0x4944
    #define NON_PAGED_DEBUG_SIGNATURE      0x494E
#else
    #define IMAGE_SEPARATE_DEBUG_SIGNATURE 0x4449  // DI
    #define NON_PAGED_DEBUG_SIGNATURE      0x4E49  // NI
#endif

#define IMAGE_SEPARATE_DEBUG_FLAGS_MASK 0x8000
#define IMAGE_SEPARATE_DEBUG_MISMATCH   0x8000  // when DBG was updated, the
                                                // old checksum didn't match.

typedef struct _image_debug_directory {
    uint32_t   characteristics;
    uint32_t   time_date_stamp;
    uint16_t   major_version;
    uint16_t   minor_version;
    uint32_t   type;
    uint32_t   size_of_data;
    uint32_t   address_of_raw_data;
    uint32_t   pointer_to_raw_data;
} image_debug_directory, *pimage_debug_directory;

typedef struct _image_coff_symbols_header {
    uint32_t   number_of_symbols;
    uint32_t   lva_to_first_symbol;
    uint32_t   number_of_linenumbers;
    uint32_t   lva_to_first_linenumber;
    uint32_t   rva_to_first_byte_of_code;
    uint32_t   rva_to_last_byte_of_code;
    uint32_t   rva_to_first_byte_of_data;
    uint32_t   rva_to_last_byte_of_data;
} image_coff_symbols_header, *pimage_coff_symbols_header;

typedef struct _fpo_data {
    uint32_t   ul_off_start;    // offset 1st byte of function code
    uint32_t   cb_proc_size;    // # bytes in function
    uint32_t   cdw_locals;      // # bytes in locals/4
    uint16_t   cdw_params;      // # bytes in params/4
    uint16_t   cb_prolog : 8;   // # bytes in prolog
    uint16_t   cb_regs   : 3;   // # regs saved
    uint16_t   f_has_seh : 1;   // TRUE if SEH in func
    uint16_t   f_use_bp  : 1;   // TRUE if EBP has been allocated
    uint16_t   reserved  : 1;   // reserved for future use
    uint16_t   cb_frame  : 2;   // frame type
} fpo_data, *pfpo_data;

typedef struct _image_debug_misc {
    uint32_t   data_type;        // type of misc data, see defines
    uint32_t   length;           // total length of record, rounded to four
                                 // byte multiple.
    uint8_t    unicode;          // TRUE if data is unicode string
    uint8_t    reserved[3];
    uint8_t    data[1];          // Actual data
} image_debug_misc, *pimage_debug_misc;

typedef struct _image_separate_debug_header {
    uint16_t   signature;
    uint16_t   flags;
    uint16_t   machine;
    uint16_t   characteristics;
    uint32_t   time_date_stamp;
    uint32_t   check_sum;
    uint32_t   image_base;
    uint32_t   size_of_image;
    uint32_t   number_of_sections;
    uint32_t   exported_names_size;
    uint32_t   debug_directory_size;
    uint32_t   section_alignment;
    uint32_t   reserved[2];
} image_separate_debug_header, *pimage_separate_debug_header;


typedef struct _non_paged_debug_info {
    uint16_t   signature;
    uint16_t   flags;
    uint32_t   size;
    uint16_t   machine;
    uint16_t   characteristics;
    uint32_t   time_date_stamp;
    uint32_t   check_sum;
    uint64_t   image_base;
    //DebugDirectorySize
    //image_debug_directory
} non_paged_debug_info, *pnon_paged_debug_info;


//
// Thread Local Storage
//

typedef void(*pimage_tls_callback) (void* dll_handle, uint32_t reason, void * reserved);

typedef struct _image_tls_directory64 {
    uint64_t start_address_of_raw_data;
    uint64_t end_address_of_raw_data;
    uint64_t address_of_index;         // uint32_t *
    uint64_t address_of_callbacks;     // pimage_tls_callback *;
    uint32_t size_of_zero_fill;
    union {
        uint32_t characteristics;
        struct {
            uint32_t reserved0 : 20;
            uint32_t alignment : 4;
            uint32_t reserved1 : 8;
        };
    };

} image_tls_directory64, *pimage_tls_directory64;

typedef struct _image_tls_directory32 {
    uint32_t   start_address_of_raw_data;
    uint32_t   end_address_of_raw_data;
    uint32_t   address_of_index;         // uint32_t *
    uint32_t   address_of_callbacks;     // pimage_tls_callback *
    uint32_t   size_of_zero_fill;
    union {
        uint32_t characteristics;
        struct {
            uint32_t reserved0 : 20;
            uint32_t alignment : 4;
            uint32_t reserved1 : 8;
        };
    };

} image_tls_directory32, *pimage_tls_directory32;


//
// Load Configuration Directory Entry
//

#define IMAGE_HOT_PATCH_BASE_OBLIGATORY     0x00000001

#define IMAGE_HOT_PATCH_CHUNK_INVERSE       0x80000000
#define IMAGE_HOT_PATCH_CHUNK_OBLIGATORY    0x40000000
#define IMAGE_HOT_PATCH_CHUNK_RESERVED      0x3FF03000
#define IMAGE_HOT_PATCH_CHUNK_TYPE          0x000FC000
#define IMAGE_HOT_PATCH_CHUNK_SOURCE_RVA    0x00008000
#define IMAGE_HOT_PATCH_CHUNK_TARGET_RVA    0x00004000
#define IMAGE_HOT_PATCH_CHUNK_SIZE          0x00000FFF

#define IMAGE_HOT_PATCH_NONE                0x00000000
#define IMAGE_HOT_PATCH_FUNCTION            0x0001C000
#define IMAGE_HOT_PATCH_ABSOLUTE            0x0002C000
#define IMAGE_HOT_PATCH_REL32               0x0003C000
#define IMAGE_HOT_PATCH_CALL_TARGET         0x00044000
#define IMAGE_HOT_PATCH_INDIRECT            0x0005C000
#define IMAGE_HOT_PATCH_NO_CALL_TARGET      0x00064000
#define IMAGE_HOT_PATCH_DYNAMIC_VALUE       0x00078000

#define IMAGE_GUARD_CF_INSTRUMENTED                    0x00000100 // Module performs control flow integrity checks using system-supplied support
#define IMAGE_GUARD_CFW_INSTRUMENTED                   0x00000200 // Module performs control flow and write integrity checks
#define IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT          0x00000400 // Module contains valid control flow target metadata
#define IMAGE_GUARD_SECURITY_COOKIE_UNUSED             0x00000800 // Module does not make use of the /GS security cookie
#define IMAGE_GUARD_PROTECT_DELAYLOAD_IAT              0x00001000 // Module supports read only delay load IAT
#define IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION   0x00002000 // Delayload import table in its own .didat section (with nothing else in it) that can be freely reprotected
#define IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT 0x00004000 // Module contains suppressed export information. This also infers that the address taken
// taken IAT table is also present in the load config.
#define IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION       0x00008000 // Module enables suppression of exports
#define IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT          0x00010000 // Module contains longjmp target information
#define IMAGE_GUARD_RF_INSTRUMENTED                    0x00020000 // Module contains return flow instrumentation and metadata
#define IMAGE_GUARD_RF_ENABLE                          0x00040000 // Module requests that the OS enable return flow protection
#define IMAGE_GUARD_RF_STRICT                          0x00080000 // Module requests that the OS enable return flow protection in strict mode
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK        0xF0000000 // Stride of Guard CF function table encoded in these bits (additional count of bytes per element)
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT       28         // Shift to right-justify Guard CF function table stride

#define IMAGE_DYNAMIC_RELOCATION_GUARD_RF_PROLOGUE   0x00000001
#define IMAGE_DYNAMIC_RELOCATION_GUARD_RF_EPILOGUE   0x00000002


typedef struct _image_load_config_code_integrity {
    uint16_t   flags;          // Flags to indicate if CI information is available, etc.
    uint16_t   catalog;        // 0xFFFF means not available
    uint32_t   catalog_offset;
    uint32_t   reserved;       // Additional bitmask to be defined later
} image_load_config_code_integrity, *pimage_load_config_code_integrity;


typedef struct _image_dynamic_relocation32 {
    uint32_t   symbol;
    uint32_t   base_reloc_size;
    //image_base_relocation base_relocations[0];
} image_dynamic_relocation32, *pimage_dynamic_relocation32;

typedef struct _image_dynamic_relocation64 {
    uint64_t   symbol;
    uint32_t   base_reloc_size;
    //image_base_relocation base_relocations[0];
} image_dynamic_relocation64, *pimage_dynamic_relocation64;

typedef struct _image_dynamic_relocation32_v2 {
    uint32_t   header_size;
    uint32_t   fixup_info_size;
    uint32_t   symbol;
    uint32_t   symbol_group;
    uint32_t   flags;
    // ...     variable length header fields
    // uint8_t    fixup_info[fixup_info_size]
} image_dynamic_relocation32_v2, *pimage_dynamic_relocation32_v2;

typedef struct _image_dynamic_relocation64_v2 {
    uint32_t   header_size;
    uint32_t   fixup_info_size;
    uint64_t   symbol;
    uint32_t   symbol_group;
    uint32_t   flags;
    // ...     variable length header fields
    // uint8_t    fixup_info[fixup_info_size]
} image_dynamic_relocation64_v2, *pimage_dynamic_relocation64_v2;

typedef struct _image_dynamic_relocation_table {
    uint32_t   version;
    uint32_t   size;
    //  image_dynamic_relocation(32/64) DynamicRelocations[0];
} image_dynamic_relocation_table, *pimage_dynamic_relocation_table;

typedef struct _image_prologue_dynamic_relocation_header {
    uint8_t    prologue_byte_count;
    // uint8_t    PrologueBytes[PrologueByteCount];
} image_prologue_dynamic_relocation_header,*pimage_prologue_dynamic_relocation_header;

typedef struct _image_epilogue_dynamic_relocation_header {
    uint32_t   epilogue_count;
    uint8_t    epilogue_byte_count;
    uint8_t    branch_descriptor_element_size;
    uint16_t   branch_descriptor_count;
    // uint8_t    BranchDescriptors[...];
    // uint8_t    BranchDescriptorBitMap[...];
} image_epilogue_dynamic_relocation_header,*pimage_epilogue_dynamic_relocation_header;


typedef struct _image_hot_patch_info {
    uint32_t version;
    uint32_t size;
    uint32_t sequence_number;
    uint32_t base_image_list;
    uint32_t base_image_count;
    uint32_t buffer_offset; // V2 and later
} image_hot_patch_info, *pimage_hot_patch_info;

typedef struct _image_hot_patch_base {
    uint32_t sequence_number;
    uint32_t flags;
    uint32_t original_time_date_stamp;
    uint32_t original_check_sum;
    uint32_t code_integrity_info;
    uint32_t code_integrity_size;
    uint32_t patch_table;
    uint32_t buffer_offset; // V2 and later
} image_hot_patch_base, *pimage_hot_patch_base;

typedef struct _image_hot_patch_hashes {
    uint8_t  SHA256[32];
    uint8_t  SHA1[20];
} image_hot_patch_hashes, *pimage_hot_patch_hashes;

#define LOAD_CONFIG_32_V1_SIZE 0x40
#define LOAD_CONFIG_32_V2_SIZE 0x48
#define LOAD_CONFIG_32_V3_SIZE 0x5C
#define LOAD_CONFIG_32_V4_SIZE 0x68
#define LOAD_CONFIG_32_V5_SIZE 0x78
#define LOAD_CONFIG_32_V6_SIZE 0x80
#define LOAD_CONFIG_32_V7_SIZE 0x90
#define LOAD_CONFIG_32_V8_SIZE 0x98
#define LOAD_CONFIG_32_V9_SIZE 0xA0

typedef struct _image_load_config_directory32 {
    uint32_t   size;
    uint32_t   time_date_stamp;
    uint16_t   major_version;
    uint16_t   minor_version;
    uint32_t   global_flags_clear;
    uint32_t   global_flags_set;
    uint32_t   critical_section_default_timeout;
    uint32_t   decommit_free_block_threshold;
    uint32_t   decommit_total_free_threshold;
    uint32_t   lock_prefix_table;                           // VA
    uint32_t   maximum_allocation_size;
    uint32_t   virtual_memory_threshold;
    uint32_t   process_heap_flags;
    uint32_t   process_affinity_mask;
    uint16_t   csd_version;
    uint16_t   dependent_load_flags;
    uint32_t   edit_list;                                   // VA
    uint32_t   security_cookie;                             // VA
    /*end of LOADCONFIG V1*/
    uint32_t   se_handler_table;                            // VA
    uint32_t   se_handler_count;
    /*end of LOADCONFIG V2*/
    uint32_t   guard_cf_check_function_pointer;             // VA
    uint32_t   guard_cf_dispatch_function_pointer;          // VA
    uint32_t   guard_cf_function_table;                     // VA
    uint32_t   guard_cf_function_count;
    uint32_t   guard_flags;
    /*end of LOADCONFIG V3*/
    image_load_config_code_integrity code_integrity;
    /*end of LOADCONFIG V4*/
    uint32_t   guard_address_taken_iat_entry_table;         // VA
    uint32_t   guard_address_taken_iat_entry_count;
    uint32_t   guard_long_jump_target_table;                // VA
    uint32_t   guard_long_jump_target_count;
    /*end of LOADCONFIG V5*/
    uint32_t   dynamic_value_reloc_table;                   // VA
    uint32_t   chpe_meta_data_pointer;
    /*end of LOADCONFIG V6*/
    uint32_t   guard_rf_failure_routine;                    // VA
    uint32_t   guard_rf_failure_routine_function_pointer;   // VA
    uint32_t   dynamic_value_reloc_table_offset;
    uint16_t   dynamic_value_reloc_table_section;
    uint16_t   reserved2;
    /*end of LOADCONFIG V7*/
    uint32_t   guard_rf_verify_stack_pointer_function_pointer; // VA
    uint32_t   hot_patch_table_offset;
    /*end of LOADCONFIG V8*/
    uint32_t   reserved3;
    uint32_t   enclave_configuration_pointer;               // VA
    /*end of LOADCONFIG V9*/
} image_load_config_directory32, *pimage_load_config_directory32;


#define LOAD_CONFIG_64_V1_SIZE 0x70
#define LOAD_CONFIG_64_V2_SIZE 0x94
#define LOAD_CONFIG_64_V3_SIZE 0xA0
#define LOAD_CONFIG_64_V4_SIZE 0xC0
#define LOAD_CONFIG_64_V5_SIZE 0xD0
#define LOAD_CONFIG_64_V6_SIZE 0xE8
#define LOAD_CONFIG_64_V7_SIZE 0xF4
#define LOAD_CONFIG_64_V8_SIZE 0x100

typedef struct _image_load_config_directory64 {
    uint32_t   size;
    uint32_t   time_date_stamp;
    uint16_t   major_version;
    uint16_t   minor_version;
    uint32_t   global_flags_clear;
    uint32_t   global_flags_set;
    uint32_t   critical_section_default_timeout;
    uint64_t   decommit_free_block_threshold;
    uint64_t   decommit_total_free_threshold;
    uint64_t   lock_prefix_table;                       // VA
    uint64_t   maximum_allocation_size;
    uint64_t   virtual_memory_threshold;
    uint64_t   process_affinity_mask;
    uint32_t   process_heap_flags;
    uint16_t   csd_version;
    uint16_t   dependent_load_flags;
    uint64_t   edit_list;                               // VA
    uint64_t   security_cookie;                         // VA
    uint64_t   se_handler_table;                        // VA
    uint64_t   se_handler_count;
    /*end of LOADCONFIG V1*/
    uint64_t   guard_cf_check_function_pointer;         // VA
    uint64_t   guard_cf_dispatch_function_pointer;      // VA
    uint64_t   guard_cf_function_table;                 // VA
    uint64_t   guard_cf_function_count;
    uint32_t   guard_flags;
    /*end of LOADCONFIG V2*/
    image_load_config_code_integrity code_integrity;
    /*end of LOADCONFIG V3*/
    uint64_t   guard_address_taken_iat_entry_table;     // VA
    uint64_t   guard_address_taken_iat_entry_count;
    uint64_t   guard_long_jump_target_table;            // VA
    uint64_t   guard_long_jump_target_count;
    /*end of LOADCONFIG V4*/
    uint64_t   dynamic_value_reloc_table;               // VA
    uint64_t   chpe_meta_data_pointer;                  // VA
    /*end of LOADCONFIG V5*/
    uint64_t   guard_rf_failure_routine;                // VA
    uint64_t   guard_rf_failure_routine_function_pointer; // VA
    uint32_t   dynamic_value_reloc_table_offset;
    uint16_t   dynamic_value_reloc_table_section;
    uint16_t   reserved2;
    /*end of LOADCONFIG V6*/
    uint64_t   guard_rf_verify_stack_pointer_function_pointer; // VA
    uint32_t   hot_patch_table_offset;
    /*end of LOADCONFIG V7*/
    uint32_t   reserved3;
    uint64_t   enclave_configuration_pointer;           // VA
    /*end of LOADCONFIG V8*/
} image_load_config_directory64, *pimage_load_config_directory64;


//
// CLR 2.0 header structure.
//

typedef struct image_cor20_header
{
    // Header versioning
    uint32_t   cb;
    uint16_t   major_runtime_version;
    uint16_t   minor_runtime_version;

    // Symbol table and startup information
    image_data_directory   meta_data;
    uint32_t               flags;

    // If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is not set, EntryPointToken represents a managed entrypoint.
    // If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is set, EntryPointRVA represents an RVA to a native entrypoint.
    union {
        uint32_t   entry_point_token;
        uint32_t   entry_point_rva;
    };

    // Binding information
    image_data_directory   resources;
    image_data_directory   strong_name_signature;

    // Regular fixup and binding information
    image_data_directory   code_manager_table;
    image_data_directory   vtable_fixups;
    image_data_directory   export_address_table_jumps;

    // Precompiled image info (internal use only - set to zero)
    image_data_directory   managed_native_header;

} image_cor20_header, *pimage_cor20_header;



namespace pe_resource_irnl {

#define VERSION_INFO_MAGIC L"VS_VERSION_INFO"
#define FIXED_FILE_INFO_MAGIC 0xfeef04bd
#define VAR_FILE_INFO_MAGIC  L"VarFileInfo"
#define VAR_MAGIC  L"Translation"
#define STRING_FILE_INFO_MAGIC  L"StringFileInfo"


#define VS_FFI_STRUCVERSION     0x00010000L
#define VS_FFI_FILEFLAGSMASK    0x0000003FL

    /* ----- VS_VERSION.dwFileFlags ----- */
#define VS_FF_DEBUG             0x00000001L
#define VS_FF_PRERELEASE        0x00000002L
#define VS_FF_PATCHED           0x00000004L
#define VS_FF_PRIVATEBUILD      0x00000008L
#define VS_FF_INFOINFERRED      0x00000010L
#define VS_FF_SPECIALBUILD      0x00000020L

/* ----- VS_VERSION.dwFileOS ----- */
#define VOS_UNKNOWN             0x00000000L
#define VOS_DOS                 0x00010000L
#define VOS_OS216               0x00020000L
#define VOS_OS232               0x00030000L
#define VOS_NT                  0x00040000L
#define VOS_WINCE               0x00050000L

#define VOS__BASE               0x00000000L
#define VOS__WINDOWS16          0x00000001L
#define VOS__PM16               0x00000002L
#define VOS__PM32               0x00000003L
#define VOS__WINDOWS32          0x00000004L

#define VOS_DOS_WINDOWS16       0x00010001L
#define VOS_DOS_WINDOWS32       0x00010004L
#define VOS_OS216_PM16          0x00020002L
#define VOS_OS232_PM32          0x00030003L
#define VOS_NT_WINDOWS32        0x00040004L

/* ----- VS_VERSION.dwFileType ----- */
#define VFT_UNKNOWN             0x00000000L
#define VFT_APP                 0x00000001L
#define VFT_DLL                 0x00000002L
#define VFT_DRV                 0x00000003L
#define VFT_FONT                0x00000004L
#define VFT_VXD                 0x00000005L
#define VFT_STATIC_LIB          0x00000007L

/* ----- VS_VERSION.dwFileSubtype for VFT_WINDOWS_DRV ----- */
#define VFT2_UNKNOWN            0x00000000L
#define VFT2_DRV_PRINTER        0x00000001L
#define VFT2_DRV_KEYBOARD       0x00000002L
#define VFT2_DRV_LANGUAGE       0x00000003L
#define VFT2_DRV_DISPLAY        0x00000004L
#define VFT2_DRV_MOUSE          0x00000005L
#define VFT2_DRV_NETWORK        0x00000006L
#define VFT2_DRV_SYSTEM         0x00000007L
#define VFT2_DRV_INSTALLABLE    0x00000008L
#define VFT2_DRV_SOUND          0x00000009L
#define VFT2_DRV_COMM           0x0000000AL
#define VFT2_DRV_INPUTMETHOD    0x0000000BL
#define VFT2_DRV_VERSIONED_PRINTER    0x0000000CL

/* ----- VS_VERSION.dwFileSubtype for VFT_WINDOWS_FONT ----- */
#define VFT2_FONT_RASTER        0x00000001L
#define VFT2_FONT_VECTOR        0x00000002L
#define VFT2_FONT_TRUETYPE      0x00000003L


    typedef struct _vs_fixed_file_info {
        uint32_t   signature;            /* e.g. 0xfeef04bd */
        uint32_t   struct_version;       /* e.g. 0x00000042 = "0.42" */
        uint32_t   file_version_ms;      /* e.g. 0x00030075 = "3.75" */
        uint32_t   file_version_ls;      /* e.g. 0x00000031 = "0.31" */
        uint32_t   product_version_ms;   /* e.g. 0x00030010 = "3.10" */
        uint32_t   product_version_ls;   /* e.g. 0x00000031 = "0.31" */
        uint32_t   file_flags_mask;      /* = 0x3F for version "0.42" */
        uint32_t   file_flags;           /* e.g. VFF_DEBUG | VFF_PRERELEASE */
        uint32_t   file_os;              /* e.g. VOS_DOS_WINDOWS16 */
        uint32_t   file_type;            /* e.g. VFT_DRIVER */
        uint32_t   file_sub_type;        /* e.g. VFT2_DRV_KEYBOARD */
        uint32_t   file_date_ms;         /* e.g. 0 */
        uint32_t   file_date_ls;         /* e.g. 0 */
    } vs_fixed_file_info;

    typedef struct _vs_version_info {
        uint16_t   length;
        uint16_t   value_length;
        uint16_t   type;
        wchar_t    sz_key;
        uint16_t   padding1;
        vs_fixed_file_info   value;
        uint16_t   padding2;
        uint16_t   children;
    } vs_version_info;

    typedef struct {
        uint16_t  length;
        uint16_t  value_length;
        uint16_t  type;
        wchar_t   sz_key;
        uint16_t  padding;
        uint16_t  value;
    } string;

    typedef struct {
        uint16_t   length;
        uint16_t   value_length;
        uint16_t   type;
        wchar_t    sz_key;
        uint16_t   padding;
        string     children;
    } string_table;

    typedef struct {
        uint16_t   length;
        uint16_t   value_length;
        uint16_t   type;
        wchar_t    sz_key;
        uint16_t   padding;
        string_table children;
    } string_file_info;

    typedef struct {
        uint16_t   length;
        uint16_t   value_length;
        uint16_t   type;
        wchar_t    sz_key;
        uint16_t   padding;
        uint32_t   value;
    } var;

    typedef struct {
        uint16_t   length;
        uint16_t   value_length;
        uint16_t   type;
        wchar_t    sz_key;
        uint16_t   padding;
        var   children;
    } var_file_info;
};

#pragma pack(pop)



#ifndef ALIGN_DOWN
    #define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))
#endif

#ifndef ALIGN_UP
    #define ALIGN_UP(a, b) ((((a) + (b) - 1) / (b)) * (b))
#endif

#ifndef offsetof
    #define offsetof(s,m) (uint32_t)((size_t)&(((s*)0)->m))
#endif

#ifndef max
    #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define INT8_SIZE   1
#define INT16_SIZE  2
#define INT32_SIZE  4
#define INT64_SIZE  8
#define CHAR_SIZE   1
#define WCHAR_SIZE  2

#ifndef _In_
    #define _In_
#endif
#ifndef _Inout_
    #define _Inout_
#endif
#ifndef _Out_opt_
    #define _Out_opt_
#endif
#ifndef _In_opt_
    #define _In_opt_
#endif
#ifndef _Inout_opt_
    #define _Inout_opt_
#endif
#ifndef _Out_
    #define _Out_
#endif