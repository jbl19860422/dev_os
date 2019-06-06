#include "8259.h"
void init_8259()
{
	//disable int
	set_master_intmask(0xFF);
	set_slave_intmask(0xFF);
	//set master
	out8(MASTER_ICW1, 0x11);
	io_delay();
	out8(MASTER_ICW2, INT_VECTOR_IRQ0);
	io_delay();
	out8(MASTER_ICW3, 0x04);
	io_delay();
	out8(MASTER_ICW4, 0x01);
	//set slave
	io_delay();
	out8(SLAVE_ICW1, 0x11);
	io_delay();
	out8(SLAVE_ICW2, INT_VECTOR_IRQ8);
	io_delay();
	out8(SLAVE_ICW3, 0x2);
	io_delay();
	out8(SLAVE_ICW4,0x01);
	//disable interrupt
	set_master_intmask(0xFB);
	set_slave_intmask(0xFF);
}

void set_master_intmask(u8 mask)
{
	out8(MASTER_INT_MASK, mask);
}
void set_slave_intmask(u8 mask)
{
	out8(SLAVE_INT_MASK, mask);
}

void master_eoi()
{
	out8(MASTER_OCW2, 0x20);
}

void slave_eoi()
{
	out8(SLAVE_OCW2, 0x20);
}
