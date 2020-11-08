#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#define _BSD_SOURCE
#include <endian.h>

#ifdef COLOR_TEXT

#define FG_END                   "\x1B[0m"
#define FG_BLACK                 "\x1B[30m"
#define FG_RED                   "\x1B[31m"
#define FG_GREEN                 "\x1B[32m"
#define FG_YELLOW                "\x1B[33m"
#define FG_BLUE                  "\x1B[34m"
#define FG_PURPLE                "\x1B[35m"
#define FG_CYAN                  "\x1B[36m"
#define FG_LIGHT_GRAY            "\x1B[37m"
#define FG_DEFAULT               "\x1B[39m"
#define FG_DARK_GRAY             "\x1B[90m"

#define FONT_COLOR(color, str) FG_##color str FG_END

#else

#define FG_END
#define FG_BLACK
#define FG_RED
#define FG_GREEN
#define FG_YELLOW
#define FG_BLUE
#define FG_PURPLE
#define FG_CYAN
#define FG_LIGHT_GRAY
#define FG_DEFAULT
#define FG_DARK_GRAY

#define FONT_COLOR(color, str) str

#endif

#define ARRAY_STR(i) [i] = #i
#define CASE_STR(str, i)                        \
    case i:                                     \
    str = #i;                                   \
    break;

void Elf32_Ehdr_comment_dump()
{
    printf(FONT_COLOR(DARK_GRAY, "Elf32 Header Structure Info:                      \n"));
	printf(FONT_COLOR(DARK_GRAY, "  e_ident     - Magic number and other info       \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_type      - Object file type                  \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_machine   - Architecture                      \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_version   - Object file version               \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_entry     - Entry point virtual address       \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_phoff     - Program header table file offset  \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_shoff     - Section header table file offset  \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_flags     - Processor-specific flags          \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_ehsize    - ELF header size in bytes          \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_phentsize - Program header table entry size   \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_phnum     - Program header table entry count  \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_shentsize - Section header table entry size   \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_shnum     - Section header table entry count  \n"));
    printf(FONT_COLOR(DARK_GRAY, "  e_shstrndx  - Section header string table index \n"));
    return;
}

