//
// Created by coldus on 11/5/19.
//
#ifndef renderer
#define renderer

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>
#include <time.h>

#include "map.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;

/*!Az "interRenderPoint" a program egyik, ha nem a legfontosabb függvénye.
 * Ez a függvény felel azért, hogy a pontok amelyek a térben vannak a Camera
 * által meghatározott síkra legyenek vetítve.
 *
 * A matek a függvény mögött csupán annyi, hogy van egy X, Y, Z koordinátájú
 * pontunk a térben, és a kameránk koordinátái pedig A, B, C, és a sík/téglalap
 * amire vetíteni szeretnénk D távolságra helyezkedik el a kamera koordinájától a
 * Z tengelyen. Így amikor a pont vetületét szeretnénk látni a "síkunkon", akkor
 * csak valójában az érdekel minket, hogy ha a kamera pontját összekötnénk egy egyenessel,
 * akkor az az egyenes hol metszené el a síkot.
 *
 * Ezt úgy csinálhatjuk, hogy megnézzük mennyi ennek az egyenesnek a meredeksége,
 * majd ezt a meredekséget D-vel beszorozzuk D-vel.
 *
 * Tehát ha dX = (X - A) (azaz az x tengelyen vett koordináták különbsége)
 * és dY = (Y - B), és dZ = (Z - C), akkor feltéve, hogy a síkon a 0,0 pontot
 * a sík középpontjának választottuk, akkor a vetített pont x, és y (síkon vett)
 * koordinátáit úgy kapjuk, hogy: x = D*(dX/dZ), és y = D*(dY/dZ).
 * */

Point interRenderPoint(Camera cam, Point p) {

    Point rotated;
    rotated = p;

    /*! Az interRenderPoint függvényben a pontot még mielőtt "rávetítenénk" a síkra
     * előbb elforgatjuk. Ezt azért tesszük, mert a kamerát valójában nem forgatjuk,
     * hanem ha a kamera forog, akkor mindennek a képét elforgatjuk úgy, mintha a kamera
     * forgott volna.
     *
     * Például ha egy pont pontosan a kamera alatt van, de a kamerát épp elforgatjuk úgy,
     * hogy az éppen maga alá nézzen, akkor az annak felel meg, mintha a pontot forgattuk
     * volna be a kamera elé.
     * */

    rotate_Point_around_Point(cam.location, &rotated, cam.rotX, cam.rotY, cam.rotZ);

    Point vetulet;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((rotated.posY - cam.location.posY)/(rotated.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((rotated.posX - cam.location.posX)/(rotated.posZ - cam.location.posZ));

    if (rotated.posZ - cam.location.posZ <= 5) {
        vetulet.posZ = -1;
    } else {
        /*! Természetesen a képernyő úgy működik, hogy a bal-felső sarka a 0,0 pont,
         * és lefelé megy az y tengely, és jobbra az x tengely, így ahhoz, hogy a koordináták
         * jók legyenek, át kell alakítanunk őket oly módon, hogy eddig a képernyő (ebben az esetben
         * az ablak) kozéppontját vettük a origónak, és az y tengely fölfelé mutatott.
         * Az x tengely átalakítása valójában csak annyi, hogy eltoljuk az ablak szélességének felével.
         * Ezzel szemben az y-t nem elég az ablak magasságának felével eltolni, hiszen a képernyő y tengelye
         * ellentétes irányú, így az y ellentetjét kell vennünk, és ahhoz kell hozzáadnunk az
         * ablak magasságának a felét.
         */

        vetulet.posY = ((double) cam.planeSizeY / 2) - relativY;
        vetulet.posX = ((double) cam.planeSizeX / 2) + relativX;
    }

    return vetulet;
}

/*! A renderTriangle függvény felelős azért hogy ne csak pontokat, hanem háromszögeket
 * (amikből tulajdonképpen minden objuktum felépül) is megtudjunk jeleníteni.
 * Ez a függvény úgy működik, hogy vesszük a háromszög három pontját, majd mind a háromra
 * lefuttatjuk az interRenderPoint függvényt, majd az így kapott három pontra rajzoljuk
 * ki a megfelelő színű háromszöget.
 *
 * Fontos kiegészítés: A függvény képes nem wireframe-módon megjeleníteni háromszögeket, azaz
 * képes színes háromszögek kirajzolására, viszont, mint kiderült, ha színes háromszögeket rajzol ki,
 * akkor az drasztikusan befolyásolja a futási időt. (Jelen esetben ez azt jelenti, hogy a játék
 * 50 FPS-re van állítva, de ha színes háromszögeket rajzol ki, akkor 1 másodperc alatt csak 18-20 képet
 * képes kirajzolni, ami azért probléma, mert a program egy thread-en fut, így a lassú a kirajzolás
 * és szaggat, akkor a program maga is lelassul, és nehezebben/lasabban fogadja be az inputot.
 * */
void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer) {
    //------------------------------------------------------------------------------------------------------------------
    Point p1, p2, p3;

    p1 = interRenderPoint(cam, tri.p1);
    p2 = interRenderPoint(cam, tri.p2);
    p3 = interRenderPoint(cam, tri.p3);

    if (p1.posZ != -1 && p2.posZ != -1 && p3.posZ != -1) {
        //filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
        trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
    }
}

//Külön, RenderList-en kívűli objektum renderelése
/*! A renderObject függvény arra szolgál, hogy egy objektumot külön jelenítsünk meg.
 * Ebben a függvényben nincs rendezés, így a háromszögeket mindig abban a sorrendben
 * fogja kirajzoni, amiben az Modelbe be lettek olvasva.
 * Ezt a függvény nem érdemes használni, csak akkor, ha wireframe-ben jelenítjük
 * meg az objektumot.
 * */
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    int a = object.model.triangleArray.size;
    for (int i = 0; i < a; i++) {
        renderTriangle(object.model.triangleArray.triangles[i], cam, SDL_renderer);
    }
}

