
#include "minesweeper_SM.h"

int main() {
    srand(time(NULL));

    printf("Welkom bij Minesweeper!\n");

    while (1) {
        printf("Voer m'new' in om een nieuw spel te starten of 'load' om een opgeslagen spel te laden: ");
        char input[20];
        scanf("%s", input);

        if (strcmp(input, "new") == 0) {
            start_new_game();
        } else if (strcmp(input, "load") == 0) {
            load_saved_game();
        } else {
            printf("Ongeldige invoer. Voer 'new' of 'load' in.\n");
            continue;
        }

        play_game();

        printf("Opnieuw spelen? (y/n): ");
        char play_again;
        scanf(" %c", &play_again);
        if (tolower(play_again) != 'y') {
            break;
        }
    }

    return 0;
}