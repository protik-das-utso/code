#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 255
#define MAX_VALUES 1000

typedef struct Node
{
    char name[100];
    char item[100][100];
    char ans[100][100];
    struct Node* next;
} node;

typedef struct Namevalue
{
    char name[MAX_NAME_LEN];
    int value;
} nameValue;


node* head;
char randques[100];
char randitem[100];
char question[100][100];
char items[100][100];
char quesdone[100][100];
int asked;
int gameover;

void makestruct();
void addchar();
void randquestion(node* current);
void aboutus();
void mainmenu();
void deletenode(int n);
void printnode();
void addchar2(char *name);
void modifyFile(char *name);
void getTop5Values();

int compare(const void *a, const void *b)
{
    return ((nameValue *)b)->value - ((nameValue *)a)->value;
}

int main()
{
    char a;
mainmenu:
    system("cls");
    mainmenu();
    printf("\t\tPlease give an input \n\t\t->");
    scanf(" %c", &a);

    if (a == 's' || a == 'S')
    {
        asked = 0;
        gameover = 1;
        makestruct();
        node* current = head;
        while (gameover)
        {
            current = head;
            if (current->next == NULL)
            {
                gameover--;
                system("cls");
                printf("\n\n\t\tYour character is %s.\n", current->name);
                modifyFile(current->name);
                printf("\n\n\n\n\n\n\t\tPress any key to continue:\n\t\t->");
                scanf(" %c", &a);
                break;
            }
            randquestion(current);
            system("cls");
            printf("\n\n\n\n\t\tIs your character %s?\n\t\t->", randques);
            scanf(" %c", &a);

            if (a == 'y' || a == 'Y')
            {
                for(int i=0; i<100; i++)
                    deletenode(1);
            }
            else
            {
                for(int i=0; i<100; i++)
                    deletenode(0);
            }
        }
        current = head;
        while (current != NULL)
        {
            node* temp = current;
            current = current->next;
            free(temp);
        }
        goto mainmenu;
    }
    else if (a == 'a' || a == 'A')
    {
        int n;
        system("cls");
        printf("\t\tHow many characters do you want to add?\n\t\t\t->");
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
        {
            addchar();
        }
        printf("\t\tAll character has been added.\n");
        printf("\t\tPress any key to Go back\n\t\t->");
        scanf(" %c", &a);
        goto mainmenu;
    }
    else if (a == 'v' || a == 'V')
    {
        aboutus();
        printf("\t\tPress any key to Go back\n\t\t->");
        scanf(" %c", &a);
        goto mainmenu;
    }
    else if (a == 'h' || a == 'H')
    {
        system("cls");
        getTop5Values();
        printf("\t\tPress any key to Go back\n\t\t->");
        scanf(" %c", &a);
        goto mainmenu;
    }
    else if (a == 'q' || a == 'Q')
    {
        return 0;
    }
    return 0;
}

void printnode()
{
    node* current = head;
    while (current->next != NULL)
    {
        printf("%s\n", current->name);
        current = current->next;
    }
    printf("%s\n", current->name);
}

void deletenode(int n)
{
    node* current = head;
    node* prev = NULL;
    int found = 0,i=0;
    while (current != NULL)
    {
        for (i = 0; strcmp(current->item[i], "stop") != 0; i++)
        {
            if (strcmp(current->item[i], randitem) == 0)
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            if ((strcmp(current->ans[i], randques) == 0 && n == 0) ||
                    (strcmp(current->ans[i], randques) != 0 && n == 1))
            {
                if (prev == NULL)
                    head = current->next;
                else
                    prev->next = current->next;
                free(current);
                return;
            }
        }
        prev = current;
        current = current->next;
        i=0;
        found=0;
    }
}
void mainmenu()
{
    printf("\t\t      CHARACTER QUEST: GUESS WHO\n");
    printf("\n\t\t________________________________________");
    printf("\n\t\t\t        WELCOME ");
    printf("\n\t\t\t          to ");
    printf("\n\t\t\t        THE GAME ");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________\n");
    printf("\n\t\t*********THE ARDENT FLAMES**********");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________");
    printf("\n\t\t > Press S to start the game");
    printf("\n\t\t > Press H to Highest guessed character");
    printf("\n\t\t > Press A to Add character");
    printf("\n\t\t > press V to view about us            ");
    printf("\n\t\t > press Q to quit             ");
    printf("\n\t\t________________________________________\n\n");
}

