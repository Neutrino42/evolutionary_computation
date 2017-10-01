// case wi29
// (mu + lambda)-GA
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#define SIZE 200
#define N_MAX 1000000
#define LAMBDA 300
#define D 29
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
    20833.3333,20900.0000,21300.0000,21600.0000,21600.0000,21600.0000,
    22183.3333,22583.3333,22683.3333,23616.6667,23700.0000,23883.3333,
    24166.6667,25149.1667,26133.3333,26150.0000,26283.3333,26433.3333,
    26550.0000,26733.3333,27026.1111,27096.1111,27153.6111,27166.6667,
    27233.3333,27233.3333,27266.6667,27433.3333,27462.5000 };
static  double tsp_data_y[D] = {
    17100.0000, 17066.6667,13016.6667,14150.0000,14966.6667,16500.0000,
    13133.3333,14300.0000,12716.6667,15866.6667,15933.3333,14533.3333,
    13250.0000,12365.8333,14500.0000,10550.0000,12766.6667,13433.3333,
    13850.0000,11683.3333,13051.9444,13415.8333,13203.3333,9833.3333,
    10450.0000,11783.3333,10383.3333,12400.0000,12992.2222
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
    for(i=0; i<D; i++) {
        free(p[i]);
    }
    free(p);
    delete bsf;
    vector<Indvl>().swap(P);
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
