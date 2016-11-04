#include "Source.h"
Source::Source() 
{

}

Vect Source::getLightPosition()
{
	return Vect(0, 0, 0);
}

Color Source::getLightColor() 
{
	return Color(1, 1, 1, 0);
}