void Elf32_Ehdr_dump(Elf32_Ehdr *Ehdr)
{
	int i;
    static char *type_str[] = {
        ARRAY_STR(ET_NONE),
        ARRAY_STR(ET_REL),
        ARRAY_STR(ET_EXEC),
        ARRAY_STR(ET_DYN),
        ARRAY_STR(ET_CORE),
    };

    static char *class_str[] = {
        ARRAY_STR(ELFCLASSNONE),
        ARRAY_STR(ELFCLASS32),
        ARRAY_STR(ELFCLASS64),
    };

    static char *data_str[] = {
        ARRAY_STR(ELFDATANONE),
        ARRAY_STR(ELFDATA2LSB),
        ARRAY_STR(ELFDATA2MSB),
    };

    static char *version_str[] = {
        ARRAY_STR(EV_NONE),
        ARRAY_STR(EV_CURRENT),
    };

    static char *osabi_str[] = {
        ARRAY_STR(ELFOSABI_NONE),
        ARRAY_STR(ELFOSABI_SYSV),
        ARRAY_STR(ELFOSABI_HPUX),
        ARRAY_STR(ELFOSABI_NETBSD),
        ARRAY_STR(ELFOSABI_LINUX),
        ARRAY_STR(ELFOSABI_SOLARIS),
        ARRAY_STR(ELFOSABI_IRIX),
        ARRAY_STR(ELFOSABI_FREEBSD),
        ARRAY_STR(ELFOSABI_TRU64),
        ARRAY_STR(ELFOSABI_ARM),
        ARRAY_STR(ELFOSABI_STANDALONE),
    };

    static char *machine_str[] = {
        ARRAY_STR(EM_NONE),
        ARRAY_STR(EM_M32),
        ARRAY_STR(EM_SPARC),
        ARRAY_STR(EM_386),
        ARRAY_STR(EM_68K),
        ARRAY_STR(EM_88K),
        ARRAY_STR(EM_860),
        ARRAY_STR(EM_MIPS),
        ARRAY_STR(EM_S370),
        ARRAY_STR(EM_MIPS_RS3_LE),
        ARRAY_STR(EM_PARISC),
        ARRAY_STR(EM_VPP500),
        ARRAY_STR(EM_SPARC32PLUS),
        ARRAY_STR(EM_960),
        ARRAY_STR(EM_PPC),
        ARRAY_STR(EM_PPC64),
        ARRAY_STR(EM_S390),
        ARRAY_STR(EM_V800),
        ARRAY_STR(EM_FR20),
        ARRAY_STR(EM_RH32),
        ARRAY_STR(EM_RCE),
        ARRAY_STR(EM_ARM),
        ARRAY_STR(EM_FAKE_ALPHA),
        ARRAY_STR(EM_SH),
        ARRAY_STR(EM_SPARCV9),
        ARRAY_STR(EM_TRICORE),
        ARRAY_STR(EM_ARC),
        ARRAY_STR(EM_H8_300),
        ARRAY_STR(EM_H8_300H),
        ARRAY_STR(EM_H8S),
        ARRAY_STR(EM_H8_500),
        ARRAY_STR(EM_IA_64),
        ARRAY_STR(EM_MIPS),
        ARRAY_STR(EM_COLDFIRE),
        ARRAY_STR(EM_68HC12),
        ARRAY_STR(EM_MMA),
        ARRAY_STR(EM_PCP),
        ARRAY_STR(EM_NCPU),
        ARRAY_STR(EM_NDR1),
        ARRAY_STR(EM_STARCORE),
        ARRAY_STR(EM_ME16),
        ARRAY_STR(EM_ST100),
        ARRAY_STR(EM_TINYJ),
        ARRAY_STR(EM_X86_64),
        ARRAY_STR(EM_PDSP),
        ARRAY_STR(EM_FX66),
        ARRAY_STR(EM_ST9PLUS),
        ARRAY_STR(EM_ST7),
        ARRAY_STR(EM_68HC16),
        ARRAY_STR(EM_68HC11),
        ARRAY_STR(EM_68HC08),
        ARRAY_STR(EM_68HC05),
        ARRAY_STR(EM_SVX),
        ARRAY_STR(EM_ST19),
        ARRAY_STR(EM_VAX),
        ARRAY_STR(EM_CRIS),
        ARRAY_STR(EM_JAVELIN),
        ARRAY_STR(EM_FIREPATH),
        ARRAY_STR(EM_ZSP),
        ARRAY_STR(EM_MMIX),
        ARRAY_STR(EM_HUANY),
        ARRAY_STR(EM_PRISM),
        ARRAY_STR(EM_AVR),
        ARRAY_STR(EM_FR30),
        ARRAY_STR(EM_D10V),
        ARRAY_STR(EM_D30V),
        ARRAY_STR(EM_V850),
        ARRAY_STR(EM_M32R),
        ARRAY_STR(EM_MN10300),
        ARRAY_STR(EM_MN10200),
        ARRAY_STR(EM_PJ),
        ARRAY_STR(EM_OPENRISC),
        ARRAY_STR(EM_ARC_A5),
        ARRAY_STR(EM_XTENSA),
    };

	printf(FONT_COLOR(CYAN, "Elf32 Header:\n"));
	printf("  e_ident: ");
	for (i = 0; i < EI_NIDENT; ++i) {
		printf("%02x ", Ehdr->e_ident[i]);
    }
    printf("\n");
    printf("           [ %c%c%c %s %s %s %s ]\n",
           Ehdr->e_ident[EI_MAG1],
           Ehdr->e_ident[EI_MAG2],
           Ehdr->e_ident[EI_MAG3],
           class_str[Ehdr->e_ident[EI_CLASS]],
           data_str[Ehdr->e_ident[EI_DATA]],
           version_str[Ehdr->e_ident[EI_VERSION]],
           osabi_str[Ehdr->e_ident[EI_OSABI]]);
	printf("  e_type:      0x%x (%s)\n", Ehdr->e_type, type_str[Ehdr->e_type]);
    printf("  e_machine:   0x%x (%s)\n", Ehdr->e_machine, machine_str[Ehdr->e_machine]);
    printf("  e_version:   0x%x\n", Ehdr->e_version  );
    printf("  e_entry:     0x%x\n", Ehdr->e_entry    );
    printf("  e_phoff:     0x%x\n", Ehdr->e_phoff    );
    printf("  e_shoff:     0x%x\n", Ehdr->e_shoff    );
    printf("  e_flags:     0x%x\n", Ehdr->e_flags    );
    printf("  e_ehsize:    0x%x\n", Ehdr->e_ehsize   );
    printf("  e_phentsize: 0x%x\n", Ehdr->e_phentsize);
    printf("  e_phnum:     0x%x\n", Ehdr->e_phnum    );
    printf("  e_shentsize: 0x%x\n", Ehdr->e_shentsize);
    printf("  e_shnum:     0x%x\n", Ehdr->e_shnum    );
    printf("  e_shstrndx:  0x%x\n", Ehdr->e_shstrndx );
    return;
}

