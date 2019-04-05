/* ------------------------------ Algorithme de fourmi
 * ------------------------------ */
/*
/*
/* -----------------------   il y a trois problemes de TSP
----------------------- */
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// ** alpha is  information constant
// ** beta is expectation constant
// ** rou is global parameter of decrease of information
// ** alpha1 is local parameter of decrease of information
// ** pzero is a constant to compare
//----we have three questions here:1,2,3

/*
//----------(1)Question 1 ：Oliver 30 cities TSP question best_length =
423.7406; ------------------------
//Best result is 423.741，run it too many times to get the result
//the number of cities
#define N 30
//coordinate of the city
double C[N][2] =
{
        {2,99},{4,50},{7,64},{13,40},{18,54},{18,40},{22,60},{24,42},{25,62},{25,38},
        {37,84},{41,94},{41,26},{44,35},{45,21},{54,67},{54,62},{58,35},{58,69},{62,32},
        {64,60},{68,58},{71,44},{71,71},{74,78},{82,7},{83,46},{83,69},{87,76},{91,38}
};
//----------the variables above are constant，the variables below are
changeable-----------
//number of ants
#define M 30
//the largest cycle-index is NcMax
int NcMax = 500;
double alpha = 2, beta = 3, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------End of question
1------------------------------------------------------------------------
*/

/*
//----------(2)Question 2     ：Elion50 城市 TSP 问题 best_length = 427.96;
----------------------------
//The best result is 428.468，run it too many times to get the result
//number of cities
#define N 50
//coordinate of the city
double C[N][2] =
{
        {5,64}, {5,25}, {5,6}, {7,38}, {8,52}, {10,17},
        {12,42}, {13,13}, {16,57}, {17,33}, {17,63},
        {20,26}, {21,47}, {21,10}, {25,32}, {25,55},
        {27,68}, {27,23}, {30,48}, {30,15}, {31,62},
        {31,32}, {32,22}, {32,39}, {36,16}, {37,69},
        {37,52}, {38,46}, {39,10}, {40,30}, {42,57},
        {42,41}, {43,67}, {45,35}, {46,10}, {48,28},
        {49,49}, {51,21}, {52,33}, {52,41}, {52,64},
        {56,37}, {57,58}, {58,27}, {58,48}, {59,15},
        {61,33}, {62,42}, {62,63}, {63,69}
};
//----------the variables above are constant，the variables below are
changeable-----------
//number of ants
#define M 50
//the largest cycle-index is NcMax
int NcMax = 1000;
double alpha = 2, beta = 4, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------End of question
2------------------------------------------------------------------------
*/

//----------(3)Question 3：Elion75 city TSP question best_length = 542.31;
// The best result is 542.309，we can get it by running the programe a lot of
// times number of the cities
#define N 75
// coordinate of the city
double C[N][2] = {
    {6, 25},  {7, 43},  {9, 56},  {10, 70}, {11, 28}, {12, 17}, {12, 38},
    {15, 5},  {15, 14}, {15, 56}, {16, 19}, {17, 64}, {20, 30}, {21, 48},
    {21, 45}, {21, 36}, {22, 53}, {22, 22}, {26, 29}, {26, 13}, {26, 59},
    {27, 24}, {29, 39}, {30, 50}, {30, 20}, {30, 60}, {31, 76}, {33, 34},
    {33, 44}, {35, 51}, {35, 16}, {35, 60}, {36, 6},  {36, 26}, {38, 33},
    {40, 37}, {40, 66}, {40, 60}, {40, 20}, {41, 46}, {43, 26}, {44, 13},
    {45, 42}, {45, 35}, {47, 66}, {48, 21}, {50, 30}, {50, 40}, {50, 50},
    {50, 70}, {50, 4},  {50, 15}, {51, 42}, {52, 26}, {54, 38}, {54, 10},
    {55, 34}, {55, 45}, {55, 50}, {55, 65}, {55, 57}, {55, 20}, {57, 72},
    {59, 5},  {60, 15}, {62, 57}, {62, 48}, {62, 35}, {62, 24}, {64, 4},
    {65, 27}, {66, 14}, {66, 8},  {67, 41}, {70, 64}};
