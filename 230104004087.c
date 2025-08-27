#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int row;
    int column;
    int **data;
} matrix;

typedef struct {
    int row;
    int column;
    char **data;
} table;

typedef enum {
    HUMAN =-1,
    EMPTY = 0,
    COMPUTER=1,
    POSSIBLE_MOVE=2
}Cell_state;

typedef enum {
    char_human ='O',
    char_copm='X'
}player;


char *letters(int size); /*tabloyu yazdırma ksımındaki harfler için var size kadar harf kouyor a b c d ..*/

matrix create_matrix(int row, int column); /*tahtanın sayılsal ifadesini bulundurma*/

void free_matrix(matrix *m); /*sonda oluşturduğum hafızayı boşa alma*/

table create_table (int row, int column, matrix *m); /*tabloyu yazdırma ksımı*/

void free_table(table *t) ; /*sonda oluşturduğum hafızayı boşa alma*/

void print_table(table *t, int row, int column, char letter[]); /*her seferinde terminale tabloyu bastırma*/

int get_coordinate(int *col_out, int *row_out, int max_row, int max_col, matrix *m);  /*kullanıcıdan doğru koordinat girişi */

int game_state(table *t, matrix *m);  /*oyun bitti mi devam mı ona karar veriyor*/

void mark_neighbors(matrix *m); /*matrix de ki oyuncu veya bilgisayr değriene göre komşu yerlere possible _move bırakma*/

void  change_table(table *t, matrix *m, int player, char p); /*her hamleden sonra sağdan sola asağıdan yukarı çapraz hameleye göre tahtayı yeniden dizayn etme*/

int test_flip_count(matrix m, int i, int j, int player);/*verilen koordinat seçilirse karşı oyuncunun kaç taşını yok edecek*/

void find_best_move(matrix m, int player, int *best_i, int *best_j,char *letter); /*maximum sayıda tahtada bulunmka için en iyi hamle*/

void computer_move(matrix *m, table *t,char *letter);  /*bilgisayarın hamleleri yazdırma vs */

