# Programozás alapjai házi feladat

## Programról röviden:
A program egy rövid autós játék, melyben egy kisautót irányíthatunk fölülről (az alapbeállítás) alapján a W,A,S,D gombokkal. A játék addig tart, amíg körbe nem megy a játékos a körpályán, vagy amíg ki nem lép az Escape gomb lenyomásával.

## A program valódi célja:
Már nagyon régóta érdekelt a 3D-s grafika működése, így a programozás alapjai 1 nagy házi feladatnak egy 3D-s játékot szerettem volna csinálni. Ahelyett hogy utána olvastam volna egy 3D engine működésének, megpróbáltam újra feltalálni a kereket, így a program nem is vektoralgebrát használ a megjelenítéshez, hanem az egész matek vissza van vezetve trigonometriára gyakorlatilag.

A megjelenítéshez egyébként az SDL2 könyvtárat, és annak a különböző (mint utólag kiderült nem hardveresen gyorsított) rajzoló függvényeit használtam.

## Problémák
Habár a 3D-s megjelenítés működik, és a prezentációként készült kis autós játék is megy, ha nem WireFrame megjelenítésben vannak az objektumok, akkor a futásidő megközelítőleg 2/3-át megeszi az SDL_gfxPrimitives "filledTrigonRGBA" függvénye. Így a zökkenőmentes futás érdekében a prezentációs program csupán WireFrame megjelenítést alkalmaz.

## Kép egy 3D kockáról
![Egy kép egy kockáról](https://github.com/Coldus12/prog-NagyHF/blob/v11-07/Kocka.png)
