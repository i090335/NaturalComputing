#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Point{
  private:
    int x_;
    int y_;
  public:
    Point(int x, int y);
    int calcDistance(Point* p);
    int getX();
    int getY();
};

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

class TSPSolver{
  private:
    int dimension_;
    int** matrix_;
    vector<Point*> loadPoints(string filename, int dimension);
    int** createMatrix(vector<Point*> points);

  public:
    TSPSolver(string filename);
    int calcAscendingCost(int begin);
    int getDimension();
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
  matrix_ = createMatrix(points);
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
    Point* p = new Point((int)x, (int)y);
    points.push_back(p);
  }
  return points;
}

int** TSPSolver::createMatrix(vector<Point*> points){
  int dimension = points.size();
  int** matrix = new int*[dimension];
  for(int i = 0; i < dimension; ++i) {
    matrix[i] = new int[dimension];
  }
  for(int i = 0; i < dimension; ++i) {
    for(int j = i; j < dimension; ++j) {
      int distance = points[i]->calcDistance(points[j]);
      matrix[i][j] = distance;
      matrix[j][i] = distance;
    }
  }
  return matrix;
}

int TSPSolver::calcAscendingCost(int begin) {
  int current = begin;
  int sum = 0;
  int next;
  do {
    next = (current + 1) % dimension_;
    sum += matrix_[current][next];
    current = next;
  } while(current != begin);
  return sum;
}

int TSPSolver::getDimension() {
  return dimension_;
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
  TSPSolver* solver = new TSPSolver(*filename);
  int d = solver->getDimension();
  for(int i = 0; i < d; ++i) {
    int cost = solver->calcAscendingCost(i);
    cout << i << ", " << cost << endl;
  }
  delete filename;
  delete solver;
  fclose( fp );
}
