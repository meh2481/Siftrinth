#ifndef CIRCLE_H
#define CIRCLE_H

#include <sifteo.h>
using namespace Sifteo;

class Circle
{
public:
	Float2 pos;
	float radius;

	bool touching(Circle* other);	//AVOID if possible, since sqrt() is notoriously slow 
	bool touching(Float2* pt);
};


























#endif