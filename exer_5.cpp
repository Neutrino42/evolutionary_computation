
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#define SIZE 50
#define N_MAX 3000
#define D 60
#define LAMBDA 60
using namespace std;

typedef struct ind {
    int item[D];
    int outcome;
} Indvl;

void crossover(Indvl *parent1, Indvl *parent2, Indvl *child);
void mutation(Indvl *child);

bool cmp(Indvl x, Indvl y) {
    return x.outcome > y.outcome;
}
static double mut_rate = 1/D;


int main() {
    int t = 1;
    int mu = SIZE; // the population size
    int lambda = LAMBDA; // the children size
    int n = 1;
    vector<Indvl> P;
    Indvl *parent1;
    Indvl *parent2;
    Indvl *child = new Indvl();
    Indvl *bsf = new Indvl(); // best-so-far solution
    Indvl *individual = new Indvl();
    int outcome;
    vector<Indvl> R;
    srand((unsigned int)(time(NULL)));
    
    int i, j;
    for(j=0; j<D; j++) { /* bsf initialization */
        bsf->item[j] = 0;
    }
    bsf->outcome = 0;
    // population initialization.
    for(i=0; i<mu; i++) {
        outcome = 0;
        for(j=0; j<D; j++) {
            if(rand()/(double)RAND_MAX < 0.5 ) {
                individual->item[j] = 0;
            } else {
                individual->item[j] = 1;
                outcome++;
            }
        }
        individual->outcome = outcome;
        P.push_back(*individual);
        
        if(individual->outcome > bsf->outcome) {
            for(j=0;j<D;j++) {
                bsf->item[j] = individual->item[j];
            }
            bsf->outcome = individual->outcome;
        }
    }
    
    while(n <= N_MAX) {
        for(i=0;i<lambda;i++) {
            int index1 = rand()%mu;
            int index2;
            while(index1 == (index2 = rand()%mu)) {
            }
            parent1 = &P[index1];
            parent2 = &P[index2];
            
            crossover(parent1, parent2, child);
            mutation(child);
            n++;
            R.push_back(*child);
            
            // step 5, update the best-so-far solution
            if(child->outcome > bsf->outcome) {
                for(j=0;j<D;j++) {
                    bsf->item[j] = child->item[j];
                }
                bsf->outcome = child->outcome;
            }
        }
        
        // step 6, environmental selection
        for(j=0;j<mu;j++) {
            R.push_back(P[j]);
        }
        stable_sort(R.begin(), R.end(), cmp);
        vector<Indvl>().swap(P); // empty P

        int k;
        mu = (mu+lambda)/2;
        for(k=0;k<mu;k++) {
            P.push_back(R[k]);
        }
        t++;
        printf("The best-so-far solution: ");
        for(j=0;j<D;j++) {
            printf("%d",bsf->item[j]);
        }
        printf("\tObjective function value: %d\n", bsf->outcome);
        
        vector<Indvl>().swap(R); // delete R
        if(bsf->outcome == D) break;
        
    }
    
    delete child;
    delete individual;
    vector<Indvl>().swap(P);
    delete bsf;
    return 0;
}


void crossover(Indvl *parent1, Indvl *parent2, Indvl *child) {
    int j;
    int out = 0;
    for(j=0;j<D;j++) {
        if(rand()/(double)RAND_MAX < 0.5) {
            child->item[j] = parent1->item[j];
            out += child->item[j];
        } else {
            child->item[j] = parent2->item[j];
            out += child->item[j];
        }
    }
    child->outcome = out;
}

void mutation(Indvl *child) {
    int j = 0;
    int out = 0;
    out = child->outcome;
    for(;j<D;j++) {
        if(rand()/(double)RAND_MAX < mut_rate) {
            if(child->item[j] == 0) {
                child->item[j] = 1;
                out++;
            } else {
                child->item[j] = 0;
                out--;
            }
        }
    }
    child->outcome = out;
}
