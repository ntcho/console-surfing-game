#include "Health.h"

extern int life_count;

//Constructors
Health::Health() : Item(){}
Health::Health(float x, float y) : Item(x, y){}

void Health::HitBy(GameObject& object) 
{
	life_count++;
}