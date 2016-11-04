#include "Camera.h"

Camera::Camera () {
	campos = Vect(0,0,0);
	camdir = Vect(0,0,1);
	camright = Vect(0,0,0);
	camdown = Vect(0,0,0);
}

Camera::Camera (Vect pos, Vect dir, Vect right, Vect down) {
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;
}

Vect Camera::getCameraPosition () 
{ 
	return campos; 
}
Vect Camera::getCameraDirection () 
{ 
	return camdir; 
}
Vect Camera::getCameraRight () 
{ 
	return camright; 
}
Vect Camera::getCameraDown ()
{ 
 	return camdown; 
}