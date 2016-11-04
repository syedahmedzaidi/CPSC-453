#ifndef _RAY_H
#define _RAY_H

#include "Vect.h"

class Ray {
	
public:
	Vect origin, direction;

	Ray ();
	
	Ray (Vect, Vect);
	
	// method functions
	Vect getRayOrigin ();
	Vect getRayDirection (); 
	
};

#endif
