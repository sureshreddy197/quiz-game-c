#ifndef UI_H
#define UI_H

#include <stddef.h>

void ui_init(void);
void ui_shutdown(void);
void clear_screen(void);
void pause_for_enter(void);
void print_banner(const char *title);
void print_section_title(const char *title);
void print_wrapped(const char *text, int indent);
void print_success(const char *text);
void print_error(const char *text);
void print_info(const char *text);
void print_warning(const char *text);
void print_separator(void);
int prompt_line(const char *label, char *buffer, size_t buffer_size);
int prompt_menu_choice(const char *label, int min_value, int max_value);
int terminal_width(void);

#endif
