#include "../zad1/list.h"
#include "../zad1/person.h"
#include "../zad1/tree.h"
#include <dlfcn.h>
#include <stdio.h>
#include <time.h>
#include <sys/resource.h>

#define PPL 1000

static void printTimes(double real, double user, double sys)
{
    printf("=============================================\n");
    printf("real\t%fs\n",real);
    printf("user\t%fs\n",user);
    printf("sys\t%fs\n",sys);
    printf("=============================================\n");
}

void checkTime(){
    static int count = 0;
    static double	real1, user1, sys1,real2, user2, sys2;
    struct rusage rusage;
    clock_t my_clock;

    if (count != 0){
      real2 = real1;
      sys2 = sys1;
      user2 = user1;
    }

    count += 1;
    getrusage(RUSAGE_SELF, &rusage);
    my_clock = clock();

    real1 = my_clock / (double)CLOCKS_PER_SEC;
    user1 = rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / (double)10e6;
    sys1 = rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / (double)10e6;

    if (count == 1){
      real2 = real1;
      sys2 = sys1;
      user2 = user1;
    }

    printTimes(real1-real2,sys1-sys2,user1-user2);
}

void check_for_dlerror()
{
    char *error;
    if ((error = dlerror()) != NULL) {
        fputs(error, stderr);
        exit(1);
    }
    free(error);
}

    typedef Person* (*dynamic_makePersonT)(const char *fst_n,const char *lst_n,const char *b_d,const char *email,const char *phone,const char *adrs);
    typedef List* (*dynamic_initListT)(void);
    typedef List* (*dynamic_pushBackT)(List *list,Person *p);
    typedef Node* (*dynamic_findNodeT) (List *list, char *mail);
    typedef List* (*dynamic_removeNodeT)(List *list, Node *node);
    typedef List* (*dynamic_qSortT)(List *list, char *key);

    typedef TNode* (*dynamic_insertPersonT)(TNode *root, Person *p);
    typedef TNode* (*dynamic_findElementT)(TNode *root, Person *p);
    typedef TNode* (*dynamic_removeElementT)(TNode *root, TNode *elem);
    typedef TNode* (*dynamic_rebuildTreeT)(TNode *root, char *key);