//                                              RenderArray
//----------------------------------------------------------------------------------------------------------------------

/*! Az általam írt 3 dimenziós grafikus megjelenítőnek hibája a sebessége.
 *
 * Ezt először annak tulajdonítottam, hogy nem hatékony algoritmusokat írtam,
 * és használtam. Hiszen nem hatékony az, hogy a renderList-et, ami egy láncolt lista,
 * buborékrendezéssel rendezem, oly módon, hogy amikor i és j-edik elemet ki szeretném cserélni,
 * akkor végig kell rohanni a listán i-ig és j-ig, és utána lehet csak őket kicserélni.
 *
 * Ezért itt bevezettem a RenderArray struktúrát, és megírtam a RenderList összes függvényét
 * erre a RenderArray-re is. A RenderArray egy dinamikus tömb, így rendezése egyszerűbb, és
 * hatékonyabb.
 *
 * De sajnos ez sem segített a program sebességén, majd több órás debug-olás után derült ki az,
 * hogy a programot a gfxPrimitives "filledTrigonRGBA" függgvénye lassítja drasztikusan, ami
 * a futási idő megközelítőleg 2/3-át elviszi.
 *
 * Így miután eredetileg RenderList-et használtam, visszaáltam annak a használatára,
 * de lévén, hogy ezt külön könyvtárnak terveztem, ezért a RenderArray-t benne hagyom,
 * mert egyéb projekteknél jól jöhet.
 * */

typedef struct RenderArray{triangle **tri; int size} RenderArray;

void init_render_array(RenderArray* din_array, int size) {
    din_array->tri = (triangle**) malloc(size * sizeof(triangle*));
    din_array->size = size;
}

bool resize_render_array(RenderArray *din_array, int new_size) {
    triangle **new_triangle = malloc(new_size* sizeof(triangle*));

    for (int i = 0; i < new_size-1; i++) {
        new_triangle[i] = din_array->tri[i];
    }

    free(din_array->tri);
    din_array->size = new_size;
    din_array->tri = new_triangle;
}

void free_render_array(RenderArray* din_array) {
    free(din_array->tri);
}

void add_tri_to_RenderArray(RenderArray *ra, triangle* tri, double dist) {
    resize_render_array(ra,ra->size+1);
    tri->dist = dist;
    ra->tri[ra->size-1] = tri;
}

