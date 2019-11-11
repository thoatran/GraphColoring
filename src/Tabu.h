#include<iostream>
#include<vector>
#include<list>
#include <unistd.h>


#include "Individual.h"

using namespace std;

class Tabu {
    private:

        Individual  bestSol,
                    curSol;

        Individual initial_solution;

        vector<Individual> critical_solutions;

        vector< pair<int,int> > tabu_list;
        unsigned int tabu_size;
        unsigned int neighborhood_size;

        unsigned int    n_nodes, 
                        n_edges;
        vector<vector<unsigned int>> main_graph;




    public:

        Tabu(unsigned int neighborhood_size, unsigned int n_nodes, unsigned int n_edges, vector<vector<unsigned int>> main_graph);

        /*  It will find the first collision of a individual
            if there is no collision it will return a random node of the individual
        */
        int findCollision();

        /*  Checks if the move is not in the tabu list
            p: position
            c: color
        */
        bool inTabu(int p, int c);

        /*  It will create a new neighborhood and choose the next neighbor based
            on his fitness and if it is not in the memory
        */
        void findNeighbor();

        void mainProcess(unsigned int max_iterations, unsigned int &total_iterations, unsigned int min_colors);

        unsigned int CalculateFitness(Individual &ind);

        double getRandom(double start, double finish){
            random_device rd;
            mt19937 mt(rd());
            uniform_real_distribution<double> dist(start, finish);
        return dist(mt);
        }

        


};