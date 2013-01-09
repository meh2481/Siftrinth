#include "Circle.h"

bool Circle::touching(Circle* other)
{
	Float2 dist = other->pos - pos;
	return (dist.len() < radius + other->radius);
}

bool Circle::touching(Float2* pt)
{
	Float2 dist = pos - *pt;
	return (dist.len2() < radius * radius);
}