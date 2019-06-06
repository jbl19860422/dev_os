ENTRY(_start)
SECTIONS
{
	. = 0xA400;	
	.text : {*(.text)}
	.data : {*(.data)}
	.bss : {*(.bss)}
	iKernelSize = .;
}
