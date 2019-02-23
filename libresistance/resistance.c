/** @file */
/** \file resistance.c
    \brief Library to calculate the equivalence resistance from a number of serial- or parallel
    connected resistances.
    
    This library contains  a function to calculate the equivalence resistance, or total resistance, 
    from a number of serial or parallel connected resistances.
*/

#include <stdio.h>
#include "lib_resistance.h"

/** \fn float calc_resistance(int count, char conn, float *array)
    Calculate the equivalence resistance, or total resistance, from "count" numbers of resistors  in "array" 
    \param[in] count Number of connected resistances
    \param[in] conn Connection type, serial (S) or parallel (P)
    \param[in] array Pointer to an array including "count" number of resistance values

    \param[out] resistance Total equivalent resistance, or -1 if invalid input parameters
*/
float calc_resistance(int count, char conn, float *array)
{
    float resistance = 0;
    int i;

    // Return -1 if NULL pointer or if number of resistors  are zero
    if (array == NULL || count == 0) {
        return -1;
    }

    // Calculate total resistance for serial connected resistors if 'S' or 's' are received
    if (conn == 'S' || conn == 's')
    {
        for (i = 0; i < count; i++){
            resistance += array[i];
        }
    }
    // Calculate total resistance for parallel connected resistors if 'P' or 'p' are received
    else if (conn == 'P' || conn == 'p')
    {
        i = 0;

        for (i = 0; i < count; i++){
            if (array[i] == 0) {
                // If one of the resistors  is zero in a parallel connection, the equivalence resistance is always zero
                return 0;
            }
            else {
                resistance += 1 / array[i];
            } 
        }
        resistance = 1 / resistance;
    }
    // Return -1 if any other connection type than 'S' or 'P' are received
    else{
        return -1;
    }

    return resistance;
}
