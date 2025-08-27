#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[20];
    int quantity;
    float price;
} item;

void add_item(item **card, int *index, int *count);
void remove_item(item **card, int *index, int *count);
void search_item(item *card, int count);
void print_card(item *card, int count);

int main()
{
    int count;
    int index = 0;
    int choose;
    int exit = 0;

    printf("Please initial cart capacity: ");
    scanf("%d", &count);
    /*kart oluştuerm */
    item *card = malloc(count * sizeof(item));


    while (!exit)
    {
        printf("1. Add item\n2. Remove item\n3. Search item\n4. Print card\n5. Exit\nChoice: ");
        scanf("%d", &choose);

        switch (choose)
        {
        case 1:
            add_item(&card, &index, &count);
            break;
        case 2:
            remove_item(&card, &index, &count);
            break;
        case 3:
            search_item(card, index);
            break;
        case 4:
            print_card(card, index);
            break;
        case 5:
            printf("freeing memory... Good bye\n");
            exit = 1;
            break;
        default:
            printf("invalid value\n");
        }
    }
    /*free işelmi */
    free(card);
    return 0;
}

void add_item(item **card, int *index, int *count)
{
    if (*index == *count)
    {
        *count *= 2;
        *card = realloc(*card, (*count) * sizeof(item));
        if (*card == NULL)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    printf("Please enter item name: ");
    scanf("%s", (*card)[*index].name);
    /*negatif değerler girmesin*/
    do
    {
    printf("Please enter the price: ");
    scanf("%f", &(*card)[*index].price);

    } 
    while((*card)[*index].price<=0);
     /*negatif değerler girmesin*/
    do{

    printf("Please enter the quantity: ");
    scanf("%d", &(*card)[*index].quantity);

    }while((*card)[*index].quantity<=0);

    printf("item added.\n");
    (*index)++;
}

void remove_item(item **card, int *index, int *count)
{
    char name[20];
    int flag = -1;
    
    if (*index == *count / 2 && *count > 1)
    {
        *count /= 2;
        *card = realloc(*card, (*count) * sizeof(item));
        if (*card == NULL)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    printf("please enter a name to remove: ");
    scanf("%s", name);

    for (int i = 0; i < *index; i++)
    {
        if (strcmp((*card)[i].name, name) == 0)
        {
            flag = i;
            break;
        }
    }

    if (flag == -1)
    {
        printf("item not found\n");
        return;
    }

    for (int i = flag; i < *index - 1; i++)
    {
        (*card)[i] = (*card)[i + 1];
    }
    (*index)--;
    printf("item removed\n");
}

void search_item(item *card, int count)
{
    char name[20];
    int found = 0;
    printf("Please enter a name for search: ");
    scanf("%s", name);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(card[i].name, name) == 0)
        {
            printf("item name: %s\n", card[i].name);
            printf("item price: %0.2f\n", card[i].price);
            printf("item quantity: %d\n", card[i].quantity);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("item is not found\n");
    }

}

void print_card(item *card, int count)

{
    float sum = 0;
    printf("--------------------------------------\n"
           "%-8s %-8s %-8s %-8s\n"
           "--------------------------------------\n", "Name", "Price", "Quantity", "Subtotal");
    for (int i = 0; i < count; i++)
    {
        printf("%-8s %f %-8d %f\n", card[i].name, card[i].price, card[i].quantity, card[i].price * card[i].quantity);
        sum += card[i].price * card[i].quantity;
    }
    printf("---------------------------------------\nTotal card value: %0.2f\n", sum);
}


/*kullanıcı önce boyut girmeli */
/*dah sonra işlem menüsü çıkacak */
/* işlem işlem ilerle her işlem için bir fonk yaz */
/*ekleme fonk count kadar çalışacak*/
/*çıkarmada negatife düşmemeli*/