// case dj38
// (mu + lambda)-GA

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#define SIZE 200
#define N_MAX 100000
#define LAMBDA 300
#define D 38
using namespace std;

typedef struct ind {
    int item[D];
    double outcome;
} Indvl;

int crossover(Indvl *parent1, Indvl *parent2, Indvl *child);
int mutation(Indvl *child);
int dist_init(double ** &p);
int calc_dist(Indvl *individual);
bool cmp(Indvl x, Indvl y) {
    return x.outcome < y.outcome;
}
static double tsp_data_x[D] = {
    11003.611100,11108.611100,11133.333300,11155.833300,11183.333300,
    11297.500000,11310.277800,11416.666700,11423.888900,11438.333300,
    11461.111100,11485.555600,11503.055600,11511.388900,11522.222200,
    11569.444400,11583.333300,11595.000000,11600.000000,11690.555600,
    11715.833300,11751.111100,11770.277800,11785.277800,11822.777800,
    11846.944400,11963.055600,11973.055600,12058.333300,12149.444400,
    12286.944400,12300.000000,12355.833300,12363.333300,12372.777800,
    12386.666700,12421.666700,12645.000000 };
static  double tsp_data_y[D] = {
    42102.500000,42373.888900,42885.833300,42712.500000,42933.333300,
    42853.333300,42929.444400,42983.333300,43000.277800,42057.222200,
    43252.777800,43187.222200,42855.277800,42106.388900,42841.944400,
    43136.666700,43150.000000,43148.055600,43150.000000,42686.666700,
    41836.111100,42814.444400,42651.944400,42884.444400,42673.611100,
    42660.555600,43290.555600,43026.111100,42195.555600,42477.500000,
    43355.555600,42433.333300,43156.388900,43189.166700,42711.388900,
    43334.722200,42895.555600,42973.333300
};


static double **p; // the distance between every two cities.

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
    
    dist_init(p); // initialize the distance matrix
    
    int i, j, k;
    int tmpv;
    outcome = 0;
    /* bsf initialization*/
    for(j=0; j<D; j++) {
        bsf->item[j] = j;
    }
    calc_dist(bsf);
    /* end bsf initialization*/
    
    // population initialization.
    for(i=0; i<mu; i++) {
        outcome = 0;
        
        /* individual initialization */
        for(j=0; j<D; j++) {
            individual->item[j] = j;
            outcome += j;
        }
        for(j=0; j<D-1; j++) { /* Fisher–Yates shuffle */
            k = rand()%(D-j) + j;
            tmpv = individual->item[j];
            individual->item[j] = individual->item[k];
            individual->item[k] = tmpv;
        }
        calc_dist(individual);
        /* end individual initialization */
        
        P.push_back(*individual); // add individual to the population
        
        if(individual->outcome < bsf->outcome) {
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
            if(child->outcome < bsf->outcome) {
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
        printf("The best-so-far solution (city index): ");
        for(k=0;k<D;k++) {
            printf("%d ",bsf->item[k]);
        }
        printf("\tObjective function value: %f\n", bsf->outcome);
        
        vector<Indvl>().swap(R); // delete R
        t++;
        
        
    }
    delete individual;
    delete child;
    vector<Indvl>().swap(P);
    for(i=0; i<D; i++) {
        free(p[i]);
    }
    free(p);
    delete bsf;
    return 0;
}


int crossover(Indvl *parent1, Indvl *parent2, Indvl *child) {
    int j;
    int c1,c2,tmp;
    int h,l;
    for(j=0;j<D;j++) {
        child->item[j] = parent1->item[j];
    }
    child->outcome = parent1->outcome;
    
    c1 = rand() % D;
    while(c1 == (c2 = rand() % D)) {
    }
    if(c1>c2) {
        tmp = c1;
        c1 = c2;
        c2 = tmp;
    }
    
    for(j=c1; j<=c2; j++) {
        h = 0;
        while(parent2->item[j] != child->item[h]) {
            h++;
        }
        l = h + 1;
        while(l!=c2+1) {
            if(h==D) {
                h = 0;
                l = 1;
            }
            if(l==D) {
                l = 0;
            }
            child->item[h] = child->item[l];
            h++;
            l++;
        }
    }
    
    for(j=c1;j<=c2;j++) {
        child->item[j] = parent2->item[j];
    }
    calc_dist(child); // calculate the outcome (total distance)
    
    return 0;
}

int mutation(Indvl *child) {
    int k,h;
    int tmp,m1,m2;
    double l;
    int out = 0;
    out = child->outcome;
    
    m1 = rand()%D;
    while(m1 == (m2=rand()%D)) {
    }
    if(m1 > m2) {
        tmp = m2;
        m1 = m2;
        m2 = tmp;
    }
    h = (m2-m1+1)/2;
    for(k=0; k<h; k++) {
        l = child->item[m1+k];
        child->item[m1+k] = child->item[m2-k];
        child->item[m2-k] = l;
    }
    calc_dist(child); // calculate the outcome (total distance)
    return 0;
}

int dist_init(double ** &p) { /* distance initialization */
    int i,j;
    p = (double **)malloc(sizeof(double)*D*D);
    for(i=0; i<D; i++) {
        p[i] = (double *)malloc(sizeof(double)*D);
        for(j=0; j<D; j++) {
            p[i][j] = sqrt(pow((tsp_data_x[i]-tsp_data_x[j]),2)+pow((tsp_data_y[i]-tsp_data_y[j]),2));
        }
    }
    return 0;
}

int calc_dist(Indvl *individual) { // calculate the outcome (total distance).
    int j;
    int index1, index2 = 0;
    double L = 0;
    for(j=0;j<D-1;j++) {
        index1 = individual->item[j];
        index2 = individual->item[j+1];
        L = L + p[index1][index2];
    }
    index1 = individual->item[0];
    L += p[index2][index1];
    individual->outcome = L;
    return 0;
}
