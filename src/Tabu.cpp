#include "Tabu.h"


Tabu::Tabu(unsigned int neighborhood_size, unsigned int n_nodes, unsigned int n_edges, vector<vector<unsigned int>> main_graph){
    this->neighborhood_size = neighborhood_size;
    this->n_nodes = n_nodes;
    this->n_edges = n_edges;
    this->main_graph = main_graph;
    this->tabu_size = n_edges/4;
    
    Individual newInd(n_nodes);
    this->bestSol = this->curSol = newInd;

}



int Tabu::findCollision(){
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

bool Tabu::inTabu(int p, int c){
    bool in_tabu_list = false;

    for(unsigned int i = 0; i < tabu_list.size() && !in_tabu_list ;i++){
        
        if(tabu_list[i].first == p && tabu_list[i].second == c)
            in_tabu_list = true;
    }

    return in_tabu_list;
};



void Tabu::findNeighbor(){
    //Find a node to create a neighborhood
    int node = this->findCollision();
    set<int> neighborhood;
    set<int>::iterator it;
    pair<int,int> best_move;
    Individual best_neighbor;
    Individual local_current = best_neighbor = this->curSol;
    best_neighbor.setFitness(0);
    unsigned int failed_neighborhood = 0; 
    int index = 0;

    while(neighborhood.size() != neighborhood_size){
        if(failed_neighborhood == neighborhood_size*2){
            node = getRandom(0, n_nodes);
            if(critical_solutions.size() > 0){
                local_current = critical_solutions.at(critical_solutions.size()-1);
                critical_solutions.pop_back();
            }   
            failed_neighborhood = 0;
        }




        //Pick a color different from the one in the node
        unsigned int color = this->getRandom(0,n_nodes);

        
        if(curSol.at(node) == color)
            color = (color + 1) % n_nodes;

        bool found = false; 

        //Check if that color is already in the neighborhood
        if(neighborhood.size() > 0){
            it = neighborhood.find(color);
            if(it != neighborhood.end())
                found = true; 
        }
        

        //Check if the move is in the tabu list
        if(!found && !this->inTabu(node, color)){
            neighborhood.insert(color);

            local_current.setFitness(0);
            local_current.insertColor(node,color);
            this->CalculateFitness(local_current);

            if( best_neighbor.getFitness() < local_current.getFitness()){
                best_neighbor = local_current;
            }
        }
        else{
            failed_neighborhood++;
        }
        index++;
    }

    best_move.first = node;
    best_move.second = best_neighbor.at(node);    

    if(tabu_list.size() >= tabu_size){
        tabu_list.erase(tabu_list.begin());
    }
    
    tabu_list.push_back(best_move);
    this->curSol = best_neighbor;
    

};

void Tabu::mainProcess(unsigned int max_iterations, unsigned int &total_iterations, unsigned int min_colors){

    int minNumberofSolutions = 0;
    bool found = false;
    for (total_iterations = 0; total_iterations < max_iterations && !found; total_iterations++){

        //cout << "IT: " << i << endl;
        //curSol.printChromosome();

        this->CalculateFitness(curSol);
        if(curSol.getNumOfColours() == min_colors){
            minNumberofSolutions++;
            if(minNumberofSolutions == 10)
                found = true;
        }

        this->findNeighbor();
        if(bestSol.getFitness() < curSol.getFitness()){
            bestSol = curSol;
            critical_solutions.push_back(curSol);
        }

    }

    cout << "\n\nBest Solution:" << endl;
    bestSol.printChromosome();
    cout << "FIT: " << bestSol.getFitness() << endl;


};

unsigned int Tabu::CalculateFitness(Individual& ind){
    
    unsigned int fit = 0;
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