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
  //DrawCircleLines(this->getPosition().x,this->getPosition().y,BIRDCLOSE,RED);
  //DrawCircleLines(this->getPosition().x,this->getPosition().y,BIRDFAR,RED);

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
  
	if(this->getVelocity().x == 0.0 || this->getVelocity().x == -0.0){
		this->setVelocity({0.0000001,this->getVelocity().y});
	}
	float sign =   this->getVelocity().y*this->getVelocity().x < 0.0  ? -1.0 : 1.0 ;
	float slope=0.0;
  //Control Orientation of birds
  if(this->getVelocity().x != 0.0){
   slope = this->getVelocity().y / this->getVelocity().x;
  }
  else{
	slope*=sign;
  }
  
  Orientation =std::atan( slope )-3.14159/2;
  Orientation+= this->getVelocity().x < 0.0 ? 3.14159 : 0;

  Matrix2 rotation_matrix = {std::cos(Orientation),-std::sin(Orientation),std::sin(Orientation),std::cos(Orientation)};

  //limit params
  //if(this->getAcceleration() > 1){this->setAcceleration(this->getAcceleration().normalize()*1);}
  if(this->getVelocity() > 10){this->setVelocity(this->getVelocity().normalize()*10);} //max speed of bird is 10
  if(this->getVelocity() > 5){this->setVelocity(this->getVelocity()*DRAG);} // slow down fast bird
  if(this->getVelocity() < 2){this->setAcceleration(this->getVelocity().normalize()*2);} // speed up slow bird



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

//target a repelling direction
void Bird::targetRepel(Vector2 targetDirection, float weight){
  targetDirection.normalize();
  targetDirection = targetDirection*MAXSPEED;
  targetDirection = targetDirection - this->getVelocity();
  targetDirection = targetDirection * weight;

  if(targetDirection.mag() > MAXREPULSIONFORCE){targetDirection = targetDirection.normalize()*MAXREPULSIONFORCE;}

  //targetDirection = targetDirection.normalize()*MAXFORCE; //limit applied for to MAXFORCE

  this->setAcceleration(targetDirection + this->getAcceleration());

}
//target a direction
void Bird::target(Vector2 targetDirection, float weight){
  targetDirection.normalize();
  targetDirection = targetDirection*MAXSPEED;
  targetDirection = targetDirection - this->getVelocity();
  targetDirection = targetDirection * weight;

  if(targetDirection.mag() > MAXFORCE){targetDirection = targetDirection.normalize()*MAXFORCE;}

  //targetDirection = targetDirection.normalize()*MAXFORCE; //limit applied for to MAXFORCE

  this->setAcceleration(targetDirection + this->getAcceleration());

}


void Bird::guide(std::vector<std::unique_ptr<Bird>>& birds){

  int numOfBirdsClose=0;
  int numOfBirdsFar=0;
  Vector2 RepulsionDistanceAvg = {0.0,0.0};
  Vector2 AlignmentVelocityAvg = {0.0,0.0};
  Vector2 CohesionPositionAvg = {0.0 , 0.0};
  for(auto& bird : birds)
  {
    Vector2 dist = this->getPosition() - bird->getPosition();
    // repulsion force
    if(dist <  BIRDCLOSE && dist > 0)
    {
    	this->targetRepel(dist,REPULSION);///dist.mag());
      RepulsionDistanceAvg=dist+RepulsionDistanceAvg;
      numOfBirdsClose++;
    }

    //coheshion force && alignment force
    if( dist < BIRDFAR && dist > 0)
    {
      AlignmentVelocityAvg = AlignmentVelocityAvg + bird->getVelocity();
      CohesionPositionAvg = CohesionPositionAvg-dist;// + bird->getPosition();
      numOfBirdsFar++;
    }
  }
  
  if(numOfBirdsClose > 0)
  {
    //this->target(RepulsionDistanceAvg,REPULSION/(RepulsionDistanceAvg/numOfBirdsClose).mag());
  }
  if(numOfBirdsFar > 0)
  {
//    this->target(AlignmentVelocityAvg,DIRECTED/(float)numOfBirdsFar);
    this->target(AlignmentVelocityAvg,.001);
    //this->target({-1,-1},1);
    
    this->target(CohesionPositionAvg, .0001);
  //  this->target(CohesionPositionAvg, COHESION/(float)numOfBirdsFar);

    

  }
  
//  this->setAcceleration(accelerationChangeRepulsion/numOfBirdsClose + accelerationChangeCoheshion/numOfBirdsFar );





}

