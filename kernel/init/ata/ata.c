#include "ata.h"

#define TIMEOUT 0
#define BSY 1
#define NOT_BSY 2
#define NOT_BSY_DRQ 3
#define NOT_BSY_NOT_DRQ 4
#define NOT_BSY_RDY 5

#define IDE_TIMEOUT 32000u //32 seconds max for IDE ops

void DetectATA()
{
	/*
	Bit16u ebda_seg = *((unsigned short*)0x40e);
	ebda_data_t *pEbda = ebda_seg<<4;
	Bit8u  hdcount, cdcount, device, type;
	Bit8u  buffer[0x0200];
	unsigned char i = 0;
	pEbda->ata.channels[0].iface = ATA_IFACE_ISA;
	pEbda->ata.channels[0].iobase1 = 0x1f0;
	pEbda->ata.channels[0].iobase2 = 0x3f0;
	pEbda->ata.channels[0].irq = 14;
	pEbda->ata.channels[1].iface = ATA_IFACE_ISA;
	pEbda->ata.channels[1].iobase1 = 0x170;
	pEbda->ata.channels[1].iobase2 = 0x370;
	pEbda->ata.channels[1].irq = 15;
	pEbda->ata.channels[2].iface = ATA_IFACE_ISA;
	pEbda->ata.channels[2].iobase1 = 0x1e8;
	pEbda->ata.channels[2].iobase2 = 0x3e0;
	pEbda->ata.channels[2].irq = 12;
	pEbda->ata.channels[3].iface = ATA_IFACE_ISA;
	pEbda->ata.channels[3].iobase1 = 0x168;
	pEbda->ata.channels[3].iobase2 = 0x360;
	pEbda->ata.channels[3].irq = 11;
	for(device=0; device< BX_MAX_ATA_DEVICES; device++) //或者扫描pci设备也可以获取到
	{
		Bit16u iobase1, iobase2;
		Bit8u  channel, slave, shift;
		Bit8u  sc, sn, cl, ch, st;

		channel = device / 2;
		slave = device % 2;
		//得到设备基地址 
		iobase1 = pEbda->ata.channels[channel].iobase1;
		iobase2 = pEbda->ata.channels[channel].iobase2;
		//往设备中写入，看返回内容
		WritePortChar(iobase1+ATA_CB_DH, slave ? ATA_CB_DH_DEV1 : ATA_CB_DH_DEV0);
		WritePortChar(iobase1+ATA_CB_SC, 0x55);
		WritePortChar(iobase1+ATA_CB_SN, 0xaa);
		WritePortChar(iobase1+ATA_CB_SC, 0xaa);
		WritePortChar(iobase1+ATA_CB_SN, 0x55);
		WritePortChar(iobase1+ATA_CB_SC, 0x55);
		WritePortChar(iobase1+ATA_CB_SN, 0xaa);
		
		// If we found something
		sc = ReadPortChar(iobase1+ATA_CB_SC);
		sn = ReadPortChar(iobase1+ATA_CB_SN);
		//存在设备
		if ( (sc == 0x55) && (sn == 0xaa) )
		{//检查是ata还是atapi设备
			pEbda->ata.devices[device].type = ATA_TYPE_UNKNOWN;
			ata_reset(device);
			WritePortChar(iobase1+ATA_CB_DH, slave ? ATA_CB_DH_DEV1 : ATA_CB_DH_DEV0);
			sc = ReadPortChar(iobase1+ATA_CB_SC);
			sn = ReadPortChar(iobase1+ATA_CB_SN);
			if ((sc==0x01) && (sn==0x01)) 
			{
				PrintMsg("is 0101\r\n");
				cl = ReadPortChar(iobase1+ATA_CB_CL);
				ch = ReadPortChar(iobase1+ATA_CB_CH);
				st = ReadPortChar(iobase1+ATA_CB_STAT);

				if ((cl==0x14) && (ch==0xeb)) 
				{
					PrintMsg("Find atapi device\r\n");
					pEbda->ata.devices[device].type = ATA_TYPE_ATAPI;
				} 
				else if ((cl==0x00) && (ch==0x00) && (st!=0x00)) 
				{
					PrintMsg("Find ata device\r\n");
					pEbda->ata.devices[device].type = ATA_TYPE_ATA;
				} 
				else if ((cl==0xff) && (ch==0xff)) 
				{
					PrintMsg("Find non type device\r\n");
					pEbda->ata.devices[device].type = ATA_TYPE_NONE;
				}
			}
			
			type = pEbda->ata.devices[device].type;
			
			if(type == ATA_TYPE_ATA) 
			{
				Bit32u sectors_low, sectors_high;
				Bit16u cylinders, heads, spt, blksize;
				Bit8u  translation, removable, mode;

				//Temporary values to do the transfer
				pEbda->ata.devices[device].device = ATA_DEVICE_HD;
				pEbda->ata.devices[device].mode = ATA_MODE_PIO16;

				if (ata_cmd_data_in(device,ATA_CMD_IDENTIFY_DEVICE, 1, 0, 0, 0, 0L, 0L, get_SS(),buffer) !=0 )
				{
					PrintMsg("ata_cmd_data_in err");
				}
			
				removable = buffer[0] & 0x80 ? 1 : 0;
				mode      = buffer[96] ? ATA_MODE_PIO32 : ATA_MODE_PIO16;
				blksize   = buffer[10];

				cylinders = *((Bit16u *)buffer+1);// word 1
				heads     = *((Bit16u *)buffer+2); // word 3
				spt       = *((Bit16u *)buffer+6); // word 6

				if (*((Bit16u *)buffer+83) & (1 << 10)) 
				{ // word 83 - lba48 support
					sectors_low  = *((Bit32u *)buffer+50); // word 100 and word 101
					sectors_high = *((Bit32u *)buffer+51); // word 102 and word 103
				} 
				else 
				{
					sectors_low = *((Bit32u *)buffer+30) // word 60 and word 61
					sectors_high = 0;
				}

				pEbda->ata.devices[device].device,ATA_DEVICE_HD);
				pEbda->ata.devices[device].removable, removable);
				pEbda->ata.devices[device].mode, mode);
				pEbda->ata.devices[device].blksize, blksize);
				pEbda->ata.devices[device].pchs.heads, heads);
				pEbda->ata.devices[device].pchs.cylinders, cylinders);
				pEbda->ata.devices[device].pchs.spt, spt);
				pEbda->ata.devices[device].sectors_low, sectors_low);
				pEbda->ata.devices[device].sectors_high, sectors_high);

				translation = inb_cmos(0x39 + channel/2);
				for (shift=device%4; shift>0; shift--) 
					translation >>= 2;
				translation &= 0x03;

				pEbda->ata.devices[device].translation, translation);
				switch (translation) 
				{
					case ATA_TRANSLATION_NONE:
					{
						break;
					}
					case ATA_TRANSLATION_LBA:
					{
						spt = 63;
						sectors_low /= 63;
						heads = sectors_low / 1024;
						if (heads>128) 
						{
							heads = 255;
						}
						else if (heads>64) 
						{
							heads = 128;
						}
						else if (heads>32) 
						{
							heads = 64;
						}
						else if (heads>16) 
						{
							heads = 32;
						}
						else 
						{
							heads=16;
						}
						cylinders = sectors_low / heads;
						break;
					}
					case ATA_TRANSLATION_RECHS:
					{
					  // Take care not to overflow
						if (heads==16) 
						{
							if(cylinders>61439) 
							{
								cylinders=61439;
							}
							heads=15;
							cylinders = (Bit16u)((Bit32u)(cylinders)*16/15);
						}
					}
					// then go through the large bitshift process
					case ATA_TRANSLATION_LARGE:
					{
						while(cylinders > 1024) 
						{
							cylinders >>= 1;
							heads <<= 1;
							// If we max out the head count
							if (heads > 127) break;
						}
						break;
					}

					// clip to 1024 cylinders in lchs
					if (cylinders > 1024) 
					{
						cylinders=1024;
					}

					pEbda->ata.devices[device].lchs.heads, heads);
					pEbda->ata.devices[device].lchs.cylinders, cylinders);
					pEbda->ata.devices[device].lchs.spt, spt);

					// fill hdidmap
					pEbda->ata.hdidmap[hdcount], device);
					hdcount++;
				}
			}
			
			// Now we send a IDENTIFY command to ATAPI device
			if(type == ATA_TYPE_ATAPI) 
			{
				Bit8u  type, removable, mode;
				Bit16u blksize;

				//Temporary values to do the transfer
				pEbda->ata.devices[device].device = ATA_DEVICE_CDROM;
				pEbda->ata.devices[device].mode = ATA_MODE_PIO16;

				if (ata_cmd_data_in(device,ATA_CMD_IDENTIFY_DEVICE_PACKET, 1, 0, 0, 0, 0L, 0L, get_SS(),buffer) != 0)
				{
					PrintMsg("ata-detect: Failed to detect ATAPI device\n");
				}

				type      = buffer[1] & 0x1f;
				removable = (buffer[0] & 0x80) ? 1 : 0;
				mode      = buffer[96] ? ATA_MODE_PIO32 : ATA_MODE_PIO16;
				blksize   = 2048;

				pEbda->ata.devices[device].device = type;
				pEbda->ata.devices[device].removable = removable;
				pEbda->ata.devices[device].mode = mode;
				pEbda->ata.devices[device].blksize = blksize;

				// fill cdidmap
				pEbda->ata.cdidmap[cdcount] = device;
				cdcount++;
			}
			
			{
				Bit32u sizeinmb;
				Bit16u ataversion;
				Bit8u  c, i, version, model[41];

				switch (type) {
					case ATA_TYPE_ATA:
					{
						sizeinmb = (read_dword(ebda_seg,&EbdaData->ata.devices[device].sectors_high) << 21)| (read_dword(ebda_seg,&EbdaData->ata.devices[device].sectors_low) >> 11);
					}
					case ATA_TYPE_ATAPI:
					// Read ATA/ATAPI version
					{
						ataversion=((Bit16u)(read_byte(get_SS(),buffer+161))<<8)|read_byte(get_SS(),buffer+160);
						for(version=15;version>0;version--) 
						{
							if((ataversion&(1<<version))!=0)
							break;
						}

						// Read model name
						for(i=0;i<20;i++) 
						{
							write_byte(get_SS(),model+(i*2),read_byte(get_SS(),buffer+(i*2)+54+1));
							write_byte(get_SS(),model+(i*2)+1,read_byte(get_SS(),buffer+(i*2)+54));
						}

						// Reformat
						write_byte(get_SS(),model+40,0x00);
						for(i=39;i>0;i--)
						{
							if(read_byte(get_SS(),model+i)==0x20)
							{
								write_byte(get_SS(),model+i,0x00);
							}
							else break;
						}
						if (i>36) 
						{
							write_byte(get_SS(),model+36,0x00);
							for(i=35;i>32;i--)
							{
								write_byte(get_SS(),model+i,0x2E);
							}
						}
						break;
					}

					switch (type) 
					{
						case ATA_TYPE_ATA:
						{
							printf("ata%d %s: ",channel,slave?" slave":"master");
							i=0;
							while(c=read_byte(get_SS(),model+i++))
							{
								printf("%c",c);
							}
							if (sizeinmb < (1UL<<16))
							{
								printf(" ATA-%d Hard-Disk (%4u MBytes)\n", version, (Bit16u)sizeinmb);
							}
							else
							{
								printf(" ATA-%d Hard-Disk (%4u GBytes)\n", version, (Bit16u)(sizeinmb>>10));
							}
							break;
						}
						case ATA_TYPE_ATAPI:
						{
							printf("ata%d %s: ",channel,slave?" slave":"master");
							i=0; 
							while(c=read_byte(get_SS(),model+i++)) 
							{
								printf("%c",c);
							}
							if(read_byte(ebda_seg,&EbdaData->ata.devices[device].device)==ATA_DEVICE_CDROM)
							{
								printf(" ATAPI-%d CD-Rom/DVD-Rom\n",version);
							}
							else
							{
								printf(" ATAPI-%d Device\n",version);
							}
							break;
						}
						case ATA_TYPE_UNKNOWN:
						{
							printf("ata%d %s: Unknown device\n",channel,slave?" slave":"master");
							break;
						}
					}
				}
			}

			// Store the devices counts
			pEbda->ata.hdcount, hdcount);
			pEbda->ata.cdcount, cdcount);
			*(unsigned char *)0x475 = hdcount;
		}
	}
	*/
}

