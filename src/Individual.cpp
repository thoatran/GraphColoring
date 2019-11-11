#include "Individual.h"


    Individual :: Individual(unsigned int n_genes){
        this->n_genes = n_genes;
        //chromosome.resize(n_genes);


        
        int random;
        unsigned int colour = 0;
        
        for(unsigned int i = 0; i < n_genes; i++){

            
            random = this->getRandom(0.0 , n_genes);

            if(random == 0 || i == 0)
                chromosome.push_back(colour);
            else{
                colour++;
                chromosome.push_back(colour);
            } 
        }
    };

    Individual::Individual(vector<unsigned int> chromosome){
        this->chromosome = chromosome;
        this->n_genes = chromosome.size();
    };

    unsigned int Individual::getNumOfColours(){
        set<int> distinct_colors;

        for(auto curr_int = chromosome.begin(), end = chromosome.end(); curr_int != end; ++curr_int)
        {
            distinct_colors.insert(*curr_int);
        }

        return distinct_colors.size();
    };

    void Individual::printChromosome(){

        for(unsigned int i = 0; i < chromosome.size(); i++){
            cout << GREEN << BLACK_FOREGRND << " " << this->chromosome[i]  << END ;
        }
        cout << " N of colour used: " <<this->getNumOfColours() <<endl;            
    };

    unsigned int Individual::at(unsigned int i){
        return this->chromosome[i];
    };

    void Individual::setFitness(unsigned int fitness){
        this->fitness = fitness;
    };

    unsigned int Individual::getFitness(){
        return this->fitness;
    };



    void Individual::insertColor(int p, int c){
        chromosome[p] = c;
    };


