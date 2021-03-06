#include "MovableObject.h"
#include <cmath>

// Constructors
MovableObject::MovableObject() : GameObject() {}
MovableObject::MovableObject(float x, float y) : GameObject(x, y) {}

void MovableObject::Move(double delta_time)
{
	SetCenter(
		GetCenterX() + delta_time * ratio * velocity_x,
		GetCenterY() + delta_time * ratio * velocity_y
	);
 }

void MovableObject::Stop() 
{
	this->velocity_x = 0;
	this->velocity_y = 0;
}

// Getters
float MovableObject::GetVelocityX() { return this->velocity_x; }
float MovableObject::GetVelocityY() { return this->velocity_y; }
float MovableObject::GetSpeed()
{
	return sqrt(pow(this->velocity_x, 2) + pow(this->velocity_y, 2));
}

// Setters
void MovableObject::SetVelocityX(float velocity_x) { this->velocity_x = velocity_x; }
void MovableObject::SetVelocityY(float velocity_y) { this->velocity_y = velocity_y; }
void MovableObject::SetSpeedByFactor(float factor)
{
	this->velocity_x = this->velocity_x * factor;
	this->velocity_y = this->velocity_y * factor;
}

void MovableObject::SetDirection(int d) {
	if (-2 <= d && d <= 2)
		direction = d;
	else if (d < -2)
		direction = -2;
	else if (d > 2)
		direction = 2;

	switch (direction) {
	case -2:/*8'o clock dir*/
		this->velocity_x = this->velocity_y * tan((4.8 / 4.0) * PI); break; // -45 DEGREE
	case -1:/*7'o clock dir*/						 
		this->velocity_x = this->velocity_y * tan((4.4 / 4.0) * PI); break; // -22.5 DEGREE
	case 0:/*6'o clock dir*/						 
		this->velocity_x = this->velocity_y * tan(PI); break;               // 0 DEGREE
	case 1:/*5'o clock dir*/						 
		this->velocity_x = this->velocity_y * tan((3.6 / 4.0) * PI); break; // +22.5 DEGREE
	case 2:/*4'o clock dir*/						 
		this->velocity_x = this->velocity_y * tan((3.2 / 4.0) * PI); break; // +45 DEGREE
	}

}
void MovableObject::RotateRight()
{
	SetDirection(direction + 1);
	//this->velocity_x = tan(atan(this->velocity_x) - (22 * PI / 180));
}
void MovableObject::RotateLeft() 
{
	SetDirection(direction - 1);
	//this->velocity_x = tan(atan(this->velocity_x) + (22 * PI / 180));
}
void MovableObject::ResetRotate() 
{ 
	SetDirection(0);
	//this->velocity_y = 1.0f;
}

bool MovableObject::HasIntersected_(MovableObject* object)
{
	// collision detected!
	float dx = this->GetCenterX() - object->GetCenterX();
	float dy = this->GetCenterY() - object->GetCenterY();
	int dw = this->GetWidth() + object->GetWidth();
	int dh = this->GetHeight() + object->GetHeight();
	if (dx < 0) dx *= -1;
	if (dy < 0) dy *= -1;
	if (dx < dw && dy < dh) return true;
	else return false;
}