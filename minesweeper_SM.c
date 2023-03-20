#include "minesweeper_SM.h"

// Left To Do
   
    //  - Fix once double print at start of game                       4 hours in impossible..  XD
   


// Link to Miro flowchart: https://miro.com/app/board/uXjVP1X_2Oo=/?share_link_id=688311395178

int main()
{
    srand(time(NULL));

        welcomeMSG();
        inputName();
        
        char input[10];
        while (1)
        {
            printf("Enter 'new' to start a new game or 'load' to continue a saved game: \n");
            scanf("%s", input);
            if (strcmp(input, "new") == 0)
            {
                printf("Enter the number of bombs: ");
                scanf("%d", &numBombs);
                initBoard();
                placeBombs(numBombs);
                while (!gameOver)
                {
                    printBoard();
                    calculatePercentage();
                    playGame();   // loss is checked inside playGame  
                    checkWin();
                }
            }
            else if (strcmp(input, "load") == 0)
            {
                printf("Enter the bomb bits in hexadecimal format: \n");
                scanf("%llx", &bombBits);

                printf("Enter the opened bits in hexadecimal format: \n");
                scanf("%llx", &openedBits);

                loadGame(bombBits, openedBits);
                while (!gameOver)
                {
                    gameOver = false;
                    printBoard();
                    calculatePercentage();
                    playGame(); // loss is checked inside playGame
                    checkWin();
                }
            }
            else
            {
                printf("Invalid input. Please try again.\n");
            }

        }
        return 0;
    }