void Elf32_Phdr_comment_dump()
{
    printf(FONT_COLOR(DARK_GRAY, "Elf32 Promgram Header Structure Info:\n"));
	printf(FONT_COLOR(DARK_GRAY, "  p_type   - Segment type              \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_offset - Segment file offset       \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_vaddr  - Segment virtual address   \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_paddr  - Segment physical address  \n"));
	printf(FONT_COLOR(DARK_GRAY, "  p_filesz - Segment size in file      \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_memsz  - Segment size in memory    \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_flags  - Segment flags             \n"));
    printf(FONT_COLOR(DARK_GRAY, "  p_align  - Segment alignment         \n"));
    return;
}

void Elf32_Phdr_dump(Elf32_Ehdr *Ehdr, Elf32_Phdr *Phdr)
{
    static char *type_str[] = {
        ARRAY_STR(PT_NULL),
        ARRAY_STR(PT_LOAD),
        ARRAY_STR(PT_DYNAMIC),
        ARRAY_STR(PT_INTERP),
        ARRAY_STR(PT_NOTE),
        ARRAY_STR(PT_SHLIB),
        ARRAY_STR(PT_PHDR),
        ARRAY_STR(PT_TLS),
    };
    char *type = "PT_UNKNOWN";

    printf(FONT_COLOR(CYAN, "Elf32 Program Header(offset 0x%x):\n"),
           (void *)Phdr - (void *)Ehdr);
    if (Phdr->p_type < PT_NUM) {
        type = type_str[Phdr->p_type];
    } else {
        switch (Phdr->p_type) {
            CASE_STR(type, PT_LOOS);
            CASE_STR(type, PT_GNU_EH_FRAME);
            CASE_STR(type, PT_GNU_STACK);
            CASE_STR(type, PT_GNU_RELRO);
            CASE_STR(type, PT_SUNWSTACK);
            CASE_STR(type, PT_LOPROC);
            CASE_STR(type, PT_HIPROC);
            CASE_STR(type, PT_LOSUNW|PT_SUNWBSS);
            CASE_STR(type, PT_HISUNW|PT_HIOS);
        default:
            break;
        }
    }

    printf("  p_type:      0x%x (%s)\n", Phdr->p_type, type);
    printf("  p_offset:    0x%x\n", Phdr->p_offset);
    printf("  p_vaddr:     0x%x\n", Phdr->p_vaddr);
    printf("  p_paddr:     0x%x\n", Phdr->p_paddr);
    printf("  p_filesz:    0x%x\n", Phdr->p_filesz);
    printf("  p_memsz:     0x%x\n", Phdr->p_memsz);
    printf("  p_flags:     0x%x (", Phdr->p_flags);
    if (Phdr->p_flags & PF_R) {
        printf(" PF_R ");
    }

    if (Phdr->p_flags & PF_W) {
        printf(" PF_W ");
    }

    if (Phdr->p_flags & PF_X) {
        printf(" PF_X ");
    }
    printf(")\n");
    printf("  p_align:     0x%x\n", Phdr->p_align);
    return;
}

