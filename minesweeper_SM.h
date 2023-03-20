#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "stdbool.h"
#include <inttypes.h>

// Function prototypes
void initBoard();
void printBoard();
void placeBombs(int numBombs);
int playGame();
int countAdjBombs(int row, int col);
void floodFill(int row, int col);
void saveGame();
void calculatePercentage();
void revealBombs();
void inputName();

// Defines
#define ROWS 8
#define COLS 8

// Board initialisations
    // Initializes the game board with all cells unrevealed
    char board[ROWS][COLS];

    // Initializes the bomb locations on the game board
    char bombs[ROWS][COLS];

    // Stores whether each cell on the game board has been revealed
    bool revealed[ROWS][COLS]; 


// Global Variables
    // Indicates whether the game is over
    bool gameOver = false;

    // Stores the number of bombs on the game board
    int numBombs = 0;

    // Stores the locations of the bombs on the game board as a series of bits
    long long bombBits = 0;

    // Stores the revealed cells on the game board as a series of bits
    long long openedBits = 0;



//Allows the user to print their name at the start of the game
void inputName()
{
    char name[50];
    printf("\nEnter your name: ");
    scanf("%s", name);
    printf("Hello, %s!\n", name);
}

// Initializes the board with '-' in each cell
void initBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = '-';
        }
    }
}

// Prints the current state of the board
void printBoard()
{
    int i, j;

    printf("  ");
    for (i = 0; i < COLS ; i++)
    {
        printf(" %c  ", 'A' + i);
    }
    printf("\n");

    for (i = 0; i < ROWS; i++)
    {
        printf(" +");
        for (j = 0; j < COLS; j++)
        {
            printf("---+");
        }
        printf("\n");

        printf("%d|", i + 1);
        for (j = 0; j < COLS; j++)
        {
            printf(" %c |", board[i][j]);
        }
        printf("\n");
    }

    printf(" +");
    for (j = 0; j < COLS; j++)
    {
        printf("---+");
    }
    printf("\n");
}




// Calculates the percentage of cells that have been revealed on the board
// and prints the result to the console
void calculatePercentage()
{
    int revealed = 0;
    int totalNonBomb = ROWS * COLS - numBombs;

    // count number of revealed cells
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != '-')
            {
                revealed++;
            }
        }
    }

    double percentage = (double)(revealed) / (double)(totalNonBomb)*100;
    printf("You have unlocked: %.2lf%% of the playing field.\n", ceil(percentage * 100) / 100);
}

// Counts the number of bombs in the cells immediately adjacent to (row, col)
// and returns the result (count)
int countAdjBombs(int row, int col)
{
    int count = 0; // number of bombs found
    if (row > 0)
    {
        count += (bombs[row - 1][col] == '*');
    }
    if (row < ROWS - 1)
    {
        count += (bombs[row + 1][col] == '*');
    }
    if (col > 0)
    {
        count += (bombs[row][col - 1] == '*');
    }
    if (col < COLS - 1)
    {
        count += (bombs[row][col + 1] == '*');
    }
    return count;
}

// Performs a flood fill operation starting at the cell (row, col)
// in the minesweeper board. If the cell contains a bomb, the number of
// bombs in the surrounding cells is displayed in the cell. Otherwise,
// the cell is revealed and the flood fill operation is recursively performed on all unrevealed, neighboring cells.
void floodFill(int row, int col)
{
    // return if (row, col) is not a valid cell on the board or if it has already been revealed
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || board[row][col] != '-')
    {
        return;
    }

    int bombCount = countAdjBombs(row, col); // count number of bombs in the cells surrounding (row, col)

    // if there are no bombs surrounding (row, col), reveal the cell and 
    // recursively call floodFill on all unrevealed, neighboring cells
    if (bombCount == 0)
    {
        board[row][col] = ' ';
        floodFill(row - 1, col);
        floodFill(row + 1, col);
        floodFill(row, col - 1);
        floodFill(row, col + 1);
    }
    // if there are bombs surrounding (row, col), display the number of bombs in the cell
    else
    {
        board[row][col] = bombCount + '0';
    }
}


