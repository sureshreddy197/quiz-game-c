#ifndef QUESTION_H
#define QUESTION_H

#include <stddef.h>

#define MAX_OPTIONS 4

typedef enum {
    CATEGORY_C_PROGRAMMING = 0,
    CATEGORY_GENERAL_KNOWLEDGE,
    CATEGORY_SCIENCE,
    CATEGORY_COUNT
} Category;

typedef struct {
    const char *prompt;
    const char *options[MAX_OPTIONS];
    int correct_index;
    Category category;
    const char *explanation;
} Question;

const char *category_to_string(Category category);
const Question *get_questions_by_category(Category category, size_t *count);
const Question *get_all_questions(size_t *count);

#endif
