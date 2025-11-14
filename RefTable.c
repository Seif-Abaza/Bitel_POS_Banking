
/*******************************************************************************
         	     Reference Table to support Static Library working 
*******************************************************************************/

extern int main(void);
extern void utils_function(void);
extern void (*hal_interrupt_handlers[30])(void);

//extern void GetChinaLCDFONT(void);
//extern long GetChinaPRTFONT(int);

extern int __com_rom_start;
extern int __com_ram_start;
extern int __com_ram_end;

extern int __usr_rom_start;
extern int __usr_ram_start;
extern int __usr_ram_end;

extern int __lib_bss_start;
extern int __lib_bss_end;	

extern int __usr_bss_start;
extern int __usr_bss_end;	



// Please put Application name in here ( Max : 16byte size )
const char AppStrName[] = { "Application name" };


const long REFTABLE[] __attribute__ ((section (".reftable"))); 	
const long REFTABLE[] = {


/*******************************************************************************
         	         Please do not change this part 
*******************************************************************************/


/*-- content of search table -------------------------------------------------*/

	0x50524F47,
	0x52455800,	

/*-- 1. C main function ------------------------------------------------------*/

	(long)&main,

/*-- 2. C interrupt table ---------------------------------------------------*/

	(long)&hal_interrupt_handlers,				 
			 
						 
/*******************************************************************************
         	     Multi language display & printing support 
         	   -- Please adjust this part, If necessary -- 
*******************************************************************************/			 
			 

/*--- 3. double or single byte language ----------------------------------------
 Default setup for single or double byte language mode
		0x00000000,
		      ----
                |-> Reserved
            -- 
            |----> 2'nd : printer routine mode   ( 0= 1 byte, NZ = 2 bytes code )
          -- 
		  |------> 1'st : Lcd display mode       ( 0= 1 byte, NZ = 2 bytes code )
       
 ------------------------------------------------------------------------------*/

	0x00000000,      
/*  0x01010000,      */


/*--- 4. C function name for LCD display -------------------------------------*/

//	(long)&GetChinaLCDFONT,				/* 2 bytes code */
	0x00000000,				 			/* 1 byte code  */


/*--- 5. C function name for printing    -------------------------------------*/

//	(long)&GetChinaPRTFONT,				/* 2 bytes code */
	0x00000000,				 			/* 1 byte code  */


/*******************************************************************************
         	     Reference Table Related working RAM 
           --  Please do not change this part6 to part 18 --
*******************************************************************************/


	(long)&__com_rom_start,			// 6.	For variable initialize value 
	(long)&__com_ram_start,			// 7.	
	(long)&__com_ram_end,			// 8.


	(long)&__usr_rom_start,			// 9.	For variable initialize value 
	(long)&__usr_ram_start,			// 10.
	(long)&__usr_ram_end,			// 11.

    (long)&__lib_bss_start,			// 12.	For library RAM clear
    (long)&__lib_bss_end,			// 13	

    (long)&__usr_bss_start,			// 14.	For working RAM clear
    (long)&__usr_bss_end,			// 15.	


/*--- 16. C library function location    -------------------------------------*/

	(long)&utils_function,


/*--- 17. program size ( handle by PC downloading software )  ----------------*/

	0x00000000,      
	0x00000000,	

/*******************************************************************************
         	              Application file name 
         	       -- Please put your application file name  -- 
*******************************************************************************/
/*--- 19-22( max :16bytes ) Application file name         --------------------*/	


	(long)&AppStrName
 	
	
};


