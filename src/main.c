#include "quiz.h"
#include "ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    int running = 1;

    srand((unsigned int)time(NULL));
    ui_init();

    while (running) {
        clear_screen();
        print_banner("Main Menu");
        print_info("A polished console quiz game with categories, lifelines, and leaderboard support.");
        putchar('\n');
        printf("1. Play Quiz\n");
        printf("2. View Leaderboard\n");
        printf("3. How to Play\n");
        printf("4. About Project\n");
        printf("5. Reset Leaderboard\n");
        printf("6. Exit\n\n");

        switch (prompt_menu_choice("Choose an option: ", 1, 6)) {
            case 1:
                run_quiz_game();
                break;
            case 2:
                show_leaderboard_screen();
                break;
            case 3:
                show_help_screen();
                break;
            case 4:
                show_about_screen();
                break;
            case 5:
                reset_leaderboard_screen();
                break;
            case 6:
                running = 0;
                break;
            default:
                break;
        }
    }

    clear_screen();
    print_banner("Goodbye");
    print_info("Thanks for playing quiz-game-c.");
    ui_shutdown();
    return 0;
}
