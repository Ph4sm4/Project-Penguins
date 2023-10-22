#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_ORANGE "\x1b[38;5;208m" // This is an approximation
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_GRAY "\x1b[90m"

#define printfWhite(fmt, ...) printf(ANSI_COLOR_WHITE fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfGray(fmt, ...) printf(ANSI_COLOR_GRAY fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfRed(fmt, ...) printf(ANSI_COLOR_RED fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfGreen(fmt, ...) printf(ANSI_COLOR_GREEN fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfYellow(fmt, ...) printf(ANSI_COLOR_YELLOW fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfBlue(fmt, ...) printf(ANSI_COLOR_BLUE fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfMagenta(fmt, ...) printf(ANSI_COLOR_MAGENTA fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfCyan(fmt, ...) printf(ANSI_COLOR_CYAN fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printfOrange(fmt, ...) printf(ANSI_COLOR_ORANGE fmt ANSI_COLOR_RESET, ##__VA_ARGS__)