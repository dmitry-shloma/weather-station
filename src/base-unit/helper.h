#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define ADDR_SIZE 8

bool addrcmp(const unsigned char addr1[], const unsigned char addr2[]);
void addrtoa(char *buf, const unsigned char *addr, const char *delim);

bool addrcmp(const unsigned char addr1[], const unsigned char addr2[])
{
    while(*addr1++ != *addr2++) {
        return false;
    }
    return true;
}

void addrtoa(char *buf, const unsigned char *addr, const char *delim)
{
    for (uint8_t i = 0; i < ADDR_SIZE; ++i) {
//        if (i < ADDR_SIZE - 1) {
//            sprintf(buf, "%02X%s", addr[i], delim);
//            buf+=3;
//        }
//        if (i < ADDR_SIZE - 1) {
            sprintf(buf, "%02X%s", addr[i]);
            buf+=2;
//        }

    }
}
//9236744484
//660118
//345623
#endif // HELPER_H
