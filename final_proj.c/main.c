#include <stdio.h> // lets us print stuf on the screen 
#include <stdlib.h> // for doing malloc and EXIT things 
#include <string.h> // string magic like copying and breaking them
#include <stdbool.h> // lets us say "true" or "false" 
#include <time.h> // so we can see how long our stuff takes 
#include <ctype.h> // helps us know if a thingy is a letter or space or sumthin

#define MAX_LINE_LENGTH 1024 // this is just a big number for lines alloweed

// This checks if all the {} and () are happy and matched
bool CheckBraces(const char *code) {
    int balance = 0; // start with zero match score
    for (size_t i = 0; code[i] != '\0'; i++) { // go over each letter till the end
        if (code[i] == '{') {
            balance++; // open one brace, +1
        } else if (code[i] == '}') {
            balance--; // close brace, -1
            if (balance < 0) {
                return false;  
            }
        }
    }
    
    for (size_t i = 0; code[i] != '\0'; i++) { // now check for round ()
        if (code[i] == '(') {
            balance++;
        } else if (code[i] == ')') {
            balance--;
            if (balance < 0) {
                return false; // again, we are putting too many closing braces
            }
        }
    }
    return balance == 0; // if final score is 0, then that means its all good
}

// This one checks if "if" is used properly
bool CheckIfStatements(const char *code) {
    char *copy = strdup(code); // make a clone of the code to mess with
    if (!copy) return false; // if clone failed, just nope out

    char *line = strtok(copy, "\n"); // split the code by lines
    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++; // skip spaces at the start

        if (strncmp(line, "if", 2) == 0 && (isspace((unsigned char)line[2]) || line[2] == '(')) {
            char *paren = strchr(line, '('); // find first (
            if (paren == NULL) {
                free(copy); // clean up first b4 running away
                return false;
            }

            char *close_paren = strchr(paren, ')'); // find close )
            if (close_paren == NULL) {
                free(copy);
                return false;
            }

            if (close_paren == paren + 1) { // nothing between the () checker
                free(copy);
                return false;
            }
        }
        line = strtok(NULL, "\n"); // next line 
    }

    free(copy); // all done, clean it 
    return true; // all the ifs look good 
}

// This does the same thing but for "while" loops 
bool CheckWhileStatements(const char *code) {
    char *copy = strdup(code);
    if (!copy) return false;

    char *line = strtok(copy, "\n");
    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++; // removing spaces

        if (strncmp(line, "while", 5) == 0 && (isspace((unsigned char)line[5]) || line[5] == '(')) {
            char *paren = strchr(line, '(');
            if (paren == NULL) {
                free(copy);
                return false;
            }

            char *close_paren = strchr(paren, ')');
            if (close_paren == NULL) {
                free(copy);
                return false;
            }

            if (close_paren == paren + 1) {
                free(copy);
                return false; //  nothing inside while() 
            }
        }

        line = strtok(NULL, "\n");
    }

    free(copy); // cleanup time 
    return true;
}

// Now we check for "for" loops, those weird triple-part thingys
bool CheckForStatements(const char *code) {
    char *copy = strdup(code); // clone again
    if (!copy) return false;

    char *line = strtok(copy, "\n");
    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++; // skip them dumb spaces

        if (strncmp(line, "for", 3) == 0 && (isspace((unsigned char)line[3]) || line[3] == '(')) {
            char *paren = strchr(line, '(');
            if (paren == NULL) {
                free(copy);
                return false;
            }

            char *close_paren = strchr(paren, ')');
            if (close_paren == NULL) {
                free(copy);
                return false;
            }

            // Count how many ; are inside () like for(int i=0; i<10; i++)
            int semicolons = 0;
            char *ptr = paren + 1;
            while (ptr < close_paren) {
                if (*ptr == ';') semicolons++;
                ptr++;
            }

            if (semicolons != 2) {
                free(copy);
                return false; // 3 things needed in "for", 2 ; between them
            }
        }

        line = strtok(NULL, "\n");
    }

    free(copy);
    return true;
}

// Read file and give us all the text inside 
char* ReadFile(const char* filename) {
    FILE *file = fopen("mycode2.txt", "r"); // try open file (hardcoded oops)
    if (!file) {
        perror("Failed to open file"); // uh oh
        return NULL;
    }

    fseek(file, 0, SEEK_END); // go to end
    long length = ftell(file); // how big is it?
    fseek(file, 0, SEEK_SET); // go back to start

    char *buffer = malloc(length + 1); // memory allocation for the file 
    if (!buffer) {
        fclose(file); // close file if no memory 
        return NULL;
    }

    fread(buffer, 1, length, file); // reading the entire file 
    buffer[length] = '\0'; // end it nicely so that the compiler knows where to end it

    fclose(file); // done reading
    return buffer;
}

// Main function — this is where the party starts 
int main() {
    char *code = ReadFile("sample.c"); // read our code file
    if (!code) {
        fprintf(stderr, "Failed to read file\n"); // big problem
        return EXIT_FAILURE; // we done here
    }

    bool braceMatch, ifStatementsCorrect, whileStatementsCorrect, forStatementsCorrect;

    clock_t start = clock(); // start timing now 

    braceMatch = CheckBraces(code); // are all {} and () matching?
    ifStatementsCorrect = CheckIfStatements(code); // ifs check
    whileStatementsCorrect = CheckWhileStatements(code); // whiles check
    forStatementsCorrect = CheckForStatements(code); // for looops check

    clock_t end = clock(); // stop timing 
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC; // math to get time

    printf("--------------------------------\n");
    if (braceMatch) {
        printf("Braces matched successfully.\n"); 
    } else {
        printf("Braces do not match.\n"); 
    }

    if (ifStatementsCorrect) {
        printf("'If' statements are correct.\n"); 
    } else {
        printf("'If' statements are incorrect.\n"); 
    }

    if (whileStatementsCorrect) {
        printf("'While' statements are correct.\n");
    } else {
        printf("'While' statements are incorrect.\n");
    }

    if (forStatementsCorrect) {
        printf("'For' statements are correct.\n");
    } else {
        printf("'For' statements are incorrect.\n");
    }

    printf("Finished checks in %.6f seconds\n", elapsed); //  speed
    printf("--------------------------------\n");

    if (braceMatch && ifStatementsCorrect && whileStatementsCorrect && forStatementsCorrect) { //checks if all required things are there
        printf("Code is valid.\n"); 
    } else {
        printf("Code is invalid.\n"); 
    }

    free(code); // clean up the memory we used 
    return 0; 
}