void ata_reset(Bit16u device)
{
	Bit16u ebda_seg= *((short *)0x40e);
	ebda_data_t *pEbda = ebda_seg<<4;
	Bit16u iobase1, iobase2;
	Bit8u  channel, slave, sn, sc;
	Bit8u  type;
	Bit16u max;

	channel = device / 2;
	slave = device % 2;
	
	iobase1 = pEbda->ata.channels[channel].iobase1;
	iobase2 = pEbda->ata.channels[channel].iobase2;

	// Reset

	// 8.2.1 (a) -- set SRST in DC
	WritePortChar(iobase2+ATA_CB_DC, ATA_CB_DC_HD15 | ATA_CB_DC_NIEN | ATA_CB_DC_SRST);

	// 8.2.1 (b) -- wait for BSY
	await_ide(BSY, iobase1, 20);

	// 8.2.1 (f) -- clear SRST
	WritePortChar(iobase2+ATA_CB_DC, ATA_CB_DC_HD15 | ATA_CB_DC_NIEN);

	type = pEbda->ata.devices[device].type;
	if (type != ATA_TYPE_NONE) 
	{
		// 8.2.1 (g) -- check for sc==sn==0x01
		// select device
		WritePortChar(iobase1+ATA_CB_DH, slave?ATA_CB_DH_DEV1:ATA_CB_DH_DEV0);
		sc = ReadPortChar(iobase1+ATA_CB_SC);
		sn = ReadPortChar(iobase1+ATA_CB_SN);

		if ( (sc==0x01) && (sn==0x01) ) 
		{
			if (type == ATA_TYPE_ATA) //ATA
				await_ide(NOT_BSY_RDY, iobase1, IDE_TIMEOUT);
			else //ATAPI
				await_ide(NOT_BSY, iobase1, IDE_TIMEOUT);
		}

		// 8.2.1 (h) -- wait for not BSY
		await_ide(NOT_BSY, iobase1, IDE_TIMEOUT);
	}
	// Enable interrupts
	WritePortChar(iobase2+ATA_CB_DC, ATA_CB_DC_HD15);
}

