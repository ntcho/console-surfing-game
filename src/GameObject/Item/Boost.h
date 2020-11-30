#ifndef BOOST_H
#define BOOST_H

#include "Item.h"

class Boost : public Item {
	
public:
	Boost();
	Boost(float x, float y);

public:
	void HitBy(GameObject& object);
};

#endif