//
// Created by coldus on 11/17/19.
//
#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdio.h>
#include "object.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"
/*! A map struktúra valójában egy objektumokat tartalmazó láncolt lista.
 * Ez azért van így, mert amikor betöltünk egy pályát, akkor nem tudjuk elsőre, hogy
 * hány objektuma van a pályának, és így egyszerűen hozzá tudunk adni a listához
 * új objektumokat.
 * */
typedef struct object_list{Object obj; struct object_list *next} map;

/*! Modeleket tartalmazó láncolt lista nem szerepelt a specifikációban,
 * de szükség van rá, hiszen a programnak tudnia kell, hogy a pályán
 * lévő objektumoknak hol találja a model fájlját, ami alapján
 * ki tudja őket rajzolni.
 *
 * A struktúra tartalmazza a pályán megjelenő összes objektum modeljét, és a
 * modelhez tartozó rövid, "hivatkozási nevet".
 * A "hivatkozási név" az a név, ami a map fájlban szerepel az objektumok mellett.
 * */
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;

/*! Az invis_wall egy olyan struktúra, ami azért néz ki, és működik úgy, ahogy
 * mert nem gondoltam rá időben.
 *
 * Erre a struktúrára valójában nem lenne szükség, és a kezelésére sem, viszont akkor
 * a map fájl kezelése nem úgy nézne ki, mint ahogy az a specifikációban szerepel.
 *
 * Az invis_wall egy láncolt lista, melynek minden eleme tartalmaz két pontot,
 * egy relatív helyzetet meghatározó pontot, amire azért van szükség, hogy
 * ha műveletet szeretnénk a láthatatlan fallal csinálni, akkor el kell tolni
 * azt az origóba, viszont ehhez ismerni kell azt, hogy hol van a fal...
 * Ezért a lista összes eleme tartalmazza a láthatatlan fal középpontjának koordinátáit.
 * A második pont a fal egyik pontja.
 * Ennek a pontnak a fal jelenlegi műkédésének az esetében csak x, z koordinátákra van szükség.
 *
 * A láthatatlan fal úgy működik, hogy van egy n oldalú konvex sokszögünk (a síkban), amiről el lehet dönteni,
 * hogy egy pont a sokszögön belül, vagy kívül helyezkedik el, és magát az n oldalú sokszöget tekintjük a
 * láthatatlan falnak. Így ha egy pont nem ott van ahol lennie kellene (a sok szögön kívűl, vagy éppen belül)
 * akkor "ütközés" történt, és átment a pont a láthatatlan falon.
 * */
typedef struct invis_wall{Point location; Point p; struct invis_wall* next; double size} invis_wall;

//        Függvény, ami eldönti, hogy egy pont jobbra vagy balra van két másik pontoz képest (Jobb körüljárással)
//----------------------------------------------------------------------------------------------------------------------
/*! Ez a függvény felelős azért, hogy eldöntse egy pontról, hogy az egy egyenes
 * fölött, vagy alatt helyezkedik el, vagy jelen esetben körüljárást tekintve, az
 * egyenes jobb, vagy bal oldalán helyezkedik el.
 *
 * A függvény mögötti logika a következő: ha egy egyenes egyenletébe (0-ra rendezve) behelyettesítünk egy pontot, akkor
 * ha az 0-nál kisebb, akkor az egyenes alatt van a pont, ha nagyobb, akkor felette van, és ha 0, akkor az
 * egyenes egyik pontja a vizsgált pont.
 *
 * A jobb és bal oldalt pedig úgy vezettem be, hogy ha az első pont x koordinátája kisebb mint a másodiké, akkor bal
 * oldalt van az a pont ami az egyenes felett van, és jobb, ha alatt.
 * És ha ez nem igaz (az első pont x koordinátája nagyobb, mint a másodiké), akkor az a pont ami az egyenes fölött
 * van, az az egyenestől jobbra van, és ami alatta van, az pedig balra van.
 *
 * Ez a függvény azért fontos, mert egy n oldalú konvex sokszög esetén ha egy pontot a sokszög minden oldalára vizsgálva
 * mindig azt tapasztaljuk, hogy a pont az oldalak jobb oldalán van (jobb körüljárás esetén), akkor a pont a sokszögön
 * belül van, más esetben pedig a sokszögön kívűl.
 * */
