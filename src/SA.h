#include<iostream>
#include<vector>
#include<cmath>

#include "Individual.h"

using namespace std;

class SA{
    private:
        Individual bestSol;
        Individual curSol;
        double  temperature,
                temperature0,
                alpha = 0.02;
        unsigned int    n_nodes, 
                        n_edges;
        int coolMode;
        vector<vector<unsigned int>> main_graph;

    public:

        SA(double initial_temperature,unsigned int n_nodes,unsigned int n_edges ,vector<vector<unsigned int>> main_graph);

        //
        SA(double initial_temperature,int coolMode, int alpha,unsigned int n_nodes,unsigned int n_edges ,vector<vector<unsigned int>> main_graph);

        Individual bestState();

        /*
        */
        int findCollision();

        /*  It will find in the neighborhood a suitable neighbor
            Collision = Two nodes with the same colour and with a common edge
        */
        void findNeighbor();

        /*  Reduce the temperature according to a function
        */
        void coolDown(int k);

        void mainProcess(unsigned int max_iterations, double min_temp, unsigned int &total_iterations, unsigned int min_colors);

        unsigned int CalculateFitness(Individual &ind);


        double getRandom(double start, double finish){
            random_device rd;
            mt19937 mt(rd());
            uniform_real_distribution<double> dist(start, finish);
        return dist(mt);
        }


};