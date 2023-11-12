#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char functionName[100];
    char line[256];
    FILE *file = fopen("manual.txt", "r");

    if (file == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    printf("Enter the function name you are looking for ('exit' to quit): ");
    scanf("%s", functionName);

    while (strcmp(functionName, "exit") != 0) {
        int found = 0;

        // Clear the terminal
        system("clear");

        // Search for the function name
        while (fgets(line, sizeof(line), file) != NULL) {
            if (line[0] == '#' && strstr(line, functionName) != NULL) {
                found = 1;
                // Print the function description until '/end' is encountered
                while (fgets(line, sizeof(line), file) != NULL && strstr(line, "/end") == NULL) {
                    printf("%s", line);
                }
                break;
            }
        }

        // Reset the file pointer to the beginning of the file for the next search
        fseek(file, 0, SEEK_SET);

        if (!found) {
            printf("Function not found. Please try again or type 'exit' to quit.\n");
        } else {
            printf("\nEnter the function name you are looking for ('exit' to quit): ");
        }

        scanf("%s", functionName);
    }

    fclose(file);
    return 0;
}
