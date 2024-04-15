#ifndef MINESWEEPER_SM_H
#define MINESWEEPER_SM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

// Definieer constanten
#define ROWS 8
#define COLS 8

// Declareer globale variabelen
char field[ROWS][COLS];
char visible[ROWS][COLS];
int bombs;
int remaining;

// Declareer functies
void init_field();
void place_bombs();
int count_adjacent_bombs(int row, int col);
void reveal_zeros(int row, int col);
void print_field();
int is_valid_move(int row, int col);
void make_move(int row, int col);
void save_game();
int load_game(unsigned long long bomb_mask, unsigned long long visible_mask);
void start_new_game();
void load_saved_game();
void play_game();

// Initialiseer het speelveld
void init_field() {
    memset(field, '-', sizeof(field));
    memset(visible, '*', sizeof(visible));
    remaining = ROWS * COLS - bombs;
}

// Plaats bommen op willekeurige posities
void place_bombs() {
    int placed = 0;
    while (placed < bombs) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        if (field[row][col] != 'X') {
            field[row][col] = 'X';
            placed++;
        }
    }
}

// Tel het aantal aangrenzende bommen voor een bepaalde cel
int count_adjacent_bombs(int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < ROWS && j >= 0 && j < COLS && field[i][j] == 'X') {
                count++;
            }
        }
    }
    return count;
}

// Onthul alle aangrenzende cellen met nul aangrenzende bommen
void reveal_zeros(int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visible[row][col] != '*') {
        return;
    }
    int count = count_adjacent_bombs(row, col);
    visible[row][col] = count + '0';
    remaining--;
    if (count == 0) {
        reveal_zeros(row - 1, col - 1);
        reveal_zeros(row - 1, col);
        reveal_zeros(row - 1, col + 1);
        reveal_zeros(row, col - 1);
        reveal_zeros(row, col + 1);
        reveal_zeros(row + 1, col - 1);
        reveal_zeros(row + 1, col);
        reveal_zeros(row + 1, col + 1);
    }
}

// Toon het speelveld op het scherm
void print_field() {
    printf("   A B C D E F G H\n");
    for (int i = 0; i < ROWS; i++) {
        printf("%d  ", i + 1);
        for (int j = 0; j < COLS; j++) {
            printf("%c ", visible[i][j]);
        }
        printf("\n");
    }
}

// Controleer of een zet geldig is
int is_valid_move(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && visible[row][col] == '*';
}

// Voer een zet uit op het speelveld
void make_move(int row, int col) {
    if (field[row][col] == 'X') {
        printf("Game over! Je hebt een bom geraakt.\n");
        memcpy(visible, field, sizeof(field));
    } else {
        reveal_zeros(row, col);
    }
}

// Sla de huidige spelstatus op
void save_game() {
    unsigned long long bomb_mask = 0;
    unsigned long long visible_mask = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (field[i][j] == 'X') {
                bomb_mask |= 1ULL << (i * COLS + j);
            }
            if (visible[i][j] != '*') {
                visible_mask |= 1ULL << (i * COLS + j);
            }
        }
    }
    printf("Spel opgeslagen. Bommenmaskering: %llX, Zichtbaarheidsmaskering: %llX\n", bomb_mask, visible_mask);
}

// Laad een opgeslagen spelstatus
int load_game(unsigned long long bomb_mask, unsigned long long visible_mask) {
    int loaded_bombs = 0;
    remaining = ROWS * COLS;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (bomb_mask & (1ULL << (i * COLS + j))) {
                field[i][j] = 'X';
                loaded_bombs++;
            } else {
                field[i][j] = '-';
            }
            if (visible_mask & (1ULL << (i * COLS + j))) {
                visible[i][j] = count_adjacent_bombs(i, j) + '0';
                remaining--;
            } else {
                visible[i][j] = '*';
            }
        }
    }
    return loaded_bombs == bombs && remaining >= 0;
}

// Start een nieuw spel
void start_new_game() {
    char input[20];
    int valid_input = 0;
    
    while (!valid_input) {
        printf("Voer het aantal bommen in: ");
        scanf("%s", input);
        
        // Check if the input is a valid integer
        int i = 0;
        while (input[i] != '\0') {
            if (!isdigit(input[i])) {
                printf("Ongeldig aantal bommen. Voer een geldig getal in.\n");
                break;
            }
            i++;
        }
        
        if (input[i] == '\0') {
            bombs = atoi(input);
            if (bombs > 0 && bombs < ROWS * COLS) {
                valid_input = 1;
            } else {
                printf("Ongeldig aantal bommen. Voer een getal tussen 1 en %d in.\n", ROWS * COLS - 1);
            }
        }
    }
    
    init_field();
    place_bombs();
}

// Laad een opgeslagen spel
void load_saved_game() {
    unsigned long long bomb_mask, visible_mask;
    printf("Voer de bommenmaskering in: ");
    scanf("%llX", &bomb_mask);
    printf("Voer de zichtbaarheidsmaskering in: ");
    scanf("%llX", &visible_mask);
    bombs = __builtin_popcountll(bomb_mask);
    if (!load_game(bomb_mask, visible_mask)) {
        printf("Ongeldige opgeslagen spelgegevens.\n");
    }
}

// Speel het spel
void play_game() {
    while (remaining > 0) {
        print_field();
        printf("Overgebleven: %.2f%%\n", (float)remaining / (ROWS * COLS - bombs) * 100);
        printf("Voer een zet in (bijv. 'A1' of 'save'): ");
        char input[20];
        scanf("%s", input);

        if (strcmp(input, "save") == 0) {
            save_game();
            continue;
        }

        int row = -1, col = -1;
        if (strlen(input) == 2) {
            if (isdigit(input[0]) && isalpha(input[1])) {
                row = input[0] - '1';
                col = tolower(input[1]) - 'a';
            } else if (isalpha(input[0]) && isdigit(input[1])) {
                row = input[1] - '1';
                col = tolower(input[0]) - 'a';
            }
        }

        if (!is_valid_move(row, col)) {
            printf("Ongeldige zet.\n");
            continue;
        }

        make_move(row, col);
        if (field[row][col] == 'X') {
            print_field();
            break;
        }
    }

  if (remaining == 0) {
        printf("Gefeliciteerd! Je hebt gewonnen.\n");
    }
}
#endif