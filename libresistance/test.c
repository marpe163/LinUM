#include <stdio.h>
#include <stdlib.h>

#include "lib_resistance.h"


int main(int argc, char *argv[])
{
    float *resistance;
    int count;
    char conn;
    int i;

    float replacement_resistance = 0.0;

    printf("Ange koppling [S | P]: ");
    scanf(" %c", &conn);
    printf("Antal komponenter: ");
    scanf("%d", &count);
    resistance = malloc(count * sizeof(float));

    for (i = 0; i < count; i++) {
        printf("Komponent %d i ohm: ", i + 1);
        scanf("%f", &resistance[i]);
    }

    replacement_resistance = calc_resistance(count, conn, resistance);

    printf("Ersättningsresistans:\n%.1f ohm\n", replacement_resistance);

    free(resistance);

    replacement_resistance = calc_resistance(2, 'S', NULL);
    printf("NULL pointer handling: expected result: -1, received result: %.0f\n", replacement_resistance);

    count = 2;
    resistance = malloc(count * sizeof(float));
    for (i = 0; i < count; i++)
    {
        resistance[i] = i + 5;
    }

    replacement_resistance = calc_resistance(0, 'S', resistance);
    printf("Zero count handling: expected result: -1, received result: %.0f\n", replacement_resistance);

    replacement_resistance = calc_resistance(2, 'T', resistance);
    printf("Erroneous conn type: expected result: -1, received result: %.0f\n", replacement_resistance);

    replacement_resistance = calc_resistance(count, 'S', resistance);
    printf("Serial connected: expected result: 11, received result: %.0f\n", replacement_resistance);

    replacement_resistance = calc_resistance(count, 'P', resistance);
    printf("Parallel connected: expected result: 2.7, received result: %.1f\n", replacement_resistance);

    resistance[1] = 0;
    replacement_resistance = calc_resistance(count, 'P', resistance);
    printf("Parallel connected: expected result: 0, received result: %.0f\n", replacement_resistance);

    free(resistance);

    count = 3;
    resistance = malloc(count * sizeof(float));
    for (i = 0; i < count; i++)
    {
        resistance[i] = 100;
    }

    replacement_resistance = calc_resistance(count, 'S', resistance);
    printf("Serial connected: expected result: 300, received result: %.0f\n", replacement_resistance);

    replacement_resistance = calc_resistance(count, 'P', resistance);
    printf("Parallel connected: expected result: 33.3, received result: %.1f\n", replacement_resistance);

    free(resistance);

    count = 200;
    resistance = malloc(count * sizeof(float));
    for (i = 0; i < count; i++)
    {
        resistance[i] = 10;
    }

    replacement_resistance = calc_resistance(count, 'S', resistance);
    printf("Serial connected: expected result: 2000, received result: %.0f\n", replacement_resistance);

    replacement_resistance = calc_resistance(count, 'P', resistance);
    printf("Parallel connected: expected result: 0.05, received result: %.2f\n", replacement_resistance);

    free(resistance);
}
