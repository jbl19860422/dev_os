#define MAX_INIT_FUN 20
#include "8259.h"
#include "protect.h"
//#include "interrupt.h"

extern void int_process0x20();
extern void int_process0x21();
extern void int_process0x22();
extern void int_process0x23();
extern void int_process0x24();
extern void int_process0x25();
extern void int_process0x26();
extern void int_process0x27();
extern void int_process0x28();
extern void int_process0x29();
extern void int_process0x2A();
extern void int_process0x2B();
extern void int_process0x2C();
extern void int_process0x2D();
extern void int_process0x2E();
extern void int_process0x2F();

//descriptor_t gdt[6];
gate_t idt[256];
u8 idt_ptr[6];
//typedef void* (*init_fun_t)();

//init_fun_t init_funs;//[MAX_INIT_FUN];
//init_funs[0] = init_8259;

#define load_idtr(idtr) __asm__ \
(\
	"LIDT "#idtr"\n\t"\
);

void set_idtr(u8 *idtr, u32 *idt, u16 idt_count)
{
	u16 idt_len = idt_count*6;
	u32 idt_base = (u32)idt;
	*((u16 *)idtr) = idt_len;
	*((u32 *)(idtr+2)) = idt_base;
}

extern void KeyBoard();
extern void TimerInt();
extern void common_int();
extern void MouseInt();
void init_idt()
{
	PrintMsg("int_process0x2C=");PrintDword(int_process0x2C);PrintMsg("\r\n");

	int i=0;
	for(i=0;i<256;i++)
	{
		set_gate(idt+i, 8, (u32)common_int, DA_DPL_0|DA_P_1|DA_TYPE_386INTGATE);
	}
	set_gate(&idt[INT_VECTOR_IRQ0+0], 8, (u32)TimerInt, DA_DPL_0|DA_P_1|DA_TYPE_386INTGATE);
	
	set_gate(&idt[INT_VECTOR_IRQ0+1], 8, (u32)KeyBoard,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	
	
	set_gate(&idt[INT_VECTOR_IRQ8+4], 8, (u32)int_process0x2C,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+5], 8, (u32)MouseInt,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+6], 8, (u32)MouseInt,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+7], 8, (u32)MouseInt,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	/*
	set_gate(&idt[INT_VECTOR_IRQ0+2], 8, (u32)int_process0x22,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ0+3], 8, (u32)int_process0x23,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ0+4], 8, (u32)int_process0x24,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ0+5], 8, (u32)int_process0x25,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ0+6], 8, (u32)int_process0x26,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ0+7], 8, (u32)int_process0x27,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	
	set_gate(&idt[INT_VECTOR_IRQ8+0], 8, (u32)int_process0x28,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+1], 8, (u32)int_process0x29,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+2], 8, (u32)int_process0x2A,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+3], 8, (u32)int_process0x2B,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+4], 8, (u32)int_process0x2C,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+5], 8, (u32)int_process0x2D,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+6], 8, (u32)int_process0x2E,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	set_gate(&idt[INT_VECTOR_IRQ8+7], 8, (u32)int_process0x2F,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	*/
	//set_gate(&idt[0x80], 8, (u32)UserInt,DA_P_1|DA_DPL_0|DA_TYPE_386INTGATE);
	PrintMsg("idtr=");PrintDword(idt_ptr);PrintMsg("\r\n");
	set_idtr(idt_ptr, (u32 *)idt, 256);
	load_idtr(idt_ptr);
}
#define PORT_KEYDAT 0x60
#define PORT_KEYSTA	0x64
#define PORT_KEYCMD	0x64
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYSTA_READ_NOTREADY 0x01
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void wait_KBC_sendready()
{
	while(1)
	{
		if((in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
		{
			break;
		}
	}
}

void wait_KBC_readready()
{
	io_delay();
	return;
	while(1)
	{
		if((in8(PORT_KEYSTA) & KEYSTA_READ_NOTREADY) == 0)
		{
			break;
		}
	}
}
void enable_mouse()
{
	wait_KBC_sendready();
	out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	out8(PORT_KEYDAT, MOUSECMD_ENABLE);
}

#define KEYBOARD_STATUS 	0x64
#define KEYBOARD_COMMAND	0x64
#define KEYBOARD_DATA		0x60
#define PAR_ERR				0x80
#define TIMEOUT_ERR			0x40
#define DATA_TYPE			0x20//if bit0 is 1 and check this bit,if it's 1,then can read from 0x60
#define KEYBOARD_LOCKED		0x10
#define COMMAND_DATA		0x08//last write command(0x64) or data(0x60)
#define SYSTEM_FLAG			0x04
#define INPUT_NOT_EMPTY		0x02//input buff is not empty,don't write
#define OUTPUT_FULL			0x01//output buff is empty,don't read

/***************define of the command****************/
#define CMD_READ_CCB			0x20
#define CMD_WRITE_CCB			0x60
#define CMD_SELF_TEST			0xAA
#define CMD_INTFACE_TEST		0xAB
#define CMD_DISABLE_KEYBOARD	0xAD
#define CMD_ENABLE_KEYBOARD		0xAE
#define CMD_READ_INPUT_PORT		0xC0
#define CMD_READ_OUTPUT_PORT	0xD0
#define CMD_WRITE_OUTPUT_PORT	0xD1
#define CMD_READ_TEST_INPUT		0xE0
#define CMD_SYSTEM_RESET		0xFE

#define CMD_DISABLE_MOUSE_PORT	0xA7
#define CMD_ENABLE_MOUSE_PORT	0xA8
#define CMD_TEST_MOUSE_PORT		0xA9
#define CMD_WRITE_TO_MOUSE		0xA4

void init_keyboard(void)
{
	/* キーボードコントローラの初期化 */
	wait_KBC_sendready();
	out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	out8(PORT_KEYDAT, KBC_MODE);
	return;
}

void init()
{
	init_idt();
	init_8259();
	__asm__(
		"STI\n\t"
		:
		:
	);
	out8(MASTER_INT_MASK, 0xf9); /* PIC1とキーボードを許可(11111001) */
	out8(SLAVE_INT_MASK, 0xef); /* マウスを許可(11101111) */
	init_keyboard();
	//mouse_init();
	enable_mouse();
	/*
	__asm__(
		"INT $0x2c\n\t"
		"INT $0x2c\n\t"
		:
		:
	);
	*/
	//PrintMsg("start to init mouse\r\n");
	//mouse_install();
	//PrintMsg("end of init mouse\r\n");
}
