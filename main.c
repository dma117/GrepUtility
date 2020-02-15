#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 500

char array[MAX_SIZE];
FILE *fin;
int position = 1;
int iPattern;
int counter = 0;
int counterDOC = 0;
char OPTIONS[5];
long valueForMoptions = -1;
unsigned indexM;
int arr_m[MAX_SIZE] = {0};

void search_options(int, char *[]);
void search_pattern(int, char *[]);
void search_documents(int, char *[]);
void output_file_name(char *[], int);
int checkCorrectOptions(char *[], int);
char search_doubleH_option(int, char *[], int);
void check_Moption(int, char *[], int);
void unknown_function(char *[], int);

int main(int argc, char *argv[]) {
    fin = stdin;

    search_pattern(argc, argv);
    search_options(argc, argv);
    search_documents(argc, argv);

    if (position == argc)
        position--;

    for (int i = position; i < argc; i++) {
        if (arr_m[i] != 0 && counterDOC) //not file
            continue;

        if (argv[i][0] != '-' || !counterDOC) {
            if (counterDOC != 0) {
                fin = fopen(argv[i], "r");

                //Если файла нет - реализовать функцией
                if (fin == NULL) {
                    printf("grep: %s: No such file or directory\n", argv[i]);
                    continue;
                }
            }

            while ((valueForMoptions != counter) && fgets(array, 499, fin) != NULL) {
                if ( (!strchr(OPTIONS, 'v') && strstr(array, argv[iPattern]))
                     || (strchr(OPTIONS, 'v') && !strstr(array, argv[iPattern])))
                    unknown_function(argv, i);
            }

            if (strchr(OPTIONS, 'c')) {
                output_file_name(argv, i);
                printf("%d\n", counter);
                counter = 0;
            }
        }
    }

    return 0;
}

void search_options(int argc, char *argv[]) {
    int k = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (checkCorrectOptions(argv, i)) {
                if (!strchr(OPTIONS, argv[i][1])) {
                    if ((argv[i][1] == 'h') || (argv[i][1] == 'H')) {
                        OPTIONS[k++] = search_doubleH_option(argc, argv, i);
                    }
                    else {
                        OPTIONS[k++] = argv[i][1];
                    }
                }
                if (argv[i][1] == 'm') {
                    check_Moption(argc, argv, i);
                    if (!strchr(OPTIONS, 'm'))
                        OPTIONS[k++] = argv[i][1];
                }
            }
        }
    }
}

void search_pattern(int argc, char *argv[]) {
    int i = 1;
    M1: while (i < argc && argv[i][0] == '-') {
    i++; position++;
}
    if (i - 1 >= 1)
        if (argv[i - 1][1] == 'm') {
            i++;
            position++;
            goto M1;
        }

    if (argv[position] != NULL)
        iPattern = position++;
    else {
        printf("grep: wrong command --\nИспользование: grep [ПАРАМЕТР]… ШАБЛОН [ФАЙЛ]…");
        exit(1);
    }
}

void search_documents(int argc, char *argv[]) {
    for (int i = position; i < argc; i++) {
        if (argv[i][0] != '-' && arr_m[i] == 0)
            counterDOC++;
    }
}

char search_doubleH_option(int argc, char *argv[], int index) {
    char option = argv[index][1];
    index++;
    for (; index < argc; index++) {
        if ( (argv[index][1] == 'h') || (argv[index][1] == 'H') ) {
            option = argv[index][1];
        }
    }
    return option;
}

void output_file_name(char *argv[], int index) {
    if (counterDOC > 1 && (!strchr(OPTIONS, 'h') || strchr(OPTIONS, 'H')))
        printf("<%s>: ", argv[index]);
    if (counterDOC == 1 && strchr(OPTIONS, 'H'))
        printf("<%s>: ", argv[index]);
    if ( (!counterDOC) && strchr(OPTIONS, 'H') )
        printf("Standart input: ");
}

int checkCorrectOptions(char *argv[], int i) {
    if (strlen(argv[i]) == 2 && (argv[i][1] == 'h' || argv[i][1] == 'v' || argv[i][1] == 'c' || argv[i][1] == 'm' || argv[i][1] == 'H'))
        return 1;
    else
        printf("grep: unknown option -- %s\n\nИспользование: grep [ПАРАМЕТР]… ШАБЛОН [ФАЙЛ]…", argv[i]);

    exit(1);
}

void check_Moption(int argc, char *argv[], int index) {
    indexM = index + 1;
    if (indexM >= argc) {
        puts("grep: неверное максимальное количество совпадений");
        exit(1);
    }
    arr_m[index]++;
    arr_m[indexM]++;
    index += 2;
    if (index == iPattern)
        arr_m[index]++;

    for (unsigned long k = 0; k < strlen(argv[indexM]); k++) {
        if (!(argv[indexM][k] >= '0' && argv[indexM][k] <= '9')) {
            puts("grep: неверное максимальное количество совпадений");
            exit(1);
        }
    }
    valueForMoptions = strtol(argv[indexM], 0, 10);
}

void unknown_function(char *argv[], int i) {
    if (strchr(OPTIONS, 'm'))
        if (valueForMoptions == counter) {
            if (!strchr(OPTIONS, 'c')) {
                counter = 0;
                return;
            } else {
                goto M2;
            }
        }

    counter++;

    M2: if (!strchr(OPTIONS, 'c')) {
    output_file_name(argv, i);
    printf("%s", array);
    }
}