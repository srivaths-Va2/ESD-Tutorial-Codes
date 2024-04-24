// TO IMPLEMENT BINARY SEARCH

#include<stdio.h>

int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};          // Declaring the array
int lower = 0;                                          // index of first element in array
int upper = 0;                                          // index of last element in array
int element;                                            // element to be seearched for

// Function binary search
int Binary_Search(int arr[10], int l, int u, int elem)
{
    int len = sizeof(arr) / sizeof(arr[0]);             // dynamically calculating length of array

    while(l <= u)
    {
        int mid = (l + u) / 2;                          // mid index of array

        if(arr[mid] == elem)                            // obtaining middle element of subarray
        {
            return mid;
        }

        if(arr[mid] < elem)                             // altering lower index to procure subarray
        {
            l = mid + 1;
        }

        if(arr[mid] > elem)                             // altering upper index to procure subarray
        {
            u = mid - 1;
        }
    }

    return -1;                                         // element not found
}

void main()
{
    printf("Enter the element to search for : ");
    scanf("%d", &element);

    upper = (sizeof(arr) / sizeof(arr[0])) - 1;


    int idx = Binary_Search(arr, lower, upper, element);

    if(idx == -1)
    {
        printf("Element not found!");
    }

    else
    {
        printf("Element found at %d", idx);
    }

}