// Plays one move of the game
// Returns 1 if player loses, 0 otherwise
// Sets gameOver to true when conditions are met to interrupt main while loop
int playGame()
{
    char loc[10];
    int row;
    int col;
   printf("Enter coordinate or 'save' the game: \n");
    fgets(loc, 10, stdin);
    loc[strlen(loc) - 1] = '\0'; // \n verwijderen
    for (int i = 0; i < strlen(loc); i++)
    {
        if (isupper(loc[i]) == 0)
        {
            loc[i] = toupper(loc[i]);
        }
        if (strcmp("SAVE", loc) == 0)
        {
           saveGame();
        exit(0);
        }
        else if (isalpha(loc[i]) > 0)
        {
            col = loc[i] - 'A'; 
        }
        else
        {
            row = atoi(&loc[i]) - 1; 
        }
    }
    if (bombs[row][col] == '*')
    {
        gameOver = true;
            revealBombs();
            printBoard();
            printf("Game over.. Better luck next time! ;)\n");
    }
    else
    {
        floodFill(row, col);
        
    }
    return 0;
}

// Checks for win
// Sets gameOver to true when conditions are met to interrupt main while loop
void checkWin()
{
    int revealed = 0;
    int totalNonBomb = ROWS * COLS - numBombs;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != '-')
            {
                revealed++;
            }
        }
    }
    // printf("revealed: %d\n", revealed);              Test to reveal amount of revealed spaces
    // printf("totalNonBomb: %d\n", totalNonBomb);      Test to check whether totalNonBomb calculation works
    if (revealed == totalNonBomb)
    {

        gameOver = true;
        revealBombs();
        printBoard();
        printf("Congratulations! You have revealed all non-bomb cells and won the game!\n");
    }
}

// Randomly places 'numBombs' number of bombs on the board
void placeBombs(int numBombs)
{

    for (int i = 0; i < numBombs; i++)
    {
        int x = rand() % ROWS;
        int y = rand() % COLS;

        if (bombs[x][y] != '*')
        {
            bombs[x][y] = '*';
        }
        else
        {
            i--;
        }
    }
}

// Reveals the location of the current bombs in the game
void revealBombs()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (bombs[i][j] == '*')
            {
                board[i][j] = '*';
            }
        }
    }
}

// Saves the current state of the game by printing two 64-bit hexadecimal values
void saveGame()
{

    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (bombs[i][j] == '*')
            {
                bombBits |= (1LL << (i * COLS + j));
            }
            else if (board[i][j] != '-')
            {
                openedBits |= (1LL << (i * COLS + j));
            }
        }
    }
    printf("The location of all the bombs and already opened spaces will be displayed below.\n");
    printf("Bomb Bits: %llx\n", bombBits);
    printf("Opened Bits: %llx\n", openedBits);
}

// Welcomes you to the game
void welcomeMSG()
{
    printf("+------------------------------------------------+\n");
    printf("|                                                |\n");
    printf("|     Welcome to Samir's Minesweeper game!       |\n");
    printf("|                                                |\n");
    printf("+------------------------------------------------+\n");
}

void loadGame(long long bombBits, long long openedBits)
{
    // Convert the bombBits and openedBits values to arrays of bool values
    bool bombBitArray[ROWS * COLS];
    bool openedBitArray[ROWS * COLS];
    for (int i = 0; i < ROWS * COLS; i++)
    {
        bombBitArray[i] = (bombBits >> i) & 1;
        openedBitArray[i] = (openedBits >> i) & 1;
    }

    // Initialize the board, bombs, and revealed arrays based on the values in the bit arrays
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            int index = i * COLS + j;
            if (bombBitArray[index])
            {
                bombs[i][j] = '*';
            }
            else
            {
                bombs[i][j] = '-';
            }

            revealed[i][j] = openedBitArray[index];

            // If the cell has been revealed, set the corresponding cell in the board array
            // to the number of adjacent bombs or ' ' if there are no adjacent bombs
            if (revealed[i][j])
            {
                if (bombs[i][j] == '*')
                {
                    board[i][j] = '*';
                }
                else if (bombs[i][j] == '-')
                {
                    int bombCount = countAdjBombs(i, j);
                    if (bombCount > 0)
                    {
                        board[i][j] = bombCount + '0';
                    }
                    else
                    {
                        board[i][j] = ' ';
                    }
                }
            }
            // If the cell has not been revealed, set the corresponding cell in the board array to '-'
            else
            {
                board[i][j] = '-';
            }
        }
    }
}