void addchar2(char *name)
{
    FILE *file = fopen("data.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    int value=0;
    fprintf(file, "%s %d\n", name, value);
    fclose(file);
}

void modifyFile(char *name)
{
    FILE *file = fopen("data.txt", "r+");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char buffer[255];
    int val;
    long pos;
    while (!feof(file))
    {
        pos = ftell(file);
        if (fscanf(file, "%s %d\n", buffer, &val) != 2) break;
        if (strcmp(buffer, name) == 0)
        {
            val++;
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s %d\n", name, val);
            break;
        }
    }
    fclose(file);
}

void randquestion(node* current)
{
    int count = 0;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    srand(time(NULL));
    int random_index = rand() % count;

    current = head;
    for (int i = 0; i < random_index; i++)
        current = current->next;

    int j = 0;
    while (strcmp(current->item[j], "stop") != 0)
    {
        strcpy(question[j], current->ans[j]);
        strcpy(items[j], current->item[j]);
        j++;
    }

    if (!asked)
    {
        srand(time(NULL));
        int rand_index = rand() % j;

        strcpy(randitem, items[rand_index]);
        strcpy(randques, question[rand_index]);
        strcpy(quesdone[asked++], question[rand_index]);
    }
    else
    {
        for (int i = 0; i < asked; i++)
        {
            for (int k = 0; k < j; k++)
            {
                if (strcmp(quesdone[i], question[k]) == 0)
                {
                    for (int l = k; l < j; l++)
                    {
                        strcpy(question[l], question[l + 1]);
                        strcpy(items[l], items[l + 1]);
                    }
                    strcpy(question[j - 1], "");
                    strcpy(items[j - 1], "");
                    j--;
                    break;
                }
            }
        }
        int rand_index = rand() % j;
        strcpy(randitem, items[rand_index]);
        strcpy(randques, question[rand_index]);
        strcpy(quesdone[asked++], question[rand_index]);
    }
}

void makestruct()
{
    head = NULL;
    node* temp = NULL;
    FILE* flname = fopen("filename.txt", "r");
    FILE* flans = fopen("fileans.txt", "r");
    FILE* fl = fopen("file.txt", "r");
    char temp_name[100];

    while (fscanf(flname, " %s", temp_name) == 1)
    {
        node* new_node = (node*)malloc(sizeof(node));
        strcpy(new_node->name, temp_name);
        int i = 0;
        while (1)
        {
            fscanf(fl, " %s", new_node->item[i]);
            fscanf(flans, " %s", new_node->ans[i]);
            if (strcmp(new_node->item[i], "stop") == 0)
                break;
            i++;
        }
        new_node->next = NULL;
        if (head == NULL)
        {
            head = new_node;
            temp = new_node;
        }
        else
        {
            temp->next = new_node;
            temp = temp->next;
        }
    }
    fclose(fl);
    fclose(flname);
    fclose(flans);
}

void addchar()
{
    node* new_node = (node*)malloc(sizeof(node));
    FILE* flname = fopen("filename.txt", "a");
    printf("What is the name of the character?\n");
    scanf(" %s", new_node->name);
    fprintf(flname, "%s\n", new_node->name);
    fclose(flname);
    addchar2(new_node->name);
    int i = 0;
    FILE* fl = fopen("file.txt", "a");
    FILE* flans = fopen("fileans.txt", "a");
    printf("Write 'stop' if you want to stop writing.\n");
    while (1)
    {
        printf("Input info type: ");
        scanf(" %s", new_node->item[i]);
        if (strcmp(new_node->item[i], "stop") == 0)
        {
            fprintf(fl, "stop\n");
            fprintf(flans, "stop\n");
            break;
        }
        fprintf(fl, "%s ", new_node->item[i]);
        printf("Input info based on type: ");
        scanf(" %s", new_node->ans[i]);
        fprintf(flans, "%s ", new_node->ans[i++]);
    }
    fclose(fl);
    fclose(flans);
    free(new_node);
}

void getTop5Values()
{
    FILE *file = fopen("data.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    nameValue data[MAX_VALUES];
    int count = 0;
    while (fscanf(file, "%s %d\n", data[count].name, &data[count].value) != EOF && count < MAX_VALUES)
    {
        count++;
    }
    fclose(file);

    qsort(data, count, sizeof(nameValue), compare);

    printf("Top 5 guessed values:\n");
    for (int i = 0; i < 5 && i < count; i++)
    {
        printf("\t\t%s: %d\n", data[i].name, data[i].value);
    }
}

void aboutus()
{
    system("cls");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________\n");
    printf("\n\t\t***********KAZI ANAMUL HAQUE************");
    printf("\n\t\t              TEAM LEADER");
    printf("\n\t\t          ID:0242320005101210");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________\n");
    printf("\n\t\t*************PROTIK DAS UTSO************");
    printf("\n\t\t               CO-LEADER");
    printf("\n\t\t          ID:0242320005101197");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________\n");
    printf("\n\t\t**********MD ABDULLAH AL MARUF**********");
    printf("\n\t\t           PRODUCTION LEADER");
    printf("\n\t\t          ID:0242320005101180");
    printf("\n\t\t________________________________________");
    printf("\n\t\t________________________________________\n");
    printf("\n\n\tpress Q to quit or press any key to go to the main menu: ");
}
