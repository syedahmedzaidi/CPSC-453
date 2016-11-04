#include "Object.h"


Object::Object () 
{
	
}

Color Object::getColor () 
{ 
	return Color (0.0, 0.0, 0.0, 0); 
}
	
Vect Object::getNormalAt(Vect intersection_position)
{
	return Vect (0, 0, 0);
}
	
double Object::findIntersection(Ray ray) 
{
	return 0;
}