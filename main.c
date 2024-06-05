#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "turtle.h"

//glowna struktura listy 
typedef struct Node {
    char value;
    struct Node *next;
} Node;

//struktura do przechowywania pozycji - rysowanie
typedef struct Position {
    float x;
    float y;
    float direction;
    struct Position *next;
} Position;

//deklaracje funkcji

//pokaz config - funkcja do wyswietlania obecnego configu
int showConfig();

//zmien obecny config
int changeConfig();

//wyswietlenie podstawowych inf o programie
void readAboutProgram();

//zacznij symulacje
int startSimulation();

//wczytaj config z pliku
void loadConfig(FILE*, char*, char*, char*, int*, float*, char*, char*, char*, char*, int*);

//do listy z arg dodaj wartosc z arg2
Node* append(Node *head, char value);

//zastosowanie zasad symulacji
Node* applyRules(Node *head, char*, char*, char, char);

//wypisz liste na ekran
void printList(Node *head);

//narysuj i zapisz roslinke
void drawPlant(Node *head, float angle, int step, char* color, char other);

//zwolnij pamiec listy
void freeList(Node *head);

//dodaj pozycje do stosu
Position* pushPosition(Position *head, float x, float y, float direction);

//zdejmij pozycje ze stosu
Position* popPosition(Position *head, float *x, float *y, float *direction);

//glowna funkcja z menu
int main() {
    int choice;
    //petla menu
    while (1) {
        printf("--------------------------\n");
        printf("SYMULACJA WZROSTU ROSLIN\n");
        printf("--------------------------\n\n");
        printf("1. Pokaz obecny plik config.txt\n");
        printf("2. Zmien informacje w pliku config.txt teraz jezeli nie zrobiles tego predzej\n");
        printf("3. Podstawowe informacje o programie\n");
        printf("4. Uruchom symulacje\n");
        printf("5. Wyjscie z programu\n");
        printf("--------------------------\n");
        printf("Podaj swoj wybor: ");
        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                showConfig();
                break;
            case 2:
                changeConfig();
                break;
            case 3:
                readAboutProgram();
                break;
            case 4:
                startSimulation();
                break;
            case 5:
                printf("--------------------------\n");
                printf("\n\nDziekuje za skorzystanie z programu symulacji wzrotu roslin.\n");
                return 0;
            default:
                printf("Bledny wybor. Prosze wybrac numer od 1 do 5.\n");
        }
    }

    return 0;
}



// FUNCTIONS


int showConfig() {
    //zmienne do wczytania z pliku
    char ruleX[100], ruleF[100], start, sun, other, color[100], random_generate;
    int iterations, step;
    float angle;
    //otwarcie pliku w trybie do odczytu
    FILE *plik = fopen("config.txt", "r");
    if (plik == NULL) {
        printf("blad pliku konfiguracyjnego\n");
        return 100;
    }
    //wczytanie z pliku
    loadConfig(plik, ruleX, ruleF, &start, &iterations, &angle, &random_generate, &sun, &other, color, &step);
    //wypisanie na ekran obecnego configu
    printf("\n\n");
    printf("--------------------------\n");
    printf("Obecny config:\n");
    printf("ZasadaX: %s\n", ruleX);
    printf("ZasadaF: %s\n", ruleF);
    printf("Startujacy element: %c\n", start);
    printf("Liczba iteracji: %d\n", iterations);
    printf("Kat: %f\n", angle);
    printf("Losowosc generowania: %c\n", random_generate);
    printf("Kierunek z jakiego swieci slonce: %c\n", sun);
    printf("Inne rosliny: %c\n", other);
    printf("Kolor: %s\n", color);
    printf("Dlugosc kroku: %d\n", step);
    printf("\n\n");
    //zamkniecie pliku
    fclose(plik);
    return 0;
}


