//
// Created by coldus on 12/1/19.
//

#ifndef records
#define records

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debugmalloc.h"
#include "debugmalloc-impl.h"

/*! Egy egyszerű struktúra, amely, mint a neve is sugallja,
 * arra szolgál, hogy legyen egy dinamikus valós számokat
 * tároló tömb.
 * */
typedef struct double_array{double* data; int size} DoubleArray;

void init_double_array(DoubleArray *array, int size) {
    double *newarray = malloc(size*sizeof(double));
    array->size = size;
    array->data = newarray;
}

/*! resize_double_array feladata az, hogy átméretezze a dinamikusan
 * foglalt valós számokat tároló tömböt.
 * */
void resize_double_array(DoubleArray *array, int new_size) {
    double *newarray = malloc(new_size*sizeof(double));

    int min = (array->size < new_size) ? array->size : new_size;
    for (int i = 0; i < min; i++) {
        newarray[i] = array->data[i];
    }

    free(array->data);
    array->data = newarray;
    array->size = new_size;
}

/*! Ez a függvény gondolkodás nélkül megnöveli egy dinamikusan foglalt
 * valós számokból álló tömbnek a méretét eggyel, és ide az új indexhez
 * illeszti be az új valós számot.
 * Így ezt a függvényt csak akkor érdemes használni, ha csak ezt a függvényt használjuk,
 * és nem szeretnénk oda figyelni arra, hogy mikor kell átmerétezni a tömböt.
 * */
void add_double_to_array(DoubleArray *da, double number) {
    resize_double_array(da, da->size+1);
    da->data[da->size-1] = number;
}

/*! Ennek a függvénynek csupán annyi a feladata, hogy felszabadítsa a dinamikusan
 * lefoglalt memóriaterületet.
 * */
void free_double_array(DoubleArray *array) {
    free(array->data);
}

/*!*/
void sort_double_array(double *array, int min, int max) {
    int i, j, pivot;
    double temp;
    if (min < max) {
        pivot = min;
        i = min;
        j = max;

        while (i<j) {
            while(array[i] <= array[pivot] && i<max)
                i++;
            while(array[j] > array[pivot])
                j--;
            if(i<j) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        temp = array[pivot];
        array[pivot] = array[j];
        array[j] = temp;

        sort_double_array(array, min, j-1);
        sort_double_array(array, j+1, max);
    }
}

/*! A megadott maphez betölti a rekord időket egy valós számokból álló tömbbe,
 * és ezt a tömböt adja vissza visszatérési értkként.
 * Fontos megjegyezni, hogy habár a függvényben MapName van, ide nem simán a pálya
 * nevét kell átadni, hanem a pálya neve után lévő .txt-t is.
 * */
DoubleArray load_time(char* MapName) {
    FILE *fp = fopen("dicsoseglista.txt", "r");

    char lines[100][100] = {0};
    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        strcpy(lines[i], line);
        i++;
    }

    char mapname[100] = {0};
    strncpy(mapname,MapName, sizeof(MapName)-4);

    bool is_it_in = false;
    int sorszam;

    for (sorszam = 0; sorszam < 99; sorszam++) {
        if (strstr(lines[sorszam],mapname) != NULL) {
            is_it_in = true;
            break;
        }
    }

    if (!strstr(lines[sorszam],mapname))
        sorszam = ++i;

    DoubleArray top;
    init_double_array(&top,0);
    int adatszam = 0;

    if (is_it_in) {
        for (int j = sorszam+2; strstr(lines[j],"End") == NULL; j++) {
            adatszam++;
            char nmbr[10];
            char str[10];
            sscanf(lines[j],"%s %s",nmbr,str);
            add_double_to_array(&top, strtod(str, NULL));
        }
    }

    fclose(fp);
    return top;
}

/*! A megfelelő pályához betölti az eddigi rekordokat, és hozzájuk adja az új időt, majd
 * ezt gyorsrendezéssel rendezi, és csak a legjobb 10-et menti el, így ha az új idő ebben
 * nincs benne, akkor az nem kerül mentésre.
 * */
void save_time(double time, char* path_to_map) {
    FILE *fp = fopen("dicsoseglista.txt", "r");
    FILE *temp = fopen("dicsoseglista.tmp","w");

    char lines[100][100] = {0};
    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        strcpy(lines[i], line);
        i++;
    }

    char mapname[100] = {0};
    strncpy(mapname,path_to_map, sizeof(path_to_map)-4);

    bool is_it_in = false;
    int sorszam;

    for (sorszam = 0; sorszam < 99; sorszam++) {
        if (strstr(lines[sorszam],mapname) != NULL) {
            is_it_in = true;
            break;
        }
    }

    if (!strstr(lines[sorszam],mapname))
        sorszam = ++i;

    DoubleArray top;
    init_double_array(&top,0);
    int adatszam = 0;

    if (is_it_in) {
        for (int j = sorszam+2; strstr(lines[j],"End") == NULL; j++) {
            adatszam++;
            char nmbr[10];
            char str[10];
            sscanf(lines[j],"%s %s",nmbr,str);
            add_double_to_array(&top, strtod(str, NULL));
        }
    }

    add_double_to_array(&top, time);

    sort_double_array(top.data,0,top.size-1);
    //------------------------------------------------------------------
    for (int k = 0; k <= sorszam+1; k++) {
        fprintf(temp,"%s",lines[k]);
    }

    int min = (top.size < 10) ? top.size : 10;
    for (int k = 0; k < min; k++) {
        fprintf(temp, "%d. %.2lf\n", k+1, top.data[k]);
    }

    for (int m = sorszam+2+adatszam; m < i; m++) {
        fprintf(temp, "%s",lines[m]);
    }

    free_double_array(&top);
    fclose(temp);
    fclose(fp);

    remove("dicsoseglista.txt");
    rename("dicsoseglista.tmp","dicsoseglista.txt");
}

#endif