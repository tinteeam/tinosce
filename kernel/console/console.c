#include "../lib/kprint.h"
#include "../drivers/keyboard/keyboard.h"
#include <stdbool.h>
#include <stddef.h>

#define MAX_INPUT 256

typedef void (*command_func)(const char* args);

typedef struct {
    const char* name;
    command_func func;
    const char* help;
} command_entry;

void cmd_help(const char* args);
void cmd_clear(const char* args);
void cmd_echo(const char* args);

// Add your commands here
command_entry commands[] = {
    { "help", cmd_help, "List available commands" },
    { "clear", cmd_clear, "Clear the screen" },
    { "echo", cmd_echo, "Echo back input" },
};

const int command_count = sizeof(commands) / sizeof(commands[0]);

void read_input(char* buffer, int max) {
    int index = 0;
    while (1) {
        char c = getchar();

        if (c == '\n') {
            kprint_char('\n');
            buffer[index] = '\0';
            return;
        }

        if (c == '\b') {
            if (index > 0) {
                index--;
                kprint_char('\b');
            }
        } else if (index < max - 1) {
            buffer[index++] = c;
            kprint_char(c);
        }
    }
}

void console_prompt() {
    kprint("> ");
}

int str_compare(const char* a, const char* b) {
    while (*a && *b && *a == *b) {
        a++; b++;
    }
    return *a - *b;
}

char* str_token(char* str, const char delim) {
    static char* src = NULL;
    if (str != NULL)
        src = str;

    if (src == NULL || *src == '\0')
        return NULL;

    char* end = src;
    while (*end && *end != delim)
        end++;

    if (*end) {
        *end = '\0';
        end++;
    }

    char* result = src;
    src = end;
    return result;
}

void run_command(char* input) {
    char* cmd = str_token(input, ' ');
    char* args = str_token(NULL, '\0');

    if (!cmd) return;

    for (int i = 0; i < command_count; i++) {
        if (str_compare(cmd, commands[i].name) == 0) {
            commands[i].func(args);
            return;
        }
    }

    kprint("Unknown command. Try 'help'.\n");
}

// ───── COMMAND IMPLEMENTATIONS ─────

void cmd_help(const char* args) {
    for (int i = 0; i < command_count; i++) {
        kprint(commands[i].name);
        kprint(" - ");
        kprint(commands[i].help);
        kprint("\n");
    }
}

void cmd_clear(const char* args) {
    kprint_clear();
}

void cmd_echo(const char* args) {
    if (args) {
        kprint(args);
        kprint("\n");
    }
}

// ───── ENTRY POINT ─────

void console_init() {
    kprint("Welcome to TINOS!\nType 'help' for a list of commands.\n");
}

void console_run() {
    char input[MAX_INPUT];
    while (true) {
        console_prompt();
        read_input(input, MAX_INPUT);
        run_command(input);
    }
}
