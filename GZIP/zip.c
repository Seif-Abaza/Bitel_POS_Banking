/* zip.c -- compress files to the gzip or pkzip format
* Copyright (C) 1992-1993 Jean-loup Gailly
* This is free software; you can redistribute it and/or modify it under the
* terms of the GNU General Public License, see the file COPYING.
*/
#include "gzip.h"
local unsigned long crc; /* crc on uncompressed file data */
/* zip.c -- compress files to the gzip or pkzip format
* Copyright (C) 1992-1993 Jean-loup Gailly
* This is free software; you can redistribute it and/or modify it under the
* terms of the GNU General Public License, see the file COPYING.
*/
#include "gzip.h"
local unsigned long crc = 0; /* crc on uncompressed file data */
/* ===========================================================================
* Deflate in to out.
* IN assertions: the input and output buffers are cleared.
* The variables time_stamp and save_orig_name are initialized.
*/
int zip( void )
{
	unsigned char flags = 0; /* general purpose bit flags */
	unsigned short attr = 0; /* ascii/binary flag */
	unsigned short deflate_flags = 0; /* pkzip -es, -en or -ex equivalent */
	outcnt = 0; /* Write the header to the gzip file. See algorithm.doc for the format */
	method = DEFLATED;
	DBGLine_m("Zip1");

	put_byte( GZIP_MAGIC[0] ); /* magic header */
	put_byte( GZIP_MAGIC[1] );
	put_byte( DEFLATED ); /* compression method */
	put_byte( flags ); /* general flags */
	put_long( time_stamp ); /* Write deflated file to zip file */
	DBGLine_m("Zip2");

	crc = updcrc( 0, 0 );
	DBGLine_m("Zip3");

	bi_init();
	DBGLine_m("Zip4");

	ct_init(& attr, & method );
	DBGLine_m("Zip5");
	DBGLine_m("level:%d",level);
	level = 5;
	DBGLine_m("level:%d",level);
	DBGLine_m("level addr:%08lX",&level);

	//lm_init( level, & deflate_flags );
	lm_init( 6, & deflate_flags );
	DBGLine_m("Zip6");

	put_byte(( unsigned char )deflate_flags ); /* extra flags */
	put_byte( 0 ); /* OS identifier */
	DBGLine_m("Zip7");
	( void )deflate(); /* Write the crc and uncompressed size */
	DBGLine_m("Zip8");
	put_long( crc );
	put_long( isize );
	flush_outbuf();
	return 0;
}

char * zip_mem_inptr = NULL; /* 输入缓存 */
int zip_mem_insize = 0; /* 输入缓存长度 */
int zip_mem_inpos = 0; /* 已经使用的位置 */
int mem_read( char * buf, unsigned size )
{
	int len;
#define min( a, b ) ((( int )( a ) < ( int )( b )) ? ( a ) : ( b ))
	len = min( zip_mem_insize - zip_mem_inpos, size );
	if( len > 0 )
	{
		memcpy( buf, & zip_mem_inptr[zip_mem_inpos], len );
		crc = updcrc(( unsigned char  *)buf, len );
		isize += ( unsigned long )len;
		zip_mem_inpos += len;
	}
	else
		len = - 1;
	return ( int )len;
}








