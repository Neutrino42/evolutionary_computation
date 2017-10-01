#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#define SIZE 100
#define N_MAX 100000000
#define D 5
#define LAMBDA 200
#define min(a, b) ((a) < (b) ? (a) : (b)) 
#define max(a, b) ((a) > (b) ? (a) : (b))
#define OPTIMAL_SLN 0
#define FUNC_SWITCH 3
                /* switch for different objective function
                    1 for sphere_func()
                    2 for rastrigin_func()
                    3 for rosenbrock_func()
                */
using namespace std;

typedef struct ind {
    double item[D];
    double outcome;
} Indvl;

int crossover(Indvl *parent1, Indvl *parent2, Indvl *child, double x_max, double x_min);
int sphere_func(Indvl *individual);
int rastrigin_func(Indvl *individual);
int rosenbrock_func(Indvl *individual);
bool cmp(Indvl x, Indvl y) {
    return x.outcome < y.outcome;
}
static int func = FUNC_SWITCH;

int main() {
    int t = 1;
    int mu = SIZE; // the population size
    int lambda = LAMBDA; // the children size
    int n = 1;
    double x_max, x_min; // the search space boundary
    vector<Indvl> P;
    Indvl *parent1;
    Indvl *parent2;
    Indvl *child = new Indvl();
    Indvl *bsf = new Indvl(); // best-so-far solution
    Indvl *individual = new Indvl();
    vector<Indvl> R;
    srand((unsigned int)(time(NULL)));
    int i, j;

    // initialization for search space boundary
    switch (func) {
        case 1:
            x_max = 100;
            x_min = -100;
            break;
        case 2:
            x_max = 5.12;
            x_min = -5.12;
            break;
        case 3:
            x_max = 30;
            x_min = -30;
            break;
        default:
            x_max = 100;
            x_min = -100;
            break;
    }
    
    // population initialization.
    for(i=0; i<mu; i++) {
        for(j=0; j<D; j++) {
            individual->item[j] = (x_max - x_min) * (double)rand()/double(RAND_MAX) + x_min;
            if(j==0) { // bsf item[] initialization
                bsf->item[j] = individual->item[j];
            }
        }
        switch (func) { /* calculate the outcome */
            case 1:
                sphere_func(individual);
                break;
            case 2:
                rastrigin_func(individual);
                break;
            case 3:
                rosenbrock_func(individual);
                break;
            default:
                sphere_func(individual);
                break;
        }
        bsf->outcome = individual->outcome; // bsf outcome initialization
        
        P.push_back(*individual);
        
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
            
            crossover(parent1, parent2, child, x_max, x_min);
            
            n++;
            R.push_back(*child);
            // update the best-so-far solution
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
        t++;
        
        printf("The best-so-far solution: ");
        for(j=0;j<D;j++) {
            printf("%f\t",bsf->item[j]);
        }
        cout << "\tObjective function value: " << bsf->outcome << endl;
        
        vector<Indvl>().swap(R); // delete R
        
        if(fabs(bsf->outcome - OPTIMAL_SLN) <= pow(10,-8)) break;
        
    }
    cout << "calculation: " << n << " times." << endl;
    delete child;
    delete individual;
    delete bsf;
    vector<Indvl>().swap(P);
    return 0;
}

// Blend Crossover alpha
int crossover(Indvl *parent1, Indvl *parent2, Indvl *child, double x_max, double x_min) {
    int j;
    double alpha = 0.5;
    double tmp;
    double diff, p1, p2, lower_bound, upper_bound;
    for(j=0;j<D;j++) {
        p1 = parent1->item[j];
        p2 = parent2->item[j];
        diff = fabs(p1-p2);
        lower_bound = min(p1, p2) - alpha * diff;
        upper_bound = max(p1, p2) + alpha * diff;
        tmp = (double)rand() / ((double)RAND_MAX) * (upper_bound-lower_bound) + lower_bound;
        
        if(tmp < x_min) { // test if the child value exceeds the boundary
            tmp = x_min;
        } else if(tmp > x_max) {
            tmp = x_max;
        }
        child->item[j] = tmp;
        
    }
    switch (func) {
        case 1:
            sphere_func(child);
            break;
        case 2:
            rastrigin_func(child);
            break;
        case 3:
            rosenbrock_func(child);
            break;
        default:
            sphere_func(child);
            break;
    }
    return 0;
}



int sphere_func(Indvl *individual) {
    int j;
    double out = 0;
    double tmp = 0;
    for(j=0; j<D; j++) {
        tmp = individual->item[j];
        out += pow(tmp, 2);
    }
    individual->outcome = out;
    return 0;
}

int rastrigin_func(Indvl *individual) {
    int j;
    double out = 0;
    double tmp = 0;
    for(j=0; j<D; j++) {
        tmp = individual->item[j];
        out += (pow(tmp, 2)-10*cos(2*M_PI*tmp)+10);
    }
    individual->outcome = out;
    return 0;
}

int rosenbrock_func(Indvl *individual) {
    int j;
    double out = 0;
    double x1 = 0;
    double x2 = 0;
    for(j=0; j<D-1; j++) {
        x1 = individual->item[j];
        x2 = individual->item[j+1];
        out += (100*pow((x2-pow(x1,2)),2)+pow(x1-1,2));
    }
    individual->outcome = out;
    return 0;

}


