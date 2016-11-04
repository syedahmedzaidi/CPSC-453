#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
	
public:
	
	double x, y, z;

	Vect ();
	
	Vect (double, double, double);
	
	// method functions
	double getVectX();
	double getVectY();
	double getVectZ(); 
	
	double magnitude (); 
	
	Vect normalize (); 
	Vect negative (); 
	double dotProduct(Vect v); 
	
	Vect crossProduct(Vect v);
	
	Vect vectAdd (Vect v); 

	Vect vectMult (double scalar);
};


#endif
