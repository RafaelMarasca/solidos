#ifndef MENU_H
#define MENU_H

#include "gui.h"

typedef enum{SPHERE, CUBE, ICOSAHEDRON, TORUS} shape;

void menuClickShape(int ID);
//void menuClickSel(int ID);
void menuClickOpt(int ID);
void menuClickRot(int ID);
void menuClickTrans(int ID);
void menuClickScale(int ID);
//void menuClick3(int ID);
//void menuClick4(int ID);
void menuClickPopUp(int ID);
void menuClickCube(int ID);
void menuClickSphere(int ID);
void menuClickIsocahedron(int ID);
void menuClickTorus(int ID);
void menuClickHexa(int ID);

frame* newShapeMenu();
frame* newSphere();
frame* newCube();
frame* newTorus();
frame* newIcosahedron();
frame* newHexa();
frame* newAddMenu();
frame* newOptMenu();
frame* newPopUp();
frame* newRotMenu();
frame* newTransMenu();
frame* newScaleMenu();




#endif //MENU_H