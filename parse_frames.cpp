/*
The MIT License (MIT)

Copyright (c) 2016 Ryan Shuttleworth, rshuttle@gmail.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <stdint.h>
#include "parse_frames.h"


//#define PARSE_FRAMES_STAND_ALONE
#define WS (" \t\r\n")

int isAmoung(char c, const char *s) {
    while (*s != 0) {
        if (c == *s++) {
            return 1;
        }
    }

    return 0;
}

void lstrip(char *s) {
    char *p = s;

    while ((isAmoung(*s, WS) == 1) && (*s != 0)) {
        s++;
    }

    while (*s != 0) {
        *p++ = *s++;
    }

    *p = 0;
}

void rstrip(char *s) {
    char *p = s;

    while (*s != 0) {
        s++;
    }

    s -= 1;

    while ((s > p) && (isAmoung(*s, WS) == 1)) {
        *s-- = 0;
    }
}

void strip(char *s) {
    lstrip(s);
    rstrip(s);
}

int32_t xtoi(char * s) {
    int32_t o = 0;

    while (*s != 0) {
        o <<= 4;
        if ((*s >= '0') && (*s <= '9')) {
            o += *s - '0';
        } else if ((*s >= 'a') && (*s <= 'f')) {
            o += *s - 'a';
        } else if ((*s >= 'A') && (*s <= 'F')) {
            o += *s - 'A';
        } else {
            break;
        }
        s++;
    }

    return o;
}

int32_t btoi(char * s) {
    int32_t o = 0;

    while (*s != 0) {
        o <<= 1;
        if ((*s == '0') || (*s == '1')) {
            o += *s - '0';
        } else {
            break;
        }
        s++;
    }

    return o;
}

int32_t otoi(char *s) {
    int32_t o = 0;

    while (*s != 0) {
        o <<= 3;
        if ((*s >= '0') && (*s <= '7')) {
            o += *s - '0';
        } else {
            break;
        }
        s++;
    }

    return o;
}

int32_t dtoi(char *s) {
    int32_t o = 0;

    while (*s != 0) {
        o *= 10;
        if ((*s >= '0') && (*s <= '9')) {
            o += *s - '0';
        } else {
            break;
        }
        s++;
    }

    return o;
}

int32_t anytoi(char *s) {
    int32_t o = 0;
    int32_t n = 1;
    char *p = s;

    while (isAmoung(*s, WS) == 1) {
        *s++;
    }

    while (*s == 0) {
        s++;
    }

    if (*s == '-') {
        n = -1;
        s++;
    }

    while (*s == '0') {
        s++;
    }

    switch(*s) {
        case 'x':
        o = xtoi(s+1);
        break;
        case 'o':
        o = otoi(s+1);
        break;
        case 'b':
        o = btoi(s+1);
        break;
        default:
        o = dtoi(s+1);
    }

    return n * o;
}

int parse_csv(const char *path, const char *sep, parser_callback cb) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *pch;
    int32_t r = 0;
    uint32_t line_n = 0;
    uint32_t col = 0;

    fp = fopen(path, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        line_n++;

        strip(line);

        if ((line[0] == '#') || (line[0] == 0)) {
            continue;
        }

        col = 0;
        pch = strtok(line, sep);
        while (pch != NULL) {
            col++;
            if (cb != NULL) {
                cb(line_n, col, pch);
            }
            pch = strtok(NULL, sep);
        }
    }

   free(line);
}

#ifdef PARSE_FRAMES_STAND_ALONE
int main (int argc, char *argv[]) {
    char s[] = "        this is a test        ";
    strip(s);
    printf("\nStripped: >%s<\n", s);
    printf("\n");
    parse_csv("./sample-frames.txt", ",");
}
#endif
