/*===========================================================================
 *
 *  mlt_usip_debug.ld
 *
 *===========================================================================
 *
 * This is a sample linker command file specifying how the object files
 * in a build might be linked and located. Combined with the other
 * linker command files provided with the compiler, it provides a base
 * to build your own file for your particular system.
 *
 * For a description of the different statements in this file, please
 * refer to the LD section of the User's Guide.
 *
 *===========================================================================*/

OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH(mips)
ENTRY(int_vector)


MEMORY
{
	flash(rwx)	: ORIGIN = 0x40000000, LENGTH = 32M
	sram(rwx)	: ORIGIN = 0x50000000, LENGTH = 2M
}

SECTIONS
{
	.com.vect.r 0x40000000:
	{
		KEEP(*(.cstartup))
	} > flash
	
	.usr.text :
	{
		_stext = .;	_ftext = .;
		*(.lib.prog)
		*(.lib.text)
		hal*(.text .rodata*)
		emv*(.text .rodata*)
		at83*(.text .rodata*)
		sc_at83*(.text .rodata*)
		*(.reftable)
		*(.text)
		*(.rodata*)
		*(.text*)
		*(.gnu.linkonce.t*)
    	*(.gnu.linkonce.r*)
    	*(.rel.dyn*)
	} > flash
	_etext = .; PROVIDE (etext = .);
}

SECTIONS
{
	/* Flash working buffer */
	.flash.buffer 0x50000000:
	{
		. += 0x20000 ;
	} > sram
	
	.com.text :
	{
		__com_ram_start = .;
		*(.text.ram)
	} > sram
	
	__com_ram_end = .;
	__com_rom_start = LOADADDR(.com.text);
	
	.lib.bss ALIGN(4) :
	{
		__lib_bss_start = .;
		*(.lib.ram)
	} > sram
	
	.emv.bss :
	{
		hal*(.bss COMMON .sbss .scommon)
		emv*(.sbss .scommon .bss COMMON)
		sc_at83*(.bss COMMON .sbss .scommon)
		*(SORT(.sys_table*emv*))
		*(SORT(.sys_table*))
	} > sram
	
	__lib_bss_end = .;
	
	.usr.sbss ALIGN(4) :
	{
		__usr_bss_start = .; _fbss = .;
		*(.sbss .scommon)
	} > sram
	
	.usr.bss ALIGN(4):
	{
		*(.bss COMMON)
	} > sram
	
	__usr_bss_end = .;
	__usr_end = .;
	
	.usr.data : AT( ALIGN(LOADADDR(.usr.text) + SIZEOF(.usr.text),16) )
	{
		__usr_ram_start = .; _fdata = .;
		*(.data .sdata)
		*(.lit4 .lit8)
		KEEP(*(SORT (.dev*)));
		_edata  = ABSOLUTE(.);
	} > sram
	
	__usr_ram_end = .;
	__usr_rom_start = LOADADDR(.usr.data);
	
	PROVIDE(edata = .);
}

SECTIONS
{
	/* Must reside in uncached internal RAM */
	__com_bss_start = 0xa0008000;
	.com.bss __com_bss_start :
	{
		/* . += 32K; */
		*(.bss.usb)
		. = ALIGN(32);
	}
	
	__com_bss_end = .;
	. = __com_bss_end & 0xdFFFFFFF;
	
	/* Must reside in uncached internal RAM */
	.hal.heap 0xa001c500 :
	{
		__hal_heap_start = .;
		/* Allocate room for heap */
		. += 8K;
		__hal_heap_end = .;
	}
}

