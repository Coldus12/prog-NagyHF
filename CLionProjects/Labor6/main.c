#include <stdio.h>
#include <stdbool.h>

//szamtani sor fuggveny
int szamtaniSorN(int n, int kezdo, int diff) {
    if (n == 1) {
        return kezdo;
    } else {
        return szamtaniSorN(n-1, kezdo+diff, diff);
    }
}

//mertani sor fuggveny
int mertaniSorN(int n, int kezdo, int hanyados) {
    if (n == 1) {
        return kezdo;
    } else {
        return mertaniSorN(n-1, kezdo*hanyados, hanyados);
    }
}

//mertani vagy szamtani sor egyben
int mertaniSzamtaniN(int n, int kezdo, int diff, bool mertani) {
    if (mertani) {
        if (n == 1) {
            return kezdo;
        } else {
            return mertaniSzamtaniN(n-1, kezdo*diff, diff, true);
        }
    } else {
        if (n == 1) {
            return kezdo;
        } else {
            return mertaniSzamtaniN(n-1, kezdo+diff, diff, false);
        }
    }
}

//Fibonacci rekurziv fuggveny
int fibonacci(int n, int elso, int masodik) {
    if (n == 1) {
        return elso;
    } else if (n == 2) {
        return masodik;
    } else {
        return (fibonacci(n-1, elso, masodik) + fibonacci(n-2, elso, masodik));
    }
}

//Feladat4Fuggvenyei
void sztringet_kiir_1(char *szoveg) {
    if (szoveg[0] == '\0')
        return;
    putchar(szoveg[0]);
    printf("%s", szoveg + 1);     /* ! */
}


void sztringet_kiir_2(char *szoveg) {
    if (szoveg[0] == '\0')
        return;
    putchar(szoveg[0]);
    sztringet_kiir_2(szoveg + 1); /* ! */
}

//5. tomb elore es hatra
//Iterativ fuggveny
void iterativHatra(int tomb[], int meret, bool elore) {
    if (elore) {
        for (int i = 0; i < meret; i++)
            printf("%d ", tomb[i]);
        printf("\n");
    } else {
        for (int i = 0; i < meret; i++)
            printf("%d ", tomb[(meret-1)-i]);
        printf("\n");
    }
}

//Rekurziv fuggveny


int main() {
#ifdef Feladat2
    //printf("%d\n",szamtaniSorN(10,5,2));
    //printf("%d\n",mertaniSorN(10,5,2));
    for (int i = 1; i <= 10; i++) {
        printf("%4d ",mertaniSzamtaniN(i,5,2,false));
    }
    printf("\n");
    for (int i = 1; i <= 10; i++) {
        printf("%4d ",mertaniSzamtaniN(i,5,2,true));
    }
    printf("\n");
#endif

#ifdef FibonacciFeladat3
    printf("%d\n", fibonacci(10,0,1));
#endif

#ifdef Feladat4Szoveg
    sztringet_kiir_1("alma");
    sztringet_kiir_2("alma");
#endif

#ifndef Feladat5
    int tomb[] = {12, 5, 78, 9, 12, 54, 85, 45, 8, 10};
    iterativHatra(tomb, 10, true);

#endif

    return 0;
}