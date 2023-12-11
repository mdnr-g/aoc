#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool is_digit(char c);
bool is_symbol(char c);
uint32_t dig_pos(uint32_t digit, uint8_t pos);
bool is_any_symbol_around(char *line0, char *line1, char *line2, uint16_t offset);

int main(void) {
    FILE *f = NULL;
    if ((f = fopen("puzzle", "r")) == NULL) {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    ssize_t nread;
    size_t len = 0;
    char *line = NULL;
    uint16_t lineNo = 0;
    uint32_t sum = 0;
    char lineBuf[3][1024];
    bool finished = false;
    ssize_t lineSize = 0;
    while (!finished) {
        nread = getline(&line, &len, f);
        if (nread != -1) lineSize = nread;
        lineNo++;
        strcpy(lineBuf[0], lineBuf[1]);
        strcpy(lineBuf[1], lineBuf[2]);
        strcpy(lineBuf[2], line);
        if (lineNo >= 2) {
            bool validNumber = false;
            uint8_t digitCount = 0;
            uint16_t number = 0;
            for (int i = lineSize; i >= 0; i--) {
                if (is_digit(lineBuf[1][i])) {
                    number += dig_pos(lineBuf[1][i] - '0', digitCount);
                    digitCount++;
                    if (is_any_symbol_around(lineBuf[0], lineBuf[1], lineBuf[2], i)) {
                        validNumber = true;
                    }
                } 
                if (!is_digit(lineBuf[1][i]) || i == 0) {
                    if (validNumber) 
                        sum += number;
                    validNumber = false;
                    digitCount = 0;
                    number = 0;
                }
            }
        }
        if (nread == -1) finished = true;
    }
    printf("sum = %u\n", sum);

    if (line) free(line);
    if (f) fclose(f);
    return 0;
}

bool is_digit(char c) {
    for (uint8_t i = 0; i <= 9; i++) {
        if (c - '0' == i) return true;
    }
    return false;
}

bool is_symbol(char c) {
    // if (c == '#' || c == '+' || c == '$' || c == '*') {
    if (c == 61 || (33 <= c && c <= 47 && c != '.')) {
        return true;
    }
    return false;
}

uint32_t dig_pos(uint32_t digit, uint8_t pos) {
    uint32_t pow = 1;
    for (uint8_t i = 0; i < pos; i++) {
        pow *= 10;
    }
    return digit * pow;
}

bool is_any_symbol_around(char *line0, char *line1, char *line2, uint16_t offset) {
    for (int i = offset - 1; i < offset + 2; i++) {
        if (i < 0) continue;
        if (is_symbol(line0[i]) || is_symbol(line1[i]) || is_symbol(line2[i])) return true;
    }
    return false;
}