void Elf32_Shdr_comment_dump()
{
    printf(FONT_COLOR(DARK_GRAY, "Elf32 Section Header Structure Info:\n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_name      - Section name (string tbl index)  \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_type      - Section type                     \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_flags     - Section flags                    \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_addr      - Section virtual addr at execution\n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_offset    - Section file offset              \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_size      - Section size in bytes            \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_link      - Link to another section          \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_info      - Additional section information   \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_addralign - Section alignment                \n"));
	printf(FONT_COLOR(DARK_GRAY, "  sh_entsize   - Entry size if section holds table\n"));
    return;
}

void Elf32_Shdr_dump(Elf32_Ehdr *Ehdr, Elf32_Shdr *Shdr, char *shstrtab)
{
    static char *type_str[] = {
        ARRAY_STR(SHT_NULL),
        ARRAY_STR(SHT_PROGBITS),
        ARRAY_STR(SHT_SYMTAB),
        ARRAY_STR(SHT_STRTAB),
        ARRAY_STR(SHT_RELA),
        ARRAY_STR(SHT_HASH),
        ARRAY_STR(SHT_DYNAMIC),
        ARRAY_STR(SHT_NOTE),
        ARRAY_STR(SHT_NOBITS),
        ARRAY_STR(SHT_REL),
        ARRAY_STR(SHT_SHLIB),
        ARRAY_STR(SHT_DYNSYM),
        ARRAY_STR(SHT_INIT_ARRAY),
        ARRAY_STR(SHT_FINI_ARRAY),
        ARRAY_STR(SHT_PREINIT_ARRAY),
        ARRAY_STR(SHT_GROUP),
        ARRAY_STR(SHT_SYMTAB_SHNDX),
    };
    char *type = "SHT_UNKNOWN";

    printf(FONT_COLOR(CYAN, "Elf32 Section Header(offset 0x%x):\n"),
           (void *)Shdr - (void *)Ehdr);
    printf("  sh_name:      0x%x (%s)\n", Shdr->sh_name, shstrtab + Shdr->sh_name);

    if (Shdr->sh_type < SHT_NUM) {
        type = type_str[Shdr->sh_type];
    } else {
        switch (Shdr->sh_type) {
            CASE_STR(type, SHT_LOOS);
            CASE_STR(type, SHT_GNU_ATTRIBUTES);
            CASE_STR(type, SHT_GNU_HASH);
            CASE_STR(type, SHT_GNU_LIBLIST);
            CASE_STR(type, SHT_CHECKSUM);
            CASE_STR(type, SHT_LOSUNW|SHT_SUNW_move);
            CASE_STR(type, SHT_SUNW_COMDAT);
            CASE_STR(type, SHT_SUNW_syminfo);
            CASE_STR(type, SHT_GNU_verdef);
            CASE_STR(type, SHT_GNU_verneed);
            CASE_STR(type, SHT_GNU_versym|SHT_HISUNW|SHT_HIOS);
            CASE_STR(type, SHT_LOPROC);
            CASE_STR(type, SHT_HIPROC);
            CASE_STR(type, SHT_LOUSER);
            CASE_STR(type, SHT_HIUSER);
        default:
            break;
        }
    }

    printf("  sh_type:      0x%x (%s)\n", Shdr->sh_type, type);
    printf("  sh_flags:     0x%x (", Shdr->sh_flags);
    if (Shdr->sh_flags & PF_R) {
        printf(" SHF_WRITE ");
    }

    if (Shdr->sh_flags & SHF_ALLOC) {
        printf(" SHF_ALLOC ");
    }

    if (Shdr->sh_flags & SHF_EXECINSTR) {
        printf(" SHF_EXECINSTR ");
    }

    if (Shdr->sh_flags & SHF_MASKPROC) {
        printf(" SHF_MASKPROC ");
    }
    printf(")\n");
    printf("  sh_addr:      0x%x\n", Shdr->sh_addr);
    printf("  sh_offset:    0x%x\n", Shdr->sh_offset);
    printf("  sh_size:      0x%x\n", Shdr->sh_size);
    printf("  sh_link:      0x%x\n", Shdr->sh_link);
    printf("  sh_info:      0x%x\n", Shdr->sh_info);
    printf("  sh_addralign: 0x%x\n", Shdr->sh_addralign);
    printf("  sh_entsize:   0x%x\n", Shdr->sh_entsize);
    return;
}


