
#include "Entity.hpp"
#include "include/raylib.h"

Entity::Entity(Vector2 Position,Vector2 Velocity, Vector2 Acceleration){
  this->Position = Position;
  this->Velocity = Velocity;
  this->Acceleration = Acceleration;
}


Vector2 Entity::getPosition(){
  return this->Position;
}
Vector2 Entity::getVelocity(){
  return this->Velocity;
}
Vector2 Entity::getAcceleration(){
  return this->Acceleration;
}

void Entity::setPosition(Vector2 Position){
  this->Position = Position;
}

void Entity::setVelocity(Vector2 vec){
	this->Velocity = vec;
}
void Entity::setAcceleration(Vector2 acc){
	this->Acceleration = acc;
}


