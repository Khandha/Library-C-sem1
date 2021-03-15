#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "library.h"

ksiazka *create(ksiazka *head){
    ksiazka *ptr = malloc(sizeof(ksiazka));
    printf("Podaj tytul ksiazki: ");
    scanf("%[^\n]s", ptr->title);
    flush();
    printf("Podaj imie autora: ");
    scanf("%[^\n]s", ptr->first);
    flush();
    printf("Podaj nazwisko autora: ");
    scanf("%[^\n]s", ptr->last);
    flush();
    printf("Podaj kategorie: ");
    scanf("%[^\n]s", ptr->cat);
    flush();
    ptr->borrowed = false;
    memset(ptr->borrowersFirst, 0, sizeof(ptr->borrowersFirst));
    memset(ptr->borrowersLast, 0, sizeof(ptr->borrowersLast));
    ptr->date.day = 0;
    ptr->date.month = 0;
    ptr->date.year = 0;
    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->ID = lowestIdFinder(head);
    printf("Czy ksiazka jest w wypozyczalni (y/n)? ");
    char input[MAX];
    scanf("%s", input);
    flush();
    if (input[0] == 'n' || input[0] == 'N') {
        ptr->czytelnia = false;
        printf("Ksiazka w czytelni.\n");
    }
    else {
        ptr->czytelnia = true;
        printf("Ksiazka w wypozyczalni.\n");
    }

    printf("Dodano.\n");
    return ptr;
}
ksiazka *delete(ksiazka *head) {   //usuwanie po ID lub tytule
    //funkcja znajdowania po ID / tytule
    int deleted = 0;
    while(1) {
        printf("---USUWANIE KSIAZEK---\nZnajdowanie po ID - 1, znajdowanie po tytule - 2, wroc - 3: ");
        char input[MAX];
        scanf("%s", input);
        flush();
        int option = atoi(input);

        //szukanie po ID
        if (option == 1) {
            usuwaniepoid:
            printf("Podaj ID: ");
            scanf("%s", input);
            flush();
            int id = atoi(input);
            if (id < 1) {
                printf("Nieprawidlowy argument!\n");
                continue;
            }
            ksiazka *ptr = head;
            if (head == NULL) {
                printf("Baza danych jest pusta.\nPowracanie do menu...\n\n");
                return head;
            }
            else if (ptr->next == NULL){
                free(ptr);
                printf("Jedyny element zostal usuniety.\n\n");
                return NULL;
            }
            if (head->ID == id) { //jesli usuwany head
                head = head->next;
                head->prev = NULL;
                free(ptr);
                deleted++;
                goto home;
            }
            else { //jesli usuwany srodkowy lub koncowy element listy
                for(; ptr->next != NULL; ptr = ptr->next){ //iteruje przez liste
                    if (ptr->next->ID == id){
                        printf("Czy na pewno usunac?(Y/N) ");
                        scanf("%s", input);
                        flush();
                        if (input[0] == 'N' || input[0] == 'n') {
                            printf("Powracanie...\n\n");
                            continue;
                        }
                        ksiazka *ptr2 = ptr->next;
                        ptr->next = ptr2->next;
                        if(ptr2->next != NULL){ //jesli ptr2 to nie koncowy element
                            ptr2->next->prev = ptr;
                        }
                        free(ptr2);
                        deleted++;
                        goto home;
                    }
                }
            }
            if(deleted == 0){
                printf("Podane ID nie zostalo znalezione.\n");
                break;
            }
        }

        //szukanie po tytule
        else if (option == 2) {
            printf("Podaj tytul: ");
            scanf("%[^\n]s", input);
            flush();
            ksiazka *ptr = head;
            int count = 0;
            for(; ptr; ptr = ptr->next){
                if (strcmp(input, ptr->title) == 0) count++;
            }
            char input2[MAX];
            int id;
            if (count > 1){
                printf ("Wiecej niz jedna ksiazka o podanym tytule.");
                goto usuwaniepoid;
            }
            else {
                ptr = head;
                if (head == NULL) {
                    printf("Baza danych jest pusta.\nPowracanie do menu...\n\n");
                    return head;
                } else if (ptr->next == NULL) {
                    free(ptr);
                    printf("Jedyny element zostal usuniety.\n\n");
                    return NULL;
                }
                if (strcmp(head->title, input) == 0) { //jesli usuwany head
                    head = head->next;
                    head->prev = NULL;
                    free(ptr);
                    deleted++;
                    goto home;
                } else { //jesli usuwany srodkowy lub koncowy element listy
                    for (; ptr->next != NULL; ptr = ptr->next) { //iteruje przez liste
                        if (strcmp(ptr->next->title, input) == 0) {
                            printf("Czy na pewno usunac?(Y/N) ");
                            scanf("%s", input);
                            flush();
                            if (input[0] == 'N' || input[0] == 'n') {
                                printf("Powracanie...\n\n");
                                continue;
                            }
                            ksiazka *ptr2 = ptr->next;
                            ptr->next = ptr2->next;
                            if (ptr2->next != NULL) { //jesli ptr2 to nie koncowy element
                                ptr2->next->prev = ptr;
                            }
                            free(ptr2);
                            deleted++;
                            goto home;
                        }
                    }
                }
            }
            if(deleted == 0){
                printf("Podany tytul nie zostal znaleziony.\n");
                break;
            }
        }

        else if (option == 3){
            printf("Wracanie do glownego menu.\n\n");
            break;
        }

        else {
            printf("Nieprawidlowy argument!\n");
            break;
        }
        home:
            printf("Usunieto.\n\n");
    }
    return head;
}
void edit(ksiazka *head){
    //id lub tytul
    //te same dane co w dodawaniu
    //jesli enter - te same dane co wczesniej
    char input[MAX];

    printf("---EDYTOWANIE KSIAZEK---\n"
           "Szukanie po ID - 1, Szukanie po tytule - 2, wyjdz - 3: ");
    int option, id;
    scanf("%s", input);
    flush();
    option = atoi(input);
    ksiazka *ptr = head;
    switch (option){
        case 1:
            printf("Podaj szukane ID: ");
            scanf("%s", input);
            flush();
            id = atoi(input);
            for(; ptr; ptr = ptr->next){
                if (ptr->ID == id){
                    printf("Znaleziono: \n"
                           "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                    printf("Podaj tytul ksiazki: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj imie autora: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj nazwisko autora: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj kategorie: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    char input2[MAX];
                    printf("Czy ksiazka jest w wypozyczalni (y/n)?");
                    scanf("%s", input2);
                    if (input2[0] == 'n' || input2[0] == 'N') {
                        ptr->czytelnia = true;
                    }
                    else ptr->czytelnia = false;

                    printf("Zmieniono.\n");
                    return;
                }
            }
            printf("Nie znaleziono danego ID.\n\n");
            break;
        case 2: //po tytule
            printf("Podaj szukany tytul: ");
            scanf("%[^\n]s", input);
            flush();
            for(; ptr; ptr=ptr->next){
                if (strcmp(ptr->title, input) == 0){

                    printf("Znaleziono: \n"
                           "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                    printf("Podaj nowy tytul ksiazki: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj nowe imie autora: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj nowe nazwisko autora: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    printf("Podaj nowa kategorie: ");
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        strtok(input, "\n");
                        strcpy(ptr->title, input);
                    }
                    else printf("Zapisano poprzednia wartosc.\n");

                    char input2[MAX];
                    printf("Czy ksiazka jest w wypozyczalni (y/n)?");
                    scanf("%s", input2);
                    if (input2[0] == 'n' || input2[0] == 'N') {
                        ptr->czytelnia = true;
                    }
                    else ptr->czytelnia = false;

                    printf("Zmieniono.\n");
                    return;
                }
            }
            printf("Nie znaleziono danego tytulu.\n\n");
            break;
        default:
            printf("Nieprawidlowy argument.\n");
            break;
    }


}
void status(ksiazka *head){  //id lub tytul
    //jesli nie jest wypozyczona - od razu dane wypozyczajacego i data?
    //jesli wypozyczona - od razu dane wypozyczajacego i data. pyta czy zwrocona. usuwa dane i date.
    char input[MAX];
    int option;
    ksiazka *ptr = head;
    loop:
        printf("---ZMIANA STATUSU---\n"
               "Szukanie po ID - 1, szukanie po tytule - 2: ");
        scanf("%s", input);
        flush();
        option = atoi(input);

        time_t date = time(NULL);
        struct tm time = *localtime(&date);

        switch(option){
            case 1:
                //szukanie po ID
                printf("Podaj szukane ID: ");
                scanf("%s", input);
                flush();
                int id = atoi(input);
                for (; ptr; ptr=ptr->next){
                    if (ptr->ID == id) {
                        if (ptr->borrowed==false){ //nie jest wypozyczona

                            printf("Znaleziono:\n"
                                   "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n"
                                   "Ksiazka nie jest wypozyczona\n"
                                   "-Wypozyczanie-\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);

                            if(ptr->czytelnia == true){
                                printf("Ksiazka jest w czytelni.\n");
                                break;
                            }
                            printf("Podaj imie wypozyczajacego:");
                            scanf("%[^\n]s", ptr->borrowersFirst);
                            flush();
                            printf("Podaj nazwisko wypozyczajacego: ");
                            scanf("%[^\n]s", ptr->borrowersLast);
                            flush();

                            printf("Podaj rok wypozyczenia (lub wcisnij ENTER by wstawic dzisiejsza date: ");
                            fgets(input, sizeof(input), stdin);
                            if (strlen(input) == 1){
                                ptr->date.year = time.tm_year + 1900;
                                ptr->date.month = time.tm_mon + 1;
                                ptr->date.day = time.tm_mday;
                            }
                            else {
                                ptr->date.year = atoi(input);
                                bool dateCheck = false;
                                do {
                                    printf("Podaj miesiac wypozyczenia (1-12): ");
                                    scanf("%s", input);
                                    flush();
                                    option = atoi(input);
                                    if (option > 0 && option < 13) dateCheck = true;
                                    else printf("Bledny miesiac.\n");
                                } while (dateCheck == false);
                                ptr->date.month = option;
                                dateCheck = false;
                                do {
                                    printf("Podaj dzien wypozyczenia (1-31): ");
                                    scanf("%s", input);
                                    flush();
                                    option = atoi(input);
                                    if (option > 0 && option < 32) dateCheck = true;
                                    else printf("Bledny dzien.\n");
                                } while (dateCheck == false);
                                ptr->date.day = option;
                            }

                            ptr->borrowed = true;
                            printf("Ksiazka zostala wypozyczona.\nPowrot do glownego menu...\n\n");
                            _sleep(1500);
                        }
                        else{ //jest wypozyczona
                            printf("---Ksiazka jest wypozyczona---\n"
                                   "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n\t"
                                   "Wypozyczajacy: %s %s, data: %d.%d.%d\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID,
                                   ptr->borrowersFirst, ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
                            printf("Czy zmienic status na: oddana? (Y/N)");
                            scanf("%s", input);
                            flush();
                            if(input[0] == 'Y' || input[0] == 'y'){
                                memset(ptr->borrowersFirst, 0, sizeof(ptr->borrowersFirst));
                                memset(ptr->borrowersLast, 0, sizeof(ptr->borrowersLast));
                                ptr->date.year = 0;
                                ptr->date.month = 0;
                                ptr->date.day = 0;
                                ptr->borrowed = false;
                                printf("Oddano.\n");
                                printf("Powrot do glownego menu...\n\n");
                                _sleep(1500);
                            }
                            else{
                                printf("Status nie zostal zmieniony.\nPowrot do glownego menu...\n\n");
                                _sleep(1500);
                            }
                        }
                    }
                }
                break;
            case 2:
                 //szukanie po tytule
                printf("Podaj szukany tytul: ");
                scanf("%[^\n]s", input);
                flush();
                for (; ptr; ptr=ptr->next){
                    if (strcmp(ptr->title, input) == 0) {
                        if (ptr->borrowed==false){ //nie jest wypozyczona
                            printf("Znaleziono:\n"
                                   "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n"
                                   "Ksiazka nie jest wypozyczona\n"
                                   "-Wypozyczanie-\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                            if(ptr->czytelnia == true){
                                printf("Ksiazka jest w czytelni.\n");
                                break;
                            }
                            printf("Podaj imie wypozyczajacego: ");
                            scanf("%[^\n]s", ptr->borrowersFirst);
                            flush();
                            printf("Podaj nazwisko wypozyczajacego: ");
                            scanf("%[^\n]s", ptr->borrowersLast);
                            flush();
                            printf("Podaj rok wypozyczenia (lub wcisnij ENTER by wstawic dzisiejsza date: ");
                            fgets(input, sizeof(input), stdin);
                            if (strlen(input) == 1){
                                ptr->date.year = time.tm_year + 1900;
                                ptr->date.month = time.tm_mon + 1;
                                ptr->date.day = time.tm_mday;
                            }
                            else {
                                ptr->date.year = atoi(input);
                                bool dateCheck = false;
                                do {
                                    printf("Podaj miesiac wypozyczenia (1-12): ");
                                    scanf("%s", input);
                                    flush();
                                    option = atoi(input);
                                    if (option > 0 && option < 13) dateCheck = true;
                                    else printf("Bledny miesiac.\n");
                                } while (dateCheck == false);
                                ptr->date.month = option;
                                dateCheck = false;
                                do {
                                    printf("Podaj dzien wypozyczenia (1-31): ");
                                    scanf("%s", input);
                                    flush();
                                    option = atoi(input);
                                    if (option > 0 && option < 32) dateCheck = true;
                                    else printf("Bledny dzien.\n");
                                } while (dateCheck == false);
                                ptr->date.day = option;
                            }
                            ptr->borrowed = true;
                            printf("Ksiazka zostala wypozyczona.\nPowrot do glownego menu...\n\n");
                            _sleep(1500);
                        }
                        else{ //jest wypozyczona
                            printf("---Ksiazka jest wypozyczona---\n"
                                   "Tytul: %-26.26s\t Autor: %13.13s %-16.16s\t Kategoria: %-16.16s (ID: %5.d)\n\t"
                                   "Wypozyczajacy: %s %s, data: %d.%d.%d\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID,
                                   ptr->borrowersFirst, ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
                            printf("Czy zmienic status na: oddana? (Y/N)");
                            scanf("%s", input);
                            flush();
                            if(input[0] == 'Y' || input[0] == 'y'){
                                memset(ptr->borrowersFirst, 0, sizeof(ptr->borrowersFirst));
                                memset(ptr->borrowersLast, 0, sizeof(ptr->borrowersLast));
                                ptr->date.year = 0;
                                ptr->date.month = 0;
                                ptr->date.day = 0;
                                ptr->borrowed = false;
                                printf("Oddano.\n");
                                printf("Powrot do glownego menu...\n\n");
                                _sleep(1500);
                            }
                            else{
                                printf("Status nie zostal zmieniony.\nPowrot do glownego menu...\n\n");
                                _sleep(1500);
                            }
                        }
                    }
                }
                break;
            default:
                printf("Bledny argument.\n");
                goto loop;
    }

}
void search(ksiazka *head) {
    char input[MAX];
    home:
        printf("---WYSZUKIWANIE---\n"
               "Wyszukiwanie po ID - 1, po tytule - 2, po nazwisku wypozyczajacego - 3, wyjdz - 4: ");
        scanf("%s", input);
        flush();
        int option = atoi(input);
        ksiazka *ptr = head;
        switch(option){
            case 1: //po ID
                printf("Wpisz poszukiwane ID: ");
                scanf("%s", input);
                flush();
                int id = atoi(input);
                for(; ptr; ptr=ptr->next)
                {
                    if(ptr->ID == id){

                        printf("Znaleziono: \n"
                               "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                        if(ptr->borrowed == true) {
                            printf("\t Wypozyczona przez: %s %s, dnia %d.%d.%d\n", ptr->borrowersFirst, ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
                        }
                        else printf("\tNie jest wypozyczona.\n");
                        printf("Wpisz cokolwiek by wyjsc.");
                        fgets(input, sizeof(input), stdin);
                        goto home;
                    }
                }
                printf("Nie znaleziono ksiazki o podanym ID. \n");
                goto home;
            case 2: //po tytule
                printf("Wpisz poszukiwany tytul: ");
                scanf("%[^\n]s", input);
                flush();
                for(; ptr; ptr=ptr->next)
                {
                    if(strcmp(input, ptr->title) == 0){
                        printf("Znaleziono: \n"
                               "Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                        if(ptr->borrowed == true) {
                            printf("\t Wypozyczona przez: %s %s, dnia %d.%d.%d\n", ptr->borrowersFirst, ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
                        }
                        else printf("\tNie jest wypozyczona.\n");
                        printf("Wpisz cokolwiek by wyjsc.");
                        fgets(input, sizeof(input), stdin);
                        goto home;
                    }
                }
                printf("Nie znaleziono ksiazki o podanym tytule. \n");
                goto home;
            case 3:
                printf("Wpisz poszukiwane nazwisko wypozyczajacego: ");
                scanf("%[^\n]s", input);
                flush();
                printf("Ksiazki wypozyczone przez %s:\n", input);
                bool result = false;
                for(; ptr; ptr=ptr->next) {
                    if(strcmp(input, ptr->borrowersLast) == 0){
                        printf("Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
                        printf("\t Wypozyczona przez: %s %s, dnia %d.%d.%d\n", ptr->borrowersFirst, ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
                        result = true;
                    }
                }
                if(result == false) printf("Nie znaleziono ksiazek wypozyczonych przez %s. \n", input);
                printf("Wpisz cokolwiek by wyjsc.");
                fgets(input, sizeof(input), stdin);
                _sleep(1500);
                goto home;
            case 4:
                printf("Wychodzenie...\n\n");
                _sleep(1500);
                return;
            default:
                printf("Bledny argument. \n");
                goto home;
        }

}
void titlePrint(ksiazka *head){ //wyswietla dane po tytulach, dane autora, kategorie
    // Drukowanie alfabetyczne!

    printf("---DRUKOWANIE TYTULOW---\n");
    if (head == NULL){
        printf("Baza danych jest pusta. \n");
        return;
    }

    int n = 0; //ilosc elementow na liscie

    ksiazka *ptr = head, *tmp = head;
    for (; ptr; ptr = ptr->next){ //liczenie ilosci elementow na liscie
        n++;
    }
    int *tab = malloc (sizeof(int) * n);
    for (int i = 0; i < n; i++){ //populowanie zerami
        tab[i] = 0;
    }

    for (int i = 0; i < n; i++) { //iteruje przez liste n razy
        tmp = head;
        ptr = head;
        int x = 0;

        for (int j = 0; j < n; j++){ //znajduje niewypisany element i przypisuje do tmp
            if (tab[j] == 0) {
                tmp = ptr;
                break;
            }
            ptr = ptr->next;
        }

        ptr = head;
        bool lowerfound = false;
        for (int j = 0; j < n; j++){ //iteruje przez liste n razy i znajduje najnizszy element
            if (tab[j] == 0) {
                if (strcmp(tmp->title, ptr->title) > 0) {
                    tmp = ptr;
                    x = j;
                    lowerfound = true;
                }
                else if (lowerfound == false && (strcmp(tmp->title, ptr->title) == 0)) x = j; //jesli nie znaleziono nizszego wypisuje samego siebie
            }
            ptr = ptr->next;
            if (!ptr) break;
        }
        printf("Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d), czytelnia: %s\n",
               tmp->title, tmp->first, tmp->last, tmp->cat, tmp->ID, tmp->czytelnia ? "tak" : "nie");
        tab[x] = 1;
    }


//    printf("---DRUKOWANIE TYTULOW---\n");
//    if (head == NULL){
//        printf("Baza danych jest pusta. \n");
//        return;
//    }
//    for(; ptr; ptr = ptr->next){
//        printf("Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n", ptr->title, ptr->first, ptr->last, ptr->cat, ptr->ID);
//    }


    printf("\nWpisz cokolwiek by wyjsc.");
    char input[MAX];
    fgets(input, sizeof(input), stdin);
    printf("Powrot do glownego menu...\n\n");
    _sleep(1500);
}
void borrowedPrint(ksiazka *head){ //wyswietla wypozyczone i kto wypozyczyl
    ksiazka *ptr = head;
    printf("---WYPOZYCZONE KSIAZKI---\n");
    bool result = false;
    for(; ptr; ptr = ptr->next){
        if (ptr->borrowed == true){
            printf("Tytul: %-31.31s  Autor: %13.13s %-16.16s  Kategoria: %-12.12s (ID: %4.d)\n"
                   "\tWypozyczona przez: %s %s, %d.%d.%d\n", ptr->title,
                   ptr->first, ptr->last, ptr->cat, ptr->ID, ptr->borrowersFirst,
                   ptr->borrowersLast, ptr->date.day, ptr->date.month, ptr->date.year);
            result = true;
        }
    }
    if (result == false) printf("Nie znaleziono zadnych wypozyczonych ksiazek.\n");
    printf("\nWpisz cokolwiek by wyjsc.");
    char input[MAX];
    fgets(input, sizeof(input), stdin);
    printf("Powrot do glownego menu...\n\n");
    _sleep(1500);

}
bool save(char *name, ksiazka *head){ //zapisuje baze danych do pliku
    char *extension = ".csv";
    char fileName[MAX + 5];
    snprintf(fileName, sizeof(fileName), "%s%s", name, extension);
    FILE *file = fopen(fileName, "w");
    if(!file){
        return false;
    }
    ksiazka *ptr = head;
    while (ptr){
        fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%d,%d,%s,%s,%d\n",
                ptr->ID, ptr->title, ptr->first, ptr->last, ptr->cat,
                ptr->date.year, ptr->date.month, ptr->date.day, ptr->borrowed,
                strlen(ptr->borrowersFirst) != 0 ? ptr->borrowersFirst : "0",
                strlen(ptr->borrowersLast) != 0 ? ptr->borrowersLast : "0", ptr->czytelnia);
        ptr = ptr->next;
    }
    fclose(file);
    return true;
}
ksiazka *load(char *name, ksiazka *head){ //wczytuje baze danych z pliku
    char *extension = ".csv", fileName[MAX + 5], line[1024], *result;
    ksiazka *tmp = NULL;
    int count = 0;
    snprintf(fileName, sizeof(fileName), "%s%s", name, extension);

    FILE *file = fopen(fileName, "r");
    if(!file){
        printf("Nie udalo sie otworzyc pliku.\n\n");
        return false;
    }
    //wczytywanie do nowej listy
    result = fgets(line, sizeof(line), file);
    if (result == NULL) {
        printf("Plik jest pusty.\n\n");
        return head;
    }

    unload(head);
    head = NULL;

    do {
        strtok(line, "\n");
        tmp = malloc(sizeof(ksiazka));
        tmp->ID = atoi(strtok(line, ","));
        strcpy(tmp->title, strtok(NULL, ","));
        strcpy(tmp->first, strtok(NULL, ","));
        strcpy(tmp->last, strtok(NULL, ","));
        strcpy(tmp->cat, strtok(NULL, ","));
        tmp->date.year = atoi(strtok(NULL, ","));
        tmp->date.month = atoi(strtok(NULL, ","));
        tmp->date.day = atoi(strtok(NULL, ","));
        tmp->borrowed = atoi(strtok(NULL, ","));
        strcpy(tmp->borrowersFirst, strtok(NULL, ","));
        if (strcmp(tmp->borrowersFirst, "0") == 0) {
            memset(tmp->borrowersFirst, 0, sizeof(tmp->borrowersFirst));
        } else {
            true;
        }
        strcpy(tmp->borrowersLast, strtok(NULL, ","));
        if (strcmp(tmp->borrowersLast, "0") == 0) {
            memset(tmp->borrowersLast, 0, sizeof(tmp->borrowersLast));
        } else {
            true;
        }
        tmp->czytelnia = atoi(strtok(NULL, ","));
        tmp->prev = NULL;
        tmp->next = NULL;
        head = insert(head, tmp);
        count++;
    } while (fgets(line, sizeof(line), file) != NULL);
    fclose(file);
    printf("Wczytanych pozycji: %d.\n\n", count);
    return head;
}
int lowestIdFinder(ksiazka *head){ //iteruje przez liste i zwraca najnizsze wolne ID
    if (head == NULL) return 1;
    else{
        int i = 0, found=1;
        ksiazka *ptr = head;
        while (found == 1){
            found = 0;
            i++;
            for(; ptr; ptr = ptr->next){
                if (ptr->ID == i){
                    found = 1;
                    break;
                }
            }
        }
        return i;
    }
}
ksiazka *insert(ksiazka *head, ksiazka *tmp){
    ksiazka *ptr = head, *ptr2;
    if (head == NULL) {
        head = tmp; //lista jest pusta, pierwszy element to tmp
        return head;
    }
    else if (ptr->next == NULL) { //jesli to jedyny element na liscie to:
        if (ptr->ID < tmp->ID) { //wstaw za head
            ptr->next = tmp;
            tmp->prev = ptr;
            return head;
        } else { //wstaw przed head
            tmp->next = head;
            tmp->prev = NULL;
            ptr->prev = tmp;
            head = tmp;
            return head;
        }
    }
        //wiecej niz jeden element na liscie
    else {

        if (ptr->ID > tmp->ID){ //wstawianie jako pierwszy element
            ptr->prev = tmp;
            tmp->next = ptr;
            head = tmp;
            return head;
        }
        for(;ptr->next != NULL; ptr = ptr->next) //ID nastepnego elementu jest wyzsze od danego
        {
            if (ptr->next->ID > tmp->ID){
                ptr2 = ptr->next;
                ptr->next = tmp;
                tmp->prev = ptr;
                tmp->next = ptr2;
                ptr2->prev = tmp;
                return head;
            }
        }
        ptr->next = tmp; //wstawianie na koniec
        tmp->prev = ptr;
        return head;
    }
}
void flush(){
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);
}
void unload(ksiazka *head){ //checked with valgrind
    ksiazka *ptr = head;
    ksiazka *clean = ptr;
    while (ptr){
        clean = ptr;
        ptr = clean->next;
        free(clean);
    }
}