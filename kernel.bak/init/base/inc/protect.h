#ifndef _PROTECT_H_
#define _PROCTEC_H_
#include "typedefs.h"


#pragma pack(1)
#define DA_TYPE_ACCESSED 	0x1
#define DA_TYPE_EXECUTE		0x8
#define DA_TYPE_DATA_WRITE	0x2
#define DA_TYPE_DATA_ED		0x4
#define DA_TYPE_CODE_READ	0x2
#define DA_TYPE_CODE_CONFIRM	0x4

#define DA_TYPE_286TSS		0x1
#define DA_TYPE_LDT			0x2
#define DA_TYPE_BUSY_286TSS	0x3
#define DA_TYPE_286GATE		0x4
#define DA_TYPE_TASKGATE	0x5
#define DA_TYPE_286INTGATE	0x6
#define DA_TYPE_286TRAPGATE	0x7
#define DA_TYPE_386TSS		0x9
#define DA_TYPE_BUSY_386TSS	0xb
#define DA_TYPE_386GATE		0xc
#define DA_TYPE_386INTGATE	0xe
#define DA_TYPE_386TRAPGATE	0xf

#define DA_S_DC				0x10	//data and code
#define DA_S_SG				0x0	//system gate
	
#define DA_DPL_0			0x0
#define DA_DPL_1			0x20
#define DA_DPL_2			0x40
#define DA_DPL_3			0x60

#define DA_P_1	0x80  //segmennt is present
#define DA_P_0	0x0

#define DA_DB_SEGLIMIT_4G		0x400
#define DA_DB_SEGLIMIT_64K		0x0
#define DA_DB_STACK_ESP32		0x400
#define DA_DB_STACK_SP16		0x0

#define DA_G_LIMIT4K			0x800
#define DA_G_LIMITBYTE			0x0

#pragma pack(1)
typedef struct 
{
	u16 limit_low;
	u16 base_low;
	u8 	base_mid;
	u8 	attr1;
	u8 	attr_limit;
	u8 	base_high;
}descriptor_t;

typedef struct
{
	u16 offset_low;
	u16 selector;
	u8	dcount;/*这个字段保留，可以直接写入0*/
	u8	attr;
	u16	offset_high;
}gate_t;
#pragma pack()
void set_descriptor(descriptor_t *pDesc, u32 base, u32 limit/*only use 24bits*/, u16 attr/*only use 12bits*/);
void set_gate(gate_t *pGate, u16 selector, u32 offset, u8 attr);
#endif
