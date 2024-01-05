#include <stdio.h>
#include <dirent.h>
#include <stdlib.h> // for dynamic memory allocation

int main()
{
    // Specify the folder path
    const char *folderPath = "data/mps";

    // Open the directory
    DIR *dir = opendir(folderPath);

    if (dir != NULL)
    {
        // Read the directory entries
        struct dirent *entry;
        int fileCount = 0;

        // Count the number of files in the directory
        while ((entry = readdir(dir)) != NULL)
        {
            fileCount++;
        }

        // Close and reopen the directory to reset the position
        closedir(dir);
        dir = opendir(folderPath);

        // Allocate memory for an array to store file names
        char **fileNames = (char **)malloc(fileCount * sizeof(char *));

        // Read the directory entries and store file names in the array
        for (int i = 0; i < fileCount; i++)
        {
            entry = readdir(dir);
            fileNames[i] = strdup(entry->d_name); // strdup allocates memory and copies the string
        }

        // Close the directory
        closedir(dir);

        // Print the list of file names
        printf("List of files:\n");
        for (int i = 0; i < fileCount; i++)
        {
            printf("%s\n", fileNames[i]);
            free(fileNames[i]); // free allocated memory for each file name
        }

        // Free the array itself
        free(fileNames);
    }
    else
    {
        // Print an error message if the directory couldn't be opened
        perror("Unable to open directory");
        return 1; // Return an error code
    }

    return 0; // Return success
}