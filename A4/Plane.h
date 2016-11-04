#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {
	
public:
	Vect normal;
	double distance;
	Color color;
		
	Plane ();
	
	Plane (Vect, double, Color);
	
	// method functions
	Vect getPlaneNormal ();
	double getPlaneDistance ();
	virtual Color getColor ();
	
	virtual Vect getNormalAt(Vect point);
	
	virtual double findIntersection(Ray ray);
	
};

#endif
