#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#define SIZE 10
#define N_MAX 2000
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
    int tmp;
    int *parent1;
    int *parent2;
    int child[D];
    int child_copy[D];
    int outcome;
    
    int *bsf; // best-so-far solution
    int bsf_out = 0;
    srand((unsigned int)(time(NULL)));
    // population initialization.
    int i, j;
    bsf = P[0];
    for(i=0; i<mu; i++) {
        for(j=0; j<D; j++) {
            if(rand()/(double)RAND_MAX < 0.5 ) {
                P[i][j] = 0;
            } else {
                P[i][j] = 1;
            }
        }
        tmp = calc_outcome(P[i]);
        bsf_out = calc_outcome(bsf);
        if(tmp > bsf_out) {
            bsf = P[i];
            bsf_out = tmp;
        }
    }
    while(n <= N_MAX ) {
        
        int index1 = rand()%mu; // Use indices to refer to each individual.
        int index2, index3, index4;
        
        while(index1 == (index2 = rand()%mu)) {
        } // Make sure each individual is different from others.
        while(index2 == (index3 = rand()%mu)) {
        }
        while(index3 == (index4 = rand()%mu)) {
        }
        parent1 = (calc_outcome(P[index1]) > calc_outcome(P[index2])) ? P[index1] : P[index2];
        parent2 = (calc_outcome(P[index3]) > calc_outcome(P[index4])) ? P[index3] : P[index4];
        
        crossover(parent1, parent2, child);
        mutation(child);
        
        outcome = calc_outcome(child);
        n++;
        // step 5, update the best-so-far solution
        if(outcome > bsf_out) {
            for(j=0;j<D;j++) {
                child_copy[j] = child[j];
            }
            bsf = child_copy;
            bsf_out = outcome;
        }
        // step 6, environmental selection
        int tmp = rand()%mu;
        if(outcome >= calc_outcome(P[tmp])) {
            for(j=0;j<D;j++) {
                P[tmp][j] = child[j];
            }
        }
        t++;
        
       /* for(i=0; i<mu; i++) {
            for(j=0; j<D; j++) {
                printf("%d",P[i][j]);
            }
            printf("\n");
        }*/
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