int main(void) {

    /*ilk tabloları oluştur*/
    table my_tab;
    matrix my_matrix;
    int row, column;
    int col, row_idx;

    /*tahtanın boyutunu alma*/

do {
        printf("Enter size of table: ");

        scanf("%d", &row);

        while (getchar() != '\n');

        if (row % 2 != 0) printf("Size cannot be an odd number!\n");

        if(row<4)printf("size must be at least 4!\n");

    } while (row % 2 != 0 || row<3);

    column = row;

    my_matrix = create_matrix(row, column);

    my_tab = create_table(row, column, &my_matrix);

    mark_neighbors(&my_matrix);

    char *letter = letters(column);



    /*oyun kısmı burada*/
    print_table(&my_tab, row, column, letter);

    while (game_state(&my_tab, &my_matrix) != 1) {

        if (get_coordinate(&col, &row_idx, row, column, &my_matrix))
        {

            my_tab.data[row_idx][col] = char_human;

            my_matrix.data[row_idx][col] = HUMAN;

            change_table(&my_tab, &my_matrix, HUMAN, char_human);

            mark_neighbors(&my_matrix);

            print_table(&my_tab, row, column, letter);
            
            computer_move(&my_matrix, &my_tab,letter);

            print_table(&my_tab, row, column, letter);
        }

    }


    free_matrix(&my_matrix);
    free_table(&my_tab);
    free(letter);
    return 0;
}
char *letters(int size) {
    int i;
    char *lettr = malloc(sizeof(char) * (size + 1));  
    if (lettr == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (i = 0; i < size; i++) {
        lettr[i] = 'a' + i;}

    lettr[size] = '\0';
    return lettr;
}

matrix create_matrix(int row, int column) {
    int i, j;
    matrix m;
    m.row = row;
    m.column = column;
    m.data = (int **)malloc(sizeof(int *) * row);
    if (m.data == NULL) {
        printf("Malloc failed for matrix rows!\n");
        exit(1);}

    for (i = 0; i < row; i++) {
        m.data[i] = (int *)malloc(sizeof(int) * column);
        if (m.data[i] == NULL) {
            printf("Malloc failed for matrix row!\n");
            exit(1);
        }
        for (j = 0; j < column; j++) {
            m.data[i][j] = EMPTY;
        }
    }
    return m;
}

void free_matrix(matrix *m) {
    int i;
    for (i = 0; i < m->row; i++){
        free(m->data[i]);}
    free(m->data);
}

table create_table(int row, int column, matrix *m) {
    int i, j;
    table t;
    t.row = row;
    t.column = column;

    t.data = (char **)malloc(sizeof(char *) * row);
    if (t.data == NULL) {
        printf("Malloc failed for table rows!\n");
        exit(1);
    }

    for (i = 0; i < row; i++) {
        t.data[i] = (char *)malloc(sizeof(char) * column);
        if (t.data[i] == NULL) {
            printf("Malloc failed for table row!\n");
            exit(1);
        }
        for (j = 0; j < column; j++) {
            t.data[i][j] = '.';  
        }
    }

        t.data[row/2-1][column/2-1] = char_human;
        t.data[row/2][column/2-1] = char_copm;
        t.data[row/2][column/2] = char_human;
        t.data[row/2-1][column/2] = char_copm;

        m->data[row/2-1][column/2-1] = HUMAN;
        m->data[row/2][column/2-1] = COMPUTER;
        m->data[row/2][column/2] = HUMAN;
        m->data[row/2-1][column/2] = COMPUTER;

    return t;
}

void free_table(table *t) {
    int i;
    for (i = 0; i < t->row; i++){
        free(t->data[i]);}
    free(t->data);
}

void print_table(table *t, int row, int column, char letter[]) {
    int i, j;
    printf("  ");  
    for (i = 0; i < column; i++) 
        printf(" %c", letter[i]);
    printf("\n");

    for (i = 0; i < row; i++) {
        printf("%2d ", i + 1);  
        for (j = 0; j < column; j++) {
            printf("%c ", t->data[i][j]);
        }
        printf("\n");
    }
}

int get_coordinate(int *col_out, int *row_out, int max_row, int max_col, matrix *m) {
    char input[10];
    char col_char;
    int row_num;
    int return_val;
    printf("Enter coordinate (e.g., a4): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error!\n");
        return_val = 0;
    }

    input[strcspn(input, "\n")] = '\0';  

    if (sscanf(input, " %c%d", &col_char, &row_num) != 2) {
        printf("Invalid format! Please enter like A4.\n");
        return 0;
    }

    if (col_char >= 'A' && col_char <= 'Z') {
        col_char = col_char + ('a' - 'A');
    }

    if (col_char < 'a' || col_char >= 'a' + max_col) {
        printf("Column out of bounds! Allowed: a-%c\n", 'a' + max_col - 1);
        return 0;
    }

    if (row_num < 1 || row_num > max_row) {
        printf("Row out of bounds! Allowed: 1-%d\n", max_row);
        return 0;
    }

    if (m->data[row_num-1][col_char - 'a'] == HUMAN || m->data[row_num-1][col_char - 'a'] == COMPUTER) {
        printf("Enter different coordinate!\n");
        return 0;
    }

    if (m->data[row_num-1][col_char - 'a'] == POSSIBLE_MOVE) {
        *col_out = col_char - 'a';   
        *row_out = row_num - 1;
        return_val = 1;
    }
    return return_val;
}

int game_state(table *t, matrix *m) {
    int i, j;
    int game_over = 1;
    int user_count = 0;
    int comp_count = 0;

    for (i = 0; i < m->row; i++) {
        for (j = 0; j < m->column; j++) {
            if (m->data[i][j] == POSSIBLE_MOVE) {
                game_over = 0;
                break;
            }
            if (t->data[i][j] == char_human) user_count++;
            if (t->data[i][j] == char_copm) comp_count++;
        }
    }
    if (game_over == 1) {
        if (user_count > comp_count) {
            printf("You win!\n");
        } else {
            printf("You lose!\n");
        }
    }
    return game_over;
}

