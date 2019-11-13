#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//Feladat3 Dinamikus tomb "class"
//------------------------------------------------------
typedef struct din_array{int size; int *data} din_array;

bool din_array_alloc(din_array *da, int size) {
    da->size = size;
    da->data = (int*) calloc(size, sizeof(int));
    return da->data != NULL;
}

bool din_array_resize(din_array *da, int new_size) {
    da->size = new_size;
    da->data = realloc(da->data,new_size);
}

void din_array_free(din_array *da) {
    free(da->data);
}
//------------------------------------------------------

//Feladat4 Dinamikus string "class"
//------------------------------------------------------
typedef struct din_string{int size; char *data} din_string;

bool din_string_alloc(din_string *da, int size) {
    da->size = size;
    da->data = (char*) calloc(size, sizeof(int));
    return da->data != NULL;
}

bool din_string_resize(din_string *da, int new_size) {
    da->size = new_size;
    da->data = (char*) realloc(da->data,new_size);
}

void din_string_free(din_string *da) {
    free(da->data);
}
//------------------------------------------------------


int main() {

#ifdef Feladat12
    int valosDB;
    printf("Hany darab valos szamot olvvasson be a program?\n");
    scanf("%d", &valosDB);

    double *tomb;
    tomb = calloc(valosDB, sizeof(double));

    int i = 0;
    do {
        scanf("%lf", &tomb[i]);
        printf("tombi: %lf\n", tomb[i]);
        i++;
    } while (valosDB > i);

    //for (int j = valosDB; j > 0; j--) {
    //   printf("%0.2lf ", tomb[i]);
    //}

    for (int i = valosDB-1; i >= 0; i--) {
        printf("%0.1lf ", tomb[i]);
    }

    printf("\nThats it folks..\n");

    free(tomb);
#endif

#ifdef Feladat3
    printf("Addig olvasok be szamokat, ameddig nem kapok -1-et.\n");
    bool exit_condition = false;
    din_array da;
    int kezdetimeret = 0;
    din_array_alloc(&da, kezdetimeret);

    while(!exit_condition) {
        int temp;
        scanf("%d", &temp);
        if (temp == -1) {
            exit_condition = true;
            break;
        } else {
            din_array_resize(&da, ++kezdetimeret);
            da.data[kezdetimeret-1] = temp;
        }
    }

    for (int i = da.size-1; i >= 0; i--) {
        printf("%d ", da.data[i]);
    }
    printf("\n");

    din_array_free(&da);
#endif

#ifndef Feladat4
    bool exit_condition = false;
    din_string string;
    din_string_alloc(&string, 0);
    do {
        char temp;
        scanf("%c",&temp);
        if((char *) temp == "\n") {
            exit_condition = true;
            break;
        } else {

        }
    } while (!exit_condition);

    printf("%s", string);

    din_string_free(&string);
#endif

    return 0;
}