//----------the variables above are constant，the variables below are
// changeable----------- number of ants
#define M 75
// the largest cycle-index is NcMax
int NcMax = 3500;
double alpha = 2, beta = 5, rou = 0.1, alpha1 = 0.1, qzero = 0.1;
//-----------End of question
// 3------------------------------------------------------------------------

//===========================================================================================================
// What is the closest methods? It is that every time the ant departs from the
// original point and choose a city(point) whose distance to the original point
// is the shortest to go. each panel point can be used as the original point
double Lnn; // it is a constant which we can get from the methods of nearest
            // neighbor analysis, we use it to as a constant to update the local
            // environment
// the matrix is the distance of the two cities
double allDistance[N][N];

/*----------------------------- there are some useful fonctions
 * -----------------------------*/

// Calculate the distance of the two cities
double calculateDistance(int i, int j) {
  return sqrt(pow((C[i][0] - C[j][0]), 2.0) +
              pow((C[i][1] - C[j][1]), 2.0)); // we have used the fonction pow()
}

// We use a matrix to prensent the distance of the two cities
void calculateAllDistance() {
  int i, j;
#pragma omp parallel for
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (i != j) {
        allDistance[i][j] = calculateDistance(i, j);
        allDistance[j][i] = allDistance[i][j];
      }
    }
  }
}

// Get the length of the path through "n"(number) cities
double calculateSumOfDistance(int *tour) {
  double sum = 0;
  int i;
  int temp[N];

#pragma omp parallel for reduction(+sum)
  for (i = 0; i < N; i++) {
    int row = *(tour + 2 * i);
    int col = *(tour + 2 * i + 1);
    sum += allDistance[row][col];
  }
  return sum;
}

// The fonction of absolute . I don't know why I need to write this fonction
// myself...omg
int abs(double x) {
  if (x < 0)
    x = -x;
  return x;
}

/*------------------------------- There are two classes and some fonctions about
 * the two classes -------------------------------*/
/* two classes define the law of ants' moving */

class AntColonySystem // definition the class of the law of the colony
{
private:
  double info[N][N],
      visible[N][N]; // the intensity of  pheromone of the two cities, the
                     // visibility of the two cities. All are the viriables of
                     // the the two cities.
public:
  AntColonySystem() {}
  double Transition(int i, int j); // Calculate the probability of the transfer
                                   // of the current point to the next point
  void UpdateLocalPathRule(int i,
                           int j);  // The law of the update of the local path
  void InitParameter(double value); // Initialization of the parameters
  void
  UpdateGlobalPathRule(int *bestTour,
                       int globalBestLength); // Update of the global pheromone
};

class ACSAnt // Definition the class of ants
{
private:
  AntColonySystem *antColony;

protected:
  int startCity, cururentCity; // number of original city, number of current
                               // city
  int allowed[N];              // list"never go"
  int Tour[N][2];              // current path we have visited
  int currentTourIndex; // current path , start from 0 and store the number of
                        // the city which the ant has been to
public:
  ACSAnt(AntColonySystem *acs, int start) {
    antColony = acs;
    startCity = start;
  }
  int *Search();                     // begin to search
  int Choose();                      // choose next point
  void MoveToNextCity(int nextCity); // move to next point
};

// Calcul the probability of the transfer of the current point to the next point
double AntColonySystem::Transition(int i, int j) {
  if (i != j) {
    return (pow(info[i][j], alpha) *
            pow(visible[i][j], beta)); // with the increasing of the number of
                                       // the cities, beta is also increasing
  } else {
    return 0.0;
  }
}

// The law of the update of the local path
void AntColonySystem::UpdateLocalPathRule(int i, int j) {
  info[i][j] = (1.0 - alpha1) * info[i][j] + alpha1 * (1.0 / (N * Lnn));
  info[j][i] = info[i][j];
}
// Initialization
void AntColonySystem::InitParameter(double value) {
  int i, j;
// Initilization of the pheromone of the path
#pragma omp parallel for
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      info[i][j] = value;
      info[j][i] = value;
      if (i != j) {
        visible[i][j] =
            1.0 /
            allDistance[i][j]; // the distance is longer ,the visible is littler
        visible[j][i] = visible[i][j];
      }
    }
  }
}

