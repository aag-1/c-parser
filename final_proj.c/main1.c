#include <stdio.h> // lets us print stuff on the screen 
#include <stdlib.h> // for doing malloc and EXIT things 
#include <string.h> // string magic like copying and breaking them
#include <stdbool.h> // lets us say "true" or "false" 
#include <time.h> // so we can see how long our stuff takes 
#include <ctype.h> // helps us know if a thingy is a letter or space or sumthin

#define MAX_LINE_LENGTH 1024 // this is just a big number for lines allowed

/**
 * @brief Checks if all curly braces {} and parentheses () in the code are balanced and matched.
 * * @param code The string containing the C code to check.
 * @return true if braces and parentheses are balanced, false otherwise.
 */
bool CheckBraces(const char *code) {
    int brace_balance = 0; // Balance for curly braces {}
    int paren_balance = 0; // Balance for parentheses ()

    for (size_t i = 0; code[i] != '\0'; i++) { // Go over each character till the end
        if (code[i] == '{') {
            brace_balance++; // Open one brace, +1
        } else if (code[i] == '}') {
            brace_balance--; // Close brace, -1
            if (brace_balance < 0) {
                return false; // Too many closing braces
            }
        } else if (code[i] == '(') {
            paren_balance++; // Open one parenthesis, +1
        } else if (code[i] == ')') {
            paren_balance--; // Close parenthesis, -1
            if (paren_balance < 0) {
                return false; // Too many closing parentheses
            }
        }
    }
    // If both balances are 0, then all are good
    return (brace_balance == 0 && paren_balance == 0); 
}

/**
 * @brief Checks if "if" statements are properly formatted (e.g., `if (condition)`).
 * Ensures there are parentheses around the condition is not empty.
 * * @param code The string containing the C code to check.
 * @return true if all 'if' statements appear correctly formatted, false otherwise.
 */
bool CheckIfStatements(const char *code) {
    // strtok modifies the string, so we make a copy of the code
    // to avoid changing the original code.
    char *copy = strdup(code);  // Make a clone of the code to mess with - use of strdup
    if (!copy) {
        perror("Failed to allocate memory for code copy");
        return false; // If clone failed, just nope out
        
    }

    char *line = strtok(copy, "\n"); // Split the code by lines, the pointer named line - indicates to induvidual lines of code
    //assinges \0 for everytime it encounters \n, so that we can check line by line and not mess up checking multiple lines of code
    while (line != NULL) {
        // Skip leading whitespace characters
        while (isspace((unsigned char)*line)) line++;  //checks if whitespace is there - standard library function in C

        // Check if the line starts with "if" followed by a space or '('
        if (strncmp(line, "if", 2) == 0 && (isspace((unsigned char)line[2]) || line[2] == '(')) {
            char *paren = strchr(line, '('); // Find first '(' after "if"
            if (paren == NULL) {
                free(copy); // Clean up first before running away
                return false; // No opening parenthesis found
            }

        /*
        
        */

            char *close_paren = strchr(paren + 1, ')'); // Find closing ')' after the opening one
            if (close_paren == NULL) {
                free(copy);
                return false; // No closing parenthesis found
            }

            // Check if there's nothing between the parentheses, e.g., if()
            if (close_paren == paren + 1) { 
                free(copy);
                return false; // Empty condition in if()
            }
        }
        line = strtok(NULL, "\n"); // Move to the next line 
    }

    free(copy); // All done, clean it 
    return true; // All the ifs look good 
}

/**
 * @brief Checks if "while" statements are properly formatted (e.g., `while (condition)`).
 * Ensures there are parentheses around the condition and the condition is not empty.
 * * @param code The string containing the C code to check.
 * @return true if all 'while' statements appear correctly formatted, false otherwise.
 */