void Elf32_Sym_comment_dump()
{
    printf(FONT_COLOR(DARK_GRAY, "Elf32 Symbol Structure Info:\n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_name  - Symbol name (string tbl index)\n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_value - Symbol value                  \n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_size  - Symbol size                   \n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_info  - Symbol type and binding       \n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_other - Symbol visibility             \n"));
	printf(FONT_COLOR(DARK_GRAY, "  st_shndx - Section index                 \n"));
    return;
}

void Elf32_Sym_dump(Elf32_Ehdr *Ehdr, Elf32_Sym *sym, int sym_size,
                    char *strtab, char *prefix)
{
    int i;

    static char *info_bind_str[] = {
        ARRAY_STR(STB_LOCAL),
        ARRAY_STR(STB_GLOBAL),
        ARRAY_STR(STB_WEAK),
        ARRAY_STR(STB_NUM),
        ARRAY_STR(STB_LOOS),
        ARRAY_STR(STB_GNU_UNIQUE),
        ARRAY_STR(STB_HIOS),
        ARRAY_STR(STB_LOPROC),
        ARRAY_STR(STB_HIPROC),
    };

    static char *info_type_str[] = {
        ARRAY_STR(STT_NOTYPE),
        ARRAY_STR(STT_OBJECT),
        ARRAY_STR(STT_FUNC),
        ARRAY_STR(STT_SECTION),
        ARRAY_STR(STT_FILE),
        ARRAY_STR(STT_COMMON),
        ARRAY_STR(STT_TLS),
        ARRAY_STR(STT_NUM),
        ARRAY_STR(STT_LOOS|STT_GNU_IFUNC),
        ARRAY_STR(STT_HIOS),
        ARRAY_STR(STT_LOPROC),
        ARRAY_STR(STT_HIPROC),
    };

    static char *other_str[] = {
        ARRAY_STR(STV_DEFAULT),
        ARRAY_STR(STV_INTERNAL),
        ARRAY_STR(STV_HIDDEN),
        ARRAY_STR(STV_PROTECTED),
    };

    if (!sym) return;

    Elf32_Sym_comment_dump();
    for (i = 0; i < sym_size; i += sizeof(*sym), sym++) {
        printf(FONT_COLOR(CYAN, "%s Symbol (offset 0x%x):\n"), prefix,
               (void *)sym - (void *)Ehdr);
        printf("  st_name:   0x%x", sym->st_name);
        if (strtab) {
            printf(" (%s)\n", &strtab[sym->st_name]);
        } else {
            printf("\n");
        }
        printf("  st_value:  0x%x\n", sym->st_value);
        printf("  st_size:   0x%x\n", sym->st_size);
        printf("  st_info:   0x%x (%s,%s)\n", sym->st_info,
               info_bind_str[ELF32_ST_BIND(sym->st_info)],
               info_type_str[ELF32_ST_TYPE(sym->st_info)]);
        printf("  st_other:  0x%x (%s)\n", sym->st_other,
               other_str[sym->st_other]);
        printf("  st_shndx:  0x%x\n", sym->st_shndx);
    }
    return;
}

void Elf32_Dyn_comment_dump()
{
    printf(FONT_COLOR(DARK_GRAY, "Elf32 Dyn Structure Info:\n"));
	printf(FONT_COLOR(DARK_GRAY, "d_tag  - Dynamic entry type\n"));
	printf(FONT_COLOR(DARK_GRAY, "d_val  - Integer value     \n"));
	printf(FONT_COLOR(DARK_GRAY, "d_ptr  - Address value     \n"));
    return;
}

