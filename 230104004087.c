#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rules() {
    int code_length, min_digit, max_digit, duplicates, attempts, points_correct, points_misplaced, penalty_wrong;
    FILE* eyp = fopen("vault_config.txt", "w");
    if (eyp == NULL) {
        printf("ERROR: File cannot be opened!\n");
        return;
    }

    /*kuralları al*/
    do {
        printf("Code length (e.g., 4): ");
        scanf("%d", &code_length);
    } while (code_length <= 0);

    do {
        printf("Digit range (min: 0 max: 9, e.g., 4 9): ");
        scanf("%d %d", &min_digit, &max_digit);
    } while (min_digit < 0 || max_digit <= min_digit);

    do {
        printf("Allow duplicates (0 = No, 1 = Yes): ");
        scanf("%d", &duplicates);
    } while (duplicates < 0 || duplicates > 1);

    do {
        printf("Maximum number of attempts: ");
        scanf("%d", &attempts);
    } while (attempts <= 0);

    do {
        printf("Points for correct digit in correct place (C): ");
        scanf("%d", &points_correct);
    } while (points_correct <= 0);

    do {
        printf("Points for misplaced digit (M): ");
        scanf("%d", &points_misplaced);
    } while (points_misplaced <= 0);

    do {
        printf("Penalty for wrong digit (W): ");
        scanf("%d", &penalty_wrong);
    } while (penalty_wrong <= 0);

    /*dosyaya atma*/
    fprintf(eyp, "CODE_LENGTH= %d\nDIGIT_MIN= %d\nDIGIT_MAX= %d\nMAX_ATTEMPTS= %d\nALLOW_DUPLICATES= %d\n",
            code_length, min_digit, max_digit, attempts, duplicates);
    fprintf(eyp, "POINTS_CORRECT= %d\nPOINTS_MISPLACED= %d\nPENALTY_WRONG= %d\n", points_correct, points_misplaced, penalty_wrong);

    fclose(eyp);
}

int* generate_code() {
    int code_length, min_digit, max_digit, duplicates,i,j;
    FILE* eyp = fopen("vault_config.txt", "r");
    if (eyp == NULL) {
        printf("ERROR: vault_config.txt not found!\n");
        return NULL;
    }

    fscanf(eyp, "CODE_LENGTH= %d\nDIGIT_MIN= %d\nDIGIT_MAX= %d\nMAX_ATTEMPTS= %*d\nALLOW_DUPLICATES= %d\n", 
            &code_length, &min_digit, &max_digit, &duplicates);

    fclose(eyp);

    int* code = (int*)malloc(code_length * sizeof(int));
    if (code == NULL) {
        printf("ERROR: Memory allocation failed for code.\n");
        return NULL;
    }

    for (i = 0; i < code_length; i++) {
        if (duplicates == 0) {
            int unique;
            do {
                unique = 1;
                code[i] = rand() % (max_digit - min_digit + 1) + min_digit;
                for (j = 0; j < i; j++) {
                    if (code[i] == code[j]) {
                        unique = 0;
                        break;
                    }
                }
            } while (!unique);
        } else {
            code[i] = rand() % (max_digit - min_digit + 1) + min_digit;
        }
    }

    /*dosyaya atma*/
    FILE* vault_code_file = fopen("vault_code.txt", "w");
    if (vault_code_file == NULL) {
        printf("ERROR: File vault_code.txt could not be opened for writing.\n");
        free(code);
        return NULL;
    }

    for (i = 0; i < code_length; i++) {
        fprintf(vault_code_file, "%d ", code[i]);
    }
    fprintf(vault_code_file, "\n");

    fclose(vault_code_file);

    return code;
}

int* get_guess(int code_length) {
    int i;
    int* guess = (int*)malloc(code_length * sizeof(int));
    if (guess == NULL) {
        printf("ERROR: Memory allocation failed for guess.\n");
        return NULL;
    }

    printf("Enter your guess (e.g., 1 2 3 4): ");
    for (i = 0; i < code_length; i++) {
        scanf("%d", &guess[i]);
    }

    return guess;
}

char* check_guess_and_vault_code(int* vault, int* guess, int length) {
    int i,j;
    char* feedback = (char*)malloc(length * sizeof(char));
    if (feedback == NULL) {
        printf("ERROR: Memory allocation failed for feedback.\n");
        return NULL;
    }

    /*yeri doğru mu*/
    for (i = 0; i < length; i++) {
        if (guess[i] == vault[i]) {
            feedback[i] = 'C';  
        } else {
            feedback[i] = 'W';  /*yanlış yer*/
        }
    }

    /*kalanları m yap*/
    for (i = 0; i < length; i++) {
        if (feedback[i] == 'W') {  
            for (j = 0; j < length; j++) {
                if (guess[i] == vault[j] && i != j && feedback[j] != 'C') {
                    feedback[i] = 'M'; 
                    break;
                }
            }
        }
    }

    return feedback;
}

