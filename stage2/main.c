void main()
{
	Move_To_ZeroMem();
	Init_Gdt();
	Init_Idt();
	Init_Page();
	Init_Process();
}
