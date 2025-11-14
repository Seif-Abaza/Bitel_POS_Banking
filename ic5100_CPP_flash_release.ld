
/*===========================================================================
 *
 * This is a sample linker command file for Bitel terminal
 *
 *===========================================================================*/
 
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH(mips)
ENTRY(int_vector)	

 
MEMORY
{

 	flash(rwx)  : ORIGIN = 0x40000000, LENGTH = 8M
	sram(rwx)  	: ORIGIN = 0x50000000, LENGTH = 2M
}


SECTIONS
{

	.com.vect.r :  
    {
   		KEEP(* (.cstartup))	
 		 
    } > flash   

	.usr.text :
    {
    	_stext = .;	_ftext = . ;
	 	*(.lib.prog)
    	*(.lib.text)
		hal*(.text .rodata*)
		emv*(.text .rodata*)
		at83*(.text .rodata*)
		sc_at83*(.text .rodata*)
		*(.reftable)
    	*(.text)
    	*(.rodata*)
  
	    } > flash	   	   
    _etext = .; PROVIDE (etext = .);


}


SECTIONS
{


	/* Flash working buffer */
	.flash.buffer 0x50000000: 
    {
		. += 0x10000 ; 
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


   .hal.heap :
    {
		__hal_heap_start = .;
		/* Allocate room for heap */
		. += 4K ;
		__hal_heap_end = .;
    } > sram


	.emv.bss : 
	{
		hal*(.bss COMMON .sbss .scommon )
		emv*(.sbss .scommon .bss COMMON)
		sc_at83*(.bss COMMON .sbss .scommon )
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
   	__usr_end = . ;


   .usr.data : /*AT( ALIGN(LOADADDR(.usr.text) + SIZEOF(.usr.text),16) )*/
	{		
		__usr_ram_start = .; _fdata = . ;
		*(.data .sdata)   	
    	*(.lit4 .lit8) 
     	KEEP(*( SORT (.dev*))) ; 	
    	_edata  = ABSOLUTE(.);	
    } > sram

	__usr_ram_end = .;
	__usr_rom_start = LOADADDR(.usr.data);

    PROVIDE(edata = .);
 
}

