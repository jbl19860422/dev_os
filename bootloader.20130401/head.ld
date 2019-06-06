ENTRY(_start)
SECTIONS
{
	DriverNum = 444;
	.text : {*(.text)}
	.data	: {*(.data)}
}
