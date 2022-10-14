#ifndef MENU_H
#define MENU_H

#include "gui.h"

typedef enum{SPHERE, CUBE, ICOSAHEDRON, TORUS} shape;

void menuClickShape(int ID);
void menuClick1(int ID);
void menuClick2(int ID);
void menuClick3(int ID);
void menuClick4(int ID);
void menuClickCube(int ID);
void menuClickSphere(int ID);
void menuClickIsocahedron(int ID);
void menuClickTorus(int ID);

frame* newShapeMenu();
frame* newSphere();
frame* newCube();
frame* newTorus();
frame* newIcosahedron();
frame* newAddMenu();
frame* newOptMenu();
frame* newPopUp();




#endif //MENU_H