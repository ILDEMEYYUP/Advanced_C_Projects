#include <stdio.h>
#include <math.h>

void menu() 
{
    printf("\nWelcome to the rocket launch simulator!\n");
    printf("---------------------------------------\n");
    printf("1. Enter launch parameters\n");
    printf("2. Simulate and graph the rocket's trajectory\n");
    printf("3. Display and save the trajectory data\n");
    printf("4. Exit the program\n");
}

void parameter() 
{
    double initial_velocity, initial_height, gravity = 9.8;
    FILE *eyp = fopen("rocket_data.txt", "w");
    if (eyp == NULL) 
    {
        printf("File cannot be opened.\n");
        return;
    }

    do {
        printf("Enter initial velocity: ");
        scanf("%lf", &initial_velocity);
        if (initial_velocity < 0)
            printf("Velocity cannot be negative! Try again.\n");
    } while (initial_velocity < 0);

    do {
        printf("Enter gravity (default 9.8 m/s²): ");
        scanf("%lf", &gravity);
        if (gravity <= 0)
            printf("Gravity must be greater than 0! Try again.\n");
    } while (gravity <= 0);

    do {
        printf("Enter launch height: ");
        scanf("%lf", &initial_height);
        if (initial_height <= 0)
            printf("Height must be greater than 0! Try again.\n");
    } while (initial_height <= 0);

    fprintf(eyp, "%.2lf %.2lf %.2lf\n", initial_height, initial_velocity, gravity);
    fclose(eyp);
}

double calculate_max_h() 
{
    double initial_height, initial_velocity, gravity, max_h;
    FILE *data = fopen("rocket_data.txt", "r");
    if (data == NULL) {
        printf("Error: rocket_data.txt not found! Please enter parameters first.\n");
        return -1;
    }
    fscanf(data, "%lf %lf %lf", &initial_height, &initial_velocity, &gravity);
    fclose(data);
    max_h = (initial_velocity * initial_velocity) / (2 * gravity) + initial_height;
    return max_h;
}

double calculate_max_t() 
{
    double initial_height, initial_velocity, gravity, t_flight;
    FILE *data = fopen("rocket_data.txt", "r");
    if (data == NULL) {
        printf("Error: rocket_data.txt not found! Please enter parameters first.\n");
        return -1;
    }
    fscanf(data, "%lf %lf %lf", &initial_height, &initial_velocity, &gravity);
    fclose(data);
    t_flight = (initial_velocity + sqrt(initial_velocity * initial_velocity + 2 * gravity * initial_height)) / gravity;
    return t_flight;
}

void graph_and_save() 
{
    double gravity, initial_velocity, initial_height, h_max, t_max;
    double x_olcek;
    int y_olcek, max_lines;
    
    FILE *data, *graph_file;
    int line, i, j;
    double current_height, t, h;

    data = fopen("rocket_data.txt", "r");
    if (data == NULL) {
        printf("Error: rocket_data.txt not found! Please enter parameters first.\n");
        return;
    }
    fscanf(data, "%lf %lf %lf", &initial_height, &initial_velocity, &gravity);
    fclose(data);
    printf("Equation: h(t)= -%0.1lf * t * t + %0.1lf * t + %0.1lf \n", gravity/2, initial_velocity, initial_height);
    printf("simulating trajectory ...\n");

    h_max = calculate_max_h();
    t_max = calculate_max_t();
    if (h_max < 0 || t_max < 0)
        return;

    /* y ölçek */
    if (h_max <= 100)
        y_olcek = 10;
    else if (h_max <= 500)
        y_olcek = 20;
    else
        y_olcek = 50;

    max_lines = (int)(h_max / y_olcek) + 2;
    if (max_lines > 15) 
    {
        y_olcek *= 2;
        max_lines = (int)(h_max / y_olcek) + 2;
    }

    x_olcek = 40.0 / t_max;

    graph_file = fopen("trajectory.txt", "w");
    if (graph_file == NULL) 
    {
        printf("Error: Could not open trajectory.txt for writing!\n");
        return;
    }

    printf("\n\n");
    fprintf(graph_file, "\n\n");

    /* y ekseni */
    printf("      ^\n");
    fprintf(graph_file, "      ^\n");

    for (line = max_lines; line >= 0; line--) 
    {
        current_height = line * y_olcek;
        printf("%5.1lf |", current_height);
        fprintf(graph_file, "%5.1lf |", current_height);
        for (t = 0.0; t <= t_max; t += t_max/40.0) 
        {
            h = initial_velocity * t - 0.5 * gravity * t * t + initial_height;
            if (h >= current_height - y_olcek/2.0 && h <= current_height + y_olcek/2.0) 
            {
                int space = (int)(t * x_olcek);
                for (i = 0; i < space; i++) 
                {
                    printf(" ");
                    fprintf(graph_file, " ");
                }
                printf("#");
                fprintf(graph_file, "#");
                break;
            }
        }
        printf("\n");
        fprintf(graph_file, "\n");
    }

    /* X ekseni */
    printf("      ");
    fprintf(graph_file, "      ");
    for (i = 0; i < 10; i++) 
    {
        printf("|---");
        fprintf(graph_file, "|---");
    }

    printf(">\n");
    fprintf(graph_file, ">\n");

    printf("      ");
    fprintf(graph_file, "      ");

    for (t = 0.0; t <= t_max; t += t_max/10.0) 
    {
        printf("%-4.1lf", t);
        fprintf(graph_file, "%-4.1lf", t);
    }
    printf("\n");
    fprintf(graph_file, "\n");

    fprintf(graph_file, "\nMax altitude: %.2lf m\n", h_max);
    fprintf(graph_file, "Total flight duration: %.2lf s\n", t_max);

    fclose(graph_file);
}

void save_trajectory_data() 
{
    double h_max, t_max;
    FILE *eyp;
    h_max = calculate_max_h();
    t_max = calculate_max_t();
    if (h_max < 0 || t_max < 0)
        return;
    
    eyp = fopen("trajectory.txt", "a");
    if (eyp != NULL) 
    {
        fprintf(eyp, "\nMax altitude: %.2lf m\n", h_max);
        fprintf(eyp, "Total flight duration: %.2lf s\n", t_max);
        fclose(eyp);
        printf("saving trajectory data... \n");
        printf("\nMax altitude: %.2lf meters\n", h_max);
        printf("Total flight duration: %.2lf seconds\n", t_max);
        printf("Graph saved to trajectory.txt\n");
    } else {
        printf("Error: Could not save trajectory data!\n");
    }
}

int main() 
{
    int choice;
    do {
        menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                parameter();
                break;
            case 2:
                printf("reading rocket parameters from file ...\n");
                graph_and_save();
                break;
            case 3:
                save_trajectory_data();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice >= 1 && choice <= 4);
    return 0;
}