// Update of the global pheromone
void AntColonySystem::UpdateGlobalPathRule(int *bestTour,
                                           int globalBestLength) {
  int i;
#pragma omp parallel for
  for (i = 0; i < N; i++) {
    int row = *(bestTour + 2 * i);     // row  0,2,4,6...
    int col = *(bestTour + 2 * i + 1); // clo  1,3,5,7...
    info[row][col] =
        (1.0 - rou) * info[row][col] + rou * (1.0 / globalBestLength);
    info[col][row] = info[row][col];
  }
}

// begin to search
int *ACSAnt::Search() {
  int toCity, i;
  cururentCity = startCity;
  currentTourIndex = 0;
#pragma omp parallel for
  for (i = 0; i < N; i++) {
    allowed[i] = 1;
  }
  allowed[cururentCity] = 0;
  int endCity;
  int count = 0;
  do {
    count++;
    endCity = cururentCity;
    toCity = Choose();
    if (toCity >= 0) {
      MoveToNextCity(toCity);
      antColony->UpdateLocalPathRule(endCity, toCity);
      cururentCity = toCity;
    }
  } while (toCity >= 0);

  MoveToNextCity(startCity);
  antColony->UpdateLocalPathRule(endCity, startCity);

  return *Tour;
}

// choose next point
int ACSAnt::Choose() {
  int nextCity = -1;
  int i;
  double q = rand() / (double)RAND_MAX;
  if (q <=
      qzero) // q is a random number, we have given q0 , it is a probability
  {
    double probability = -1.0; // the probability of moving to the next point

    for (i = 0; i < N; i++) {
      if (allowed[i] ==
          1) // delete the point of the list"never go", and choose the point
             // whose probability is the largest then go to this point
      {
        double prob = antColony->Transition(
            cururentCity, i); // prob is the probability of transition
        if (prob > probability) {
          nextCity = i;
          probability = prob;
        }
      }
    }
  } else {
    double p = rand() / (double)RAND_MAX; // Get a random number
    double sum = 0.0;
    double probability = 0.0;
    int j;
    for (i = 0; i < N; i++) {
      if (allowed[i] == 1) {
        sum += antColony->Transition(cururentCity, i);
      }
    }
    for (int j = 0; j < N; j++) {
      if (allowed[j] == 1 && sum > 0) {
        probability += antColony->Transition(cururentCity, j) / sum;
        if (probability >= p || (p > 0.9999 && probability > 0.9999)) {
          nextCity = j;
          break;
        }
      }
    }
  }
  return nextCity;
}

// move to the next city
void ACSAnt::MoveToNextCity(int nextCity) {
  allowed[nextCity] = 0;
  Tour[currentTourIndex][0] = cururentCity;
  Tour[currentTourIndex][1] = nextCity;
  currentTourIndex++;
  cururentCity = nextCity;
}

/*------------------- we use the nearest neighbour analysis to calculate a
 * length -------------------*/

// choose the next point and calcul the length
int ChooseNextNode(int currentNode, int visitedNode[]) {
  int nextNode = -1;
  int i;
  double shortDistance = 0.0;

  for (i = 0; i < N; i++) {
    if (visitedNode[i] == 1) {
      if (shortDistance == 0.0) {
        shortDistance = allDistance[currentNode][i];
        nextNode = i;
      }
      if (shortDistance < allDistance[currentNode][i]) {
        nextNode = i;
      }
    }
  }
  return nextNode;
}

double CalAdjacentDistance(int node) {
  double sum = 0.0;
  int j;
  int visitedNode[N];
#pragma omp parallel for
  for (j = 0; j < N; j++) {
    visitedNode[j] = 1;
  }
  visitedNode[node] = 0;
  int currentNode = node;
  int nextNode;
  do {
    nextNode = ChooseNextNode(currentNode, visitedNode);
    if (nextNode >= 0) {
      sum += allDistance[currentNode][nextNode];
      currentNode = nextNode;
      visitedNode[currentNode] = 0;
    }
  } while (nextNode >= 0);
  sum += allDistance[currentNode]
                    [node]; // The last step is to add the distance of the last
                            // node we have visited and the original node
  return sum;
}

