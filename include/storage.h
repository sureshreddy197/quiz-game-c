#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>

#define MAX_NAME_LENGTH 63
#define MAX_CATEGORY_LENGTH 31
#define MAX_TIMESTAMP_LENGTH 31
#define MAX_LEADERBOARD_ENTRIES 10

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    int score;
    int correct_answers;
    int total_questions;
    char category[MAX_CATEGORY_LENGTH + 1];
    char timestamp[MAX_TIMESTAMP_LENGTH + 1];
} LeaderboardEntry;

int load_leaderboard(LeaderboardEntry *entries, size_t max_entries, size_t *loaded_count);
int save_score(const LeaderboardEntry *entry);
int reset_leaderboard(void);
void sort_leaderboard(LeaderboardEntry *entries, size_t count);
void get_timestamp_now(char *buffer, size_t buffer_size);
const char *get_leaderboard_path(void);

#endif
