#include <stdio.h>
#include <base64.h>
#include <cstring>
#include <string>
#include "lib.h"

int main() {
    //int a = cxor( 5, 6 );
    // printf("hello,world!");
    const char * msg = "hello,world!";
    std::string base64code;
    base64::Base64Encode( (int8_t*)msg, strlen(msg), base64code);
    printf("base64 code : %s\n", base64code.c_str());
    return 0;
}