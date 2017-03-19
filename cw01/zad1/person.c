
#include "person.h"

void printPerson(Person *person) {
    if (person != NULL) {
        printf("first_name: %s \n", person->first_name);
        printf("last_name: %s \n", person->last_name);
        printf("birth_date: %s \n", person->birth_date);
        printf("email: %s \n", person->email);
        printf("phone_number: %s \n", person->phone_number);
        printf("address: %s \n", person->address);
    }
    else {
        printf("There is no such person \n");
    }
}
Person *makePerson(const char *fst_n,const char *lst_n,const char *b_d,const char *email,const char *phone,const char *adrs){
    Person *p = (Person *) malloc(sizeof(Person));
    char *first_name2 = (char *) malloc(strlen(fst_n) * sizeof(char)+1);
    char *last_name2 = (char *) malloc(strlen(lst_n) * sizeof(char)+1);
    char *birth_date2 = (char *) malloc(strlen(b_d) * sizeof(char)+1);
    char *email2 = (char *) malloc(strlen(email) * sizeof(char)+1);
    char *phone_number2 = (char *) malloc(strlen(phone) * sizeof(char)+1);
    char *address2 = (char *) malloc(strlen(adrs) * sizeof(char)+1);
    strcpy(first_name2,fst_n);
    strcpy(last_name2,lst_n);
    strcpy(birth_date2,b_d);
    strcpy(email2,email);
    strcpy(phone_number2,phone);
    strcpy(address2,adrs);

    p->first_name = first_name2;
    p->last_name = last_name2;
    p->birth_date = birth_date2;
    p->email = email2;
    p->phone_number = phone_number2;
    p->address = address2;
    return p;
}

void destroyPerson(Person *person){
    free(person->first_name);
    free(person->last_name);
    free(person->birth_date);
    free(person->email);
    free(person->phone_number);
    free(person->address);
    free(person);
}

int personEqual(Person *p1, Person *p2){ //0 -> rowne, -1 -> nie istnieje/nie rowne
    if (p1 == NULL || p2 == NULL) return -1;
    if (p1->first_name == p2->first_name
        && p1->last_name == p2->last_name
        && p1->birth_date == p2->birth_date
        && p1->email == p2->email
        && p1->phone_number == p2->phone_number
        && p1->address == p2->address) return 0;
    else
        return -1;
}


int compareBy(Person *p1, Person *p2, char *key){
    if (strcmp(key,"first")==0) return firstNameComparator(p1,p2);
    else if (strcmp(key,"last")==0) return lastNameComparator(p1,p2);
    else if (strcmp(key,"birth")==0) return birthDateComparator(p1,p2);
    else if (strcmp(key,"email")==0) return emailComparator(p1,p2);
    else if (strcmp(key,"phone")==0) return phoneComparator(p1,p2);
    else if (strcmp(key,"address")==0) return addressComparator(p1,p2);
    else return 0;
}

int firstNameComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->first_name != NULL
        && person2->first_name != NULL) {
        return strcmp(person1->first_name, person2->first_name);
    }
    return 0;
}

int lastNameComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->last_name != NULL
        && person2->last_name != NULL) {
        return strcmp(person1->last_name, person2->last_name);
    }
    return 0;
}

int birthDateComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->birth_date != NULL
        && person2->birth_date != NULL) {
        return strcmp(person1->birth_date, person2->birth_date);
    }
    return 0;
}

int emailComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->email != NULL
        && person2->email != NULL) {
        return strcmp(person1->email, person2->email);
    }
    return 0;
}

int phoneComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->phone_number != NULL
        && person2->phone_number != NULL) {
        return strcmp(person1->phone_number, person2->phone_number);
    }
    return 0;
}

int addressComparator(Person *person1, Person *person2) {
    if (person1 != NULL && person2 != NULL
        && person1->address != NULL
        && person2->address != NULL) {
        return strcmp(person1->address, person2->address);
    }
    return 0;
}
