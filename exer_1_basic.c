#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define SIZE 20
#define N_MAX 1000
#define D 60

void crossover(int *parent1, int *parent2, int *child);
void mutation(int *child);
int calc_outcome(int *individual);

static double mut_rate = 1/(double)D;

int main() {
    int t = 1;
    int mu = SIZE; // the population size
    int n = 1;
    int P[mu][D];
    int *parent1;
    int *parent2;
    int child[D];
    int child_copy[D];
    int outcome;
    
    int *bsf; // best-so-far solution
    srand((unsigned int)(time(NULL)));
    // population initialization.
    int i, j;
    bsf = P[0];
    for(i=0; i<mu; i++) {
        for(j=0; j<D; j++) {
            if((double)rand()/(double)RAND_MAX < 0.5 ) {
                P[i][j] = 0;
            } else {
                P[i][j] = 1;
            }
        }
        if(calc_outcome(P[i]) > calc_outcome(bsf)) {
            bsf = P[i];
        }
    }
    
    while(n <= N_MAX ) {
        
        int index1 = rand()%mu;
        int index2;
        parent1 = P[index1];
        while(index1 == (index2 = rand()%mu)) {
        }
        parent2 = P[index2];
        crossover(parent1, parent2, child);
        mutation(child);
        
        outcome = calc_outcome(child);
        n++;
        
        // step 5, update the best-so-far solution
        int bsf_out = calc_outcome(bsf);
        if(outcome > bsf_out) {
            for(j=0;j<D;j++) {
                child_copy[j] = child[j];
            }
            bsf = child_copy;
        }
        // step 6, environmental selection
        int tmp = rand()%mu;
        if(outcome >= calc_outcome(P[tmp])) {
            for(j=0;j<D;j++) {
                P[tmp][j] = child[j];
            }
        }
        t++;
        printf("The best-so-far solution: ");
        for(j=0;j<D;j++) {
            printf("%d",bsf[j]);
        }
        printf("\tObjective function value: %d\n", bsf_out);
        if(bsf_out == D) break;
    }
    
    return 0;
}


void crossover(int *parent1, int *parent2, int child[]) {
    int j;
    for(j=0;j<D;j++) {
        if((double)rand()/(double)RAND_MAX < 0.5) {
            child[j] = parent1[j];
        } else {
            child[j] = parent2[j];
        }
    }
}

void mutation(int *child) {
    int j = 0;
    for(;j<D;j++) {
        if((double)rand()/(double)RAND_MAX < mut_rate) {
            if(child[j] == 0) {
                child[j] = 1;
            } else {
                child[j] = 0;
            }
        }
    }
}

int calc_outcome(int *individual) {
    int out = 0;
    int j = 0;
    for(;j<D;j++) {
        out += individual[j];
    }
    return out;
}
