#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdbool.h>

#define MAX 127

typedef struct ksiazka
{
    int ID;
    char title[MAX];
    char first[MAX];
    char last[MAX];
    char cat[MAX];
    struct date {
        int year, month, day;
    } date;
    bool borrowed;
    char borrowersFirst[MAX];
    char borrowersLast[MAX];
    struct ksiazka *next;
    struct ksiazka *prev;
    bool czytelnia;
} ksiazka;


ksiazka *create(ksiazka *head);
ksiazka *delete(ksiazka *head);
void edit(ksiazka *head);
void status(ksiazka *head);
void search(ksiazka *head);
void titlePrint(ksiazka *head);
void borrowedPrint(ksiazka *head);
bool save(char *name, ksiazka *head);
ksiazka *load(char *name, ksiazka *head);
int lowestIdFinder(ksiazka *head);
ksiazka *insert(ksiazka *head, ksiazka *tmp);
void unload(ksiazka *head);
void flush();
#endif
