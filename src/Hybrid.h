#include<iostream>
#include<vector>
#include<cmath>

#include "Individual.h"

using namespace std;
class Hybrid {
 	private:
 		Individual bestSol, curSol;
 		Individual initial_solution;
        
        double  temperature, temperature0,
                alpha = 0.02;
        vector<Individual> critical_solutions;

        vector< pair<int,int> > tabu_list;
        unsigned int tabu_size;
        unsigned int    n_nodes, 
                        n_edges;
        int coolMode;
        vector<vector<unsigned int>> main_graph;
 	public:
 		Hybrid(double initial_temperature, unsigned int n_nodes, unsigned int n_edges, vector<vector<unsigned int>> main_graph);
 		Hybrid(double initial_temperature, int coolMode, int alpha,unsigned int n_nodes,unsigned int n_edges ,vector<vector<unsigned int>> main_graph);

 		Individual bestState();

    	int findCollision();

    	bool inTabu(int p, int c);

    	void coolDown(int k);

    	unsigned int CalculateFitness(Individual& ind);

    	void findNeighbor();

    	//void SearchNeighborhood();

    	void mainProcess(unsigned int max_iterations, double min_temp, unsigned int &total_iterations,unsigned int min_colors);

    	double getRandom(double start, double finish){
            random_device rd;
            mt19937 mt(rd());
            uniform_real_distribution<double> dist(start, finish);
        return dist(mt);
        }
 	
};