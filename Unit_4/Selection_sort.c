// TO IMPLEMENT THE SELECTION SORT ALGORITHM

#include<stdio.h>

// Selection sort function
void selection_sort(int A[], int n)
{
    int i = 0;                                          // loop variable 1
    int j = 0;                                          // loop variable 1
    int min_idx = 0;                                    // minimum element index of sorted array

    for(i = 0; i < n; i++)
    {
        min_idx = i;                                    // assuming first element of sorted array

        for(j = i + 1; j < n; j++)                      // iterating across unsorted array
        {
            if(A[j] < A[min_idx])                       // swapping smallest element in unsorted array with defined element in sorted array
            {
                // SWAP
                int temp = A[j];
                A[j] = A[min_idx];
                A[min_idx] = temp;
            }
        }
    }

}

void main()
{
    int arr[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};                              // unsorted array to be sorted
    int N = (sizeof(arr) / sizeof(arr[0]));                                     // dynamic length of array

    selection_sort(arr, N);                                                     // invoking function selection sort

    for(int i = 0; i < N; i++)                                                  // printing array elements before sorting
    {
        printf('%d', arr[i]);
        printf('\t');
    }
}