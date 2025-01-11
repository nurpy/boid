
#ifndef ENITIY_H
#define ENITIY_H

#include"include/raylib.h"

class Entity{
  private:

	 Vector2 Velocity;
	 Vector2 Acceleration;
    Vector2 Position;

	public:

		Entity(Vector2 Position,Vector2 Velocity, Vector2 Acceleration);
		Vector2 getPosition();
		Vector2 getAcceleration();
		Vector2 getVelocity();
    void setPosition(Vector2 pos);
    void setVelocity(Vector2 vec);
    void setAcceleration(Vector2 acc);
	 

};


#endif
