/*
 * File:   main.c
 * Author: Nikos Bafatakis A.E.M 2383
 *!!!Gia perissoteres plirofories sxetika me thn ylopoihsh Anatrexte sthn Tekmiriwsh.!!!
 * Created on 7 Ιανουαρίου 2015, 2:27 μμ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WORD 20

struct word {
    char en[WORD];
    char gr[WORD];
    struct word *next;
} *head = NULL;

typedef int bool;
#define true 1
#define false 0

/*]
Dilwseis twn synarthsewn
*/
struct word *findWord();
struct word *deleteWord();
bool updateTerm();
int menu_select();
void printLex();
void translate_sentense();
struct word *insertTerm();
char *levenshtein();
void WriteBinaryFile();
struct word *ReadBinaryFile();
//H main tou programmatos mas.
int main(int argc, char** argv) {
    printf("Welcome to the Dictionary!!\n\n");
    int choice;
    char s[2];
    int c;
    char eng[WORD];
    char gr[WORD];
    char key[WORD];
    char sent[250];
    head = ReadBinaryFile(head);
    fflush(stdin);
    while (true) {
        choice = menu_select();
        switch (choice) {
            case 1:
                printf("Give English Word:\n");
                scanf("%s", eng);
                printf("Give Greek Word:\n");
                scanf("%s", gr);
                if(findWord(eng,head)!=NULL){
                    printf("Sorry, this word allready exists in the Dictionary\n");
                    break;
                }
                head = insertTerm(eng, gr, head);
                printf("\n\nPress any key to Return to Main Menu\n");
                getchar();
                break;
            case 2:
                printf("Give English or Greek Word to Update term:\n");
                gets(key);
                printf("Give English Word:\n");
                gets(eng);
                printf("Give Greek Word:\n");
                gets(gr);
                bool done = updateTerm(key, head, eng, gr);
                if (done) printf("Word Updated!!!\n");
                else printf("Error\n");
                break;
            case 3:
                printf("Give English or Greek Word to Delete term:\n");
                gets(key);
                head = deleteWord(head, key);
                printf("Word Deleted");
                printf("\n\nPress any key to Return to Main Menu\n");
                getchar();
                break;
            case 4:
                printf("Translate:\n1.From Greek to English\n2.From English to Greek\n");
                do {
                    printf("\nEnter your choice:\n");
                    gets(s);
                    c = atoi(s);
                } while (c < 1 || c > 3);
                bool eng;
                if (c == 1) eng = true;
                else eng = false;
                printf("Give a Word to Translate:\n");
                gets(key);
                struct word *temp = findWord(key, head);
                if (temp != NULL) {
                    printf("English Word=%s\n Greek Word=%s\n", temp->en, temp->gr);
                } else {
                    printf("Closest word based on Levenshtein Distance\n");
                    char *s = levenshtein(key, eng, head);
                    printf("%s", s);
                }
                printf("\n\nPress any key to Return to Main Menu\n");
                getchar();
                break;
            case 5:
                printf("Translate:\n1.From Greek to English\n2.From English to Greek\n");
                do {
                    printf("\nEnter your choice:\n");
                    gets(s);
                    c = atoi(s);
                } while (c < 1 || c > 3);
                switch (c) {
                    case 1:
                        printf("Give sentence to Translate:\n");
                        gets(sent);
                        translate_sentense(sent, false, head);
                        printf("\n\n");
                        break;
                    case 2:
                        printf("Give sentence to Translate:\n");
                        gets(sent);
                        translate_sentense(sent, true, head);
                        printf("\n\n");
                        break;
                }
                break;
            case 6:
                printLex(head);
                break;
            case 7:
                printf("\nDo u want to save changes to Lexicon?(yes/no)\n");
                gets(key);
                if (strcmp(key, "yes") == 0) {
                    WriteBinaryFile(head);
                }
                exit(0);
        }
    }
}

/*H synartisi menu_display emfanizei to menu epilogwn sthn othonh gia na epilexei o xrhsths*/
int menu_select(void) {
    fflush(stdin);
    char s[2];
    int c;
    printf("1. New term:\n");
    printf("2. Edit term:\n");
    printf("3. Delete term:\n");
    printf("4. Translate Word:\n");
    printf("5. Translate sentence:\n");
    printf("6. Print All words:\n");
    printf("7. Exit.\n");
    do {
        printf("\nEnter your choice:\n");
        gets(s);
        c = atoi(s);
    } while (c < 1 || c > 7);
    return c;
}