static int await_ide(Bit8u when_done,Bit16u base,Bit16u timeout)
{
  Bit32u time=0,last=0;
  Bit16u status;
  Bit8u result;
  status = ReadPortChar(base + ATA_CB_STAT); // for the times you're supposed to throw one away
  for(;;) 
  {
    status = ReadPortChar(base+ATA_CB_STAT);
    time++;
    if (when_done == BSY)
      result = status & ATA_CB_STAT_BSY;
    else if (when_done == NOT_BSY)
      result = !(status & ATA_CB_STAT_BSY);
    else if (when_done == NOT_BSY_DRQ)
      result = !(status & ATA_CB_STAT_BSY) && (status & ATA_CB_STAT_DRQ);
    else if (when_done == NOT_BSY_NOT_DRQ)
      result = !(status & ATA_CB_STAT_BSY) && !(status & ATA_CB_STAT_DRQ);
    else if (when_done == NOT_BSY_RDY)
      result = !(status & ATA_CB_STAT_BSY) && (status & ATA_CB_STAT_RDY);
    else if (when_done == TIMEOUT)
      result = 0;

    if (result) return 0;
    if (time>>16 != last) // mod 2048 each 16 ms
    {
      last = time >>16;
     // BX_DEBUG_ATA("await_ide: (TIMEOUT,BSY,!BSY,!BSY_DRQ,!BSY_!DRQ,!BSY_RDY) %d time= %ld timeout= %d\n",when_done,time>>11, timeout);
    }
    if (status & ATA_CB_STAT_ERR)
    {
      //BX_DEBUG_ATA("await_ide: ERROR (TIMEOUT,BSY,!BSY,!BSY_DRQ,!BSY_!DRQ,!BSY_RDY) %d time= %ld timeout= %d\n",when_done,time>>11, timeout);
      return -1;
    }
    if ((timeout == 0) || ((time>>11) > timeout)) break;
  }
  //BX_INFO("IDE time out\n");
  return -1;
}

short GetEBDASeg()
{
	short seg;
	__asm__ (
		"CALL switch_protect_to_real\n\t"
		".code16\n\t"
		"MOVB $0xC1,%%AH\n\t"
		"INT $0x15\n\t"
		"MOVW %%ES,%%BX\n\t"
		"PUSHW %%CS\n\t"
		"CALL switch_real_to_protect\n\t"
		".code32\n\t"
		: "=b" (seg)
		:
	);
	return seg;
}