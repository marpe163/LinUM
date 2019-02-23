/** @file */
/** \file resistance.h
    \brief Library to calculate the equivalence resistance from a number of serial- or parallel
    connected resistances.
    
    This library contains  a function to calculate the equivalence resistance, or total resistance, 
    from a number of serial or parallel connected resistances.
*/

#ifndef _LIB_DISTANCE_H_
#define _LIB_DISTANCE_H_

    float
    calc_resistance(int count, char conn, float *array);

#endif