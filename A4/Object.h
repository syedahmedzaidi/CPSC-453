#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object {	
	public:
	
	Object ();
	
	// method functions
	virtual Color getColor (); 
	
	virtual Vect getNormalAt(Vect intersection_position); 
	
	virtual double findIntersection(Ray ray);
	
};
#endif
