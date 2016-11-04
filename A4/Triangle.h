#ifndef _Triangle_H
#define _Triangle_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object {

	
public:
	
	Vect A, B, C;
	Vect normal;
	double distance;
	Color color;

	Triangle ();
	
	Triangle (Vect, Vect, Vect, Color);
	
	// method functions
	Vect getTriangleNormal ();
	
	double getTriangleDistance ();

	virtual Color getColor ();
	
	virtual Vect getNormalAt(Vect point);
	
	virtual double findIntersection(Ray ray);
	
};


#endif