void add_Object_to_RenderArray(RenderArray *ra, Camera cam, Object *obj) {
    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        add_tri_to_RenderArray(ra, &obj->model.triangleArray.triangles[i], dist_btw_Points(cam.location, centroid_of_triangle(obj->model.triangleArray.triangles[i])));
    }
}

void addMapToRenderArray(RenderArray *ra,map *map1, Camera cam) {
    map *current = map1;
    while(current != NULL) {
        add_Object_to_RenderArray(ra,cam,&current->obj);
        current = current->next;
    }
}

/*! Ezt a gyorsrendező függvényt a(z)
 * https://beginnersbook.com/2015/02/quicksort-program-in-c/
 * oldalon lévő kód alapján írtam, azzal a céllal, hogy a program
 * sebességén javítsak.
 * */
void quicksort_on_render_array(RenderArray *ra, int min, int max) {
    int i, j, pivot;
    if(min < max){
        pivot = min;
        i = min;
        j = max;

        while(i<j){
            while(ra->tri[i]->dist >= ra->tri[pivot]->dist && i<max)
                i++;
            while(ra->tri[j]->dist < ra->tri[pivot]->dist)
                j--;
            if(i<j){
                triangle *tmp = ra->tri[i];
                ra->tri[i] = ra->tri[j];
                ra->tri[j] = tmp;
            }
        }

        triangle *temp = ra->tri[pivot];
        ra->tri[pivot] = ra->tri[j];
        ra->tri[j] = temp;

        quicksort_on_render_array(ra, min,j-1);
        quicksort_on_render_array(ra ,j+1, max);

    }
}

void update_distances_on_ra(RenderArray *ra, Camera cam) {
    for (int i = 0; i < ra->size-1; i++) {
        ra->tri[i]->dist = dist_btw_Points(cam.location, centroid_of_triangle(*ra->tri[i]));
    }
}

void render_ra(RenderArray *ra, Camera cam, SDL_Renderer *SDL_Renderer) {
    update_distances_on_ra(ra,cam);
    quicksort_on_render_array(ra, 0, ra->size-1);
    for (int i = 0; i < ra->size-1; i++) {
        if (cam.viewDistance > 0) {
            if (ra->tri[i]->dist > cam.viewDistance)
                return;
        }
        renderTriangle(*ra->tri[i],cam, SDL_Renderer);
    }
}


//                                              RenderList
//----------------------------------------------------------------------------------------------------------------------
/*! A RenderList struktúrára azért van szükség, mert sokkal egyszerűbb ha minden amit
 * megszeretnénk jeleníteni egy helyen van, és így nem kell egyenként minden objektumot, vagy kósza
 * háromszöget megjeleníteni, hanem elég ha egy a RenderList-en haladunk végig, és rajzoljuk ki a dolgokat.
 *
 * A RenderList egy másik előnye, hogy lehet rendezni. Ez azért fontos, mert így meg tudjuk azt tenni, hogy
 * a legtávolabbi dolgokat rajzoljuk ki először, és így elkerüljük azt, hogy ha valami közelebb van, mint
 * valami más, mégis rárajzoljuk az egyiket a másikra.
 * */
typedef struct RenderList{triangle *tri; double dist; bool visible; struct RenderList *next} RList;

/*! Az addtoRenderList függvény képes hozzáadni háromszögeket a RenderListhez.
 * */
RList* addtoRenderList(RList *head, triangle *tri, double dist, bool visible) {
    RList *newItem = (RList*) malloc(sizeof(RList));
    newItem->tri = tri;
    newItem->dist = dist;
    newItem->visible = visible;
    newItem->next = head;

    return newItem;
}

/*! Az addObjectToRenderList függvény végig halad egy objektum összes háromszögén, és azoknak a memória címét adja hozzá
 * a RenderList-hez. Ez azért fontos, hogy memória címet ad oda, mert így ha a háromszög pozíciója változik, akkor a
 * kirajzolt helyzete is változik.
 * */
RList* addObjectToRenderList(RList *head, Camera cam, Object *obj) {
    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        head = addtoRenderList(head, &obj->model.triangleArray.triangles[i], dist_btw_Points(cam.location, centroid_of_triangle(obj->model.triangleArray.triangles[i])), true);
    }
    return head;
}

