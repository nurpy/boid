#include"Bird.hpp"
#include "config.h"
#include "include/raylib.h"
#include <cmath>

Bird::Bird(Vector2 Position,Vector2 Velocity, Vector2 Acceleration, float Orientation) : Entity(Position,Velocity,Acceleration){
  this->Orientation=Orientation;

  Vector2 top_dist={0,40};
  Vector2 bottom_left_distance={-30,-20};
  Vector2 bottom_right_distance={30,-20};
  Matrix2 rotation_matrix = {std::cos(Orientation),-std::sin(Orientation),std::sin(Orientation),std::cos(Orientation)};
  this->topVertex = rotation_matrix*top_dist + this->getPosition();
  this->bottomRightVertex = rotation_matrix*bottom_right_distance + this->getPosition();
  this->bottomLeftVertex = rotation_matrix*bottom_left_distance + this->getPosition();
}
float Bird::getOrientation(){
  return this->Orientation;
}
void Bird::draw(){

  this->overlappingScreen();  
  
  DrawTriangleLines(topVertex, bottomLeftVertex, bottomRightVertex, RED);
//  DrawCircleLines(this->getPosition().x,this->getPosition().y,BIRDCLOSE,RED);
 // DrawCircleLines(this->getPosition().x,this->getPosition().y,BIRDFAR,RED);

}


void Bird::overlappingScreen(){
  Vector2 widthVector{SCREENWIDTH,0};
  Vector2 heightVector{0,SCREENHEIGHT};

  Vector2 closeCircleWidth{BIRDCLOSE,0};
  Vector2 closeCircleHeight{0,BIRDCLOSE};

  Vector2 farCircleWidth{BIRDFAR,0};
  Vector2 farCircleHeight{0,BIRDFAR};
  
  if(this->topVertex.x > SCREENWIDTH || this->bottomLeftVertex.x > SCREENWIDTH|| this->bottomRightVertex.x  > SCREENWIDTH)
  {
    //this->setPosition(getPosition() - widthVector);
    DrawTriangleLines( topVertex-widthVector, bottomLeftVertex-widthVector, bottomRightVertex-widthVector, RED);
  }
  if(this->topVertex.x < 0 || this->bottomLeftVertex.x < 0 || this->bottomRightVertex.x  < 0)
  {
    //this->setPosition(getPosition() + widthVector);
    DrawTriangleLines( topVertex+widthVector, bottomLeftVertex+widthVector, bottomRightVertex+widthVector, RED);
  }
  if(this->topVertex.y > SCREENHEIGHT || this->bottomLeftVertex.y > SCREENHEIGHT || this->bottomRightVertex.y  > SCREENHEIGHT)
  {
   // this->setPosition(getPosition() - heightVector);
    DrawTriangleLines(topVertex - heightVector, bottomLeftVertex - heightVector, bottomRightVertex-heightVector, RED);
  }
  if(this->topVertex.y <  0 || this->bottomLeftVertex.y < 0 || this->bottomRightVertex.y  < 0)
  {
 //   this->setPosition(getPosition() + heightVector);
    DrawTriangleLines( topVertex+heightVector, bottomLeftVertex+heightVector, bottomRightVertex+heightVector, RED);
  }

  if(this->getPosition().x + closeCircleWidth.x > SCREENWIDTH)
  {
  //DrawCircleLines(this->getPosition().x-SCREENWIDTH,this->getPosition().y,BIRDCLOSE,RED);
  }
  if(this->getPosition().x - closeCircleWidth.x < 0)
  {
  //DrawCircleLines(this->getPosition().x+SCREENWIDTH,this->getPosition().y,BIRDCLOSE,RED);
  }
  if(this->getPosition().y + closeCircleHeight.y > SCREENHEIGHT)
  {
  //DrawCircleLines(this->getPosition().x,this->getPosition().y-SCREENHEIGHT,BIRDCLOSE,RED);
  }
  if(this->getPosition().y - closeCircleHeight.y < 0)
  {
  //DrawCircleLines(this->getPosition().x,this->getPosition().y+SCREENHEIGHT,BIRDCLOSE,RED);
  }
}

void Bird::update(){
  
	
  //Control Orientation of birds
  float slope = this->getVelocity().y / this->getVelocity().x;
  Orientation =std::atan( slope )-3.14159/2;
  Orientation+= this->getVelocity().x < 0.0 ? 3.14159 : 0;

  Matrix2 rotation_matrix = {std::cos(Orientation),-std::sin(Orientation),std::sin(Orientation),std::cos(Orientation)};

  //limit params
  if(this->getAcceleration() > 1){this->setAcceleration(this->getAcceleration().normalize()*1);}
  if(this->getVelocity() > 10){this->setVelocity(this->getVelocity().normalize()*10);} //max speed of bird is 10
  if(this->getVelocity() > 5){this->setVelocity(this->getVelocity()*DRAG);} // slow down fast bird
  if(this->getVelocity() < 1){this->setAcceleration(this->getVelocity().normalize()*2);} // speed up slow bird



  //update speed
  this->setPosition(  this->getVelocity() + this->getPosition() );
  this->setVelocity(  this->getAcceleration() + this->getVelocity() );
  this->setAcceleration({0.0,0.0});


 
  /*
  * Rap birds around screen
  */
  if(this->getPosition().x > SCREENWIDTH){this->setPosition({0,this->getPosition().y});}
  if(this->getPosition().x < 0){this->setPosition({SCREENWIDTH,this->getPosition().y});}
  if(this->getPosition().y > SCREENHEIGHT){this->setPosition({this->getPosition().x,0});}
  if(this->getPosition().y < 0){this->setPosition({this->getPosition().x,SCREENHEIGHT});}


  //update character
  

  Vector2 top_dist={0,40};
  Vector2 bottom_left_distance={-30,-20};
  Vector2 bottom_right_distance={30,-20};
  this->topVertex = rotation_matrix * top_dist + this->getPosition();
  this->bottomRightVertex = rotation_matrix * bottom_right_distance + this->getPosition();
  this->bottomLeftVertex = rotation_matrix * bottom_left_distance + this->getPosition();



  return;
}




void Bird::guide(std::vector<std::unique_ptr<Bird>>& birds){

  float numOfBirdsClose=0;
  float numOfBirdsFar=0;
  Vector2 accelerationChangeRepulsion{0.0,0.0};
  Vector2 accelerationChangeCoheshion{0.0,0.0};
  for(auto& bird : birds)
  {
    //check if bird is too close (in close region)
    Vector2 dist = bird->getPosition() - this->getPosition();
    if(dist <  BIRDCLOSE && dist > 0)
    {
      float mag  = dist.mag();
      Vector2 dir = dist.normalize();
      bird->setAcceleration(dir* (REPULSION/mag) + bird->getAcceleration() );
      this->setAcceleration(dir* (-REPULSION/mag) + this->getAcceleration() );
      numOfBirdsClose++;
    //  accelerationChangeRepulsion+=dir* (-REPULSION/mag);
    }
    
    if( dist < BIRDFAR && dist > 0)
    {
      Vector2 intendedDir = dist + bird->getVelocity();
      this->setAcceleration( (intendedDir*10)/COHESION /*std::exp(mag)/COHESION)*/ + this->getAcceleration() );
  //    accelerationChangeCoheshion+=(dir * -mag)/COHESION;
      numOfBirdsFar++;
    }
  }
//  this->setAcceleration(accelerationChangeRepulsion/numOfBirdsClose + accelerationChangeCoheshion/numOfBirdsFar );





}