int changeConfig() {
    //zmienne configu-glownie
    char ruleX[100], ruleF[100], start, sun, other, color[100], change_set, random_generate;
    int iterations, step;
    float angle;

    printf("\n\n");
    printf("--------------------------\n");
    printf("Czy na pewno chcesz dokonac edycji ustawien? (y/n): ");
    //potwierdzenie zmiany configu
    scanf(" %c", &change_set);
    if (change_set == 'n') {
        printf("\n\n");
        return 0;
    }
    printf("\n\n");
    //wczytanie nowych wartosci
    if (change_set == 'y') printf("--------------------------\n");
    printf("Wprowadz nowy config symulacji.\n");
    printf("Nie uzywaj spcaji, zmiane potwierdz enterem.\n\n");
    printf("ZasadaX: ");
    scanf(" %s", ruleX);
    printf("ZasadaF: ");
    scanf(" %s", ruleF);
    printf("Startujacy element(): ");
    scanf(" %c", &start);
    printf("Liczba iteracji: ");
    scanf(" %d", &iterations);
    printf("Kat: ");
    scanf(" %f", &angle);
    printf("Losowosc generowania(y/n): ");
    scanf(" %c", &random_generate);
    printf("Kierunek z jakiego swieci slonce(l/r/c): ");
    scanf(" %c", &sun);
    printf("Inne rosliny(y/n): ");
    scanf(" %c", &other);
    printf("Kolor(red/green/blue/black): ");
    scanf(" %s", color);
    printf("Dlugosc kroku: ");
    scanf(" %d", &step);
    //zapisanie nowych wartosci do pliku
    FILE *plik = fopen("config.txt", "w");
    if (plik == NULL) {
        printf("blad pliku konfiguracyjnego\n");
        return 101;
    }

    fprintf(plik, "ruleX=%s\n", ruleX);
    fprintf(plik, "ruleF=%s\n", ruleF);
    fprintf(plik, "start=%c\n", start);
    fprintf(plik, "iterations=%d\n", iterations);
    fprintf(plik, "angle(deg)=%f\n", angle);
    fprintf(plik, "random_generate(y/n)=%c\n", random_generate);
    fprintf(plik, "sunlight_direction(l/r/c)=%c\n", sun);
    fprintf(plik, "other_plants(y/n)=%c\n", other);
    fprintf(plik, "color(red/green/blue/black)=%s\n", color);
    fprintf(plik, "step=%d\n", step);
    //zamkniecie pliku
    fclose(plik);

    printf("Dokonano zmian w pliku config.txt.\n");
    printf("\n\n");
    return 0;
}

void readAboutProgram() {
    //wyswietlenie podstawowych informacji o programie
    printf("\n\n");
    printf("--------------------------\n");
    printf("Ten program symuluje wzrost roslin.\n");
    printf("Poszczegolne znaki sa uzywane:\n");
    printf("X - zmienna do zmiany zasad\n");
    printf("F - zmienna do zmiany zasad, krok w przod przy rysowaniu\n");
    printf("+ - zmiana o kat w lewo\n");
    printf("- - zmiana o kat w prawo\n");
    printf("[ - zapisanie obecnego polozenia przy rysowaniu\n");
    printf("] - powrot do ostatniego zapisanego polozenia\n");
    printf("Program sam sczytuje dane z pliku config.txt.\n");
    printf("Istnieje mozliwosc zmiany pliku config.txt z poziomu menu (1).\n");
    printf("Symulacje uruchamia sie z poziomu menu (4).\n");
    printf("Przy uruchomieniu symulacji jest mozliwosc zapisu wyniku:\n");
    printf(" - do pliku tekstowego - output.txt\n");
    printf(" - jako graficzna reprezentacje - out.bmp\n");
    printf("Oraz bezposrednie wypisanie wyniku w konsoli\n");
    printf("\n\n");
}


int startSimulation() {
    printf("\n\n");
    printf("--------------------------\n");
    //srand zapewnia losowosc
    srand(time(0));
    char ruleX[100], ruleF[100], start, sun, other, color[100], save, display, save_turtle, random_generate;
    int iterations, step;
    float angle;
    //otwarcie pliku w trybie do odczytu
    FILE *plik = fopen("config.txt", "r");
    if (plik == NULL) {
        printf("blad pliku konfiguracyjnego\n");
        return 100;
    }
    //wczytanie configu z pliku
    loadConfig(plik, ruleX, ruleF, &start, &iterations, &angle, &random_generate, &sun, &other, color, &step);
    //zamkniecie pliku
    fclose(plik);
    //poczatek listy
    Node *root = NULL;
    //dodanie "startu" (zmienna z configu) do listy
    //to jest pierwszy element listy
    root = append(root, start);

    //petla dodajaca elementy do listy
    //na podstawie zasad symulacji
    //tyle ile jest iteracji podanych w configu
    for (int i = 0; i < iterations; i++) {
        //zastosowanie zasad symulacji
        Node *newElement = applyRules(root, ruleX, ruleF, sun, random_generate);
        //zwolnienie pamieci poprzedniej listy
        freeList(root);
        root = newElement;
    }
    //otwarce pliku do zapisu wyniku
    FILE *zapis = fopen("output.txt", "w");
    if (zapis == NULL) {
        printf("blad pliku wyjsciowego\n");
        return 100;
    }
    //zapis wyniku do pliku na zyczenie uzytkownika
    printf("Czy chcesz zapisac wynik symulacji (koncowy wzor) do pliku?(y/n)\n");
    scanf(" %c", &save);
    if (save == 'y') {
        Node *temp = root;
        while (temp != NULL) {
            fprintf(zapis, "%c", temp->value);
            temp = temp->next;
        }
    }
    fclose(zapis);
    
    //zapis wyniku do pliku graficznego na zyczenie uzytkownika
    printf("Czy chcesz zapisac graficzna reprezentacje symulacji w pliku bmp?(y/n)\n");
    scanf(" %c", &save_turtle);
    if (save_turtle == 'y') {
        //rysowanie rosliny
        //jezeli uzytkownik zdecyduje sie na zapis
        drawPlant(root, angle, step, color, other);
    } else {
        turtle_cleanup();
    }
    //wyswietlenie wyniku w terminalu na zyczenie uzytkownika
    printf("Czy chcesz zobaczyc wynik symulacji (koncowy wzor) w terminalu?(y/n)\n");
    scanf(" %c", &display);
    if (display == 'y') {
        printf("wybrano (y) oto wynik twojej symulacji:\n");
        //wywolanie funkcji wypisujacej liste
        printList(root);
    }
    printf("\n");
    //wypsianie informacji o zapisie wyniku
    //i powrot do menu glownego
    if (display == 'y') printf("Powyzej przedstawiono wynik symulacji w terminalu\n");
    printf("Dziekuje za przeprowadzenie symulacji\n");
    if (save == 'y') printf("Wynik zapisany w pliku o nazwie: output.txt\n");
    if (save_turtle == 'y') printf("Wynik w postaci graficznej reprezentacji zapisany w pliku o nazwie: out.bmp\n");
    printf("Powracasz do menu glownego");
    //zwolnienie pamieci listy
    freeList(root);
    printf("\n\n");
    return 0;
}

