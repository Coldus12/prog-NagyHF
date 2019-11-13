#include <stdio.h>
#include <stdlib.h>

typedef struct ListaElem {
    int adat;
    struct ListaElem *kov;
} ListaElem;

ListaElem *lista_letrehoz(void) {
    int szamok[] = { 8, 14, 13, 17, 1, 19, 16, 5, 3, 11, 2,
                     15, 9, 10, 6, 22, 4, 7, 18, 27, -1 };
    ListaElem *lis = NULL;

    for (int i = 0; szamok[i] != -1; ++i) {
        ListaElem *u;
        u = (ListaElem*) malloc(sizeof(ListaElem));
        u->kov = lis;
        u->adat = szamok[i];
        lis = u;
    }
    return lis;
}

void free_lista(ListaElem* eleje) {
    ListaElem *mozgo = eleje;
    while (mozgo != NULL) {
        ListaElem *kov = mozgo->kov;
        free(mozgo);
        mozgo = kov;
    }
}

int lista_meret(ListaElem* eleje) {
    int ret = 0;
    ListaElem* mozgo;
    for (mozgo = eleje; mozgo != NULL; mozgo = mozgo->kov) {
        ret++;
    }

    return ret;
}

ListaElem* lista_elejere_beszur(ListaElem *eleje, int ertek) {
    ListaElem *beszurando;
    beszurando = (ListaElem*) malloc(sizeof(ListaElem));
    beszurando->adat = ertek;
    beszurando->kov = eleje;
    return beszurando;
}

void lista_vegehez_fuz(ListaElem* eleje, int ertek) {
    ListaElem* hozzafuzendo;
    hozzafuzendo = (ListaElem*) malloc(sizeof(ListaElem));
    hozzafuzendo->adat = ertek;
    hozzafuzendo->kov = NULL;

    ListaElem* mozgo;
    ListaElem* vegelotti = NULL;
    for (mozgo = eleje; mozgo != NULL; mozgo = mozgo->kov)
        vegelotti = mozgo;
    if (vegelotti != NULL)
        vegelotti->kov = hozzafuzendo;
}

ListaElem* lista_keres(ListaElem* eleje, int keresett_ertek) {
    ListaElem* mozgo;
    for (mozgo = eleje; mozgo != NULL; mozgo = mozgo->kov) {
        if (keresett_ertek == mozgo->adat)
            return mozgo;
    }
    return NULL;
}

int main() {
    ListaElem *eleje;
    eleje = lista_letrehoz();

    eleje = lista_elejere_beszur(eleje, 2222);
    lista_vegehez_fuz(eleje, 1234);

    ListaElem* keresett = lista_keres(eleje, 15);
    if (keresett != NULL)
        keresett->adat = 1337;

    for (ListaElem* mozgo = eleje; mozgo != NULL; mozgo = mozgo->kov) {
        printf("%d ", mozgo->adat);
    }
    printf("\n");
    printf("Listameret: %d\n", lista_meret(eleje));
    //Azert vannak forditott sorrendben, mert a lista elejere a
    //rakjuk az uj elemeket, igy az elso elem a lista vegere kerul majd

    free_lista(eleje);
    return 0;
}