void log_game(int attempt, int* guess, char* feedback, int score) {
    int i;
    FILE* log_file = fopen("game_log.txt", "a");
    if (log_file == NULL) {
        printf("ERROR: Could not open game_log.txt!\n");
        return;
    }

    
    time_t t;
    struct tm* tm_info;
    char date_time[25];
    time(&t);
    tm_info = localtime(&t);
    strftime(date_time, 25, "%Y-%m-%d %H:%M:%S", tm_info);

    if (attempt == 1) {
        fprintf(log_file, "--- Vault Codebreaker Game Log ---\n");
        fprintf(log_file, "Game Date: %s\n", date_time);
        fprintf(log_file, "Secret Code: %d\n", guess[0]); 
        fprintf(log_file, "Code Length: %d\n", 4);  
        fprintf(log_file, "Digit Range: 0-9\n");
        fprintf(log_file, "Duplicates Allowed: 1\n");
        fprintf(log_file, "Max Attempts: 10\n");
        fprintf(log_file, "---------------------------------\n");
    }
        /*adımlar*/
    fprintf(log_file, "Attempt %d: ", attempt);
    for (i = 0; i < 4; i++) {
        fprintf(log_file, "%d", guess[i]);
    }
    fprintf(log_file, " => Feedback: ");
    for (i = 0; i < 4; i++) {
        fprintf(log_file, "%c ", feedback[i]);
    }
    fprintf(log_file, "| Score: %d\n", score);

    fclose(log_file);
}

void print_game_result(int score) {
    if (score >= 90) {
        printf("Code Master \n");
    } else if (score >= 70) {
        printf("Cipher Hunter \n");
    } else if (score >= 50) {
        printf("Number Sleuth \n");
    } else if (score >= 30) {
        printf("Safe Kicker \n");
    } else if (score >= 10) {
        printf("Lucky Breaker \n");
    } else {
        printf("Code Potato \n");
    }
}

void game_part() {
    int* vault_code = generate_code();
    if (vault_code == NULL) return;

    int code_length, attempts, points_correct, points_misplaced, penalty_wrong;
    FILE* eyp = fopen("vault_config.txt", "r");
    if (eyp == NULL) {
        printf("ERROR: vault_config.txt not found!\n");
        free(vault_code);
        return;
    }

    fscanf(eyp, "CODE_LENGTH= %d\nDIGIT_MIN= %*d\nDIGIT_MAX= %*d\nMAX_ATTEMPTS= %d\nALLOW_DUPLICATES= %*d\n", 
           &code_length, &attempts);
    fscanf(eyp, "POINTS_CORRECT= %d\nPOINTS_MISPLACED= %d\nPENALTY_WRONG= %d\n", 
           &points_correct, &points_misplaced, &penalty_wrong);

    fclose(eyp);

    int* guess;
    char* feedback;
    int score = 0;
    int flag = 1; 
    int attempt = 1;

    while (flag) {
        int i ;
        if (attempts <= 0) {  
            printf("Game Over! You ran out of attempts.\n");
            break;
        }

        guess = get_guess(code_length);
        if (guess == NULL) {
            free(vault_code);
            return;
        }

        feedback = check_guess_and_vault_code(vault_code, guess, code_length);
        printf("Feedback: ");
        for (i = 0; i < code_length; i++) {
            printf("%c ", feedback[i]);
        }
        printf("\n");

        for (i = 0; i < code_length; i++) {
            if (feedback[i] == 'C') {
                score += points_correct;
            } else if (feedback[i] == 'M') {
                score += points_misplaced;
            } else {
                score -= penalty_wrong;
            }
        }

        log_game(attempt, guess, feedback, score);

        // Check if player guessed the code
        int correct = 1;
        for (i = 0; i < code_length; i++) {
            if (feedback[i] != 'C') {
                correct = 0;
                break;
            }
        }

        if (correct) {
            printf("Congratulations! You've guessed the code.\n");
            printf("Final score: %d\n", score);
            flag = 0;  // End the game
        } else {
            attempts--;  // Decrease remaining attempts
        }

        free(feedback);
        free(guess);
        attempt++;
    }

    print_game_result(score);  // Print result based on score

    free(vault_code);
}

int main() {
    srand(time(NULL));

    char choice;
    printf("Enter 'A' for Admin mode or 'P' for Player mode: ");
    scanf(" %c", &choice);

    switch (choice) {
        case 'A':
            rules();
            break;
        case 'P':
            game_part();
            break;
        default:
            printf("Invalid choice. Exiting...\n");
            return 0;
    }

    return 0;
}

