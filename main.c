#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"

ksiazka *head = NULL;
ksiazka *last = NULL; //wskaznik na ostatnio dodany element listy
int main() {
    char input[MAX], input2[MAX];
    while(1) {
        printf("\t\t---PROGRAM BIBLIOTECZNY---\n");
        printf("\t\t\tMENU OPCJI:\n1. Dodaj nowa ksiazke\t\t\t"
               "2. Usun ksiazke\n"
               "3. Edytuj dane ksiazki\t\t\t"
               "4. Zmien status ksiazki\n"
               "5. Wyszukaj ksiazke\t\t\t"
               "6. Wyswietl baze ksiazek po tytulach\n"
               "7. Wyswietl wypozyczone ksiazki\t\t"
               "8. Zapisz do pliku\n"
               "9. Wczytaj z pliku\t\t\t"
               "X by wyjsc z programu.\n");

        scanf("%s", input);
        flush();
        if ((input[0] == 'X' || input[0] == 'x')&& input[1] == (int) 0) {
            printf("Czy na pewno wyjsc z programu? Niezapisane dane zostana utracone. (Y/N)");
            scanf("%s", input2);
            flush();
            if (input2[0] == 'Y' || input2[0] == 'y') {
                unload(head);
                printf("Milego dnia!");
                _sleep(3000);
                return 0;
            } else continue;
        }
        int option = atoi(input);
        switch(option){
            case 1:
                do {
                    ksiazka *node = create(head);
                    head = insert(head, node);
                    last = node;
                    falseArg:
                        printf("Czy chcesz dodac kolejna ksiazke (Y/N)?: ");
                        fgets(input, sizeof(input), stdin);
                        if (input[0] == 'N' || input[0] == 'n') break;
                        else if (input[0] == 'Y' || input[0] == 'y') continue;
                        else {
                            printf("Nieprawidlowy argument. \n");
                            goto falseArg;
                        }
                } while(1);
                break;
            case 2:
                head = delete(head);
                break;
            case 3:
                edit(head);
                break;
            case 4:
                status(head);
                break;
            case 5:
                search(head);
                break;
            case 6:
                titlePrint(head);
                break;
            case 7:
                borrowedPrint(head);
                break;
            case 8:
                printf("Podaj nazwe pliku docelowego: \n"
                       "(Uwaga - funkcja nadpisuje plik jezeli taki sam istnieje.): ");
                char *input3 = malloc(MAX);
                scanf("%s", input3);
                flush();
                bool saved = save(input3, head);
                free(input3);
                if (saved == false) printf("Nie udalo sie zapisac.\nPowracanie...\n\n");
                else printf("Plik zostal zapisany.\nPowracanie... \n\n");
                _sleep(1500);
                break;
            case 9:
                printf("Podaj nazwe pliku wczytywanego: \n"
                       "(Uwaga - funkcja usuwa biezaca baze danych!): ");
                input3 = malloc(sizeof(char) * MAX);
                scanf("%s", input3);
                flush();
                head = load(input3, head);
                break;
            default:
                printf("Bledny argument.\n");
                break;
        }

    }//while bracket
}