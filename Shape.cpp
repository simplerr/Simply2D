#include "Shape.h"

/*
	shapes uses the top left to locate themselfs
	player and objects uses the center
	only one of these should exist!
	in the editor POS = center thoughh
*/
Shape::Shape()
{
	sides = 0;
}

// the order in which the points gets added is important
// CreateShape() function should be created
// to much to know for the user about the order
void Shape::addPoint(Point point)
{
	//point.x += origin.x;
	//point.y += origin.y;

	// calculate the AABB
	if(point.x < aabb.left)
		aabb.left = point.x;
	else if(point.x > aabb.right)
		aabb.right = point.x;

	if(point.y < aabb.top)
		aabb.top = point.y;
	else if(point.y > aabb.bottom)
		aabb.bottom = point.y;

	sides++;

	pointList.push_back(point);
}

Shape::AABB Shape::getAABB(void)
{
	return aabb;
}

RECT Shape::getRect(void) 
{
	RECT r;
	r.left = aabb.left + Shape::origin.x;
	r.right = aabb.right + Shape::origin.x;
	r.top = aabb.top + Shape::origin.y;
	r.bottom = aabb.bottom + Shape::origin.y;

	return r;
}

void Shape::scale(direction side, double dwidth, double dheight)
{
	if(side == LEFT)
		{			
			for(int i = 0; i < pointList.size(); i++)	
			{
				if(pointList[i].x != aabb.left)	{
					pointList[i].x -= (double)dwidth;
					aabb.right -= (double)dwidth/2;									
				}
			}
			origin.x += (double)dwidth;
		}
	else if(side == RIGHT)
	{			
		for(int i = 0; i < pointList.size(); i++)	
		{
			if(pointList[i].x != aabb.left)	{
				pointList[i].x += dwidth;
				aabb.right += (double)dwidth/2;
			}
		}
	}
	else if(side == UP)
	{			
		for(int i = 0; i < pointList.size(); i++)
		{
			if(pointList[i].y != aabb.top)	{
				pointList[i].y += dheight;
				aabb.bottom += (double)dheight/2;
			}
				
		}
		origin.y -= (double)dheight;
	}
	else if(side == DOWN)
	{			
		for(int i = 0; i < pointList.size(); i++)
		{
			if(pointList[i].y != aabb.top)	{
				pointList[i].y += dheight;
				aabb.bottom += (double)dheight/2;
			}
		}
	}
	else if(side == ALL)
	{
		for(int i = 0; i < pointList.size(); i++)	
		{
			// down & up
			if(pointList[i].y != aabb.top)	{
				pointList[i].y += 2*dheight;
				aabb.bottom += (double)dheight;
			}
			// right & left
			if(pointList[i].x != aabb.left)	{
				pointList[i].x += 2*dwidth;
				aabb.right += (double)dwidth;
			}
		}
		origin.y -= (double)dheight;
		origin.x -= (double)dwidth;
	}
}

void Shape::setWidth(double width)
{
	int lastWidth = (int)aabb.right;
	int newWidth = (int)width;
	double nPos; // normalized position

	for(int i = 0; i < pointList.size();i++)
	{
		nPos = pointList[i].x / lastWidth;
		pointList[i].x = nPos * newWidth;
	}

	aabb.right = newWidth;
}

void Shape::setHeight(double height)
{
	int lastHeight = (int)aabb.bottom;
	int newHeight = (int)height;
	double nPos; // normalized position

	for(int i = 0; i < pointList.size();i++)
	{
		nPos = pointList[i].y / lastHeight;
		pointList[i].y = nPos * newHeight;
	}

	aabb.bottom = newHeight;
}