/*-------------------------------- Main Fonction
 * --------------------------------*/
int main(int argc, char *argv[]) {
  int i, j, k, m;
  time_t timer, timerl;
  time(&timer);
  unsigned long seed = timer;
  seed = seed % 56000;
  srand((unsigned int)seed); // we use this to prove that each time the random
                             // number produced by rand() is maybe different
  calculateAllDistance();    // execute the fonction calculateAllDistance
  AntColonySystem *acs =
      new AntColonySystem(); // definition *acs as the class AntColonySystem
  ACSAnt *ants[M];           // definition ants[M] as the class ACSAnt
  for (k = 0; k < M; k++)    // the ants distribute uniformly in the cities
  {
    ants[k] = new ACSAnt(
        acs,
        (int)(k %
              N)); // start point is < M, each number will be the start point
  }
  calculateAllDistance();
  int node = rand() % N; // get a random number as a node to calculate from this
                         // node the best path
  Lnn = CalAdjacentDistance(
      node); // form original node , the shortest distance of the path
  double initInfo =
      1 / (N * Lnn); // initialize the intensity of  pheromone of each path
  acs->InitParameter(initInfo);
  int globalTour[N][2]; // definition the matrix of the best global path
  double globalBestLength = 0.0; // definition the shorest path

  for (i = 0; i < NcMax;
       i++) // we need to do this for NcMax times to get the best result
  {
    int localTour[N][2];          // local  best path
    double localBestLength = 0.0; // local  best length
    double tourLength;            // length of current paths we have visited
    for (j = 0; j < M; j++) {
      int *tourPath = ants[j]->Search(); // use the ants-search
      tourLength = calculateSumOfDistance(
          tourPath); // we can get the length of this tour
      // local caomparaison  and write the path and the length
      if (tourLength < localBestLength ||
          abs(localBestLength - 0.0) < 0.000001) {
#pragma omp parallel for
        for (m = 0; m < N; m++) {
          int row = *(tourPath + 2 * m);
          int col = *(tourPath + 2 * m + 1);
          localTour[m][0] = row;
          localTour[m][1] = col;
        }
        localBestLength = tourLength;
      }
    }
    // the global comparaison and write the path and the length
    if (localBestLength < globalBestLength ||
        abs(globalBestLength - 0.0) < 0.000001) {
#pragma omp parallel for
      for (m = 0; m < N; m++) {
        globalTour[m][0] = localTour[m][0];
        globalTour[m][1] = localTour[m][1];
      }
      globalBestLength = localBestLength;
    }
    acs->UpdateGlobalPathRule(*globalTour, globalBestLength);
    // output each time the best
    cout << "----------------------------------------" << endl;
    cout << "The " << i + 1
         << " times interation , the best path is:" << localBestLength << "."
         << endl;
    for (m = 0; m < N; m++) {
      cout << localTour[m][0] << ".";
    }
    cout << endl;
    cout << "----------------------------------------" << endl;
  }
  cout << "----------------***********----------------------" << endl;
  cout << "---------------------fourmi----------------------" << endl;
  cout << "----------------***********----------------------" << endl;
  cout << "The length of the best global path:" << globalBestLength << endl;
  cout << "The best global path:";
  for (m = 0; m < N; m++) {
    cout << globalTour[m][0] << ".";
  }
  cout << endl;
  cout << "----------------************----------------------" << endl;
  time(&timerl); // end time
  cout << "----------------time to calculate----------------------" << endl;
  int t = timerl - timer;
  cout << "    Time begins at " << timer << endl;
  cout << "    Time ends at " << timerl << endl;
  cout << "    The time we spend is:" << t << endl;
  cout << "----------------End of all----------------------" << endl;
  return 0;
}
/*--------------------------End of main
 * fonction--------------------------------------------------*/
