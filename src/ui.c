#include "ui.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define UI_MAX_WIDTH 100
#define UI_MIN_WIDTH 48

static int g_use_color = 1;

static const char *COLOR_RESET = "\033[0m";
static const char *COLOR_CYAN = "\033[36m";
static const char *COLOR_GREEN = "\033[32m";
static const char *COLOR_RED = "\033[31m";
static const char *COLOR_YELLOW = "\033[33m";
static const char *COLOR_BOLD = "\033[1m";
static const char *COLOR_BLUE = "\033[34m";

static void print_color_line(const char *color, const char *text) {
    if (g_use_color) {
        printf("%s%s%s\n", color, text, COLOR_RESET);
    } else {
        printf("%s\n", text);
    }
}

void ui_init(void) {
    const char *no_color = getenv("NO_COLOR");
    const char *term = getenv("TERM");

    if (no_color != NULL || (term != NULL && strcmp(term, "dumb") == 0)) {
        g_use_color = 0;
    }

#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(console, &mode)) {
            SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
}

void ui_shutdown(void) {
    if (g_use_color) {
        printf("%s", COLOR_RESET);
    }
}

int terminal_width(void) {
    int width = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(console, &info)) {
        width = info.srWindow.Right - info.srWindow.Left + 1;
    }
#else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0) {
        width = size.ws_col;
    }
#endif
    if (width < UI_MIN_WIDTH) {
        width = UI_MIN_WIDTH;
    }
    if (width > UI_MAX_WIDTH) {
        width = UI_MAX_WIDTH;
    }
    return width;
}

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_separator(void) {
    int width = terminal_width();
    for (int i = 0; i < width; ++i) {
        putchar('=');
    }
    putchar('\n');
}

static void print_centered(const char *text, const char *color) {
    int width = terminal_width();
    size_t len = strlen(text);
    int padding = (int)((width - (int)len) / 2);
    if (padding < 0) {
        padding = 0;
    }

    for (int i = 0; i < padding; ++i) {
        putchar(' ');
    }

    if (g_use_color && color != NULL) {
        printf("%s%s%s\n", color, text, COLOR_RESET);
    } else {
        printf("%s\n", text);
    }
}

void print_banner(const char *title) {
    print_separator();
    print_centered("QUIZ GAME IN C", g_use_color ? COLOR_BOLD : NULL);
    print_centered(title, g_use_color ? COLOR_CYAN : NULL);
    print_separator();
}

void print_section_title(const char *title) {
    if (g_use_color) {
        printf("\n%s%s%s\n", COLOR_BLUE, title, COLOR_RESET);
    } else {
        printf("\n%s\n", title);
    }
    print_separator();
}

void print_wrapped(const char *text, int indent) {
    int width = terminal_width() - indent;
    const char *cursor = text;
    if (width < 20) {
        width = 20;
    }

    while (*cursor != '\0') {
        int line_length = 0;
        const char *line_start = cursor;
        const char *last_space = NULL;

        while (*cursor != '\0' && *cursor != '\n' && line_length < width) {
            if (isspace((unsigned char)*cursor)) {
                last_space = cursor;
            }
            ++cursor;
            ++line_length;
        }

        if (*cursor == '\n') {
            for (int i = 0; i < indent; ++i) {
                putchar(' ');
            }
            fwrite(line_start, 1, (size_t)(cursor - line_start), stdout);
            putchar('\n');
            ++cursor;
            continue;
        }

        if (*cursor != '\0' && last_space != NULL && line_length >= width) {
            cursor = last_space;
        }

        for (int i = 0; i < indent; ++i) {
            putchar(' ');
        }
        fwrite(line_start, 1, (size_t)(cursor - line_start), stdout);
        putchar('\n');

        while (isspace((unsigned char)*cursor) && *cursor != '\0') {
            ++cursor;
        }
    }
}

void print_success(const char *text) {
    print_color_line(g_use_color ? COLOR_GREEN : NULL, text);
}

void print_error(const char *text) {
    print_color_line(g_use_color ? COLOR_RED : NULL, text);
}

void print_info(const char *text) {
    print_color_line(g_use_color ? COLOR_CYAN : NULL, text);
}

void print_warning(const char *text) {
    print_color_line(g_use_color ? COLOR_YELLOW : NULL, text);
}

int prompt_line(const char *label, char *buffer, size_t buffer_size) {
    if (label == NULL || buffer == NULL || buffer_size == 0) {
        return 0;
    }

    printf("%s", label);
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}

int prompt_menu_choice(const char *label, int min_value, int max_value) {
    char buffer[32];
    char *end_ptr = NULL;
    long value = 0;

    while (1) {
        if (!prompt_line(label, buffer, sizeof(buffer))) {
            return min_value;
        }

        value = strtol(buffer, &end_ptr, 10);
        if (end_ptr != buffer && *end_ptr == '\0' && value >= min_value && value <= max_value) {
            return (int)value;
        }

        print_warning("Please enter a valid choice.");
    }
}

void pause_for_enter(void) {
    char buffer[8];
    printf("\nPress Enter to continue...");
    (void)fgets(buffer, sizeof(buffer), stdin);
}
