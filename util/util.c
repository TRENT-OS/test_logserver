/*
   *  Implementation SEOS utility functions.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/


#include "util.h"
#include <stdint.h>



static void putchar_multiple(const char chr, size_t cnt);



static void putchar_multiple(const char chr, size_t cnt){
    while (cnt > 0){
        putchar(chr);
        cnt--;
    }
}



size_t print_byte(unsigned char chr){
    static const char table_hex_chars[] = "0123456789abcdef";

    putchar( table_hex_chars[(chr>>4) & 0xf] );
    putchar( table_hex_chars[chr & 0xf] );

    return 2;
}



size_t printHexDump(
    const char           *szDescriptor,  // descriptor string or NULL
    const volatile void  *blob,
    size_t               blobSize
){
    const volatile uint8_t *charBlob = (const volatile uint8_t *)blob;
    size_t addr = (size_t)blob;

    #define CPL 0x10  // chars per line
    size_t blockLen = CPL;
    unsigned int const moreThanOneLine = (blobSize > CPL);
    size_t cnt = 0;

    // if more than one line we add a descriptor anyway
    if(NULL != szDescriptor)
    {
        cnt += (unsigned long)(printf("%s",szDescriptor));
    }
    else if(moreThanOneLine)
    {
        cnt += (unsigned long)(printf("memory dump"));
    }

    if(0 == blobSize)
    {
        // not much to do
        if(NULL != szDescriptor)
        {
            cnt += (unsigned long)(printf(": "));
        }
        cnt += (unsigned long)(printf("[length is 0]\n"));
        return cnt;
    }
    else if(moreThanOneLine)
    {
        // some details about blob
        cnt += printf(" (%p, %d bytes):\n", blob, blobSize);
    }
    else if(NULL != szDescriptor)
    {
        cnt += (unsigned long)(printf(": "));
    }

    do {
        if(blobSize < blockLen)
        {
            blockLen = blobSize;
        }

        // address
        if(moreThanOneLine)
        {
            cnt += printf("| %x | ", addr);
            addr += CPL;
        }
        // bytes as hex
        for(size_t i=0; i<blockLen; i++)
        {
            //cnt += printf("%02x ", charBlob[i]);
            // "%02x" is not supported
            print_byte(charBlob[i]);
            putchar(' ');
            cnt += 3;
        }
        // spaces if necessary
        if((blockLen < CPL) && moreThanOneLine)
        {
            size_t tmpLen = 3*(CPL-blockLen);
            putchar_multiple(' ', tmpLen);
            cnt += tmpLen;
        }
        // bytes as ASCII
        cnt += (unsigned long)(printf("| "));
        for(size_t i=0; i<blockLen; ++i)
        {
            uint8_t c = charBlob[i];
            if((c<0x20) || (c>0x7E)){ c = '.'; }
            putchar(c); // printf() does not support %c
            cnt++;
        }
        // we are done if we print just one line
        if(!moreThanOneLine)
        {
            printf("\n");
            cnt++;
            break;
        }
        // add spaces if necessary
        if((blockLen < CPL))
        {
            size_t tmpLen = CPL-blockLen;
            putchar_multiple(' ', CPL-blockLen);
            cnt += tmpLen;
        }
        cnt += (unsigned long)(printf(" |\n"));
        blobSize -= blockLen;
        charBlob = &(charBlob[blockLen]);

    } while (blobSize > 0);

    return cnt;
}
