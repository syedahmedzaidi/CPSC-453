#ifndef _COLOR_H
#define _COLOR_H

class Color {
	
public:
	
	double red, green, blue, special;

	Color ();
	
	Color (double, double, double, double);
	
	// method functions
	double getColorRed();
	double getColorGreen();
	double getColorBlue();
	double getColorSpecial();
	
	double setColorRed(double redValue);
	double setColorGreen(double greenValue);
	double setColorBlue(double blueValue);
	double setColorSpecial(double specialValue);
	
	double brightness();

	Color colorScalar(double scalar);
	
	Color colorAdd(Color color);
	
	Color colorMultiply(Color color);

	Color colorAverage(Color color);
	
	Color clamp();
};

#endif
