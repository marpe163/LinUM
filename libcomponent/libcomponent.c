#include <math.h>
#include <stdlib.h>
#include <stdio.h>
static const float e12_series_coefficients[] = {1.f, 1.2f, 1.5f, 1.8f, 2.2f, 2.7f, 3.3f, 3.9f, 4.7f, 5.6f, 6.8f, 8.2f};

typedef struct e12_series_
{
    int number_of_resistors;
    float * resistor_values;    
} e12_series;

static e12_series generate_e12_series(const float orig_resistance);
int e_resistance(float orig_resistance, float *res_array );

#ifndef HAVE_MAIN
int main()
{
    float res[3];
    int count = e_resistance(1201.01f, res);
    for(int i=0;i<3;i++)
    {
        printf("Resistor %d: %f\n", i+1,res[i]);
    }
   return 0;
}
#endif

int e_resistance(float orig_resistance, float *res_array )
{
    e12_series e12Resistors = generate_e12_series(orig_resistance);
    int number_of_resistors = 0;
    float current_e_res = -1.f;
    float best_diff = orig_resistance;

    for(int i=0; i < e12Resistors.number_of_resistors; i++)
    {
        for(int j=i; j < e12Resistors.number_of_resistors; j++)
        {
            for(int k=j; k < e12Resistors.number_of_resistors; k++)
            {
                current_e_res = e12Resistors.resistor_values[i] + 
                                e12Resistors.resistor_values[j] +
                                e12Resistors.resistor_values[k];
                float res_difference = orig_resistance - current_e_res;
                float abs_error = res_difference < 0 ? res_difference * -1.f : res_difference;
                
                if(abs_error < best_diff)
                {
                    best_diff = abs_error;
                    res_array[0]=e12Resistors.resistor_values[k];
                    res_array[1]=e12Resistors.resistor_values[j];
                    res_array[2]=e12Resistors.resistor_values[i];

                    number_of_resistors = 0;
                    for(int q=0; q<3;q++)
                    {
                        if(res_array[q] > 0)
                        {
                            number_of_resistors++;
                        }
                    }


                }
            }
        }
    }

   free(e12Resistors.resistor_values);

   return number_of_resistors; 
}

static e12_series generate_e12_series(const float orig_resistance)
{
    int number_of_resistors_in_series = (12 * ceil(log10((float) orig_resistance))) + 1;
    e12_series resistors = {.number_of_resistors = number_of_resistors_in_series,
                            .resistor_values = calloc(number_of_resistors_in_series, sizeof(float))};

    for(int resistor_idx=0; resistor_idx<number_of_resistors_in_series - 1; resistor_idx++)
    {
        resistors.resistor_values[resistor_idx + 1] = e12_series_coefficients[resistor_idx % 12] * powf(10.f, (resistor_idx / 12));
    }

    return resistors; 
}

