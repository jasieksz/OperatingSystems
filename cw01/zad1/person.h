#ifndef ZAD1_PERSON_H
#define ZAD1_PERSON_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct Person {
    char *first_name;
    char *last_name;
    char *birth_date;
    char *email;
    char *phone_number;
    char *address;
}Person;

void printPerson(Person *person);
Person *makePerson(const char *fst_n,const char *lst_n,const char *b_d,const char *email,const char *phone,const char *adrs);
void destroyPerson(Person *person);
int personEqual(Person *p1, Person *p2);
int compareBy(Person *p1, Person *p2, char *key);
int firstNameComparator(Person *person1, Person *person2);
int lastNameComparator(Person *person1, Person *person2);
int birthDateComparator(Person *person1, Person *person2);
int emailComparator(Person *person1, Person *person2);
int phoneComparator(Person *person1, Person *person2);
int addressComparator(Person *person1, Person *person2);

#endif