bool is_it_left(Point first, Point second, Point pointInQuestion) {
    if (first.posX != second.posX) {
        double m = ((double) second.posZ - first.posZ) / (double) (second.posX - first.posX);
        double konstans = m * first.posX - first.posZ;
        if (first.posX < second.posX) {
            if(konstans > (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                return false;
            } else {
                return true;
            }
        } else {
            if(konstans < (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                return false;
            } else {
                return true;
            }
        }
    } else {
        if (pointInQuestion.posX < first.posX)
            return true;
        else
            return false;
    }
}

//                                         invis_wall függvényei
//----------------------------------------------------------------------------------------------------------------------
invis_wall* add_point_to_inviswall(invis_wall* head, Point new_point, Point location, double size) {
    invis_wall* new_item = malloc(sizeof(invis_wall));

    new_point.posX *= size;
    new_point.posY *= size;
    new_point.posZ *= size;

    new_point.posX += location.posX;
    new_point.posY += location.posY;
    new_point.posZ += location.posZ;

    new_item->size = size;
    new_item->p = new_point;
    new_item->next = NULL;

    if (head == NULL)
        return new_item;

    invis_wall* iter = head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    iter->next = new_item;
    return head;
}

void free_invis_wall(invis_wall* head) {
    invis_wall* iter = head;
    while (iter != NULL) {
        invis_wall* tmp = iter->next;
        free(iter);
        iter = tmp;
    }
}

/*! Az is_it_left függvény segítségével megvizsgálja, hogy egy pont (x,z koordinátákat figyelembe véve)
 * a láthatatlan falon (mint sokszögön) belül van-e.
 * */
bool point_inside_invis_walls(invis_wall* head, Point p) {
    invis_wall* iter = head;

    while (iter != NULL) {
        if (iter->next != NULL) {
            if (!is_it_left(iter->p, iter->next->p, p))
                return false;
        } else {
            if (!is_it_left(iter->p, head->p, p))
                return false;
        }
        iter = iter->next;
    }
    return true;
}

/*! Ez a függvény tulajdonképpen ugyanazt csinálja, mint a point_inside_invis_walls, csak éppen az ellenkező
 * esetben ad igazat.
 * */
bool point_outside_invis_walls(invis_wall* head, Point p) {
    invis_wall* iter = head;
    while (iter != NULL) {
        if (iter->next != NULL) {
            if (!is_it_left(iter->p, iter->next->p, p))
                return true;
        } else {
            if (!is_it_left(iter->p, head->p, p))
                return true;
        }
        iter = iter->next;
    }
    return false;
}

/*! Ez a függvény beolvas egy láthatatlan falat egy fájlból.
 * A láthatatlan fal fájlja úgy néz ki, hogy pontok x, z koordinátái vannak
 * egymás mellett space-el elválasztva, soronként egy pont x, z koordinátái
 * szerepelnek, bármi egyéb nélkül.
 * */
invis_wall* load_invis_wall_from_file(char* path, Point location, double size) {
    invis_wall* head = NULL;
    FILE *fp = fopen(path,"r");
    if (fp == NULL) {
        printf("Nem sikerült a fájl megnyitása");
        return NULL;
    }

    char line[100];
    while (fgets(line,sizeof(line), fp) != NULL) {
        Point new_point;
        char posx[20];
        char posz[20];
        sscanf(line, "%s %s", posx, posz);
        new_point.posY = 0;
        new_point.posX = strtod(posx,NULL);
        new_point.posZ = strtod(posz,NULL);

        head = add_point_to_inviswall(head,new_point, location, size);
    }

    fclose(fp);
    return head;
}

invis_wall* rotate_invis_wall(invis_wall* head, double rotX, double rotY, double rotZ) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        rotate_Point_around_Point(iter->location,&iter->p,rotX,rotY,rotZ);
    }
    return head;
}

invis_wall* move_invis_wall(invis_wall* head, Point new_location) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        double dx = new_location.posX-iter->location.posX;
        double dy = new_location.posY-iter->location.posY;
        double dz = new_location.posZ-iter->location.posZ;

        iter->p.posX += dx;
        iter->p.posY += dy;
        iter->p.posZ += dz;

        iter->location = new_location;
    }
    return head;
}

