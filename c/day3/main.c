#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool is_digit(char c);
bool is_symbol(char c);
uint32_t dig_pos(uint32_t digit, uint8_t pos);
bool is_any_symbol_around(FILE *f, ssize_t lineSize, uint16_t line, uint16_t offset);

int main(void) {
    FILE *f = NULL;
    if ((f = fopen("test", "r")) == NULL) {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    ssize_t nread;
    size_t len = 0;
    char *line = NULL;
    uint16_t lineNo = 0;
    uint32_t sum = 0;
    while ((nread = getline(&line, &len, f)) != -1) {
        lineNo++;
        bool validNumber = false;
        uint8_t digitCount = 0;
        uint16_t number = 0;
        for (int i = nread; i >= 0; i--) {
            if (is_digit(line[i])) {
                number += dig_pos(line[i] - '0', digitCount);
                digitCount++;
                if (is_any_symbol_around(f, nread, lineNo, i)) {
                    validNumber = true;
                }
            }
            if (line[i] == '.' || i == 0) {
                if (validNumber) printf("%u\n", number);
                validNumber = false;
                digitCount = 0;
                number = 0;
            }
        }
    }

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
    if (is_digit(c) || c == '.') {
        return false;
    }
    return true;
}

uint32_t dig_pos(uint32_t digit, uint8_t pos) {
    uint32_t pow = 1;
    for (uint8_t i = 0; i < pos; i++) {
        pow *= 10;
    }
    return digit * pow;
}

bool is_any_symbol_around(FILE *f, ssize_t lineSize, uint16_t line, uint16_t offset) {
    FILE *f2 = NULL;
    if ((f2 = fopen("test", "r")) == NULL) {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    for (int j = line - 2; j <= line; j++) {
        if (j < 0) continue;
        for (int i = offset - 1; i < offset + 2; i++) {
            if (i < 0) continue;
            fseek(f2, (j * lineSize) + i, SEEK_SET);
            if (is_symbol(getc(f))) return true;
        }
    }
    if (f2) fclose(f2);
    return false;
}
