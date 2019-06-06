#include "pci.h"
#include "vga.h"
u8 	read_pci_config_reg_u8(u8 bus, u8 device, u8 function, u8 addr)
{
	out32(0xcf8, 0x80000000|(bus<<16)|(device&0x1f)<<11|(function&0xb)<<8|(addr&0xfc));
    return in8(0xcfc + (addr&3));
}

u16 read_pci_config_reg_u16(u8 bus, u8 device, u8 function, u8 addr)
{
	out32(0xcf8, 0x80000000|(bus<<16)|(device&0x1f)<<11|(function&0xb)<<8|(addr&0xfc));
    return in16(0xcfc + (addr&2));
}

u32 read_pci_config_reg_u32(u8 bus, u8 device, u8 function, u8 addr)
{
	out32(0xcf8, 0x80000000|(bus<<16)|(device&0x1f)<<11|(function&0xb)<<8|(addr&0xfc));
    return in32(0xcfc);
}

int detect_pci_device(u8 bus, u8 device, u8 function, pci_device_t *pPciDev)
{
	u16 venderid = 0xffff;
	u16 deviceid = 0xffff;
	venderid = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_venderid));
	deviceid = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_deviceid));
	if(venderid == 0xffff || deviceid == 0xffff)
	{
		return -1;
	}
	pPciDev->config_head.u16_venderid = venderid;
	pPciDev->config_head.u16_deviceid = deviceid;
	pPciDev->config_head.u16_command = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_command));
	pPciDev->config_head.u16_status = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_status));
	pPciDev->config_head.u8_revision = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_revision));
	pPciDev->config_head.u8_PIFlag = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_PIFlag));
	pPciDev->config_head.u8_subclass = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_subclass));
	pPciDev->config_head.u8_class = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_class));
	pPciDev->config_head.u8_cacheline_size = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_cacheline_size));
	pPciDev->config_head.u8_latency_timer = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_latency_timer));
	pPciDev->config_head.u8_header_type = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_header_type));
	pPciDev->config_head.u32_base_addr[0] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[0]));
	pPciDev->config_head.u32_base_addr[1] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[1]));
	pPciDev->config_head.u32_base_addr[2] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[2]));
	pPciDev->config_head.u32_base_addr[3] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[3]));
	pPciDev->config_head.u32_base_addr[4] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[4]));
	pPciDev->config_head.u32_base_addr[5] = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_base_addr[5]));
	pPciDev->config_head.u32_cis_pointer = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_cis_pointer));
	pPciDev->config_head.u16_sub_venderid = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_sub_venderid));
	pPciDev->config_head.u16_sub_deviceid = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u16_sub_deviceid));
	pPciDev->config_head.u32_exprom_addr = read_pci_config_reg_u32(bus, device, function, pci_reg_offset(u32_exprom_addr));
	pPciDev->config_head.u8_irqline = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_irqline));
	pPciDev->config_head.u8_irqpin = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u8_irqpin));
	pPciDev->config_head.u8_min_gnt = read_pci_config_reg_u8(bus, device, function, pci_reg_offset(u8_min_gnt));
	pPciDev->config_head.u8_max_lat = read_pci_config_reg_u16(bus, device, function, pci_reg_offset(u8_max_lat));
	return 0;
}

void find_pci_devices_by_class(u8 class, u8 subclass, pci_device_t devs[MAX_CLASS_DEVICE], u32 *count)
{
	pci_device_t dev_tmp;
	*count = 0;
	u16 bus = 0,device = 0,func = 0;
	for(; bus < 256; bus++)
	{
		device = 0;
		for(; device < 32; device++)
		{
			func = 0;
			for(;func < 8; func++)
			{
				if(0 == detect_pci_device(bus, device, func, &dev_tmp))
				{//find a device
					/*
					PrintDword(dev_tmp.config_head.u16_venderid);
					PrintMsg(" ");
					PrintDword(dev_tmp.config_head.u16_deviceid);
					PrintDword(dev_tmp.config_head.u8_classcode[0]);PrintMsg(" ");
					PrintDword(dev_tmp.config_head.u8_classcode[1]);PrintMsg(" ");
					PrintMsg("\r\n");
					*/
					if(dev_tmp.config_head.u8_class == class&&dev_tmp.config_head.u8_subclass == subclass)
					{
						*devs = dev_tmp;
						devs++;
						(*count)++;
						//PrintMsg("fond i\r\n");
					}
				}
			}
		}
	}
}
