#ifndef MTV_H
#define MTV_H

class Shape;

class MTV
{
public:
	MTV(){};
	MTV(double d, Shape::Point axis)	{
		pushX = d*axis.x;
		pushY = d*axis.y;
	};

	bool collision;
	double pushX;
	double pushY;
};

#endif