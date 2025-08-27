#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct {
    int total_damage_dealt;
    int mana_spent;
    int number_of_recoveries;

}battle_statu;

typedef struct {

    char name [20];
    char magic_class[20];
    int min_damage;
    int max_damage;
    int min_mana_cost;
    int max_mana_cost;

}spells;

typedef struct {

    char name [20];
    char magic_class[20];
    int HP;
    int mana ;
    int is_alive;
    battle_statu battle_stat;
    spells spell[3];

}wizard;

void declare_spell(spells *s)
{
    FILE*spell_book=fopen("spellbook.txt","r");
    if(spell_book==NULL)
    {
        printf("spellbook can not open !\n");
        exit(1);
    }

    int i=0;

    while(fscanf(spell_book,"%[^,],%[^,],%d,%d,%d,%d\n",
            s[i].name,
            s[i].magic_class
            ,&s[i].min_damage
            ,&s[i].max_damage
            ,&s[i].min_mana_cost
            ,&s[i].max_mana_cost)==6){i++;
    }
    fclose(spell_book);
    
}

void unique_numbers_within_rage(int range ,int *arr,int size)
{
    
    int is_ok=1,i;
    int counter=0;
    int num ;
    while(counter<size)
    {

         num=(rand()%range) ;
         is_ok=1;

        for(i=0;i<counter;i++)
        {
            if(arr[i]==num)
            {
                is_ok=0;
            }
        }

        if(is_ok)
        {
            arr[counter]=num;
            counter++;
        }

    }

}

void duel(wizard *attacker,wizard *defender)
{
    int max_or_min_damage=rand()%2;
    char damage_name[20];
    int damage;
    spells currnet_spell ;

    if(attacker->mana<=0)
    {
        attacker->mana+=rand()%(20-10)+1;
        attacker->battle_stat.number_of_recoveries++;
        printf("%s is low on mana and meditates...\n",attacker->name);
    }


    if(max_or_min_damage==1)
    {
        currnet_spell =attacker->spell[rand()%3];
        damage = currnet_spell.max_damage;
        strcpy(damage_name,currnet_spell.name);
        attacker->mana-=currnet_spell.max_mana_cost;
        /*battle statu*/
        attacker->battle_stat.mana_spent+=currnet_spell.max_mana_cost;
    }
    else{
        currnet_spell =attacker->spell[rand()%3];
        damage = currnet_spell.min_damage;
        strcpy(damage_name,currnet_spell.name);
        attacker->mana-=currnet_spell.min_mana_cost;
        /*battle statu*/
        attacker->battle_stat.mana_spent+=currnet_spell.min_mana_cost;

    }
    if(strcmp(attacker->magic_class,currnet_spell.magic_class)==0){
        damage+=5;
    }


    printf("%s cast %s on %s \n",attacker->name,damage_name,defender->name);
    defender->HP-=damage; 
    /*battele statu*/
    defender->battle_stat.total_damage_dealt+=damage;
    printf("damage : %d| %s's HP: %d | %s's MANA %d \n",damage, defender->name, defender->HP, attacker->name,attacker->mana);
    printf("-----------------------------------------------------------------\n");

    if(defender->HP<0)
    {
        printf("winner ! %s the %s \n",attacker->name,attacker->magic_class);
        return;
    }


    duel(defender,attacker);
}


int calculate_score(int stat[],int n)
{
    if(n==0){
        return stat[0]*2;
    }
   else if (n==1)
   {
        return stat[1]+calculate_score(stat,0);
   }
   else {
    return calculate_score(stat,1)-stat[2];

   }
}





int  main (void)
{
    srand(time(NULL));
    wizard wizards[4];

    int i;
    int select_spell[3];
    int attacker_or_defender[2];
    int w=0;
    int statu [3];
    int score ;

    /*yetenekler tanımlndı */
    int number_of_spell=6;
    spells *created_spell=malloc(number_of_spell*sizeof(spells));
    declare_spell(created_spell);


    strcpy(wizards[0].name,"Naruto");
    strcpy(wizards[0].magic_class,"ice");

    strcpy(wizards[1].name,"Madara");
    strcpy(wizards[1].magic_class,"fire");

    strcpy(wizards[2].name,"Kakashi");
    strcpy(wizards[2].magic_class,"water");

    strcpy(wizards[3].name,"Sasuke");
    strcpy(wizards[3].magic_class,"air");


        /*büyücülerin ortak özelliklerini doldurma */
        for(w=0;w<4;w++){

            wizards[w].HP=100;
            wizards[w].is_alive=1;
            wizards[w].mana=100;
            wizards[w].battle_stat.mana_spent=0;
            wizards[w].battle_stat.number_of_recoveries=0;
            wizards[w].battle_stat.total_damage_dealt=0;  

            unique_numbers_within_rage(number_of_spell,select_spell,3);
            for(i=0;i<3;i++)
            {
                strcpy(wizards[w].spell[i].name,created_spell[select_spell[i]].name);
                strcpy(wizards[w].spell[i].magic_class,created_spell[select_spell[i]].magic_class);
                wizards[w].spell[i].min_damage=created_spell[select_spell[i]].min_damage;
                wizards[w].spell[i].max_damage=created_spell[select_spell[i]].max_damage;
                wizards[w].spell[i].min_mana_cost=created_spell[select_spell[i]].min_mana_cost;
                wizards[w].spell[i].max_mana_cost=created_spell[select_spell[i]].max_mana_cost;

            }
}

    
    
    unique_numbers_within_rage(4,attacker_or_defender,2);
    printf("Wizard duel begins: %s vs  %s !\n",wizards[attacker_or_defender[0]].name,wizards[attacker_or_defender[1]].name);
    duel(&wizards[attacker_or_defender[0]],&wizards[attacker_or_defender[1]]);

        statu[0]=wizards[attacker_or_defender[0]].battle_stat.total_damage_dealt;
        statu[1]=wizards[attacker_or_defender[0]].battle_stat.mana_spent;
        statu[2]=wizards[attacker_or_defender[0]].battle_stat.number_of_recoveries;
        score=calculate_score(statu,3);
    

    printf("Battle summary: \n");
    printf("%s ---DAMAGE :%d| MANA SPENT : %d | recoveries %d |score :%d  \n",wizards[attacker_or_defender[0]].name,
                                                                    wizards[attacker_or_defender[1]].battle_stat.total_damage_dealt
                                                                    ,wizards[attacker_or_defender[0]].battle_stat.mana_spent
                                                                    ,wizards[attacker_or_defender[0]].battle_stat.number_of_recoveries
                                                                    ,score);
        statu[0]=wizards[attacker_or_defender[1]].battle_stat.total_damage_dealt;
        statu[1]=wizards[attacker_or_defender[1]].battle_stat.mana_spent;
        statu[2]=wizards[attacker_or_defender[1]].battle_stat.number_of_recoveries;
        score=calculate_score(statu,3);

    printf("%s ---DAMAGE :%d| MANA SPENT : %d | recoveries %d |score : %d \n",wizards[attacker_or_defender[1]].name,
                                                                    wizards[attacker_or_defender[0]].battle_stat.total_damage_dealt
                                                                    ,wizards[attacker_or_defender[1]].battle_stat.mana_spent
                                                                    ,wizards[attacker_or_defender[1]].battle_stat.number_of_recoveries
                                                                    ,score);

    free(created_spell);
}