#include "Hybrid.h"

Hybrid::Hybrid(double initial_temperature, unsigned int n_nodes, unsigned int n_edges, vector<vector<unsigned int>> main_graph){
    this->n_nodes = n_nodes;
    this->n_edges = n_edges;
    this->main_graph = main_graph;
    this->tabu_size = n_edges/4;
    this->temperature = initial_temperature;
    this->temperature0 =initial_temperature;
    
    Individual newInd(n_nodes);
    this->bestSol = this->curSol = newInd;

};

Hybrid::Hybrid(double initial_temperature,int coolMode, int alpha,unsigned int n_nodes,unsigned int n_edges ,vector<vector<unsigned int>> main_graph){
    this->temperature = initial_temperature;
    this->main_graph = main_graph;
    this->n_nodes = n_nodes;
    this->n_edges = n_edges;
    this->coolMode = coolMode;
    this->alpha = alpha;

    Individual newInd(n_nodes);
    this->bestSol = newInd;
    this->curSol = newInd;
};

int Hybrid::findCollision(){
    int pos,pos_col;
    bool col = false;

    // If we already know that there aren't any collisions
    if(curSol.getFitness() < n_edges){

        for (unsigned int i = 0; i < n_nodes && !col; i++){
            for(unsigned int j = 0; j < main_graph[i].size() && !col; j++){
                pos = main_graph[i][j]; 

                if(curSol.at(i) == curSol.at(pos)){
                    pos_col = i;
                    col = true;
                }
            }
        }
    }

    if(!col){
        pos_col = getRandom(0, n_nodes);
    }

    return pos_col;
};

Individual Hybrid::bestState(){
    return this->bestSol;
};

bool Hybrid::inTabu(int p, int c){
    bool in_tabu_list = false;

    for(unsigned int i = 0; i < tabu_list.size() && !in_tabu_list ;i++){
        
        if(tabu_list[i].first == p && tabu_list[i].second == c)
            in_tabu_list = true;
    }

    return in_tabu_list;
};

void Hybrid::coolDown(int k){
    switch(coolMode){
        case 0:
            //Linear
            temperature = temperature - alpha;
        break;
        case 1:
            //Geometric
            temperature = temperature*alpha;
        default:
            //Boltzmann
            temperature = temperature0*(1/(log(k+1)));
        break;
    }
};

unsigned int Hybrid::CalculateFitness(Individual& ind){
    
    unsigned int fit = 0;
    //If we calculate it previously we dont need to do it again
    if(ind.getFitness() != 0){
        fit = ind.getFitness();
    }
    else{
        
        for(unsigned int i = 0; i < n_nodes; i++){
            
            for(unsigned int j = 0; j < main_graph[i].size(); j++){
                unsigned int cmp_number = ind.at( main_graph[i][j] );
                
                if(ind.at(i) != cmp_number){
                    fit++;
               
                }
            }

        }
        if(fit == n_edges)
            fit += (n_nodes - ind.getNumOfColours());

        ind.setFitness(fit);
    }
return fit;
};

void Hybrid::findNeighbor(){

    //Find a node with a col
    int node = this->findCollision();
    pair<int,int> best_move;

    unsigned int color = this->getRandom(0,n_nodes);
    
    //cout << "New Color :" << color << endl;
    if(curSol.at(node) == color)
        color = (color + 1) % n_nodes;

    
    Individual temp_solution = curSol;
    if (!this -> inTabu(node, color)){
        temp_solution.insertColor(node,color);
        temp_solution.setFitness(0);
    }

    unsigned int temp_fit = this->CalculateFitness(temp_solution);
    unsigned int current_fit = this->CalculateFitness(curSol);

    if(temp_fit > current_fit){
        curSol = temp_solution;
    }
    else{
        double p = this->getRandom(0,1);
        int diff_fitness = current_fit - temp_fit;

        if(p < exp(-diff_fitness/temperature))
            curSol = temp_solution;
    }
    best_move.first = node;
    best_move.second = this->curSol.at(node);
    if(tabu_list.size() >= tabu_size){
        tabu_list.erase(tabu_list.begin());
    }
    
    tabu_list.push_back(best_move);
};

void Hybrid::mainProcess(unsigned int max_iterations, double min_temp, unsigned int &total_iterations,unsigned int min_colors){
	int correct = 0;
    bool correct_color = false;
    for(unsigned int i = 0; i < max_iterations && !correct_color;i++){

        this->findNeighbor(); 
        
        if(curSol.getFitness() > bestSol.getFitness()){
            bestSol = curSol;
        } 
        this->coolDown(i);

        if(temperature <= min_temp){
            break;
        }
                
        if(curSol.getNumOfColours() == min_colors)
            correct++;
        if(correct == 10){
            correct_color = true;
            total_iterations = i;
        }
    }
    
    cout << "\n\nBest by Hibrid TS-SA: " << endl;
    bestSol.printChromosome();
    cout << "FIT: " << bestSol.getFitness() << endl;
};