void Elf32_Dyn_dump(Elf32_Ehdr *Ehdr, Elf32_Dyn *dyn, int dyn_size,
                    char *dynstr)
{
    static char *tag_str[] = {
        ARRAY_STR(DT_NULL),
        ARRAY_STR(DT_NEEDED),
        ARRAY_STR(DT_PLTRELSZ),
        ARRAY_STR(DT_PLTGOT),
        ARRAY_STR(DT_HASH),
        ARRAY_STR(DT_STRTAB),
        ARRAY_STR(DT_SYMTAB),
        ARRAY_STR(DT_RELA),
        ARRAY_STR(DT_RELASZ),
        ARRAY_STR(DT_RELAENT),
        ARRAY_STR(DT_STRSZ),
        ARRAY_STR(DT_SYMENT),
        ARRAY_STR(DT_INIT),
        ARRAY_STR(DT_FINI),
        ARRAY_STR(DT_SONAME),
        ARRAY_STR(DT_RPATH),
        ARRAY_STR(DT_SYMBOLIC),
        ARRAY_STR(DT_REL),
        ARRAY_STR(DT_RELSZ),
        ARRAY_STR(DT_RELENT),
        ARRAY_STR(DT_PLTREL),
        ARRAY_STR(DT_DEBUG),
        ARRAY_STR(DT_TEXTREL),
        ARRAY_STR(DT_JMPREL),
        ARRAY_STR(DT_BIND_NOW),
        ARRAY_STR(DT_INIT_ARRAY),
        ARRAY_STR(DT_FINI_ARRAY),
        ARRAY_STR(DT_INIT_ARRAYSZ),
        ARRAY_STR(DT_FINI_ARRAYSZ),
        ARRAY_STR(DT_RUNPATH),
        ARRAY_STR(DT_FLAGS),
        ARRAY_STR(DT_ENCODING),
        ARRAY_STR(DT_PREINIT_ARRAY),
        ARRAY_STR(DT_PREINIT_ARRAYSZ),
    };

    int i;
    char *tag = "DT_UNKNOWN";

    if (!dyn) return;

    Elf32_Dyn_comment_dump();

    for (i = 0; i < dyn_size; i += sizeof(*dyn), dyn++) {
        printf(FONT_COLOR(CYAN, "Dynamic (offset 0x%x):\n"),
               (void *)dyn - (void *)Ehdr);
        printf("  d_tag: 0x%x", dyn->d_tag);
        if (dyn->d_tag < DT_NUM) {
            tag = tag_str[dyn->d_tag];
        } else {
            switch (dyn->d_tag) {
                CASE_STR(tag, DT_LOOS);
                CASE_STR(tag, DT_HIOS);
                CASE_STR(tag, DT_LOPROC);
                CASE_STR(tag, DT_HIPROC|DT_FILTER);
                CASE_STR(tag, DT_VALRNGLO);
                CASE_STR(tag, DT_GNU_PRELINKED);
                CASE_STR(tag, DT_GNU_CONFLICTSZ);
                CASE_STR(tag, DT_GNU_LIBLISTSZ);
                CASE_STR(tag, DT_CHECKSUM);
                CASE_STR(tag, DT_PLTPADSZ);
                CASE_STR(tag, DT_MOVEENT);
                CASE_STR(tag, DT_MOVESZ);
                CASE_STR(tag, DT_FEATURE_1);
                CASE_STR(tag, DT_POSFLAG_1);
                CASE_STR(tag, DT_SYMINSZ);
                CASE_STR(tag, DT_SYMINENT|DT_VALRNGHI);
                CASE_STR(tag, DT_ADDRRNGLO);
                CASE_STR(tag, DT_GNU_HASH);
                CASE_STR(tag, DT_TLSDESC_PLT);
                CASE_STR(tag, DT_TLSDESC_GOT);
                CASE_STR(tag, DT_GNU_CONFLICT);
                CASE_STR(tag, DT_GNU_LIBLIST);
                CASE_STR(tag, DT_CONFIG);
                CASE_STR(tag, DT_DEPAUDIT);
                CASE_STR(tag, DT_AUDIT);
                CASE_STR(tag, DT_PLTPAD);
                CASE_STR(tag, DT_MOVETAB);
                CASE_STR(tag, DT_SYMINFO|DT_ADDRRNGHI);
                CASE_STR(tag, DT_VERSYM);
                CASE_STR(tag, DT_RELACOUNT);
                CASE_STR(tag, DT_RELCOUNT);
                CASE_STR(tag, DT_FLAGS_1);
                CASE_STR(tag, DT_VERDEF);
                CASE_STR(tag, DT_VERNEED);
                CASE_STR(tag, DT_AUXILIARY);
            default:
                break;
            }
        }
        printf(" (%s)\n", tag);
        if (DT_NEEDED == dyn->d_tag) {
            printf("  d_val: 0x%x (%s)\n", dyn->d_un.d_val,
                   dynstr + dyn->d_un.d_val);
        } else {
            printf("  d_val: 0x%x\n", dyn->d_un.d_val);
        }
    }
    return;
}

