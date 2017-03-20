#ifndef CW02_SORT_SHUFFLE_H
#define CW02_SORT_SHUFFLE_H


#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void getTime(double *user, double *sys);

void sorter(const char* fileName, size_t recordSize, size_t recordsNumber, void (*f)(const char*, size_t, size_t));
void libSorter(const char* fileName, size_t recordSize, size_t recordsNumber);
void sysSorter(const char* fileName, size_t recordSize, size_t recordsNumber);

void shuffler(const char* fileName, size_t recordSize,size_t recordsNumber, void (*f)(const char*, size_t, size_t));
void libShuffler(const char* fileName, size_t recordSize, size_t recordsNumber);
void sysShuffler(const char* fileName, size_t recordSize, size_t recordsNumber);

void libSwap(FILE* file, size_t i, size_t j, size_t recordSize);
void sysSwap(int file, size_t i, size_t j, size_t recordSize);

void generate(char* fileName, size_t recordSize, size_t recordsNumber);


#endif //CW02_SORT_SHUFFLE_H
