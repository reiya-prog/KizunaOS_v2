#pragma once

typedef struct
{
    uint16_t offset_low_;
    uint16_t selector_; // code segment selector -> GDT or LDT
    uint8_t ist_;       // Interrupt Stack Table
    //   7                           0
    // +---+---+---+---+---+---+---+---+
    // |        Zero       |    IST    |
    // +---+---+---+---+---+---+---+---+
    uint8_t type_attr_; // Type and Attributes
    //   7                           0
    // +---+---+---+---+---+---+---+---+
    // | P |  DPL  | Z |    GateType   |
    // +---+---+---+---+---+---+---+---+
    uint16_t offset_middle_;
    uint32_t offset_high_, zero_;
} INTERRUPT_DESCRIPTOR;

/*
詳しくはIntel 64 and IA-32 Architectures Software Developer's Manual Vol.3A
・Figure 3-8. Segment Descriptor   (p. 2892)
・Figure 5-8. Call-Gate Descriptor (p. 2963)
*/
typedef struct
{
    short limit_low_, base_low_;
    char base_mid_;
    char access_right_;
    char limit_high_, base_high_;
} SEGMENT_DESCRIPTOR;

typedef struct
{
    short offset_low_, selector_;
    char dw_count_, access_right_;
    short offset_high_;
} GATE_DESCRIPTOR;

void init_gdt();
void init_idt();

// TODO: set_gdtとset_idtの実装
// void set_gdt(SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
// void set_idt(GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void set_interrupt_handler(unsigned int idt_no, __attribute__((ms_abi)) void (*handler)());

extern INTERRUPT_DESCRIPTOR descriptors[256];