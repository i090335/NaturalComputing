#include <cmath>

#include "point.h"

Point::Point(int x, int y){
  x_ = x;
  y_ = y;
}

int Point::calcDistance(Point* p){
  return round(hypot(p->getX() - x_, p->getY() - y_));
}

int Point::getX(){
  return x_;
}

int Point::getY(){
  return y_;
}

