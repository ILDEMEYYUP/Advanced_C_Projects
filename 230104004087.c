#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

enum Color {RGB, BGR, GRAY};
#define MAX 3
int TOP = 0;
char Stack[MAX];

char pop() {
    if (TOP > 0) {
        TOP--;
        return Stack[TOP];
    } else {
        return '\0';
    }
}

void push(int a) {
    if (TOP < MAX) {
        Stack[TOP] = a;
        TOP++;
    }
}

int max_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int max = 0;
    int index = 0;
    int i;
    int current_multip;

    for (i = 0; i < chunk_size; i++) {
        current_multip = chunk[index] * corresponding_chunk_part[i];
        if (current_multip >= max) {
            max = current_multip;
        }
        index++;
    }
    return max;
}

int avg_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int sum = 0;
    int i;
    int index = 0;

    for (i = 0; i < chunk_size; i++) {
        sum += chunk[index] * corresponding_chunk_part[i];
        index++;
    }
    return sum / chunk_size;
}

int apply_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int return_value;
    char operation_code=pop();
    if (operation_code == 'm') {
        return_value = max_operation(chunk, corresponding_chunk_part, chunk_size);
        return return_value;
    } else {
        return_value = avg_operation(chunk, corresponding_chunk_part, chunk_size);
        return return_value;
    }
}

void make_input_file(int* chunk, int* line2, int* line3, int* line4, int chunk_size, int line_size) {
    srand(time(NULL));
    int i;
    FILE* input;
    input = fopen("input.txt", "w");
    if (input == NULL) {
        printf("file can not open \n");
    }

    /*chunk*/
    for (i = 0; i < chunk_size; i++) {
        chunk[i] = rand() % 100;
        fprintf(input, "%d ", chunk[i]);
    }
    fprintf(input, "\n");

    /*line2*/
    for (i = 0; i < line_size; i++) {
        line2[i] = rand() % 100;
        fprintf(input, "%d ", line2[i]);
    }
    fprintf(input, "\n");

    /*line3*/
    for (i = 0; i < line_size; i++) {
        line3[i] = rand() % 100;
        fprintf(input, "%d ", line3[i]);
    }
    fprintf(input, "\n");

    /*line4*/
    for (i = 0; i < line_size; i++) {
        line4[i] = rand() % 100;
        fprintf(input, "%d ", line4[i]);
    }
    fprintf(input, "\n");

    fclose(input);
}

void print_output_file(FILE* output, int r, int g, int b, enum Color color) {
    if (color == RGB) {
        fprintf(output, "[%d,%d,%d]", r, g, b);
    } else if (color == BGR) {
        fprintf(output, "[%d,%d,%d]", b, g, r);
    }
}

void print_gray_lines(FILE* output, int* results2, int* results3, int* results4, int count) {
    int i;
    fprintf(output, "[");
    for (i = 0; i < count; i++) {
        fprintf(output, "%d", results2[i]);
        if (i != count - 1) fprintf(output, ",");
    }
    fprintf(output, "]\n");

    fprintf(output, "[");
    for (i = 0; i < count; i++) {
        fprintf(output, "%d", results3[i]);
        if (i != count - 1) fprintf(output, ",");
    }
    fprintf(output, "]\n");

    fprintf(output, "[");
    for (i = 0; i < count; i++) {
        fprintf(output, "%d", results4[i]);
        if (i != count - 1) fprintf(output, ",");
    }
    fprintf(output, "]");
}

int main() {
    int arr[] = {3, 6, 9};
    int arr2[] = {20, 50, 80};
    int color;
    int i;
    int chunk_size = arr[rand() % 3];
    int line_size = arr2[rand() % 3];
    int x;
    int* chunk = malloc(sizeof(int) * chunk_size);
    int* line2 = malloc(sizeof(int) * line_size);
    int* line3 = malloc(sizeof(int) * line_size);
    int* line4 = malloc(sizeof(int) * line_size);

    make_input_file(chunk, line2, line3, line4, chunk_size, line_size);

    printf("enter a color (0=RGB, 1=BGR, 2=GRAY): ");
    scanf("%d", &color);

    /* kullanıcıdan max/avg işlemlerini stack'e al */
    char input_op[4];

    printf("Enter operation # (max or avg): ");
    scanf("%s", input_op);
    if (strcmp(input_op, "max") == 0) {
        push('m');
    } else if (strcmp(input_op, "avg") == 0) {
        push('a');
    } else {
        printf("Invalid input.\n");
    }

    FILE* output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("file can not open \n");
        return 1;
    }


        /*x correspoing line oluşturmak için şart*/
            x = 0;
            /*output formatı*/
            switch(color){
            case RGB: fprintf(output,"RGB : ->"); break;
            case BGR: fprintf(output,"BGR : ->"); break;
            case GRAY: fprintf(output,"GRAY : ->"); break;
}
    if (color == GRAY) {
        int len = line_size - chunk_size + 1;
        int* results2 = malloc(sizeof(int) * len);
        int* results3 = malloc(sizeof(int) * len);
        int* results4 = malloc(sizeof(int) * len);

        for (i = 0; i < len; i++) {

                                                /*böylece başta ilk index sonra ilk indec artı x . indexi göndermiş oluyorum*/
            results2[i] = apply_operation(chunk, line2 + x, chunk_size);
            results3[i] = apply_operation(chunk, line3 + x, chunk_size);
            results4[i] = apply_operation(chunk, line4 + x, chunk_size);
            x++;
        }
        print_gray_lines(output, results2, results3, results4, len);
        free(results2);
        free(results3);
        free(results4);
    } 
    else {

        for (i = 0; i <= line_size - chunk_size; i++) {

            if (color == RGB) {
                int r = apply_operation(chunk, line2 + x, chunk_size);
                int g = apply_operation(chunk, line3 + x, chunk_size);
                int b = apply_operation(chunk, line4 + x, chunk_size);

                print_output_file(output, r, g, b, RGB);
                if (i != line_size - chunk_size) fprintf(output, ",");
            } else if (color == BGR) {
                
                int r = apply_operation(chunk, line4 + x, chunk_size);
                int g = apply_operation(chunk, line3 + x, chunk_size);
                int b = apply_operation(chunk, line2 + x, chunk_size);
                print_output_file(output, r, g, b, BGR);
                if (i != line_size - chunk_size) fprintf(output, ",");
            }
            x++;
        }
    }

    fclose(output);
    free(chunk);
    free(line2);
    free(line3);
    free(line4);
    return 0;
}