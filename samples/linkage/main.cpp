#include <stdio.h>
#include <base64.h>
#include <cstring>
#include <string>
#include "lib.h"

int main() {
    // printf("hello,world!");
    const char * msg = "hello,world!";
    char base64code[24];
    base64::Base64Encode( (int8_t*)msg, strlen(msg), base64code);
    printf("base64 code : %s\n", base64code);
    //int a = xorpp( 5, 6 );
    //printf( "%d\n", global_integer );
    return 0;
}