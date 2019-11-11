#include <iostream>
#include <fstream>
#include <vector>
#include <sys/resource.h>
#include <random>
#include <ctime>
#include <ratio>
#include <chrono>


using namespace std;

#define END "\e[0m"
#define BLUE "\e[44m"
#define RED "\e[45m"
#define GREEN "\e[42m"


#include "SA.h"
#include "Tabu.h"
#include "Individual.h"
#include "Hybrid.h"


class Graph{
private: 
    vector<vector<unsigned int>> graph;
    unsigned int n_nodes, n_edges;

public:
    bool loadGraph(string fname){ //graph_name: Name of the graph
        
        ifstream plain_graph (fname);
        
        if(plain_graph.fail()) throw ("Cant open the file path");

        
        string input;
        plain_graph >> input;
        while(input != "edge")
            plain_graph >> input;
        
       
        plain_graph >> n_nodes >> n_edges;

        unsigned int node1,node2;
        graph.resize(n_nodes);

        while(!plain_graph.eof()){
            plain_graph >> node1 >> node2;
            graph[node1 - 1].push_back(node2 - 1);

        }
        plain_graph.close();
        return true;
    }

    void printGraph(){
        if(!(graph.size() > 0)){
            cout << "The graph is not loaded.";
        }
        else{
            for (size_t i = 0; i < graph.size(); i++)
            { 
                cout << endl << BLUE << "[" << i << "]" << END << "->";
                for (size_t j = 0; j < graph[i].size(); j++)
                {
                    cout << " " << graph[i][j] ;
                }
            }
            cout << endl;
        }
    }


    unsigned int getNodes(){
        return this->n_nodes;
    }
    unsigned int getEdges(){
        return this->n_edges;
    }

    vector<vector<unsigned int>> getGraph(){
        return this->graph;
    }

};


int main(int argc, char *argv[]){
    using namespace std::chrono;
    struct rusage r_usage;
    Graph graph_main;
    string fname = "data.txt";
    bool error = graph_main.loadGraph(fname);
    if(!error) {
        cout << "Could not load graph." << endl;
        return 0;
    }
    graph_main.printGraph();
	
    unsigned int totalIter = 0; 

    unsigned int maxIter = 100000;   

    unsigned int min_colors = 5;

    int choice = 0;
    
        cout << "1. Simulated annealing (SA)" << endl;
        cout << "2. Tabu Search (TS)" << endl;
        cout << "3. Hybrid TS-SA " << endl;
        cout << " Enter your choice: ";
        cin >> choice;


    switch(choice){
        
        case 1: {
            
            double initial_temp = 1.0;
            double min_temp = 0.01;
            high_resolution_clock::time_point t1 = high_resolution_clock::now(); 
            SA SA_solution(initial_temp,graph_main.getNodes(),graph_main.getEdges() ,graph_main.getGraph() );
            SA_solution.mainProcess(maxIter, min_temp, totalIter, min_colors); 
            getrusage(RUSAGE_SELF,&r_usage);
            printf("Memory usage = %ld\n",r_usage.ru_maxrss);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            cout << "Time consumming of SA: " << time_span.count() << endl;
            break;
        }
        case 2: {
            
            unsigned int neighborhood_size = 5;
            high_resolution_clock::time_point t1 = high_resolution_clock::now();            
            Tabu Tabu_solution(neighborhood_size, graph_main.getNodes(),graph_main.getEdges() ,graph_main.getGraph() );
            Tabu_solution.mainProcess(maxIter,totalIter, min_colors);
            getrusage(RUSAGE_SELF,&r_usage);
            printf("Memory usage = %ld\n",r_usage.ru_maxrss);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            cout << "Time consumming of TS: " << time_span.count() << endl;
            break;
        }
        case 3: {
            double initial_temp = 1.0;
            double min_temp = 0.01;
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            Hybrid Hybrid_solution(initial_temp, graph_main.getNodes(),graph_main.getEdges() ,graph_main.getGraph());
            Hybrid_solution.mainProcess(maxIter, min_temp, totalIter, min_colors);
            getrusage(RUSAGE_SELF,&r_usage);
            printf("Memory usage = %ld\n",r_usage.ru_maxrss);
             high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            cout << "Time consumming of hybrid TS-SA: " << time_span.count() << endl;
            break;
        }
        default:{
            
            break;
        }
    }


}

