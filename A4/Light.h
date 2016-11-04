#ifndef _Light_H
#define _Light_H

#include "Source.h"
#include "Vect.h"
#include "Color.h"

class Light : public Source {
public:
	Vect position;

	Color color;
		
	Light ();
	
	Light (Vect, Color);
	
	// method functions
	virtual Vect getLightPosition ();
	virtual Color getLightColor ();
	
};

#endif
