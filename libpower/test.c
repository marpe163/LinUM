#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libpower.h"

static bool cmp(float a, float b);

int main(void)
{
    assert(cmp(calc_power_i(1, 10), 10));
    assert(cmp(calc_power_i(5, 3), 15));
    assert(!cmp(calc_power_i(10, 3.3), 999));

    assert(cmp(calc_power_r(5, 5), 5));
    assert(cmp(calc_power_r(50, 1398), 1.788));
    assert(!cmp(calc_power_r(32, 20), 10));
    assert(cmp(calc_power_r(32, 0), 0));
    assert(cmp(calc_power_r(32, -0), 0));
}

/**
 * Compare two floats by their string representation with three decimals.
 *
 * @param a     First value.
 * @param b     Second value.
 * @return      True if they match, false otherwise.
 */
static bool cmp(float a, float b)
{
    bool rv;
    char *a_str, *b_str;

    if (asprintf(&a_str, "%.3f", a) == -1) {
        return false;
    }

    if (asprintf(&b_str, "%.3f", b) == -1) {
        free(a_str);
        return false;
    }

    rv = strcmp(a_str, b_str) == 0;

    free(a_str);
    free(b_str);

    return rv;
}
