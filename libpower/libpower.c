/**
 * @file
 * @author jeil0006
 * @version 0.1
 *
 * Utility functions for power calculations.
 */
#include <math.h>

#include "libpower.h"

/**
 * Calculate the power with volt and resistance.
 *
 * @param volt          The voltage of the circuit.
 * @param resistance    The resistance of the circuit.
 * @return              The power of the circuit.
 */
float calc_power_r(float volt, float resistance)
{
    return powf(volt, 2) / resistance;
}

/**
 * Calculate the power with volt and current.
 *
 * @param volt      The voltage of the circuit.
 * @param current   The current of the circuit.
 * @return          The power of the circuit.
 */
float calc_power_i(float volt, float current)
{
    return volt * current;
}
