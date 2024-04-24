// TO IMPLEMENT LINEAR SEARCH ALGORITHM

#include<stdio.h>

int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};          // Initialising an array of 10 elements
int idx = -1;                                           // Default value of return index, default is -1 to indicate element not found yet
int elem = 0;                                           // Variable to hold search element

void main()
{
    printf("Enter the element to be found:");
    scanf("%d", &elem);

    for(int i = 0; i < 10; i++)                         // Iteratively searching for element
    {
        if(arr[i] == elem)
        {
            idx = i;
        }
    }

    if(idx != -1)                                       // Indicating element exists and found in array
    {
        printf("Element found at %d", idx);
    }
    else                                                // Indicating element not found
    {
        printf("Element not found");
    }
}