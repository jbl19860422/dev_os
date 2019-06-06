#include "protect.h"
void set_descriptor(descriptor_t *pDesc, u32 base, u32 limit/*only use 24bits*/, u16 attr/*only use 12bits*/)
{
	pDesc->base_low = base&0xFFFF;
	pDesc->base_mid = (base>>16)&0xFF;
	pDesc->base_high = (base>>24)&0xFF;
	pDesc->limit_low = limit&&0xFFFF;
	pDesc->attr1 = attr&0xFF;
	pDesc->attr_limit = ((attr>>8)&0xF)|((limit>>8)&0xF0);
}

void set_gate(gate_t *pGate, u16 selector, u32 offset, u8 attr)
{
	pGate->selector = selector;
	pGate->offset_low = offset&0xffff;
	pGate->offset_high = (offset>>16)&0xffff;
	pGate->dcount = 0;
	pGate->attr = attr;
}
