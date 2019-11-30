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

/*TODO
 * A megjelenítéssel problémák vannak, mert ha a kamera az alsó háromszög súlypontjának magasságához (y tengelyen)
 * van közelebb (mintha az oldalsó háromszögek súlypontjához lenne), akkor az alsó háromszög egy kis része látszani fog...
 * Ez javítandó... Kérdés, hogy hogyan...
 * */

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

    if (rotated.posZ - cam.location.posZ <= 0) {
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

        vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
        vetulet.posX = ((double) cam.planeSizeX/2) + relativX;
    }

    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;

    return vetulet;
}

/*! A renderTriangle függvény felelős azért hogy ne csak pontokat, hanem háromszögeket
 * (amikből tulajdonképpen minden objuktum felépül) is megtudjunk jeleníteni.
 * Ez a függvény úgy működik, hogy vesszük a háromszög három pontját, majd mind a háromra
 * lefuttatjuk az interRenderPoint függvényt, majd az így kapott három pontra rajzoljuk
 * ki a megfelelő színű háromszöget.
 * */

/*void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;

    p1 = interRenderPoint(cam, tri.p1);
    p2 = interRenderPoint(cam, tri.p2);
    p3 = interRenderPoint(cam, tri.p3);

    if (p1.posZ != -1 && p2.posZ != -1 && p3.posZ != -1) {
        filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
        trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
    }
}*/

Point inter_render_behind(Camera cam, Point behind, Point p) {
    Point rotated;
    rotated = behind;

    rotate_Point_around_Point(cam.location, &rotated, cam.rotX, cam.rotY, cam.rotZ);

    Point vetulet;
    vetulet.posZ = 0;

    double relativY = (cam.location.posY - behind.posY) - ((p.posY - behind.posY)/(p.posZ - behind.posZ))*(cam.location.posZ - behind.posZ + cam.distanceFromPlane);
    double relativX = (cam.location.posX - behind.posY) - ((p.posX - behind.posX)/(p.posZ - behind.posZ))*(cam.location.posZ - behind.posZ + cam.distanceFromPlane);

    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;


    return vetulet;
}

/*void render_trig_onePoint_behind(Point pointBehind, Point p1, Point p2, Camera cam, SDL_Renderer *SDL_renderer, SDL_Color color) {
    Point temp1, temp2;
    temp1 = pointBehind;
    temp2 = pointBehind;

    double dx, dy, dz, realDist;
    dx = pointBehind.posX-p1.posX;
    dy = pointBehind.posY-p1.posY;
    dz = pointBehind.posZ-p1.posZ;
    realDist = (cam.location.posZ - pointBehind.posZ) + cam.distanceFromPlane;

    temp1.posX = ((double) cam.planeSizeX/2.0) + ((cam.location.posX - pointBehind.posX) - ((dx/dz)*realDist));
    temp1.posY = ((double) cam.planeSizeY/2.0) - ((cam.location.posY - pointBehind.posY) - ((dy/dz)*realDist));

    //temp1.posX = ((double) cam.planeSizeX/2.0 + (pointBehind.posX - cam.location.posX)) + (dx/dz)*realDist;
    //temp1.posY = ((double) cam.planeSizeY/2.0 + (cam.location.posY-pointBehind.posY)) - (dy/dz)*realDist;
    //temp1.posZ = cam.location.posZ + cam.distanceFromPlane;

    dx = pointBehind.posX-p2.posX;
    dy = pointBehind.posY-p2.posY;
    dz = pointBehind.posZ-p2.posZ;
    realDist = (cam.location.posZ - pointBehind.posZ) + cam.distanceFromPlane;

    temp2.posX = ((double) cam.planeSizeX/2.0) + ((cam.location.posX - pointBehind.posX) - ((dx/dz)*realDist));
    temp2.posY = ((double) cam.planeSizeY/2.0) - ((cam.location.posY - pointBehind.posY) - ((dy/dz)*realDist));
    //temp2.posZ = cam.location.posZ + cam.distanceFromPlane;

    //temp1 = interRenderPoint(cam, temp1);
    //temp2 = interRenderPoint(cam, temp2);
    p1 = interRenderPoint(cam, p1);
    p2 = interRenderPoint(cam, p2);

    filledTrigonRGBA(SDL_renderer,temp1.posX,temp1.posY,p1.posX,p1.posY,p2.posX,p2.posY,color.r, color.g,color.b,255);
    filledTrigonRGBA(SDL_renderer,temp2.posX,temp2.posY,p1.posX,p1.posY,p2.posX,p2.posY,color.r,color.g,color.b,255);
    filledTrigonRGBA(SDL_renderer,temp1.posX,temp1.posY,p1.posX,p1.posY,p2.posX,p2.posY,0, 0,255,255);
    filledTrigonRGBA(SDL_renderer,temp2.posX,temp2.posY,p1.posX,p1.posY,p2.posX,p2.posY,0,0,255,255);
}

void render_trig_twoPoint_behind(Point visiblepoint, Point behind1, Point behind2, Camera cam, SDL_Renderer *SDL_renderer, SDL_Color color) {
    double dx, dy, dz, realDist;
    dx = behind1.posX-visiblepoint.posX;
    dy = behind1.posY-visiblepoint.posY;
    dz = behind1.posZ-visiblepoint.posZ;
    realDist = (cam.location.posZ - behind1.posZ) + cam.distanceFromPlane;

    behind1.posX = ((double) cam.planeSizeX/2.0) + ((cam.location.posX - behind1.posX) - ((dx/dz)*realDist));
    behind1.posY = ((double) cam.planeSizeY/2.0) - ((cam.location.posY - behind1.posY) - ((dy/dz)*realDist));
    //behind1.posZ = cam.location.posZ + cam.distanceFromPlane;

    dx = behind2.posX-visiblepoint.posX;
    dy = behind2.posY-visiblepoint.posY;
    dz = behind2.posZ-visiblepoint.posZ;
    realDist = (cam.location.posZ - behind2.posZ) + cam.distanceFromPlane;

    behind2.posX = ((double) cam.planeSizeX/2.0) + ((cam.location.posX - behind2.posX) - ((dx/dz)*realDist));
    behind2.posY = ((double) cam.planeSizeY/2.0) - ((cam.location.posY - behind2.posY) - ((dy/dz)*realDist));
    //behind2.posZ = cam.location.posZ + cam.distanceFromPlane;

    //behind1 = interRenderPoint(cam, behind1);
    //behind2 = interRenderPoint(cam, behind2);
    visiblepoint = interRenderPoint(cam, visiblepoint);

    filledTrigonRGBA(SDL_renderer,behind1.posX,behind1.posY, behind2.posX,behind2.posY,visiblepoint.posX,visiblepoint.posY,color.r,color.g,color.b,255);
}*/