struct sym_table_elem
{
    Elf32_Addr addr;
    char      *name;
};
static struct sym_table_elem *sym_table;
static int sym_num;

void Elf32_Sym_table_destroy()
{
    if (sym_table) free(sym_table);
    return;
}

int Elf32_Sym_table_compare(const void *elem1, const void *elem2)
{
    return ((struct sym_table_elem *)elem2)->addr -
        ((struct sym_table_elem *)elem1)->addr;
}

void Elf32_Sym_table_create(Elf32_Ehdr *Ehdr, Elf32_Sym *sym, int sym_size,
                            char *strtab)
{
    int i;
    struct sym_table_elem *elem;
    Elf32_Sym *_sym;

    if (!sym) return;

    /* skip addr == 0 */
    for (i = 0, _sym = sym; i < sym_size; i += sizeof(*_sym), _sym++) {
        if (!_sym->st_value) continue;
        sym_num++;
    }

    sym_table = elem = malloc(sizeof(*sym_table) * sym_num);
    memset(sym_table, 0, sizeof(*sym_table) * sym_num);

    for (i = 0, _sym= sym; i < sym_size; i += sizeof(*_sym), _sym++) {
        if (!_sym->st_value) continue;
        elem->addr = _sym->st_value;
        elem->name = strtab + _sym->st_name;
        elem++;
    }

    qsort(sym_table, sym_num, sizeof(*sym_table), Elf32_Sym_table_compare);
    return;
}

char *Elf32_Sym_table_find(Elf32_Addr addr)
{
    if (!sym_table) return NULL;
    struct sym_table_elem *norm;
    struct sym_table_elem elem = {
        .addr = addr,
    };

    norm = bsearch(&elem, sym_table, sym_num,
                   sizeof(*sym_table), Elf32_Sym_table_compare);

    return norm ? norm->name : NULL;
}

void Elf32_array_dump(Elf32_Ehdr *Ehdr, Elf32_Addr *array,
                           int array_size)
{
    int i;
    Elf32_Addr addr;

    for (i = 0; i < array_size; i+=sizeof(*array), array++) {
        if (ELFDATA2MSB == Ehdr->e_ident[EI_DATA]) {
            addr = be32toh(*array);
        } else {
            /* handle ELFDATANONE as ELFDATA2LSB*/
            addr = le32toh(*array);
        }

        printf("  [%d] 0x%x (%s)\n", i, addr, Elf32_Sym_table_find(*array));
    }

    return;
}

void Elf32_init_array_dump(Elf32_Ehdr *Ehdr, Elf32_Addr *init_array,
                           int init_array_size)
{
    if (!init_array) return;

    printf(FONT_COLOR(CYAN, "Init Array (offset 0x%x):\n"),
           (void *)init_array - (void *)Ehdr);

    Elf32_array_dump(Ehdr, init_array, init_array_size);
    return;
}

void Elf32_fini_array_dump(Elf32_Ehdr *Ehdr, Elf32_Addr *fini_array,
                           int fini_array_size)
{
    if (!fini_array) return;


    printf(FONT_COLOR(CYAN, "Fini Array (offset 0x%x):\n"),
           (void *)fini_array - (void *)Ehdr);

    Elf32_array_dump(Ehdr, fini_array, fini_array_size);
    return;
}

