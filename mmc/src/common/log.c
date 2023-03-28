#include "log.h"

static const char *level_strings[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

void log_log(int level, const char *file, int line, const char *fmt, ...) {
    /* Deliberately not using file and line for now to keep output clean.
     * Keeping the existing interface for extensibility purposes if I decide to
     * change my mind in the future. */
    (void) file;
    (void) line;

    /* Print the trace level. */
    printf("%s: ", level_strings[level]);
    /* Declare a va_list type variable */
    va_list myargs;
    /* Initialise the va_list variable with the ... after fmt */
    va_start(myargs, fmt);
    /* Forward the '...' to printf */
    vprintf(fmt, myargs);
    /* Clean up the va_list */
    va_end(myargs);
    /* Print newline character at the end. */
    printf("\n");
}