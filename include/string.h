#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include<types.h>


void* memmove(void* dest,const void* src,uint32_t len);

void* memcpy(void* dest,const void* src,uint32_t len);

void* memset(void *dest,uint8_t val,uint32_t len);

int strcmp(const char* str1,const char* str2,uint8_t len);

char* strcpy(char* dest,const char* src,uint8_t len);

char* strcat(char* dest,const char* src);

int strlen(const char* src);

#endif