void loadConfig(FILE* plik, char* ruleX, char* ruleF, char* start, int* iterations, float* angle, char* random_generate, char* sun, char* other, char* color , int* step) {
    //wczytanie wartosci z pliku
    fscanf(plik, "ruleX=%s\n", ruleX);
    fscanf(plik, "ruleF=%s\n", ruleF);
    fscanf(plik, "start=%c\n", start);
    fscanf(plik, "iterations=%d\n", iterations);
    fscanf(plik, "angle(deg)=%f\n", angle);
    fscanf(plik, "random_generate(y/n)=%c\n", random_generate);
    fscanf(plik, "sunlight_direction(l/r/c)=%c\n", sun);
    fscanf(plik, "other_plants(y/n)=%c\n", other);
    fscanf(plik, "color(red/green/blue/black)=%s\n", color);
    fscanf(plik, "step=%d\n", step);
}

Node* append(Node *head, char value) {
    //stworzenie nowego elementu listy
    Node *newNode = malloc(sizeof(Node));
    //przypisanie wartosci
    newNode->value = value;
    newNode->next = NULL;
    //jezeli lista jest pusta to zwracam od razu nowy element
    if (head == NULL) {
        return newNode;
    }
    //jezeli nie to dodaje na koniec listy nowy element
    Node *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    //zwracam poczatek listy juz z nowym elementem
    return head;
}


Node* applyRules(Node *head, char* ruleX, char* ruleF, char sun, char random_generate) {
    //musi byc long long unsigned int bo inaczej warning
    //przez strlen
    long long unsigned int i;
    int random_number;
    //nowa lista
    Node *newList = NULL;
    Node *temp = head;
    while (temp != NULL) {
        //losowanie liczby z przedzialu 1-10
        random_number = (rand() % 10) + 1;
        //dodawanie elementow do nowej listy
        //na podstawie zasad symulacji
        //jezeli znak jest X to dodaje zasady X
        if (temp->value == 'X') {
            for (i = 0; i < strlen(ruleX); i++) {
                newList = append(newList, *(ruleX + i));
            }
        } 
        //jezeli znak jest F to dodaje zasady F
        else if (temp->value == 'F') {
            for (i = 0; i < strlen(ruleF); i++) {
                newList = append(newList, *(ruleF + i));
            }
        }
        //kiedy slonce swieci z prawej strony to jest 60% szans na zmiane kierunku w prawo
        else if (sun == 'r' && temp->value == '+' && random_number <= 6) {
            newList = append(newList, '-');
        }
        //kiedy slonce swieci z lewej strony to jest 60% szans na zmiane kierunku w lewo
        else if (sun == 'l' && temp->value == '-' && random_number <= 6) {
            newList = append(newList, '+');
        }
        //w innym przypadku przepisuje znak z listy
        else {
            newList = append(newList, temp->value);
        }
        //jezeli losowosc generowania jest wlaczona
        //to jest 10% szans na zmiane znaku + na -
        if (random_generate == 'y' && temp->value == '-' && random_number <= 1) {
            newList = append(newList, '+');
        }
        if (random_generate == 'y' && temp->value == '+' && random_number <= 1) {
            newList = append(newList, '-');
        }
        temp = temp->next;
    }
    //zwracam nowa liste
    return newList;
}


