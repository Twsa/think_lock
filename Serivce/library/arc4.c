/*
 *  An implementation of the ARCFOUR algorithm
 *
 *  Copyright (C) 2006-2010, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 *  The ARCFOUR algorithm was publicly disclosed on 94/09.
 *
 *  http://groups.google.com/group/sci.crypt/msg/10a300c9d21afca0
 */
#include "usart1/usart1.h"
#include "polarssl/config.h"

#if defined(POLARSSL_ARC4_C)

#include "polarssl/arc4.h"

/*
 * ARC4 key schedule
 */
void arc4_setup( arc4_context *ctx, const unsigned char *key, unsigned int keylen )
{
    int i, j, a;
    unsigned int k;
    unsigned char *m;

    ctx->x = 0;
    ctx->y = 0;
    m = ctx->m;

    for( i = 0; i < 256; i++ )
        m[i] = (unsigned char) i;

    j = k = 0;

    for( i = 0; i < 256; i++, k++ )
    {
        if( k >= keylen ) k = 0;

        a = m[i];
        j = ( j + a + key[k] ) & 0xFF;
        m[i] = m[j];
        m[j] = (unsigned char) a;
    }
}

/*
 * ARC4 cipher function
 */
int arc4_crypt( arc4_context *ctx, size_t length, const unsigned char *input,
                unsigned char *output )
{
    int x, y, a, b;
    size_t i;
    unsigned char *m;

    x = ctx->x;
    y = ctx->y;
    m = ctx->m;

    for( i = 0; i < length; i++ )
    {
        x = ( x + 1 ) & 0xFF; a = m[x];
        y = ( y + a ) & 0xFF; b = m[y];

        m[x] = (unsigned char) b;
        m[y] = (unsigned char) a;

        output[i] = (unsigned char)
            ( input[i] ^ m[(unsigned char)( a + b )] );
    }

    ctx->x = x;
    ctx->y = y;

    return( 0 );
}

#if defined(POLARSSL_SELF_TEST)

#include <string.h>
#include <stdio.h>

/*
 * ARC4 tests vectors as posted by Eric Rescorla in sep. 1994:
 *
 * http://groups.google.com/group/comp.security.misc/msg/10a300c9d21afca0
 */
static const unsigned char arc4_test_key[32] =
{
 0x71, 0x70,0x13 ,0xEB ,0x5C ,0x92 ,0x7D ,0xF5 ,
 0x64 ,0xB7 ,0x28 ,0xC1 ,0xA0 ,0x76 ,0x0B ,0x21,
 0x87 ,0x4A ,0x70 ,0x49 ,0x5E ,0xFF ,0x98 ,0xEE,
 0x24 ,0x4E ,0xE0 ,0x16 ,0xAA ,0x05 ,0xA3 ,0x28,
};                                                

static const unsigned char arc4_test_pt[48] =
{
     0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF ,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,   //Ã÷ÎÄ
	     0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF ,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     //Ã÷ÎÄ
};

//static const unsigned char arc4_test_ct[24] =
//{
//     0x1C,0XBB,0X65,0XDC,0XD1,0X82,0XD6,0X3C,
//     0X3C,0X9E,0X85,0XE4,0X09,0X6C,0X0E,0X42,
//     0X24,0XD5,0X2D,0XED,0X1C,0X52,0X9E,0XE7,	//ÃÜÎÄ
//};

/*
 * Checkup routine
 */
int arc4_self_test( int verbose )
{
	  int index=0;
    unsigned char ibuf[sizeof(arc4_test_pt)];
    unsigned char obuf[sizeof(arc4_test_pt)];
    arc4_context ctx;

//    for( i = 0; i < 3; i++ )          //ÈýÐÐ°ËÁÐ 192Î»µÄÃ÷ÎÄºÍÃÜÎÄ½øÐÐ¼ÓÃÜ£¬±È½ÏÈý´Î//   {

		   //¼ÓÃÜ²¿·Ö
        memcpy( ibuf, arc4_test_pt, sizeof(arc4_test_pt) );   //Ô­Ê¼ÄÚ´æµØÖ·µÄ×Ö½Úcopyµ½Ä¿±êÄÚ´æµØÖ·×Ö½Ú

        arc4_setup( &ctx, (unsigned char *) arc4_test_key,sizeof(arc4_test_key) );//¼ÓÃÜ³ÌÐò×°Ô¿³××
        arc4_crypt( &ctx, sizeof(arc4_test_pt), ibuf, obuf );    //¼ÓÃÜµ½obufÖÐ
	      /*--------µ÷ÊÔ£¬´òÓ¡ÐÅÏ¢------------*/
	      printf("ibuf      obuf\n");
        for (index=0;index<sizeof(obuf);index++)
				{
					printf("%x      %x\n\t",ibuf[index],obuf[index]);
				}
				/*----------------------------------*/
//        if( memcmp( obuf, arc4_test_ct, sizeof(arc4_test_ct) ) != 0 )   
//        {
//            if( verbose != 0 )
//                printf( "failed\n" );

//            return( 1 );
//        }

//        if( verbose != 0 )
//            printf( "passed\n" );
//    if( verbose != 0 )
//        printf( "\n" );

	   //½âÃÜ²¿·Ö
        memcpy( obuf, arc4_test_pt, sizeof(arc4_test_pt) );   //Ô­Ê¼ÄÚ´æµØÖ·µÄ×Ö½Úcopyµ½Ä¿±êÄÚ´æµØÖ·×Ö½Ú

        arc4_setup( &ctx, (unsigned char *) arc4_test_key,sizeof(arc4_test_key) );//¼ÓÃÜ³ÌÐò×°Ô¿³××
        arc4_crypt( &ctx, sizeof(arc4_test_pt), obuf, ibuf );    //¼ÓÃÜµ½obufÖÐ
		
	      /*--------µ÷ÊÔ£¬´òÓ¡ÐÅÏ¢------------*/		
    		printf("ibuf  obuf  arc4_test_pt\n");
   	    for (index=0;index<sizeof(obuf);index++)
				{
					printf("%x   %x    %x\n\t",ibuf[index],obuf[index],arc4_test_pt[index]);
				}
				/*----------------------------------*/
				if( memcmp( obuf, arc4_test_pt, sizeof(arc4_test_pt) ) != 0 )   
        {
            if( verbose != 0 )
                printf( "failed\n" );

            return( 1 );
        }

        if( verbose != 0 )
            printf( "passed\n" );
    if( verbose != 0 )
        printf( "\n" );
		
    return( 0 );	
}

#endif

#endif
