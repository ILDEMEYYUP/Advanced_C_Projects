#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu() {
    printf("\n1. Perform action\n");
    printf("2. Undo last action\n");
    printf("3. Print action history\n");
    printf("4. Exit\n");
}

char **create_stack(int initial_capacity) {
    char **stack = (char **)malloc(sizeof(char *) * initial_capacity);
    if (!stack) {
        printf("ERROR: Stack could not be created!\n");
        exit(1);
    }
    return stack;
}

char **push_action_onebyone(char **stack, int *capacity, int *size, char *action) {
    (*capacity) += 1;
    stack = (char **)realloc(stack, sizeof(char *) * (*capacity));
    stack[*size] = strdup(action);
    (*size) += 1;
    printf("Stack resized: capacity = %d\n", *capacity);
    return stack;
}

char **push_action_double(char **stack, int *capacity, int *size, char *action) {
    if (*size >= *capacity) {
        (*capacity) *= 2;
        stack = (char **)realloc(stack, sizeof(char *) * (*capacity));
        printf("Stack resized: capacity = %d\n", (*capacity));
    }
    stack[*size] = strdup(action);
    (*size) += 1;
    return stack;
}

void undo_action(char **stack, int *size) {
    if (*size == 0) {
        printf("No actions!\n");
        return;
    }
    printf("DELETE: %s", stack[*size - 1]);
    free(stack[*size - 1]);
    (*size) -= 1;
}

void print_history(char **stack, int size) {
    if (size == 0) {
        printf("No actions in history.\n");
        return;
    }
    printf("Action History (Top to Bottom):\n");
    for (int i = size; i>=0; i--) {
        printf("%s", stack[i]);
    }
}

void free_stack(char **stack, int size) {
    for (int i = 0; i < size; i++) {
        free(stack[i]);
    }
    free(stack);
}

int main(void) {
    int choice, stack_type;
    char action[100];
    char **stack;
    int capacity, size;

    while (1) {
        printf("\nWhich stack will you use?\n");
        printf("1. One-by-one growth stack\n");
        printf("2. Doubling capacity stack\n");
        printf("Choice: ");
        scanf("%d", &stack_type);
        getchar();

        if (stack_type == 2) {
            printf("Enter initial capacity: ");
            scanf("%d", &capacity);
            getchar();
        } else if (stack_type == 1) {
            capacity = 1;
        } else {
            printf("Invalid stack type! Please choose 1 or 2.\n");
            continue;
        }

        stack = create_stack(capacity);
        size = 0;

        do {
            menu();
            printf("Choice: ");
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input! Exiting.\n");
                break;
            }
            getchar();

            switch (choice) {
                case 1:
                    printf("Enter action: ");

                    fgets(action, sizeof(action), stdin);
                    if (stack_type == 1){

                        stack = push_action_onebyone(stack, &capacity, &size, action);
                       
                        printf("action recorded .\n");

                    }

                    else{
                        stack = push_action_double(stack, &capacity, &size, action);
                       
                        printf("action recorded .\n");

                    }
        
                    break;
                case 2:
                    undo_action(stack, &size);
                    break;
                case 3:
                    print_history(stack, size);
                    break;
                case 4:
                    printf("Cleaning up memory... Goodbye!\n");
                    free_stack(stack, size);
                    return 0;
                default:
                    printf("Invalid input!\n");
            }
        } while (choice != 4);

        free_stack(stack, size);
    }

    return 0;
}
