#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include "mt19937.c"

using namespace std;

class MersenneTwister {
 public:
  ptrdiff_t operator()( ptrdiff_t arg ) const {
    return static_cast< ptrdiff_t >( arg * static_cast< double >( genrand() ) );
  }
};

enum solveMode {
  RandomSolve,
  HillClimbing
};

enum neighborMode {
  RandomNeighbor,
  TwoOptNeighbor
};

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
    vector<Point*> points_;
    int calcCost(vector<int> route);
    vector<int> generateRandomOrder();
    vector<int> randomNeighbor(vector<int>* src);
    vector<int> twooptNeighbor(vector<int>* src);

  public:
    TSPSolver(string filename);
    int calcAscendingCost(int begin);
    int getDimension();
    vector<int> calcMinimumRandomRoute(int max, int* minp);
    vector<Point*> getPoints();
    vector<int> hillClimbing(int max, int* minp, neighborMode mode);
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
  points_ = loadPoints(filename, dimension_);
  matrix_ = createMatrix(points_);
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

vector<Point*> TSPSolver::getPoints() {
  return points_;
}

int TSPSolver::calcAscendingCost(int begin) {
  /*
   * 開始点から全てのノードを順繰りに移動したときの総コストを返します。
   * begin : 開始点
   *
   */
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

vector<int> TSPSolver::calcMinimumRandomRoute(int max, int* minp) {
  /*
   * max回ランダムなルートを生成して、最小のルートを返します。
   * minpには、最小ルートの総コストが渡されます。
   *
   */
  vector<int>* minRoute;
  MersenneTwister* mt = new MersenneTwister();
  int min = INT_MAX;
  vector<int> order = generateRandomOrder();
  for(int n = 0; n < max; ++n) {
    random_shuffle(order.begin(), order.end(), *mt);
    int cost = calcCost(order);
    if(cost < min) {
        min = cost;
        minRoute = &order;
    }
  }
  delete mt;
  *minp = min;
  return *minRoute;
}

vector<int> TSPSolver::generateRandomOrder(){
  vector<int> order;
  for(int i = 0; i < dimension_; ++i) order.push_back(i);
  MersenneTwister* mt = new MersenneTwister();
  random_shuffle(order.begin(), order.end(), *mt);
  delete mt;
  return order;
}

vector<int> TSPSolver::hillClimbing(int max, int* minp, neighborMode mode) {
  vector<int> minRoute = generateRandomOrder();
  vector<int> neighbor;
  int cost = calcCost(minRoute);
  int neighborCost;
  for(int n = 0; n < max; ++n) {
    neighbor = randomNeighbor(&minRoute);
    neighborCost = calcCost(neighbor);
    if(neighborCost < cost) {
      minRoute = neighbor;
      cost = neighborCost;
    }
  }
  *minp = cost;
  return minRoute;
}

int TSPSolver::calcCost(vector<int> route) {
  /**
   * 渡されたルートを順に移動したときの総コストを返します。
   * route : 移動順のノード番号を含んだvector
   *
   */
  int cost = 0;
  int dimension = route.size();
  Point* currentPoint;
  Point* nextPoint;
  for (int i = 0; i < dimension; ++i) {
    int p = route.at(i);
    int next = route.at((i + 1) % dimension);
    currentPoint = points_[p];
    nextPoint = points_[next];
    cost += currentPoint->calcDistance(nextPoint);
  }
  return cost;
}

int TSPSolver::getDimension() {
  return dimension_;
}

vector<int> TSPSolver::randomNeighbor(vector<int> *src) {
  vector<int> dst;
  copy(src->begin(), src->end(), back_inserter(dst));
  int size = src->size();
  int from = (int)(genrand() * size);
  int to = (int)(genrand() * size);
  int tmp = src->at(from);
  dst[from] = src->at(to);
  dst[to] = tmp;
  return dst;
}

int main( int argc, char *argv[] ){
  FILE *fp;
  sgenrand( static_cast<long>( time(NULL) ) );
  int n;
  solveMode mode;
  if( argc <= 1 || argc >= 4 ){
    cout << "Usage: sample <input_filename>" << endl;
    exit( 1 );
  } else if(argc == 2) {
    mode = RandomSolve;
  } else if(argc == 3) {
    string* modename = new string(argv[2]);
    if (*modename == "random") {
      mode = RandomSolve;
    } else if(*modename == "hill-climbing"){
      mode = HillClimbing;
    } else {
      cout << "Invalid mode name " << *modename << "." << endl;
      exit( 1 );
    }
    delete modename;
  }

  if(( fp = fopen( argv[1], "r" )) == NULL ){
    cout << "file open Error" << endl;
    exit( 1 );
  }
  string* filename = new string(argv[1]);
  TSPSolver* solver = new TSPSolver(*filename);
  cout << "N : " << endl;
  cin >> n;
  int min;
  vector<int> minRoute;
  if (mode == RandomSolve) {
    minRoute = solver->calcMinimumRandomRoute(n, &min);
  } else if (mode == HillClimbing) {
    minRoute = solver->hillClimbing(n, &min, RandomNeighbor);
  }
  ostringstream output;
  output << "random-" << n << ".dat";
  ofstream ofs(output.str().c_str());
  cout << "minimumCost : " << min << endl;
  ofs << "minimumCost : " << min << endl;
  for(int i = 0; i < solver->getDimension(); ++i) {
    Point* p = solver->getPoints()[minRoute[i]];
    cout << minRoute[i] << ", " << p->getX() << ", " << p->getY() << endl;
    ofs << minRoute[i] << ", " << p->getX() << ", " << p->getY() << endl;
  }
  delete filename;
  delete solver;
  fclose( fp );
  return 0;
}
