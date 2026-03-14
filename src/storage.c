#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MAKE_DIR(path) mkdir(path, 0777)
#endif

#define DEFAULT_LEADERBOARD_PATH "data/leaderboard.txt"

typedef struct {
    int initialized;
    char path[256];
} StorageConfig;

static StorageConfig g_storage = {0, {0}};

static void initialize_path(void) {
    if (g_storage.initialized) {
        return;
    }

    const char *env_path = getenv("QUIZ_LEADERBOARD_PATH");
    if (env_path != NULL && env_path[0] != '\0') {
        snprintf(g_storage.path, sizeof(g_storage.path), "%s", env_path);
    } else {
        snprintf(g_storage.path, sizeof(g_storage.path), "%s", DEFAULT_LEADERBOARD_PATH);
    }

    g_storage.initialized = 1;
}

const char *get_leaderboard_path(void) {
    initialize_path();
    return g_storage.path;
}

static void ensure_parent_directory(void) {
    initialize_path();

    char directory[256];
    size_t length = strlen(g_storage.path);
    if (length >= sizeof(directory)) {
        return;
    }

    snprintf(directory, sizeof(directory), "%s", g_storage.path);
    for (size_t i = length; i > 0; --i) {
        if (directory[i] == '/' || directory[i] == '\\') {
            directory[i] = '\0';
            if (directory[0] != '\0') {
                (void)MAKE_DIR(directory);
            }
            return;
        }
    }
}

void get_timestamp_now(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    if (buffer == NULL || buffer_size == 0) {
        return;
    }

    if (time_info == NULL) {
        buffer[0] = '\0';
        return;
    }

    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", time_info);
}

static int compare_entries(const void *a, const void *b) {
    const LeaderboardEntry *left = (const LeaderboardEntry *)a;
    const LeaderboardEntry *right = (const LeaderboardEntry *)b;

    if (right->score != left->score) {
        return right->score - left->score;
    }

    if (right->correct_answers != left->correct_answers) {
        return right->correct_answers - left->correct_answers;
    }

    return strcmp(left->timestamp, right->timestamp);
}

void sort_leaderboard(LeaderboardEntry *entries, size_t count) {
    qsort(entries, count, sizeof(LeaderboardEntry), compare_entries);
}

int load_leaderboard(LeaderboardEntry *entries, size_t max_entries, size_t *loaded_count) {
    FILE *file = NULL;
    char line[512];
    size_t count = 0;

    if (loaded_count != NULL) {
        *loaded_count = 0;
    }

    initialize_path();
    file = fopen(g_storage.path, "r");
    if (file == NULL) {
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        LeaderboardEntry entry;
        char *token = NULL;

        memset(&entry, 0, sizeof(entry));
        line[strcspn(line, "\r\n")] = '\0';

        token = strtok(line, "|");
        if (token == NULL) {
            continue;
        }
        snprintf(entry.name, sizeof(entry.name), "%s", token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        entry.score = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        entry.correct_answers = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        entry.total_questions = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        snprintf(entry.category, sizeof(entry.category), "%s", token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        snprintf(entry.timestamp, sizeof(entry.timestamp), "%s", token);

        entries[count++] = entry;
    }

    fclose(file);
    sort_leaderboard(entries, count);

    if (loaded_count != NULL) {
        *loaded_count = count;
    }

    return 1;
}

int save_score(const LeaderboardEntry *entry) {
    FILE *file = NULL;
    LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES + 1];
    size_t count = 0;

    if (entry == NULL) {
        return 0;
    }

    if (!load_leaderboard(entries, MAX_LEADERBOARD_ENTRIES, &count)) {
        return 0;
    }

    if (count < MAX_LEADERBOARD_ENTRIES + 1) {
        entries[count++] = *entry;
    }

    sort_leaderboard(entries, count);
    if (count > MAX_LEADERBOARD_ENTRIES) {
        count = MAX_LEADERBOARD_ENTRIES;
    }

    ensure_parent_directory();
    file = fopen(g_storage.path, "w");
    if (file == NULL) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%s|%d|%d|%d|%s|%s\n",
                entries[i].name,
                entries[i].score,
                entries[i].correct_answers,
                entries[i].total_questions,
                entries[i].category,
                entries[i].timestamp);
    }

    fclose(file);
    return 1;
}

int reset_leaderboard(void) {
    ensure_parent_directory();
    FILE *file = fopen(get_leaderboard_path(), "w");
    if (file == NULL) {
        return 0;
    }
    fclose(file);
    return 1;
}
