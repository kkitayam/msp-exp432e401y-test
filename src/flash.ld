MEMORY
{
	FLASH : ORIGIN = 0x00000000, LENGTH = 0x100000
	SRAM  : ORIGIN = 0x20000000, LENGTH = 0x40000
}

MSP_SIZE = 2K;
PSP_SIZE = 4K;
BOOT_SIZE = 32K;

__msp_top   = ORIGIN(SRAM) + LENGTH(SRAM);
__msp_limit = __msp_top - MSP_SIZE;
__psp_top   = __msp_limit;
__psp_limit = __psp_top - PSP_SIZE;

PROVIDE(NMI_Handler            = 0xFFFFFFFF);
PROVIDE(HardFault_Handler      = 0xFFFFFFFF);
PROVIDE(MemManage_Handler      = 0xFFFFFFFF);
PROVIDE(BusFault_Handler       = 0xFFFFFFFF);
PROVIDE(UsageFault_Handler     = 0xFFFFFFFF);
PROVIDE(SVC_Handler            = 0xFFFFFFFF);
PROVIDE(DebugMon_Handler       = 0xFFFFFFFF);
PROVIDE(PendSV_Handler         = 0xFFFFFFFF);
PROVIDE(SysTick_Handler        = 0xFFFFFFFF);

ENTRY(Reset_Handler)

SECTIONS
{
	.vector :
	{
		LONG(__msp_top)
		LONG(Reset_Handler | 1)
		LONG(NMI_Handler | 1)
		LONG(HardFault_Handler | 1)
		LONG(MemManage_Handler | 1)
		LONG(BusFault_Handler | 1)
		LONG(UsageFault_Handler | 1)
		LONG(SVC_Handler | 1)
		LONG(DebugMon_Handler | 1)
		LONG(PendSV_Handler | 1)
		LONG(SysTick_Handler | 1)
	} > FLASH = 0xFF

	. = ORIGIN(SRAM);
	.text.ram ALIGN(4) :
	{
		__text = .;
		*(.text.*ramfunc*)
		. = ALIGN(4);
		__etext = .;
	} > SRAM AT > FLASH = 0xFF
	__text_load = LOADADDR(.text.ram);

	. = SIZEOF(.vector) + SIZEOF(.text.ram);
	.text ALIGN(4) :
	{
		*(.text) *(.text.*)
		. = ALIGN(4);
	} > FLASH = 0xFF

	.rodata ALIGN(4) :
	{
		*(.rodata) *(.rodata.*)
		. = ALIGN(4);
		__erodata = .;
	} > FLASH = 0xFF

	. = ORIGIN(SRAM) + SIZEOF(.text.ram);
	.data ALIGN(4) :
	{
		__data = .;
		*(.data) *(.data.*)
		*(.sdata) *(.sdata.*)
		. = ALIGN(4);
		__edata = .;
	} > SRAM AT > FLASH = 0xFF
	__data_load = LOADADDR(.data);

	.bss ALIGN(4) (NOLOAD) :
	{
		__bss = .;
		*(.bss) *(.bss.*)
		*(.sbss) *(.sbss.*)
		*(COMMON)
		. = ALIGN(4);
		__ebss = .;
		__end = .;
	} > SRAM
}

ASSERT(__msp_limit >= __ebss, "RAM overflow");
ASSERT(SIZEOF(.data) == 0, "Initialized global/static variables are forbidden in the bootloader.");