int main(int argc, char *argv[])
{
    int rc = -1;
    int fd = -1;
    int i;
    struct stat st;
    Elf32_Ehdr *Ehdr;
    Elf32_Phdr *Phdr;
    Elf32_Shdr *Shdr, *Shdr_base;
    Elf32_Shdr *Shdr_strtab, *Shdr_dynstr;
    char       *shstrtab;
    char       *strtab = NULL;
    char       *dynstr = NULL;
    Elf32_Dyn  *dyn = NULL;
    int         dyn_size;
    Elf32_Sym  *sym = NULL;
    int         sym_size;
    Elf32_Addr *init_array = NULL;
    int         init_array_size;
    Elf32_Addr *fini_array = NULL;
    int         fini_array_size;
    Elf32_Sym  *dyn_sym = NULL;
    int         dyn_sym_size;

    if (2 != argc) {
        printf("missing argument\n");
        goto out;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        printf("open failed\n");
        goto out;
    }

    if (fstat(fd, &st) < 0) goto out;

    Ehdr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == Ehdr) {
        printf("mmap failed\n");
        goto out;
    }

    if (0x7f != Ehdr->e_ident[EI_MAG0] &&
        'E' != Ehdr->e_ident[EI_MAG1] &&
        'L' != Ehdr->e_ident[EI_MAG2] &&
        'F' != Ehdr->e_ident[EI_MAG3]) {
        printf("invaild ELF file\n");
        goto out;
    }

    Elf32_Ehdr_comment_dump();
    Elf32_Ehdr_dump(Ehdr);

    Phdr = (Elf32_Phdr *)((void *)Ehdr + Ehdr->e_phoff);

    if (Ehdr->e_phnum) Elf32_Phdr_comment_dump();
    for (i = 0; i < Ehdr->e_phnum; ++i, ++Phdr) {
        Elf32_Phdr_dump(Ehdr, Phdr);
    }

    Shdr_base = Shdr = (Elf32_Shdr *)((void *)Ehdr + Ehdr->e_shoff);
    shstrtab = (char *)Ehdr + (Shdr + Ehdr->e_shstrndx)->sh_offset;

    if (Ehdr->e_shnum) Elf32_Shdr_comment_dump();
    for (i = 0; i < Ehdr->e_shnum; ++i, ++Shdr) {
        Elf32_Shdr_dump(Ehdr, Shdr, shstrtab);

        switch (Shdr->sh_type) {
        case SHT_SYMTAB:
            sym = (Elf32_Sym *)((void *)Ehdr + Shdr->sh_offset);
            sym_size = Shdr->sh_size;
            Shdr_strtab = Shdr_base + Shdr->sh_link;
            if (SHT_STRTAB != Shdr_strtab->sh_type) break;
            strtab = (char *)Ehdr + Shdr_strtab->sh_offset;
            break;
        case SHT_DYNAMIC:
            dyn = (Elf32_Dyn *)((void *)Ehdr + Shdr->sh_offset);
            dyn_size = Shdr->sh_size;
            break;
        case SHT_INIT_ARRAY:
            init_array = (Elf32_Addr *)((void *)Ehdr + Shdr->sh_offset);
            init_array_size = Shdr->sh_size;
            break;
        case SHT_FINI_ARRAY:
            fini_array = (Elf32_Addr *)((void *)Ehdr + Shdr->sh_offset);
            fini_array_size = Shdr->sh_size;
            break;
        case SHT_DYNSYM:
            dyn_sym = (Elf32_Sym *)((void *)Ehdr + Shdr->sh_offset);
            dyn_sym_size = Shdr->sh_size;
            Shdr_dynstr = Shdr_base + Shdr->sh_link;
            if (SHT_STRTAB != Shdr_dynstr->sh_type) break;
            dynstr = (char *)Ehdr + Shdr_dynstr->sh_offset;
            break;
        default:
            break;
        }
    }

    Elf32_Sym_table_create(Ehdr, sym, sym_size, strtab);
    Elf32_init_array_dump(Ehdr, init_array, init_array_size);
    Elf32_fini_array_dump(Ehdr, fini_array, fini_array_size);
    Elf32_Sym_dump(Ehdr, sym, sym_size, strtab, ".symtab");
    Elf32_Dyn_dump(Ehdr, dyn, dyn_size, dynstr);
    Elf32_Sym_dump(Ehdr, dyn_sym, dyn_sym_size, dynstr, ".dynsym");


out:
    Elf32_Sym_table_destroy();
    if (0 <= fd) close(fd);
    return rc;
}