invis_wall* resize_invis_wall(invis_wall* head, double new_size) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        iter->p.posX -= iter->location.posX;
        iter->p.posY -= iter->location.posY;
        iter->p.posZ -= iter->location.posZ;

        iter->p.posX *= (new_size/iter->size);
        iter->p.posY *= (new_size/iter->size);
        iter->p.posZ *= (new_size/iter->size);

        iter->p.posX += iter->location.posX;
        iter->p.posY += iter->location.posY;
        iter->p.posZ += iter->location.posZ;

        iter->size = new_size;
    }
    return head;
}

void print_invis(invis_wall* head) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        printf("%.0lf %.0lf\n",iter->p.posX, iter->p.posZ);
    }
}

//                                             map függvényei
//----------------------------------------------------------------------------------------------------------------------
/*! A map láncolt listájához adja az új objektumot.
 * Erre a függvényre a map fájl beolvasásánál van szükség.
 * */
map* add_object_to_map(map *head, Object obj) {
    map *newItem = (map*) malloc(sizeof(map));
    newItem->obj = obj;
    newItem->next = head;

    return newItem;
}

void free_object_list(map *head) {
    map* iter = head;
    while (iter != NULL) {
        map *next = iter->next;
        free_object(&iter->obj);
        free(iter);
        iter = next;
    }
}

//                                          model_list függvényei
//----------------------------------------------------------------------------------------------------------------------
model_list* add_to_model_list(model_list *head, char* name, Model model) {
    model_list *newItem = (model_list*) malloc(sizeof(model_list));
    newItem->model = model;
    strcpy(newItem->name, name);
    newItem->next = head;

    return newItem;
}

/*! Beolvassa a model_list fájl-ból a modeleket, oly módon, hogy beolvassa a model "hivatkozási nevét"
 * majd az útvonalat a model fájljához, és ezeket betölti a memóriban lévő model_list láncolt lista
 * elemeibe.
 * */
model_list* load_model_list(char *path_to_location) {
    char line[200];
    FILE *fp = fopen(path_to_location,"r");
    model_list* head = NULL;
    char name[50];
    char path[150];

    while(fgets(line, sizeof(line), fp)) {
        Model model;

        sscanf(line, "%s %s", name, path);
        load_model_from_file(path, &model);
        head = add_to_model_list(head, name, model);
    }

    fclose(fp);
    return head;
}

void free_model_list(model_list *head) {
    model_list* iter = head;
    while (iter != NULL) {
        model_list *next = iter->next;
        free_model(&iter->model);
        free(iter);
        iter = next;
    }
}

/*! Végig fut a láncolt listán, oly módon, hogy ellenőrzi, hogy a lista tartalmazza-e a keresett model
 * hivatkozási nevét. Ha igen akkor true-val tér vissza, ha nem, akkor pedig false-al.
 * */
bool does_model_list_contain(model_list *head, char *name) {
    model_list *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Model load_from_list(model_list *head, char *name) {
    model_list *current = head;
    while(current != NULL) {
        if (strcmp(name, current->name) == 0) {
            return current->model;
        }
        current = current->next;
    }

}

//                                     Map betöltésének függvénye
//----------------------------------------------------------------------------------------------------------------------
/*! A fenti függvényeket használva a map fájlból betölti a pályát a gép memóriájába,
 * és minden objektumot rögtön úgy ad hozzá a láncolt listához, hogy azok már
 * a megfelelő koordinán helyezkednek el, és már megfelelően is vannak elforgatva.
 * */
map* load_map_from_file(char *filename, map *map, model_list modelList) {
    FILE *fp = fopen(filename, "r");

    char string[200];
    char name[50] = {0};
    char posX[50], posY[50], posZ[50], size[50], rotX[50], rotY[50], rotZ[50];
    int b;

    while(fgets(string, sizeof(string),fp) != NULL) {
        sscanf(string, "%s %s %s %s %s %s %s %s", name, posX, posY, posZ, size, rotX, rotY, rotZ);
        if (does_model_list_contain(&modelList, name)) {
            Object obj;
            obj.location.posX = (double) strtol(posX, NULL, 10);
            obj.location.posY = (double) strtol(posY, NULL, 10);
            obj.location.posZ = (double) strtol(posZ, NULL, 10);

            load_Model_into_Object(&obj, load_from_list(&modelList, name), strtod(size, NULL));

            rotate_Object_around_Point(obj.location, &obj, strtod(rotX, NULL), strtod(rotY,NULL), strtod(rotZ, NULL));

            map = add_object_to_map(map, obj);
        }
    }

    fclose(fp);
    return map;
}

#endif