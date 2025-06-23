#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readStrings(char **arr, int n);
void sortStrings(char **arr, int n);
void displayArray(char **arr, int n);

// Read strings from user
void readStrings(char **arr, int n)
{
    char buffer[1000];
    for (int i = 0; i < n; i++)
    {
        printf("Please enter string %d: ", (i + 1));
        scanf("%s", buffer);

        // Determine length and allocate memory
        int len = strlen(buffer);
        arr[i] = (char *)malloc((len + 1) * sizeof(char));
        strcpy(arr[i], buffer);
    }
}

// Sort the strings
void sortStrings(char **arr, int n)
{
    char *temp;
    int currLoc;
    for (int i = 1; i < n; i++)
    {
        currLoc = i;
        while (currLoc > 0 && strcmp(arr[currLoc - 1], arr[currLoc]) > 0)
        {
            temp = arr[currLoc];
            arr[currLoc] = arr[currLoc - 1];
            arr[currLoc - 1] = temp;
            currLoc--;
        }
    }
}

void displayArray(char **arr, int n)
{
    printf("[");
    for (int i = 0; i < n - 1; i++)
    {
        printf("%s, ", arr[i]);
    }
    printf("%s]\n", arr[n - 1]);
}

int main(int argc, char **argv)
{
    int n;

    // Ask user for the number of strings they want to enter
    printf("Please enter number of strings to sort: ");
    scanf("%d", &n);

    // Allocate memory
    char **strings = (char **)malloc(n * sizeof(char *));

    readStrings(strings, n);
    printf("\nOriginal array of strings: ");
    displayArray(strings, n);
    sortStrings(strings, n);
    printf("Sorted array of strings: ");
    displayArray(strings, n);

    return 0;
}