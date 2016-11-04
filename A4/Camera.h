#ifndef _Camera_H
#define _Camera_H

#include "Vect.h"

class Camera {
	
public:
	Vect campos, camdir, camright, camdown;
		
	Camera ();
	
	Camera (Vect, Vect, Vect, Vect);
	
	// method functions
	Vect getCameraPosition ();
	Vect getCameraDirection ();
	Vect getCameraRight ();
	Vect getCameraDown ();
	
};

#endif
