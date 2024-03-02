/*
The above code generates a sine lookup table. This can be further integeated into the module
*/

#include <stdio.h>
#include <math.h>

#define PI 3.14

int main()
{
    int M = 5;
    int K = 3;
    
    int arr_length = pow(2, M);
    //printf("%d", arr_length);
    
    float sine_array[arr_length];
    
    for(int i = 0; i < arr_length; i++)
    {
        sine_array[i] = sin(2*PI*i/((pow(2, M))-1));
    }
    
    for(int i = 0; i < arr_length; i++)
    {
        printf("%f", sine_array[i]);
        printf("\n");
    }
}
