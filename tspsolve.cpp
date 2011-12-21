#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Point{
  private:
    float x_;
    float y_;
  public:
    Point(float x, float y);
    float calcDistance(Point* p);
    float getX();
    float getY();
};

Point::Point(float x, float y){
  x_ = x;
  y_ = y;
}

float Point::calcDistance(Point* p){
  hypot(p->getX() - x_, p->getY() - y_);
}

float Point::getX(){
  return x_;
}

float Point::getY(){
  return y_;
}

class TSPSolver{
  private:
    int dimension_;
    vector< vector<double> > matrix_;
    vector<Point*> loadPoints(string filename, int dimention);

  public:
    TSPSolver(string filename);

};

TSPSolver::TSPSolver(string filename) {
  string temp;
  ifstream ifs(filename.c_str());

  do {
    ifs >> temp;
    if( temp == "DIMENSION" ){
      ifs >> temp;
      break;
    }
    if( temp == "DIMENSION:" ) break;
  } while(true);

  ifs >> temp;
  dimension_ = atoi(temp.c_str());
  vector<Point*> points = loadPoints(filename, dimension_);
  cout << dimension_ << endl;
}

vector<Point*> TSPSolver::loadPoints(string filename, int dimension){
  vector<Point*> points;
  string temp;
  ifstream ifs(filename.c_str());
  do {
    ifs >> temp;
    if( temp == "NODE_COORD_SECTION" ) break;
  } while(true);
  for(int i = 0; i < dimension; ++i) {
    ifs >> temp;
    ifs >> temp;
    int x = atoi(temp.c_str());
    ifs >> temp;
    int y = atoi(temp.c_str());
    Point* p = new Point((float)x, (float)y);
    points.push_back(p);
  }

}

int main( int argc, char *argv[] ){
  FILE *fp;

  if( argc != 2 ){
    cout << "Usage: sample <input_filename>" << endl;
    exit( 1 );
  }

  if(( fp=fopen( argv[1], "r" )) == NULL ){
    cout << "file open Error" << endl;
    exit( 1 );
  }
  string* filename = new string(argv[1]);
  new TSPSolver(*filename);
  delete filename;
  fclose( fp );
}
