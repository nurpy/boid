#ifndef BIRD_H
#define BIRD_H
#include"Entity.hpp"
#include"config.h"
#include "include/raylib.h"
#include <cmath>
#include<memory>
#include<vector>




class Bird : public Entity{
  private:
    float Orientation;

    Vector2 bottomLeftVertex;
    Vector2 bottomRightVertex;
    Vector2 topVertex;



  public:
    Bird(Vector2 Position,Vector2 Velocity, Vector2 Acceleration, float Orientation);
    float getOrientation();
    void draw();
	 void overlappingScreen();
    void update();
	 void guide(std::vector<std::unique_ptr<Bird>>& birds);
    

};
#endif