//H synartisi ayth dexete mia opoiadipote lexh kai epistrefei ena struct me thn lexh ayth
struct word *findWord(char key[WORD], struct word *head) {
    struct word *p = head;
    while (p != NULL) {
        if (strcmp(p->en, key) == 0 || strcmp(p->gr, key) == 0) {
            return p;
        } else {
            p = p->next;
        }
    }
    return NULL;
}
//h synarthsh ayth metafrazei mia oloklhrh protash
void translate_sentense(char sent[], bool eng, struct word *head) {
    char translated[500] = "\0";
    const char s[2] = " ";
    char *token;
    struct word *p;
    if (eng) {  //metafrash apo agglika se ellhnika
        token = strtok(sent, s);
        while (token != NULL) {
            p = findWord(token, head);
            if (p != NULL) {
                strcat(translated, p->gr);
            } else if (p == NULL) { //kathe lexh gia thn opoia den yparxei sto lexiko vrikete mia paromoia me thn apostash Leveshtein
                char *lev = levenshtein(token, !eng, head);  //h synarthsh levenshtein thelei !bool.
                strcat(translated, lev);
            }
            strcat(translated, " ");
            token = strtok(NULL, s);
        }
    } else {  //metafrash apo ellhnika se agglika
        token = strtok(sent, s);
        while (token != NULL) {
            p = findWord(token, head);
            if (p != NULL) {
                strcat(translated, p->en);
            } else if (p == NULL) {
                char *lev = levenshtein(token, !eng, head);
                strcat(translated, lev);
            }
            strcat(translated, " ");
            token = strtok(NULL, s);
        }

    }
    printf("%s\n", translated);
}
//eisagwgi neas lexis
struct word * insertTerm(char enWord[WORD], char grWord[WORD], struct word * head) {
    struct word *newWord, *p = head;
    newWord = (struct word*) malloc(sizeof (struct word));
    strcpy(newWord->en, enWord);
    strcpy(newWord->gr, grWord);
    if (head == NULL) {
        newWord->next = head;
        return newWord;
    }
    while (p->next != NULL) {
        p = p->next;
    }
    newWord->next = p->next;
    p->next = newWord;
    return head;
}
//diagrafh lexis
struct word *deleteWord(struct word *head, char key[WORD]) {
    struct word *p, *pp;
    if (head == NULL) {
        return NULL;
    }
    p = head->next;
    if (strcmp(p->en, key) == 0 || strcmp(p->gr, key) == 0) {
        free(head);
        return (p);
    }
    pp = head;
    while (p != NULL) {
        if (strcmp(p->en, key) == 0 || strcmp(p->gr, key) == 0) {
            pp->next = p->next;
            free(p);
        }
        p = p->next;
        pp = pp->next;
    }
    pp = head;
    return head;
}

char *levenshtein(char wrd[], bool eng, struct word *head) { //!!!!Gia perissoteres plirofories deite tekmiriwsh selida 5.!!!
    char *found[WORD];
    int cost, i, minCost;
    char c[WORD];
    struct word *p = head;
    if (eng) {  //apo agglika se ellhnika
        minCost = strlen(wrd);
        while (p != NULL) { //diatrexete h lista
            cost = 0;
            strcpy(c, p->gr);
            if (strlen(c) >= strlen(wrd)) { //elegxos poia lexh einai megalyterh
                for (i = 0; i < strlen(wrd); i++) {
                    if ((c[i] == wrd[i]) == 0) {
                        cost++;
                    }
                }
                cost += strlen(c) - strlen(wrd); //syblirwsh kostous
            } else if (strlen(wrd) > strlen(c)) {
                for (i = 0; i < strlen(c); i++) {
                    if ((c[i] == wrd[i]) == 0) {
                        cost++;
                    }
                }
                cost += strlen(wrd) - strlen(c);
            }
            if (cost < minCost) {  //ypologismos min Costous kai apothikeysh lexis
                minCost = cost;
                strcpy(found, p->en);
            }
            p = p->next;
        }
        return found;

    } else { //apo ellhnika se agglika
        minCost = strlen(wrd);
        while (p != NULL) {
            cost = 0;
            strcpy(c, p->en);
            if (strlen(c) >= strlen(wrd)) {
                for (i = 0; i < strlen(wrd); i++) {
                    if (c[i] != wrd[i]) {
                        cost++;
                    }
                }
                cost += strlen(c) - strlen(wrd);
            } else if (strlen(c) < strlen(wrd)) {
                for (i = 0; i < strlen(c); i++) {
                    if (c[i] != wrd[i]) {
                        cost++;
                    }
                }
                cost = cost + (strlen(wrd) - strlen(c));
            }
            if (cost < minCost) {
                minCost = cost;
                strcpy(found, p->gr);

            }
            p = p->next;
        }
        return found;
    }

}
//epexergasia lexis
bool updateTerm(char key[WORD], struct word *head, char enWord[WORD], char grWord[WORD]) {
    struct word *p;
    p = findWord(key, head);
    if (p == NULL) {
        return false;
    }
    strcpy(p->en, enWord);
    strcpy(p->gr, grWord);
    return true;
}
//ektypwsh listas
void printLex(struct word * head) {
    printf("Printing Struct\n");
    struct word *p = head;
    while (p != NULL) {
        printf("%s, %s\n", p->en, p->gr);
        p = p->next;
    }
}
//eggrafh arxeiou
void WriteBinaryFile(struct word * head) {
    FILE *bin_file;

    char name[50];
    printf("\n Give filename(Default= lex.txt)\n");
    scanf("%s", name);


    if ((bin_file = fopen(name, "w")) != NULL) {
        do {
            fprintf(bin_file, "%s %s\n", head->en, head->gr);
            head = head->next;
        } while (head != NULL);

        fclose(bin_file);
    } else {
        printf("Error opening the File");
    }
}
//diavasma arxeiou
struct word * ReadBinaryFile(struct word * head) {
    FILE *bin_file;

    char name[50];
    char en[WORD];
    char gr[WORD];

    if ((bin_file = fopen("lex.txt", "r+")) != NULL) {
        while (feof(bin_file) == false) {
            fscanf(bin_file, "%s", en);
            fscanf(bin_file, " %s\n", gr);
           // printf("%s %s\n", en, gr);

            head = insertTerm(en, gr, head);
        };
        fclose(bin_file);


    } else {
        printf("Error opening the File Press Enter to Start a New Dictionary\n"); //to arxeio den vrethike, dimiourgia neou lexikou
        getchar();
    }
    return head;
}