int main(void){
    //HANDLER
    void* handler = dlopen("libzad1_shared_lib.so", RTLD_LAZY);
    if(!handler) {
        fprintf(stderr, "Failed to open library %s\n", "zad1_shared_lib.so");
        return -1;
    }

    dynamic_makePersonT dynamic_makePerson;
    *(void **) (&dynamic_makePerson) = dlsym(handler,"makePerson");
    check_for_dlerror();
    dynamic_initListT dynamic_initList;
    *(void **) (&dynamic_initList) = dlsym(handler,"initList");
	check_for_dlerror();
    dynamic_pushBackT dynamic_pushBack;
    *(void **) (&dynamic_pushBack) = dlsym(handler,"pushBack");
	check_for_dlerror();
    dynamic_findNodeT dynamic_findNode;
    *(void **) (&dynamic_findNode) = dlsym(handler,"findNode");

    dynamic_removeNodeT dynamic_removeNode;
    *(void **) (&dynamic_removeNode) = dlsym(handler,"removeNode");

    dynamic_qSortT dynamic_qSort;
    *(void **) (&dynamic_qSort) = dlsym(handler,"qSort");

    dynamic_insertPersonT dynamic_insertPerson;
    *(void **) (&dynamic_insertPerson) = dlsym(handler,"insertPerson");

    dynamic_findElementT dynamic_findElement;
    *(void **) (&dynamic_findElement) = dlsym(handler,"findElement");

    dynamic_removeElementT dynamic_removeElement;
    *(void **) (&dynamic_removeElement) = dlsym(handler,"removeElement");

    dynamic_rebuildTreeT dynamic_rebuildTree;
    *(void **) (&dynamic_rebuildTree) = dlsym(handler,"rebuildTree");




    // DANE
    int size = 10;
    srand((unsigned int) time(NULL));
    char* FIRST[] = { "Jasiek", "Krzysiek", "Kuba", "Stasiek", "Andrzej", "Ania", "Karolina", "Madzia","Frank","Josh"};
    char* LAST[] = {"Froning", "Morawski", "Kowalski", "Potocki", "Lipinski", "Smith","Bridges","Herbert","Slowacki","Polanski"};
    char* ADDRESS[] = {"Kawiory 8", "Krakowska 4", "Krolewska 43", "Czarnowiejska 25", "Grodzka 1", "Morska 23", "Jadwigi 13"
    "Krucza 13","Antalowka 35","Miedzymiastowa 9"};
    char* BIRTH[] = {"27/12/1996","04/9/1996","22/07/1995","13/05/1972","29/02/1856","14/03/2123","05/06/1960","29/05/1956","16/12/2023","05/07/1960"};
    char* PHONE[] = {"603123678", "876123657", "098123532", "775923043", "765456564", "4223232", "600504503", "777231987",
    "901504503", "717231917"};
    char* EMAIL[] = {
            "j@sz.com", "kr@gmail.com", "bit@iiet.com", "studnet@agh.edu", "mail@gmail.com", "pol@dom.pl", "dor@gmail.com", "bor@gmail.com", "cor@sz.com", "mor@gmail.com"};





    //===========================================================================================================
    printf("POMIAR 0 - START POMIAROW DLA LISTY \n");
    checkTime();
    //===========================================================================================================
    printf("POMIAR 1 - STWORZENIE I WYPELNIENIE KSIAZKI\n");
    List *lista = (*dynamic_initList)();
    for (int i=0;i<PPL;i++){
      Person *p = (*dynamic_makePerson)(FIRST[rand()%size],LAST[rand()%size],BIRTH[rand()%size],
                            EMAIL[rand()%size],PHONE[rand()%size],ADDRESS[rand()%size]);
      lista = (*dynamic_pushBack)(lista,p);
    }
    checkTime();
    //===========================================================================================================
    printf("POMIAR 2 - STWORZENIE 1 ELEMENTU I DODANIE DO KSIAZKI\n");
    Person *p1 = (*dynamic_makePerson)("Jasiek","Sznajd","27/12/1996","jasiek.sznajd@gmail.com","603000000","Krakowska 49");
    lista = (*dynamic_pushBack)(lista,p1);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 3 - ZNALEZIENIE ELEMENTU PESYMISTYCZNIE\n");
    Node *n1 = (*dynamic_findNode)(lista,"jasiek.sznajd@gmail.com");
    checkTime();
    //===========================================================================================================
    printf("POMIAR 4 - ZNALEZIENIE ELEMENTU OPTYMISTYCZNIE\n");
    Node *n2 = (*dynamic_findNode)(lista,lista->head->person->email);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 5 - USUNIECIE ELEMENTU PESYMISTYCZNIE\n");
    lista = (*dynamic_removeNode)(lista,n1);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 6 - USUNIECIE ELEMENTU OPTYMISTYCZNIE\n");
    lista = (*dynamic_removeNode)(lista,n2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 7 - ZMIANA KLUCZA KSIAZKI\n");
    lista = (*dynamic_qSort)(lista,"email");
    checkTime();

     /*
    * DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWO DRZEWODRZEWO DRZEWO
    */

    //===========================================================================================================
    printf("\n\n\nPOMIAR 0 - START POMIAROW DLA DRZEWA \n");
    checkTime();
    //===========================================================================================================
    printf("POMIAR 1 - STWORZENIE I WYPELNIENIE KSIAZKI\n");
    TNode *tree = NULL;
    for (int i=0;i<PPL-1;i++){
      Person *p = (*dynamic_makePerson)(FIRST[rand()%size],LAST[rand()%size],BIRTH[rand()%size],
                            EMAIL[rand()%size],PHONE[rand()%size],ADDRESS[rand()%size]);
      tree = (*dynamic_insertPerson)(tree,p);
    }
    checkTime();
    //===========================================================================================================
    printf("POMIAR 2 - STWORZENIE 1 ELEMENTU I DODANIE DO KSIAZKI\n");
    Person *p2 = (*dynamic_makePerson)("Jasiek","Sznajd","27/12/1996","zasiek.sznajd@gmail.com","603000000","Krakowska 49");
    tree = (*dynamic_insertPerson)(tree,p2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 3 - ZNALEZIENIE ELEMENTU PESYMISTYCZNIE\n");
    TNode *n3 = (*dynamic_findElement)(tree,p2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 4 - ZNALEZIENIE ELEMENTU OPTYMISTYCZNIE\n");
    TNode *n4 = (*dynamic_findElement)(tree,tree->person);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 5 - USUNIECIE ELEMENTU PESYMISTYCZNIE\n");
    tree = (*dynamic_removeElement)(tree,n3);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 6 - USUNIECIE ELEMENTU OPTYMISTYCZNIE\n");
    tree = (*dynamic_removeElement)(tree,n4);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 7 - ZMIANA KLUCZA KSIAZKI\n");
    tree = (*dynamic_rebuildTree)(tree,"phone");
    checkTime();

    dlclose(handler);

    return 0;
}
