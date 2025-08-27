#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void dosya_temizleme()
{
    FILE *temp = fopen("ships.txt", "w");
    if (temp == NULL) {
        printf("File can not open!\n");
    } 
    else {
        fclose(temp);  
    }
    
    FILE *temp2 = fopen("battleship_log.txt", "w");
    if (temp2 == NULL) {
        printf("file can not open ! \n");
    } 
    else {
        fclose(temp2);
    }
}

void gemi_koyma(int arr[10][10], int gemi_boyutu, int etiket) 
{
    FILE *eyp = fopen("ships.txt", "a");
    if (eyp == NULL) {
        printf("file can not open !\n");
        return;
    }
    
    int eksen;
    int flag = 0;
    int i;

    while (!flag) {
        eksen = rand() % 2;
        
        if (eksen == 0) { 
            int x = rand() % 10;
            int y = rand() % (11 - gemi_boyutu);
            int okey = 1;
            
            for (i = 0; i < gemi_boyutu; i++) {
                if (arr[x][y + i] != 0) {
                    okey = 0;
                    break;
                }
            }
            
            if (okey) {
                for (i = 0; i < gemi_boyutu; i++) {
                    arr[x][y + i] = etiket;
                    fprintf(eyp, "%d %d %d\n", x, y + i, gemi_boyutu);
                }
                flag = 1;
            }
        }
        else { 
            int x = rand() % (11 - gemi_boyutu);
            int y = rand() % 10;
            int okey = 1;
            
            for (i = 0; i < gemi_boyutu; i++) {
                if (arr[x + i][y] != 0) {
                    okey = 0;
                    break;
                }
            }
            
            if (okey) {
                for (i = 0; i < gemi_boyutu; i++) {
                    arr[x + i][y] = etiket;
                    fprintf(eyp, "%d %d %d\n", x + i, y, gemi_boyutu);
                }
                flag = 1;
            }               
        }
    }
    fclose(eyp);
}

int kullanici_girdisi_al(int *x, int *y, char konumlar[10][10]) 
{
    char input[20];

    while (1) {
        printf("Enter coordinates(X to quit): ");
        fgets(input, sizeof(input), stdin);

        if ((input[0] == 'x' || input[0] == 'X') && (input[1] == '\n' || input[1] == '\0')) {
            return -1;
        }

        if (sscanf(input, "%d %d", x, y) != 2) {
            printf("Invalid entry! Please enter two numbers between 0 and 9 or press 'X' to exit \n");
            continue;
        }

        if (*x < 0 || *x >= 10 || *y < 0 || *y >= 10) {
            printf("The coordinates must be in the range 0-9 \n");
            continue;
        }

        if (konumlar[*x][*y] == 'X' || konumlar[*x][*y] == 'O') {
            printf("You've shot at this location before! Try another location \n");
            continue;
        }
        
        return 1;
    }
}

int main() 
{
    char play_again;

    do {
        char konumlar[10][10];
        int gemi_yerleri[10][10] = {0};
        int i, j, x, y, flag;
        int count2 = 2;
        int count3 = 3;
        int count4 = 4;
        int count6 = 3;
        int number_of_shoot = 0;
        flag = 0;
        
        srand(time(NULL));
        dosya_temizleme();
        
        gemi_koyma(gemi_yerleri, 2, 2);
        gemi_koyma(gemi_yerleri, 3, 3);
        gemi_koyma(gemi_yerleri, 3, 6);
        gemi_koyma(gemi_yerleri, 4, 4);

        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                konumlar[i][j] = '-';
            }
        }

        FILE *eyp = fopen("battleship_log.txt", "a");
        if (eyp == NULL) {
            printf("file can not open !\n");
            return 0;
        }
        
        while (flag < 12) {
            int sonuc = kullanici_girdisi_al(&x, &y, konumlar);
            if (sonuc == -1) {
                printf("Game terminated by user \n");
                fclose(eyp);
                return 0;
            }

            fprintf(eyp, "Shoot : %d %d ", x, y);

            if (gemi_yerleri[x][y] == 2) {
                printf("HIT!\n");
                fprintf(eyp, "-HIT!\n");
                konumlar[x][y] = 'X';
                count2--;
                if (count2 == 0) {
                    printf("Congratulations you sank a 2-cell ship\n");
                }
                flag++;
            } 
            else if (gemi_yerleri[x][y] == 3) {
                printf("HIT!\n");
                fprintf(eyp, "-HIT!\n");
                konumlar[x][y] = 'X';
                count3--;
                if (count3 == 0) {
                    printf("Congratulations you sank a 3-cell ship\n");
                }
                flag++;
            }
            else if (gemi_yerleri[x][y] == 6) {
                printf("HIT!\n");
                fprintf(eyp, "-HIT\n");
                konumlar[x][y] = 'X';
                count6--;
                if (count6 == 0) {
                    printf("Congratulations you sank a 3-cell ship\n");
                }
                flag++;
            }
            else if (gemi_yerleri[x][y] == 4) {
                printf("HIT!\n");
                fprintf(eyp, "-HIT!\n");
                konumlar[x][y] = 'X';
                count4--;
                if (count4 == 0) {
                    printf("Congratulations you sank a 4-cell ship\n");
                }
                flag++;
            }
            else {
                printf("MISS\n");
                konumlar[x][y] = 'O';
                fprintf(eyp, "-MISS\n");
            }

            for (i = 0; i < 10; i++) {
                for (j = 0; j < 10; j++) {
                    printf("%c ", konumlar[i][j]);
                }
                printf("\n");
            }
            number_of_shoot++;
        }
        fclose(eyp);

        printf("All ships are sank! Total shoots:%d \n", number_of_shoot);
        printf("Press 'N' to play again or 'X' to exit : \n");
        
        char input[10];
        fgets(input, sizeof(input), stdin);
        play_again = input[0];
        
    } while (play_again == 'n' || play_again == 'N');

    return 0;
}