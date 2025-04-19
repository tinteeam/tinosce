#include "stdio.h"
#include "kprint.h" // tarvitaan kprint_char()
#include "../drivers/keyboard/keyboard.h" // jos sinulla on oma keyboard driver

void fgets(char* buffer, int max_len) {
    int i = 0;
    char c;

    while (i < max_len - 1) {
        c = getchar(); // implementoi omassa keyboard driverissa
        if (c == '\n' || c == '\r') {
            kprint_char('\n');
            break;
        } else if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                kprint_char('\b');
                kprint_char(' ');
                kprint_char('\b');
            }
        } else {
            buffer[i++] = c;
            kprint_char(c);
        }
    }

    buffer[i] = '\0';
}

// Yksinkertaiset string-funktiot

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, unsigned int n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    return n == 0 ? 0 : *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

