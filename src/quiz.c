#include "quiz.h"

#include "question.h"
#include "storage.h"
#include "ui.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUESTIONS_PER_ROUND 5
#define POINTS_PER_CORRECT 10
#define STREAK_BONUS_THRESHOLD 3
#define STREAK_BONUS_POINTS 5

typedef struct {
    char player_name[MAX_NAME_LENGTH + 1];
    Category category;
    int score;
    int correct_answers;
    int questions_answered;
    int used_fifty_fifty;
    int used_skip;
    int streak;
} GameState;

static void shuffle_indices(size_t *indices, size_t count) {
    if (count <= 1) {
        return;
    }

    for (size_t i = count - 1; i > 0; --i) {
        size_t j = (size_t)(rand() % (int)(i + 1));
        size_t temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

static void trim_whitespace(char *text) {
    size_t start = 0;
    size_t end = 0;

    if (text == NULL) {
        return;
    }

    while (text[start] != '\0' && isspace((unsigned char)text[start])) {
        ++start;
    }

    if (start > 0) {
        memmove(text, text + start, strlen(text + start) + 1);
    }

    end = strlen(text);
    while (end > 0 && isspace((unsigned char)text[end - 1])) {
        text[end - 1] = '\0';
        --end;
    }
}

static int choose_category(Category *selected_category) {
    clear_screen();
    print_banner("Choose a category");
    printf("1. C Programming\n");
    printf("2. General Knowledge\n");
    printf("3. Science\n");
    printf("4. Back to Main Menu\n\n");

    int choice = prompt_menu_choice("Choose an option: ", 1, 4);
    switch (choice) {
        case 1:
            *selected_category = CATEGORY_C_PROGRAMMING;
            return 1;
        case 2:
            *selected_category = CATEGORY_GENERAL_KNOWLEDGE;
            return 1;
        case 3:
            *selected_category = CATEGORY_SCIENCE;
            return 1;
        case 4:
        default:
            return 0;
    }
}

static void show_lifeline_status(const GameState *state) {
    char line[128];
    snprintf(line,
             sizeof(line),
             "Lifelines -> [F] 50-50: %s | [S] Skip: %s",
             state->used_fifty_fifty ? "used" : "available",
             state->used_skip ? "used" : "available");
    print_info(line);
}

static void print_question_card(const Question *question, int question_number, int total_questions, const int *hidden_options) {
    char title[128];
    snprintf(title,
             sizeof(title),
             "Question %d of %d | Category: %s",
             question_number,
             total_questions,
             category_to_string(question->category));

    print_section_title(title);
    print_wrapped(question->prompt, 0);
    putchar('\n');

    for (int i = 0; i < MAX_OPTIONS; ++i) {
        if (hidden_options != NULL && hidden_options[i]) {
            continue;
        }
        printf("  %d) %s\n", i + 1, question->options[i]);
    }
}

static void apply_fifty_fifty(const Question *question, int *hidden_options) {
    int removed = 0;
    for (int i = 0; i < MAX_OPTIONS && removed < 2; ++i) {
        if (i != question->correct_index) {
            hidden_options[i] = 1;
            ++removed;
        }
    }
}

static int ask_question(GameState *state, const Question *question, int question_number, int total_questions) {
    char input[32];
    int hidden_options[MAX_OPTIONS] = {0, 0, 0, 0};

    while (1) {
        clear_screen();
        print_banner("Play Quiz");
        printf("Player: %s\n", state->player_name);
        printf("Score: %d | Correct: %d | Streak: %d\n\n",
               state->score,
               state->correct_answers,
               state->streak);

        print_question_card(question, question_number, total_questions, hidden_options);
        putchar('\n');
        show_lifeline_status(state);
        print_info("Enter 1-4 to answer, F for 50-50, or S to skip.");

        if (!prompt_line("Your choice: ", input, sizeof(input))) {
            return 0;
        }

        trim_whitespace(input);
        if (input[0] == '\0') {
            print_warning("Please enter a choice.");
            pause_for_enter();
            continue;
        }

        if ((input[0] == 'f' || input[0] == 'F') && input[1] == '\0') {
            if (state->used_fifty_fifty) {
                print_warning("You already used the 50-50 lifeline.");
            } else {
                apply_fifty_fifty(question, hidden_options);
                state->used_fifty_fifty = 1;
                print_success("50-50 activated. Two wrong options were removed.");
            }
            pause_for_enter();
            continue;
        }

        if ((input[0] == 's' || input[0] == 'S') && input[1] == '\0') {
            if (state->used_skip) {
                print_warning("You already used the skip lifeline.");
                pause_for_enter();
                continue;
            }
            state->used_skip = 1;
            print_info("Question skipped. No score change.");
            pause_for_enter();
            return 1;
        }

        if (input[0] >= '1' && input[0] <= '4' && input[1] == '\0') {
            int answer_index = (int)(input[0] - '1');
            if (hidden_options[answer_index]) {
                print_warning("That option is not available after 50-50. Pick a visible option.");
                pause_for_enter();
                continue;
            }

            clear_screen();
            print_banner("Result");
            if (answer_index == question->correct_index) {
                int earned_points = POINTS_PER_CORRECT;
                state->correct_answers += 1;
                state->streak += 1;
                if (state->streak >= STREAK_BONUS_THRESHOLD) {
                    earned_points += STREAK_BONUS_POINTS;
                    print_success("Correct! Streak bonus unlocked: +5 points.");
                } else {
                    print_success("Correct answer!");
                }
                state->score += earned_points;
            } else {
                state->streak = 0;
                print_error("Not quite.");
                printf("Correct answer: %d) %s\n", question->correct_index + 1, question->options[question->correct_index]);
            }

            print_wrapped(question->explanation, 0);
            state->questions_answered += 1;
            pause_for_enter();
            return 1;
        }

        print_warning("Invalid choice. Try again.");
        pause_for_enter();
    }
}

static int ask_player_name(char *name_buffer, size_t size) {
    while (1) {
        if (!prompt_line("Enter your name: ", name_buffer, size)) {
            return 0;
        }
        trim_whitespace(name_buffer);
        if (name_buffer[0] != '\0') {
            return 1;
        }
        print_warning("Name cannot be empty.");
    }
}

static void show_round_summary(const GameState *state, int total_questions) {
    char summary[256];
    clear_screen();
    print_banner("Round Complete");
    snprintf(summary,
             sizeof(summary),
             "%s, you scored %d points and answered %d out of %d questions correctly.",
             state->player_name,
             state->score,
             state->correct_answers,
             total_questions);
    print_wrapped(summary, 0);

    if (state->score >= 45) {
        print_success("Strong run. That is leaderboard-worthy stuff.");
    } else if (state->score >= 25) {
        print_info("Nice job. You have a solid score on the board.");
    } else {
        print_warning("Good start. Another run will probably beat this score.");
    }
}

static void persist_score(const GameState *state, int total_questions) {
    LeaderboardEntry entry;
    memset(&entry, 0, sizeof(entry));

    snprintf(entry.name, sizeof(entry.name), "%s", state->player_name);
    entry.score = state->score;
    entry.correct_answers = state->correct_answers;
    entry.total_questions = total_questions;
    snprintf(entry.category, sizeof(entry.category), "%s", category_to_string(state->category));
    get_timestamp_now(entry.timestamp, sizeof(entry.timestamp));

    if (save_score(&entry)) {
        print_success("Your score was saved to the leaderboard.");
    } else {
        print_error("Could not save your score. Check file permissions and try again.");
    }
}

void show_leaderboard_screen(void) {
    LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES];
    size_t count = 0;

    clear_screen();
    print_banner("Leaderboard");

    if (!load_leaderboard(entries, MAX_LEADERBOARD_ENTRIES, &count)) {
        print_error("Unable to load the leaderboard.");
        pause_for_enter();
        return;
    }

    if (count == 0) {
        print_info("No scores yet. Be the first one on the board.");
        pause_for_enter();
        return;
    }

    printf("%-4s %-18s %-8s %-10s %-20s %-19s\n", "#", "Player", "Score", "Correct", "Category", "Played At");
    print_separator();
    for (size_t i = 0; i < count; ++i) {
        printf("%-4zu %-18s %-8d %d/%-8d %-20s %-19s\n",
               i + 1,
               entries[i].name,
               entries[i].score,
               entries[i].correct_answers,
               entries[i].total_questions,
               entries[i].category,
               entries[i].timestamp);
    }

    pause_for_enter();
}

void show_help_screen(void) {
    clear_screen();
    print_banner("How to Play");
    print_wrapped("1. Start a new game from the main menu and enter your player name.", 0);
    print_wrapped("2. Pick a category. Each round asks five random multiple-choice questions.", 0);
    print_wrapped("3. Type 1 to 4 to submit your answer.", 0);
    print_wrapped("4. You get one 50-50 lifeline and one skip lifeline per round.", 0);
    print_wrapped("5. Every correct answer gives 10 points. A streak of three or more correct answers earns bonus points.", 0);
    print_wrapped("6. Your best runs are saved to data/leaderboard.txt.", 0);
    pause_for_enter();
}

void show_about_screen(void) {
    clear_screen();
    print_banner("About This Project");
    print_wrapped("quiz-game-c is a modular console application written in standard C. It is built for GitHub-ready presentation with source separation, file persistence, and a polished terminal interface.", 0);
    print_wrapped("The project uses only the standard toolchain and does not require external libraries.", 0);
    pause_for_enter();
}

void reset_leaderboard_screen(void) {
    char input[8];
    clear_screen();
    print_banner("Reset Leaderboard");
    print_warning("This will erase all saved scores.");

    if (!prompt_line("Type YES to continue: ", input, sizeof(input))) {
        return;
    }

    trim_whitespace(input);
    if (strcmp(input, "YES") == 0) {
        if (reset_leaderboard()) {
            print_success("Leaderboard reset successfully.");
        } else {
            print_error("Could not reset the leaderboard.");
        }
    } else {
        print_info("Reset cancelled.");
    }
    pause_for_enter();
}

void run_quiz_game(void) {
    GameState state;
    const Question *questions = NULL;
    size_t question_count = 0;
    size_t indices[64];
    int total_questions = QUESTIONS_PER_ROUND;

    memset(&state, 0, sizeof(state));

    if (!choose_category(&state.category)) {
        return;
    }

    clear_screen();
    print_banner("New Game");
    if (!ask_player_name(state.player_name, sizeof(state.player_name))) {
        return;
    }

    questions = get_questions_by_category(state.category, &question_count);
    if (questions == NULL || question_count == 0) {
        print_error("No questions are available for this category.");
        pause_for_enter();
        return;
    }

    if ((size_t)total_questions > question_count) {
        total_questions = (int)question_count;
    }

    for (size_t i = 0; i < question_count; ++i) {
        indices[i] = i;
    }
    shuffle_indices(indices, question_count);

    for (int i = 0; i < total_questions; ++i) {
        if (!ask_question(&state, &questions[indices[i]], i + 1, total_questions)) {
            print_warning("The round ended unexpectedly.");
            pause_for_enter();
            return;
        }
    }

    show_round_summary(&state, total_questions);
    persist_score(&state, total_questions);
    pause_for_enter();
}
