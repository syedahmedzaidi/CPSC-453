#ifndef _SPHERE_H
#define _SPHERE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Object {
	
	
public:
	
	Vect center;
	double radius;
	Color color;

	Sphere ();
	
	Sphere (Vect, double, Color);
	
	// method functions
	Vect getSphereCenter (); 
	double getSphereRadius (); 
	
	virtual Color getColor ();
	
	virtual Vect getNormalAt(Vect point);
	
	virtual double findIntersection(Ray ray);
	
};


#endif
