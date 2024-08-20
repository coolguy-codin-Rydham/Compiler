#include <stdio.h>
#include <ctype.h>
#include <string.h>

static int Putback = 0;  // Variable to store the putback character
static FILE *Infile;     // File pointer for input

// Function to read the next character with putback capability
static int next(void) {
    int c;

    if (Putback) {          // If there's a putback character
        c = Putback;        // Use it
        Putback = 0;        // Clear the putback variable
        return c;           // Return the putback character
    }

    c = fgetc(Infile);      // Otherwise, read from the file
    return c;
}

// Function to put a character back
static void putback(int c) {
    Putback = c;            // Store the character in Putback
}

int main() {
    Infile = fopen("input.txt", "r");
    if (Infile == NULL) {
        perror("Failed to open file");
        return 1;
    }

    char word[6];           // Buffer to store the word "Hello"
    int i = 0;

    // Read each character and check if it forms the word "Hello"
    while (i < 5) {
        int c = next();
        if (isalpha(c)) {   // If the character is alphabetic
            word[i++] = c;
        } else {            // If it's not, put it back and stop
            putback(c);
            break;
        }
    }

    word[i] = '\0';         // Null-terminate the string

    // Check if the word "Hello" was found
    if (i == 5 && strcmp(word, "Helloo") == 0) {
        printf("Found the word: %s\n", word);
    } else {
        printf("Did not find the word 'Hello'.\n");
    }

    // The putback character can now be read again
    int c = next();         // Read the character that was put back
    printf("The next character is: %c\n", c);

    fclose(Infile);
    return 0;
}
