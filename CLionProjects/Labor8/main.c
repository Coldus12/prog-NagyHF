#include <stdio.h>
#include <stdlib.h>

typedef struct din_char_array{char* tomb; int meret} din_char_array;

void char_array_init(din_char_array* array, int meret) {
    array->tomb = (char*) calloc(meret, sizeof(char));
    array->meret = meret;
}

void char_array_free(din_char_array* array) {
    free(array->tomb);
}

void char_array_resize(din_char_array* array, int ujmeret) {
    array->tomb = (char*) realloc(array->tomb, ujmeret*sizeof(char));
}

void printString(din_char_array* array) {
    for (int i = 0; array->tomb[i] != '\0'; i++)
        printf("%c",array->tomb[i]);
    printf("\n");
}

//Egy dinamikus stringet kerunk be, majd azt masoljuk
//egy masik char tombbe, aminek a memoria teruletet mi
//foglaljuk le a heap-en, majd az erre a tombre mutato
//pointer adjuk vissza, de ezt majd kesobb fel kell
//szabadítanunk free()-vel.
char* strcopy(din_char_array* array) {
    char* ptr;
    ptr = (char*) calloc(array->meret, sizeof(char));

    for (int i = 0; array->tomb[i] != '\0'; i++) {
        ptr[i] = array->tomb[i];
    }
    ptr[array->meret] = '\0';

    return ptr;
}

char* str_fuz(char* str1, char* str2) {
    int size1, size2;
    for (size1 = 0; str1[size1] != '\0'; size1++);
    for (size2 = 0; str2[size2] != '\0'; size2++);

    int sum_size = size1 + size2 + 1;

    char* ptr = calloc(sum_size, sizeof(char));
    for (int i = 0; i < size1; i++)
        ptr[i] = str1[i];
    for (int j = size1; j < (size1 + size2); j++)
        ptr[j] = str2[j-size1];

    ptr[sum_size] = '\0';

    return ptr;
}

char* masol(char* str) {
    int size;
    for (size = 0; str[size] != '\0'; size++);

    char* ptr = (char*) calloc(size, sizeof(char));

    for (int i = 0; i < size; i++) {
        ptr[i] = str[i];
    }
    ptr[size] = '\0';

    return ptr;

}

char* strpart(char* tomb, int kezdo, int vegso) {
    char* ptr = (char*) calloc(vegso-kezdo, sizeof(char));
    for (int i = 0; i < vegso-kezdo; i++) {
        ptr[i] = tomb[kezdo+i];
    }
    ptr[vegso-kezdo] = '\0';

    return ptr;
}

char* strcut(char* tomb, int kezdo, int vegso) {
    int size, newsize;
    for (size = 0; tomb[size] != '\0'; size++);

    newsize = size - (vegso-kezdo);
    char* ptr = (char*) calloc(newsize, sizeof(char));

    for (int i = 0; i < kezdo; i++)
        ptr[i] = tomb[i];
    for (int i = 0; i < vegso; i++)
        ptr[kezdo+i] = tomb[vegso+i];

    ptr[newsize] = '\0';
    return ptr;
}

char* strinsert(char* str, int where, char* inserted_str) {
    int size;
    for (size = 0; str[size] != '\0'; size++);
    int size2;
    for (size2 = 0; inserted_str[size2] != '\0'; size2++);

    int sumsize = size + size2;
    char* ptr = (char*) calloc(sumsize, sizeof(char));

    for (int i = 0; i < where; i++)
        ptr[i] = str[i];

    for (int i = where; i < (size2 + where); i++)
        ptr[i] = inserted_str[i-where];

    for (int i = where+size2; i < sumsize; i++)
        ptr[i] = str[i-size2];

    ptr[sumsize] = '\0';

    return ptr;
}

int main() {

    //Feladat2
    //------------------------------------------------------
    din_char_array str;
    char_array_init(&str,10);
    str.tomb = "0123456789";

    printString(&str);

    //masolat keszitese
    char* cpy = strcopy(&str);

    printf("%s\n", cpy);

    //masolat felszabaditasa
    free(cpy);

    //Feladat3
    //------------------------------------------------------
    char* str1 = masol("Haho");
    //char* str2 = " vilag!";

    str1 = str_fuz(str1, " vilag!");

    printf("%s\n", str1);

    free(str1);

    //Feladat4
    //------------------------------------------------------
    char* reszlet;
    reszlet = strpart("Heyho vilag!", 9, 11);

    printf("%s\n", reszlet);

    free(reszlet);

    //Feladat5
    //------------------------------------------------------
    char* kivagva;
    kivagva = strcut("hello, vilag", 4, 8);

    printf("%s\n", kivagva);
    free(kivagva);

    //Feladat6
    //------------------------------------------------------
    char* uj;
    uj = strinsert("hello!", 5, ", vilag");
    printf("%s\n", uj);
    free(uj);
}