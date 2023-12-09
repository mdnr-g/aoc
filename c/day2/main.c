#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    unsigned int count;
    char color[5];
} Cube_t;

// only 12 red cubes, 13 green cubes, and 14 blue cubes
Cube_t max_numbers[3] = {
    {.count = 0, .color = "red"},
    {.count = 0, .color = "blue"},
    {.count = 0, .color = "green"},
};

size_t split(char *line, char *field, char delim);
bool check_validity(Cube_t *roll);
void get_max_values(Cube_t *roll);

int main(void) {
    FILE *f = fopen("puzzle", "r");
    if (!f) {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    ssize_t read = 0;
    char *line = NULL;
    size_t len = 0;
    unsigned int sum = 0;
    while ((read = getline(&line, &len, f)) != -1) {
    // read = getline(&line, &len, f);
        char games[1024] = {0};
        unsigned int gameNo = 0;
        sscanf(line, "Game %u:%[^\n]", &gameNo, games);
        char field[1024] = {0};
        size_t idx = 0;
        bool game_valid = true;
        do {
            idx += split(games + idx, field, ';');
            Cube_t cubes[3] = {0};
            int numb = sscanf(field, "%u %[^,], %u %[^,], %u %[^,]", 
                    &cubes[0].count, cubes[0].color,
                    &cubes[1].count, cubes[1].color,
                    &cubes[2].count, cubes[2].color);
            for (int i = 0; i < numb / 2; i++) {
                // if (!check_validity(&cubes[i])) {
                //     game_valid = false;
                //     break;
                // }
                get_max_values(&cubes[i]);
            }
            memset(field, 0, sizeof(field));
            idx += 2;
        } while (idx < strlen(games) && game_valid);
        // if (game_valid) sum += gameNo;
        sum += max_numbers[0].count * max_numbers[1].count * max_numbers[2].count;
        max_numbers[0].count = 0;
        max_numbers[1].count = 0;
        max_numbers[2].count = 0;

    }
    printf("power is %u\n", sum);

    if (f) fclose(f);
    return 0;
}

size_t split(char *line, char *field, char delim) {
    size_t i = 0;
    for (; i < strlen(line) && line[i] != delim; i++) {
        field[i] = line[i];
    }
    return i;
}

bool check_validity(Cube_t *roll) {
    for (int i = 0; i < 3; i++) {
        if (strncmp(max_numbers[i].color, roll->color, strlen(max_numbers[i].color)) == 0) {
            if (max_numbers[i].count < roll->count) return false;
            else return true;
        }
    }
    return false;
}

void get_max_values(Cube_t *roll) {
    for (int i = 0; i < 3; i++) {
        if (strncmp(max_numbers[i].color, roll->color, strlen(max_numbers[i].color)) == 0) {
            if (max_numbers[i].count < roll->count) max_numbers[i].count = roll->count;
        }
    }
}