RList* addMapToRenderList(RList *head, Camera cam, map *map1) {
    map *current = map1;
    while(current != NULL) {
        head = addObjectToRenderList(head, cam, &current->obj);
        current = current->next;
    }
    return head;
}

void freeList(RList *head) {
    RList *iter = head;
    while (iter != NULL) {
        RList *next = iter->next;
        free(iter);
        iter = next;
    }
}

//                                    Lista rendezése, és hozzátartozó függvények
//----------------------------------------------------------------------------------------------------------------------

/*! Feltehetőleg nem egy kifejezetten effektív függvény, de egyelőre nem tudom, hogyan lehetne kikerülni a használatát.
 * Ennek a függvénynek az a dolga, hogy keresztül szalad egy láncolt listán addig amíg el nem éri az X-edik elemet
 * majd azt az elemet adja vissza.
 *
 * Más szóval ez nagyjából azt csinálja, mintha lenne egy egyszerű tömb, és annak az X-edik elemével szeretnénk
 * valamit csinálni.
 * */
RList* getListItem(RList* head, int posInList) {
    RList* current = head;
    for (int i = 0; i < posInList; i++) {
        current = current->next;
    }
    return current;
}

/*! A swap függvény feladata, ahogy arra a neve is utal az, hogy egy láncolt listában (jelen esetben RenderListen)
 * két tetszőleges elemet kicseréljünk.
 *
 * Ehhez alap esetben legalább 4 pointer kell, 2 ami azokra az elemekre mutat, amiket ki szeretnénk cserélni,
 * és 2 ami pedig azokra az elemekre, amik a cserélendő elemek előtt állnak. És van egy ötödik, ami csak egy
 * ideiglenes pointer.
 *
 * Ha a pointereink c1, c2, p1, p2, temp, ahol c1, c2 a cserélendő elemre, a p1, p2, a cserélendő elemek előtti
 * elemekre mutatnak, akkor a csere úgy néz ki, mint egy tipikus csere:
 *
 * temp = c1->next;
 * c1->next = c2;
 * c2->next = temp;
 *
 * Majd a p1, p2-t rá mutatjuk az új "következő" elemre:
 * p1->next = c2;
 * p2->next = c1;
 *
 * Ezzel a cserével két esetben van probléma viszont: A) az egyik cserélendő elem a lista első eleme (ekkor nincs p1)
 *                                                    B) a két elem egymás mellett van (ez esetben sincs p1)
 *
 * A) esetben az a megoldás, hogy mindent úgyanúgy csinálunk, mint eddig, viszont most p1 helyett a lista eleje pointert
 * mutatjuk rá az új első elemre, és ezt is adjuk vissza a függvényből.
 *
 * B) esetben az a megoldás, hogy eggyel kevesebb pointert használunk, de ezt leszámítva a csere hasonlóan néz ki, mint
 * az általános esetben.
 * */
RList* swap(RList *head, int pos1, int pos2) {
    RList *ret = head;
    RList *prev1, *prev2, *c1, *c2;

    if (abs(pos1-pos2) > 1) {
        if (pos1 == 0) {
            prev2 = getListItem(head, pos2-1);
            c1 = head;
            c2 = prev2->next;
            ret = c2;

            prev2->next = c1;

            RList* temp = c1->next;
            c1->next = c2->next;
            c2->next = temp;
        } else if (pos2 == 0) {
            prev1 = getListItem(head, pos1-1);
            c2 = head;
            c1 = prev1->next;
            ret = c1;

            prev1->next = c2;

            RList* temp = c2->next;
            c2->next = c1->next;
            c1->next = temp;
        } else {
            prev1 = getListItem(head, pos1-1);
            prev2 = getListItem(head, pos2-1);

            c1 = prev1->next;
            c2 = prev2->next;

            RList* temp = c1->next;
            c1->next = c2->next;
            c2->next = temp;

            prev1->next = c2;
            prev2->next = c1;
            ret = head;
        }
    } else if (pos1 == pos2) {
        return  head;
    } else {
        if (pos1 == 0 || pos2 == 0) {
            c1 = getListItem(head, 0);
            c2 = c1->next;

            c1->next = c2->next;
            c2->next = c1;
            ret = c2;
        } else {
            if (pos1 > pos2) {
                prev2 = getListItem(head, pos2-1);
            } else {
                prev2 = getListItem(head, pos1-1);
            }

            c1 = prev2->next->next;
            c2 = prev2->next;

            RList *temp = c1->next;
            c1->next = c2;
            c2->next = temp;

            prev2->next = c1;
        }
    }

    return ret;
}


