/*
 * AirController.cpp
 *
 *  Created on: 21/07/2014
 *      Author: paco
 *
 *  Copyright 2014 Francisco Martín
 *
 *  This file is part of ATCSim.
 *
 *  ATCSim is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ATCSim is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ATCSim.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AirController.h"
#include "Airport.h"
#include "Flight.h"
#include "Position.h"
#include <list>
#include <fstream>

namespace atcsim{

AirController::AirController() {
	// TODO Auto-generated constructor stub

}

AirController::~AirController() {
	// TODO Auto-generated destructor stub
}

void
AirController::doWork()
{
  std::list<Flight*> flights = Airport::getInstance()->getFlights();
  std::list<Flight*>::iterator it, it2;

  Position pos3(-750.0, 0.0, 25.0);
  Route r3;
  r3.pos = pos3;
  r3.speed = 15.0;


  for(it = flights.begin(); it!=flights.end(); ++it)
  {
    if((*it)->getRoute()->empty())
    {
      Position pos0(3500.0, 0.0, 100.0);
      Position pos1(1500.0, 0.0, 50.0);
      Position pos2(200.0, 0.0, 25.0);

      Route r0, r1, r2;

      r0.pos = pos0;
      r0.speed = 500.0;
      r1.pos = pos1;
      r1.speed = 100.0;
      r2.pos = pos2;
      r2.speed = 19.0;


      (*it)->getRoute()->push_back(r3);
      (*it)->getRoute()->push_front(r2);
      (*it)->getRoute()->push_front(r1);
      (*it)->getRoute()->push_front(r0);

      (*it)->assign_intermediate_points();

      if( (*it)->is_it_landing() == false )
        (*it)->assign_wait_sector();




		}
	}

  for(it = flights.begin(); it!=flights.end(); ++it){
    //if( (*it)->is_it_landing() == true ){
      for(it2 = it; it2!=flights.end(); ++it2){
        if( it != it2 && (*it)->getPosition().distance((*it2)->getPosition()) < COLLISION_DISTANCE*1.5)
        {
          (*it2)->setSpeed( (*it2)->getSpeed()*0.9  );

          if( (*it2)->getSpeed() < CRASH_SPEED){
            (*it2)->setSpeed(CRASH_SPEED);

            /*if( (pos3.distance((*it)->getPosition()) > LANDING_DIST*2)  )
              (*it)->setSpeed((*it)->getSpeed()*1.01);

            else if( (*it)->getSpeed() > LANDING_SPEED)
              (*it)->setSpeed(LANDING_SPEED);*/
          }



        }


      }



    //}
  }








}

void Flight::rotate_pos(Position &pos, float angle){
  float x_orig = pos.get_x();
  float y_orig = pos.get_y();

  pos.set_x( x_orig * cos(angle) - y_orig * sin(angle));
  pos.set_y( y_orig * cos(angle) + x_orig * sin(angle));
}


void Flight::assign_intermediate_points(){

  float x1, y1, z1, x2, y2, z2;

  x1 = 8000.0;
  y1 = 0;
  z1 = STORM_MAX_ELEVATION*0.66;

  x2 = 5000.0;
  y2 = 0;
  z2 = STORM_MAX_ELEVATION*0.33;

  Position pos1(x1, y1, z1);
  Position pos2(x2, y2, z2);

  if( get_sector()== -3 || get_sector()== -2)
    rotate_pos(pos1, -M_PI/6);

  else if( get_sector()== 2 || get_sector()== 3)
    rotate_pos(pos1, M_PI/6);

  if( get_sector()== -3 || get_sector()== -2 )
    rotate_pos(pos2, -M_PI/12);

  else if( get_sector()== 3 || get_sector()== 2 )
    rotate_pos(pos2, M_PI/12);

  Route r1, r2;

  r1.pos = pos1;
  r1.speed = 200.0;
  r2.pos = pos2;
  r2.speed = 200.0;

  this->getRoute()->push_front(r2);
  this->getRoute()->push_front(r1);

}


void Flight::assign_wait_sector(){

  float x1, y1, x2, y2, x3, y3, x4, y4, z0;

  x1 = 13000.0;
  y1 = 0.0;

  x2 = 11000.0;
  y2 = 2000.0;

  x3 = 9000.0;
  y3 = 0.0;

  x4 = 11000.0;
  y4 = -2000.0;

  if( get_sector()== 1 || get_sector()== 3 || get_sector()== -2)
    z0 = STORM_MAX_ELEVATION + COLLISION_DISTANCE*1.1;

  else z0 = STORM_MAX_ELEVATION + 2.2* COLLISION_DISTANCE;

  Position pos1(x1*cos(get_sector()*M_PI/12)-y1*sin(get_sector()*M_PI/12), y1*cos(get_sector()*M_PI/12)+x1*sin(get_sector()*M_PI/12), z0);
  Position pos2(x2*cos(get_sector()*M_PI/12)-y2*sin(get_sector()*M_PI/12), y2*cos(get_sector()*M_PI/12)+x2*sin(get_sector()*M_PI/12), z0);
  Position pos3(x3*cos(get_sector()*M_PI/12)-y3*sin(get_sector()*M_PI/12), y3*cos(get_sector()*M_PI/12)+x3*sin(get_sector()*M_PI/12), z0);
  Position pos4(x4*cos(get_sector()*M_PI/12)-y4*sin(get_sector()*M_PI/12), y4*cos(get_sector()*M_PI/12)+x4*sin(get_sector()*M_PI/12), z0);

  Route r1, r2, r3, r4;

  r1.pos = pos1;
  r1.speed = 150.0;
  r2.pos = pos2;
  r2.speed = 150.0;
  r3.pos = pos3;
  r3.speed = 150.0;
  r4.pos = pos4;
  r4.speed = 150.0;

  this->getRoute()->push_front(r2);
  this->getRoute()->push_front(r1);

  this->getRoute()->push_front(r4);
  this->getRoute()->push_front(r3);
  this->getRoute()->push_front(r2);
  this->getRoute()->push_front(r1);

}








}  // namespace atcsim
