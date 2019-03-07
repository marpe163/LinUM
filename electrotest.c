#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libpower/libpower.h"
#include "libresistance/lib_resistance.h"
#include "libcomponent/libcomponent.h"
int e_resistance(float orig_resistance, float *res_array);

static char *read_str(const char *prompt);
static int read_int(const char *prompt);
static float read_float(const char *prompt);
static char read_char(const char *prompt);

int main(void)
{
    float volt, resistance, power, e12[3];
    char connection;
    int components_count, i;
    float *components;
    char *prompt;

    /*
     * Volt.
     */
    volt = read_float("Ange spänningskälla i V");

    /*
     * Connection type.
     */
    while (true) {
        connection = read_char("Ange koppling[S | P]");
        if (connection == 'S' || connection == 'P') {
            break;
        }
    }

    /*
     * Component count.
     */
    components_count = read_int("Antal komponenter");

    /*
     * Components.
     */
    components = calloc(components_count, sizeof(*components));
    if (components == NULL) {
        fprintf(stderr, "calloc: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for (i = 0; i < components_count; i++) {
        if (asprintf(&prompt, "Komponent %d i ohm", i + 1) == -1) {
            fprintf(stderr, "asprintf failed");
            free(components);
            return EXIT_FAILURE;
        }
        components[i] = read_float(prompt);
        free(prompt);
    }

    /*
     * Calculate resistance.
     */
    resistance = calc_resistance(components_count, connection, components);
    free(components);

    if (resistance == -1) {
        printf("Felaktig data.\n");
        return EXIT_FAILURE;
    }
    printf("Ersättningsresistans:\n%.1f ohm\n", resistance);

    /*
     * Calculate power.
     */
    power = calc_power_r(volt, resistance);
    printf("Effekt:\n%.2f W\n", power);

    /*
     * Calculate E12.
     */
    printf("Ersättningsresistanser i E12-serien kopplade i serie:\n");
    for (i = 0; i < e_resistance(resistance, e12); i++) {
        printf("%.0f\n", e12[i]);
    }
}

/**
 * Read an integer from stdin.
 *
 * @param prompt    Prompt to show the user.
 * @return          An integer.
 */
static int read_int(const char *prompt)
{
    char *start, *end;
    long int num;

    while (true) {
        start = read_str(prompt);
        if (start == NULL) {
            continue;
        }

        errno = 0;
        num = strtol(start, &end, 10);

        if (errno != ERANGE && start != end && *end == '\0' &&
            num >= INT_MIN && num <= INT_MAX) {
            free(start);
            return num;
        }

        free(start);
    }
}

/**
 * Read a float from stdin.
 *
 * @param prompt    Prompt to show the user.
 * @return          A float.
 */
static float read_float(const char *prompt)
{
    char *start, *end;
    float num;

    while (true) {
        start = read_str(prompt);
        if (start == NULL) {
            continue;
        }

        errno = 0;
        num = strtof(start, &end);

        if (errno != ERANGE && start != end && *end == '\0') {
            free(start);
            return num;
        }

        free(start);
    }
}

/**
 * Read a char from stdin.
 *
 * @param prompt    Prompt to show the user.
 * @return          A char.
 */
static char read_char(const char *prompt)
{
    char *str;
    char c;

    while (true) {
        str = read_str(prompt);
        if (str && strlen(str) == 1) {
            c = *str;
            free(str);
            return c;
        }
        /* 7.20.3.2 */
        free(str);
    }
}

/**
 * Read a string from stdin.
 *
 * @param prompt    Prompt to show the user.
 * @return          Success: a buffer with the string that must be freed.
 *                  Failure: a NULL-pointer.
 */
static char *read_str(const char *prompt)
{
    size_t size;
    ssize_t len;
    char *str = NULL;

    printf("%s: ", prompt);
    len = getline(&str, &size, stdin);
    if (len <= 0) {
        free(str);
        return NULL;
    }

    str[len - 1] = '\0';
    return str;
}