void render_trig_one_point_behind(SDL_Renderer *SDL_renderer, Camera cam, Point behind, Point p1, Point p2, SDL_Color color) {
    Point b1, b2;

    p1 = interRenderPoint(cam, p1);
    p2 = interRenderPoint(cam, p2);

    b1 = inter_render_behind(cam, behind, p1);
    b2 = inter_render_behind(cam, behind, p2);

    filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, b1.posX, b1.posY, color.r, color.g, color.b, 255);
    trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, b1.posX, b1.posY, 0, 0, 0, 255);

    filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, b2.posX, b2.posY, color.r, color.g, color.b, 255);
    trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, b2.posX, b2.posY, 0, 0, 0, 255);
}

void render_trig_two_point_behind(SDL_Renderer *SDL_renderer, Camera cam, Point behind1, Point behind2, Point p, SDL_Color color) {
    p = interRenderPoint(cam, p);

    behind1 = inter_render_behind(cam, behind1, p);
    behind2 = inter_render_behind(cam, behind2, p);

    filledTrigonRGBA(SDL_renderer, p.posX, p.posY, behind2.posX, behind2.posY, behind1.posX, behind1.posY, color.r, color.g, color.b, 255);
    trigonRGBA(SDL_renderer, p.posX, p.posY, behind2.posX, behind2.posY, behind1.posX, behind1.posY, 0, 0, 0, 255);
}

int vmi = 0;

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

/*void quicksort_on_render_array(RenderArray *ra, int min, int max) {
    double pivot = ra->tri[(min+max)/2]->dist;
    int i = min, j = max;
    while (i <= j) {
        while (ra->tri[i]->dist < pivot) ++i;
        while (ra->tri[i]->dist > pivot) --j;
        if (i <= j) {
            triangle *tmp = ra->tri[i];
            ra->tri[i] = ra->tri[j];
            ra->tri[j] = tmp;
            ++i;
            --j;
        }
    }

    if (min < j) quicksort_on_render_array(ra, min, j);
    if (i < max) quicksort_on_render_array(ra, i, max);
}*/

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
    //bubble_sort_ra(ra);
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
    clock_t start = clock();
    head = update_distances(head, cam);
    head = bubble_sort_by_dist(head);
    //head = selection_sort_RList(head);
    //head = change_stuff(head, cam);
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


    clock_t stop = clock();
    double deltaClock = ((double) stop-start)/CLOCKS_PER_SEC;
    printf("%d %.5lf\n", ++vmi,deltaClock);
    return head;
}

#endif