bool CheckWhileStatements(const char *code) {
    char *copy = strdup(code);
    if (!copy) {
        perror("Failed to allocate memory for code copy");
        return false;
    }

    char *line = strtok(copy, "\n");
    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++; // Removing leading spaces

        // Check if the line starts with "while" followed by a space or '('
        if (strncmp(line, "while", 5) == 0 && (isspace((unsigned char)line[5]) || line[5] == '(')) {
            char *paren = strchr(line, '(');
            if (paren == NULL) {
                free(copy);
                return false;
            }

            char *close_paren = strchr(paren + 1, ')');
            if (close_paren == NULL) {
                free(copy);
                return false;
            }

            // Check for empty condition in while()
            if (close_paren == paren + 1) {
                free(copy);
                return false; // Nothing inside while() 
            }
        }

        line = strtok(NULL, "\n");
    }

    free(copy); // Cleanup time 
    return true;
}

/**
 * @brief Checks if "for" statements are properly formatted (e.g., `for (init; condition; increment)`).
 * Ensures there are parentheses and exactly two semicolons inside the parentheses.
 * * @param code The string containing the C code to check.
 * @return true if all 'for' statements appear correctly formatted, false otherwise.
 */
bool CheckForStatements(const char *code) {
    char *copy = strdup(code); // Clone again
    if (!copy) {
        perror("Failed to allocate memory for code copy");
        return false;
    }

    char *line = strtok(copy, "\n");
    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++; // Skip them dumb spaces

        // Check if the line starts with "for" followed by a space or '('
        if (strncmp(line, "for", 3) == 0 && (isspace((unsigned char)line[3]) || line[3] == '(')) {
            char *paren = strchr(line, '(');
            if (paren == NULL) {
                free(copy);
                return false;
            }

            char *close_paren = strchr(paren + 1, ')');
            if (close_paren == NULL) {
                free(copy);
                return false;
            }

            // Count how many ; are inside () like for(int i=0; i<10; i++)
            int semicolons = 0;
            char *ptr = paren + 1; // Start checking after the opening parenthesis
            while (ptr < close_paren) {
                if (*ptr == ';') semicolons++;
                ptr++;
            }

            // A 'for' loop typically requires two semicolons to separate its three parts
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

/**
 * @brief Reads the entire content of a file into a dynamically allocated string.
 * * @param filename The path to the file to read.
 * @return A pointer to the dynamically allocated string containing the file content,
 * or NULL if the file cannot be opened or memory allocation fails.
 * The caller is responsible for freeing the returned memory.
 */
char* ReadFile(const char* filename) {
    FILE *file = fopen(filename, "r"); // Try to open the file
    if (!file) {
        perror("Failed to open file"); // Print error if opening fails
        return NULL;
    }

    fseek(file, 0, SEEK_END); // Go to the end of the file
    long length = ftell(file); // Get the current position (which is the file size)
    fseek(file, 0, SEEK_SET); // Go back to the beginning of the file

    char *buffer = (char *)malloc(length + 1); // Allocate memory for the file content + null terminator
    if (!buffer) {
        fclose(file); // Close file if memory allocation fails
        perror("Failed to allocate memory for file buffer");
        return NULL;
    }

    fread(buffer, 1, length, file); // Read the entire file content into the buffer
    buffer[length] = '\0'; // Null-terminate the string nicely

    fclose(file); // Done reading, close the file
    return buffer;
}

/**
 * @brief Displays the usage instructions for the program.
 */
void print_usage() {
    printf("Usage: ./code_analyzer [OPTIONS] [FILENAME]\n");
    printf("Analyzes C code for brace matching and statement formatting.\n\n");
    printf("Options:\n");
    printf("  --check-braces    Only check for balanced braces and parentheses.\n");
    printf("  --check-if        Only check 'if' statement formatting.\n");
    printf("  --check-while     Only check 'while' statement formatting.\n");
    printf("  --check-for       Only check 'for' statement formatting.\n");
    printf("  --help            Display this help message.\n\n");
    printf("FILENAME: The C source file to analyze. Defaults to 'sample.c' if not provided.\n");
    printf("If no specific check options are provided, all checks will be performed.\n");
}


/**
 * @brief Main function - entry point of the program.
 * Parses command-line arguments to determine which checks to run and
 * which file to analyze.
 * * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return EXIT_SUCCESS (0) on successful execution, EXIT_FAILURE (1) on error.
 */
int main(int argc, char *argv[]) {
    // Default flags: run all checks
    bool runBraceCheck = false;
    bool runIfCheck = false;
    bool runWhileCheck = false;
    bool runForCheck = false;
    bool anyCheckSpecified = false; // Flag to know if user specified any checks

    char *filename = "mycode.txt"; // Default filename

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--check-braces") == 0) {
            runBraceCheck = true;
            anyCheckSpecified = true;
        } else if (strcmp(argv[i], "--check-if") == 0) {
            runIfCheck = true;
            anyCheckSpecified = true;
        } else if (strcmp(argv[i], "--check-while") == 0) {
            runWhileCheck = true;
            anyCheckSpecified = true;
        } else if (strcmp(argv[i], "--check-for") == 0) {
            runForCheck = true;
            anyCheckSpecified = true;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage();
            return EXIT_SUCCESS;
        } else {
            // Assume it's the filename if it's not a recognized flag
            // and hasn't been set yet
            if (argv[i][0] != '-') { // Check if it doesn't start with a hyphen (likely a flag)
                filename = argv[i];
            } else {
                fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
                print_usage();
                return EXIT_FAILURE;
            }
        }
    }

    // If no specific checks were requested, enable all of them
    if (!anyCheckSpecified) {
        runBraceCheck = true;
        runIfCheck = true;
        runWhileCheck = true;
        runForCheck = true;
    }

    char *code = ReadFile(filename); // Read our code file
    if (!code) {
        fprintf(stderr, "Failed to read file '%s'\n", filename); // Big problem
        return EXIT_FAILURE; // We done here
    }

    bool overall_valid = true; // Assume valid until a check fails
    clock_t start = clock(); // Start timing now 

    printf("--------------------------------\n");
    printf("Analyzing file: %s\n", filename);
    printf("--------------------------------\n");

    if (runBraceCheck) {
        bool braceMatch = CheckBraces(code); // Are all {} and () matching?
        if (braceMatch) {
            printf("Braces and Parentheses matched successfully.\n"); 
        } else {
            printf("Braces and Parentheses do not match.\n"); 
            overall_valid = false;
        }
    }

    if (runIfCheck) {
        bool ifStatementsCorrect = CheckIfStatements(code); // Ifs check
        if (ifStatementsCorrect) {
            printf("'If' statements are correct.\n"); 
        } else {
            printf("'If' statements are incorrect.\n"); 
            overall_valid = false;
        }
    }

    if (runWhileCheck) {
        bool whileStatementsCorrect = CheckWhileStatements(code); // Whiles check
        if (whileStatementsCorrect) {
            printf("'While' statements are correct.\n");
        } else {
            printf("'While' statements are incorrect.\n");
            overall_valid = false;
        }
    }

    if (runForCheck) {
        bool forStatementsCorrect = CheckForStatements(code); // For loops check
        if (forStatementsCorrect) {
            printf("'For' statements are correct.\n");
        } else {
            printf("'For' statements are incorrect.\n");
            overall_valid = false;
        }
    }

    clock_t end = clock(); // Stop timing 
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC; // Math to get time

    printf("--------------------------------\n");
    printf("Finished checks in %.6f seconds\n", elapsed); // Speed
    printf("--------------------------------\n");

    if (overall_valid) { // Checks if all required things are there and passed
        printf("Code is valid based on selected checks.\n"); 
    } else {
        printf("Code is invalid based on selected checks.\n"); 
    }

    free(code); // Clean up the memory we used 
    return EXIT_SUCCESS; 
}
