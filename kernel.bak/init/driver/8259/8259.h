#ifndef _8259_H_
#define _8259_H_
#include "io.h"
#define MASTER_ICW1	0x20
#define MASTER_ICW2 0x21
#define MASTER_ICW3 0x21
#define MASTER_ICW4 0x21
#define MASTER_OCW1 0x21
#define MASTER_OCW2 0x20
#define MASTER_INT_MASK 0x21

#define SLAVE_ICW1 0xA0
#define SLAVE_ICW2 0xA1
#define SLAVE_ICW3 0xA1
#define SLAVE_ICW4 0xA1
#define SLAVE_OCW1 0xA1
#define SLAVE_OCW2 0xA0
#define SLAVE_INT_MASK 0xA1

#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28

void init_8259();
void set_master_intmask(u8 mask);
void set_slave_intmask(u8 mask);

void master_eoi();
void slave_eoi();

#endif