void printList(Node *head) {
    //wypisanie listy na ekran
    //przechodzenie po liscie i wypisywanie znakow
    while (head != NULL) {
        printf("%c", head->value);
        head = head->next;
    }
    printf("\n\n");
}

Position* pushPosition(Position *head, float x, float y, float direction) {
    //twozenie nowego elementu 
    Position *newPosition = malloc(sizeof(Position));
    //przypisanie wartosci
    newPosition->x = x;
    newPosition->y = y;
    newPosition->direction = direction;
    //dodanie NA POCZATEK listy nowego elementu
    newPosition->next = head;
    return newPosition;
}


Position* popPosition(Position *head, float *x, float *y, float *direction) {
    //jezeli lista jest pusta to zwracam NULL
    if (head == NULL) {
        return NULL;
    }
    //nowy element zaczyna sie od drugiego elementu
    Position *next = head->next;
    //przypisanie wartosci z pierwszego elementu
    *x = head->x;
    *y = head->y;
    *direction = head->direction;
    //zwolnienie pamieci pierwszego elementu
    free(head);
    //zwracam od drugiego elementu listy
    return next;
}


void drawPlant(Node *head, float angle, int step, char* color, char other) {
    //rysowanie rosliny
    int random;
    turtle_init(1920, 1920);
    turtle_set_heading(90);
    turtle_pen_up();
    turtle_goto(0, -950);
    turtle_pen_down();
    //ustawienie koloru
    //jezeli uzytkownik poda zly kolor to ustawiam domyslny czarny
    //stricmp - porownanie bez wzgledu na wielkosc liter
    //jezeli wyraz jest taki sam to zwraca 0
    //wiec zmieniam kolor na wybrany jezeli jest taki sam
    if (strcmp(color, "red") == 0) {
        turtle_set_pen_color(255, 0, 0);
    } else if (strcmp(color, "green") == 0) {
        turtle_set_pen_color(0, 255, 0);
    } else if (strcmp(color, "blue") == 0) {
        turtle_set_pen_color(0, 0, 255);
    } else if (strcmp(color, "black") == 0) {
        turtle_set_pen_color(0, 0, 0);
    } else {
        printf("Niepoprawny kolor, ustawiono domyslny kolor czarny\n");
        turtle_set_pen_color(0, 0, 0);
    }
    //nowa lista do zapisywania pozycji
    Position *positions = NULL;
    //kierunek poczatkowy (jak obrocony jest zolw na poczatku)
    float currentDirection = 90;
    Node *temp = head;
    //przechodzenie po glownej liscie
    while (temp != NULL) {
        //losowanie liczby z przedzialu 1-10
        random = (rand() % 10) + 1;
        //inne rysowanie wg wybranej opcji w configu
        if (other == 'y' && random <= 5 && angle > 10 && step > 5) {
            angle = 12;
            step = 10;
        }
        //zastosowanie zasad symulacji
        //jezeli znak jest X to nic nie robie
        //jezeli znak jest F to idz do przodu
        if (temp->value == 'F') {
            turtle_forward(step);
        }
        //jak - to obracam w prawo
        else if (temp->value == '-') {
            turtle_turn_right(angle);
            currentDirection -= angle;
        }
        //jak + to obracam w lewo
        else if (temp->value == '+') {
            turtle_turn_left(angle);
            currentDirection += angle;
        }
        //jak [ to zapisuje obecna pozycje uzywajac funkcji pushPosition
        else if (temp->value == '[') {
            positions = pushPosition(positions, turtle_get_x(), turtle_get_y(), currentDirection);
        }
        //jak ] to zdejmuje ostatnia zapisana pozycje uzywajac funkcji popPosition
        else if (temp->value == ']') {
            float x, y, direction;
            //wczytuje wartosci ktore byly ostatnio zapisane
            positions = popPosition(positions, &x, &y, &direction);
            //podnosze pioro zeby nie rysowac
            turtle_pen_up();
            //zmieniam polozenie zolwia
            turtle_goto(x, y);
            //ustawiam kierunek zolwia
            turtle_set_heading(direction);
            //aktualizuje obecny kierunek zeby na nowo moc go zmieniac
            currentDirection = direction;
            //opuszczam pioro zeby znowu rysowac
            turtle_pen_down();
        }
        //przechodze do nastepnego elementu listy
        temp = temp->next;
    }
    //zapisuje wynik do pliku bmp
    turtle_save_bmp("out.bmp");
    turtle_cleanup();
    //zwolnienie pamieci listy
    free(positions);
    positions = NULL;
    free(temp);
    temp = NULL;
}

void freeList(Node *head) {
    //zwolnienie pamieci listy
    Node *temp;
    //przechodzenie po liscie i zwalnianie pamieci
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