/*! A lista rendezése fontos, hiszen ha nincs rendezve, akkor lehet hogy kamerától távolabbi dolgokat
 * a kamerához közeli dolgokra rajzolunk.
 * Egyelőre a rendezés buborék rendezéssel van megoldva, ezt később ki szeretném cserélni, mert
 * valószínűleg nagyban lassítja a programot, főleg úgy, hogy itt a rendezésnél nagyon sokszor használjuk
 * a getListItem függvényt, no meg a swap függvényt, és feltehetőleg mind a kettő jelentősen ront
 * a teljesítményen, így ezek a jövőben javításra szorulnak.
 *
 * A listát egyébként úgy rendezzük, hogy minden háromszöghöz tartozik egy, a kamerától mért távolság.
 * Ezt a távolságot úgy kapjuk, hogy megnézzük a kamera, és a háromszög súlypontja közötti távolságot.
 * Később e távolság alapján rendezzük csökkenő sorrendbe a listát, hiszen azokat szeretnénk először
 * kirajzolni, amik távol vannak.
 * */
RList* bubble_sort_by_dist(RList *head) {
    int size = 0;
    RList *current = head;
    while (current != NULL) {
        size++;
        current = current->next;
    }

    for (int i = size-1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (getListItem(head, i)->dist >= getListItem(head, j)->dist) {
                head = swap(head, i, j);
            }
        }
    }

    return head;
}

RList* selection_sort_RList(RList* head) {
    int size = 0;
    RList *current = head;
    while (current != NULL) {
        size++;
        current = current->next;
    }

    for (int i = 0; i < size-1; ++i) {
        int minindex = i;
        for (int j = i+1; j < size; ++j)
            if (getListItem(head,j)->dist > getListItem(head,minindex)->dist)
                minindex = j;

        if (minindex != i) {
            head = swap(head, minindex, i);
        }
    }

    return head;
}

/*! Az update_distances függvényre azért van szükség, mert ha a kamera, vagy egy objektum helyzete
 * megváltozik, akkor a kettőjük közti távolsága is megváltozik, és ez alaján lehet, hogy a lista
 * sorrendjén is változtatni kell, így elengedhetetlen, hogy a RenderList megjelenítése elött
 * a távolságokat újra megvizsgáljuk, majd, ha szükséges rendezzük a listát.
 * */
RList* update_distances(RList* head, Camera cam) {
    RList *current = head;
    while (current != NULL) {
        Point centroid = centroid_of_triangle(*current->tri);
        current->dist = dist_btw_Points(cam.location, centroid);
        current = current->next;
    }
    return head;
}

//                                      RList render függvénye
//----------------------------------------------------------------------------------------------------------------------
/*! A render_RList függvény azt csinálja, amit a neve is sugall. Megjeleníti a RenderList-ben szereplő
 * háromszögeket a távolságuk alapján csökkenő sorrendben.
 *
 * Viszont még mielőtt ezt megtenné, először rendezi frissíti a háromszögek kamerától mért távolságát,
 * majd ez alapján rendezi a RenderList-et.
 * Ha a kamera látótávolsága kisebb mint 0, akkor azt a függvény úgy kezeli, mintha nem lenne látótávolság
 * beállítva, azaz mindent megjelenít.
 * Viszont ha nagyobb mint 0, akkor csak azokat a háromszögeket rajzolja ki, amelyek távolsága a kamerától
 * 0 és a látótávolság között van.
 * */
RList* render_RList(RList *head, Camera cam, SDL_Renderer *SDL_Renderer) {
    head = update_distances(head, cam);
    head = selection_sort_RList(head);
    RList *current = head;

    while (current != NULL) {
        if (cam.viewDistance > 0) {
            if (current->dist > cam.viewDistance) {
                return head;
            }
        }
        renderTriangle(*current->tri, cam, SDL_Renderer);
        current = current->next;
    }

    return head;
}

#endif