void mark_neighbors(matrix *m) {
    int i, j;

    for (i = 0; i < m->row; i++) {
        for (j = 0; j < m->column; j++) {
            if (m->data[i][j] == HUMAN || m->data[i][j] == COMPUTER ) {
                if (i < m->row - 1 && m->data[i + 1][j] == EMPTY) m->data[i + 1][j] = POSSIBLE_MOVE;

                if (i > 0 && m->data[i - 1][j] == EMPTY) m->data[i - 1][j] = POSSIBLE_MOVE;

                if (j < m->column - 1 && m->data[i][j + 1] == EMPTY) m->data[i][j + 1] = POSSIBLE_MOVE;

                if (j > 0 && m->data[i][j - 1] ==EMPTY) m->data[i][j - 1] = POSSIBLE_MOVE;

                if (i < m->row - 1 && j < m->column - 1 && m->data[i + 1][j + 1] == EMPTY) m->data[i + 1][j + 1] = POSSIBLE_MOVE;

                if (i > 0 && j < m->column - 1 && m->data[i - 1][j + 1] == EMPTY) m->data[i - 1][j + 1] = POSSIBLE_MOVE;

                if (i > 0 && j > 0 && m->data[i - 1][j - 1] == EMPTY) m->data[i - 1][j - 1] = POSSIBLE_MOVE;

                if (i < m->row - 1 && j > 0 && m->data[i + 1][j - 1] == EMPTY) m->data[i + 1][j - 1] = POSSIBLE_MOVE;
            }
        }
    }
}

void  change_table(table *t, matrix *m, int player, char p) {
    int i,j,f,d,dx,dy,x,y,flipped;
    int directions[8][2] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

    for (i = 0; i < m->row; i++) {
        for (j = 0; j < m->column; j++) {
            if (m->data[i][j] == player) {
                for (d = 0; d < 8; d++) {
                    dx = directions[d][0];
                    dy = directions[d][1];
                    x = i + dx;
                    y = j + dy;
                    flipped = 0;

                    while (x >= 0 && x < m->row && y >= 0 && y < m->column && m->data[x][y] == -player) {
                        flipped++;
                        x += dx;
                        y += dy;
                    }

                    if (flipped > 0 && x >= 0 && x < m->row && y >= 0 && y < m->column && m->data[x][y] == player) {
                        int back_x = i + dx;
                        int back_y = j + dy;
                        for (f = 0; f < flipped; f++) {
                            m->data[back_x][back_y] = player;
                            t->data[back_x][back_y] = p;
                            back_x += dx;
                            back_y += dy;

                        }
                    }
                }
            }
        }
    }


}

int test_flip_count(matrix m, int i, int j, int player) {
    int count = 0,d,dx,dy,x,y,flipped;
    int directions[8][2] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

    for (d = 0; d < 8; d++) {
        dx = directions[d][0];
        dy = directions[d][1];

        x = i + dx;
        y = j + dy;
        flipped = 0;

        while (x >= 0 && x < m.row && y >= 0 && y < m.column && m.data[x][y] == -player) {
            flipped++;
            x += dx;
            y += dy;
        }

        if (flipped > 0 && x >= 0 && x < m.row && y >= 0 && y < m.column && m.data[x][y] == player) {
            count += flipped;
        }
    }

    return count;
}

void find_best_move(matrix m, int player, int *best_i, int *best_j,char *letter) {
    int max_flips = 0, i, j;
    int fallback_i = -1, fallback_j = -1;  /*Eğer hiç döndürme yoksa fallback hamleye dön*/ 

    for (i = 0; i < m.row; i++) {
        for (j = 0; j < m.column; j++) {
            if (m.data[i][j] == POSSIBLE_MOVE) {
                int flips = test_flip_count(m, i, j, player);

                if (flips > max_flips) {
                    max_flips = flips;
                    *best_i = i;
                    *best_j = j;
                }

                
                if (fallback_i == -1 && flips == 0) {
                    fallback_i = i;
                    fallback_j = j;
                }
            }
        }
    }

    
    if (max_flips == 0 && fallback_i != -1) {
        *best_i = fallback_i;
        *best_j = fallback_j;
    }
    printf("Computer chose move at: (%c, %d)\n", letter[*best_i+1], *best_j);

}

void computer_move(matrix *m, table *t,char *letter) {
    int x=1,y=1;

    find_best_move(*m,COMPUTER,&x,&y,letter) ;

    m->data[x][y] = COMPUTER;

    t->data[x][y]=char_copm;

    change_table(t, m, COMPUTER, char_copm);

    mark_neighbors(m);
}
