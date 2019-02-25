#include <assert.h>

#include "libpower.h"

int main(void)
{
    assert(calc_power_i(1, 10) == 10);
    assert(calc_power_i(5, 3) == 15);
}
