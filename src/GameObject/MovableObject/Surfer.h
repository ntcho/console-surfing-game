#ifndef SURFER_H
#define SURFER_H

#include "../MovableObject.h"
#include "Player.h"

#include <random> // Random
#include <ctime>

class Surfer : public MovableObject {
private:
	bool is_collidable = true;

public:
	Surfer();
	Surfer(float x, float y);

public:
	void HitBy(MovableObject* object);
	void HitBy(Player* player);
	void Move(double delta_time);
};

#endif