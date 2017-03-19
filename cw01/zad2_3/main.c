#include "../zad1/list.h"
#include "../zad1/person.h"
#include "../zad1/tree.h"
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

int main(void){
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
    List *lista = initList();
    for (int i=0;i<PPL;i++){
      Person *p = makePerson(FIRST[rand()%size],LAST[rand()%size],BIRTH[rand()%size],
                            EMAIL[rand()%size],PHONE[rand()%size],ADDRESS[rand()%size]);
      lista = pushBack(lista,p);
    }
    checkTime();
    //===========================================================================================================
    printf("POMIAR 2 - STWORZENIE 1 ELEMENTU I DODANIE DO KSIAZKI\n");
    Person *p1 = makePerson("Jasiek","Sznajd","27/12/1996","jasiek.sznajd@gmail.com","603000000","Krakowska 49");
    lista = pushBack(lista,p1);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 3 - ZNALEZIENIE ELEMENTU PESYMISTYCZNIE\n");
    Node *n1 = findNode(lista,"jasiek.sznajd@gmail.com");
    checkTime();
    //===========================================================================================================
    printf("POMIAR 4 - ZNALEZIENIE ELEMENTU OPTYMISTYCZNIE\n");
    Node *n2 = findNode(lista,lista->head->person->email);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 5 - USUNIECIE ELEMENTU PESYMISTYCZNIE\n");
    lista = removeNode(lista,n1);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 6 - USUNIECIE ELEMENTU OPTYMISTYCZNIE\n");
    lista = removeNode(lista,n2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 7 - ZMIANA KLUCZA KSIAZKI\n");
    lista = qSort(lista,"email");
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
      Person *p = makePerson(FIRST[rand()%size],LAST[rand()%size],BIRTH[rand()%size],
                            EMAIL[rand()%size],PHONE[rand()%size],ADDRESS[rand()%size]);
      tree = insertPerson(tree,p);
    }
    checkTime();
    //===========================================================================================================
    printf("POMIAR 2 - STWORZENIE 1 ELEMENTU I DODANIE DO KSIAZKI\n");
    Person *p2 = makePerson("Jasiek","Sznajd","27/12/1996","zasiek.sznajd@gmail.com","603000000","Krakowska 49");
    tree = insertPerson(tree,p2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 3 - ZNALEZIENIE ELEMENTU PESYMISTYCZNIE\n");
    TNode *n3 = findElement(tree,p2);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 4 - ZNALEZIENIE ELEMENTU OPTYMISTYCZNIE\n");
    TNode *n4 = findElement(tree,tree->person);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 5 - USUNIECIE ELEMENTU PESYMISTYCZNIE\n");
    tree = removeElement(tree,n3);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 6 - USUNIECIE ELEMENTU OPTYMISTYCZNIE\n");
    tree = removeElement(tree,n4);
    checkTime();
    //===========================================================================================================
    printf("POMIAR 7 - ZMIANA KLUCZA KSIAZKI\n");
    tree = rebuildTree(tree,"phone");
    checkTime();

    return 0;
}
