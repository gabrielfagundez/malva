#ifndef _FUNSOFTCOMPUTING_H

#define _FUNSOFTCOMPUTING_H 1

#ifndef _TFITNESS 
#define _TFITNESS 1
typedef long double tFitness;
#endif 

tFitness Shifted_Sphere( int dim , const double* x );
tFitness Schwefel_Problem( int dim , const double* x);
tFitness Shifted_Rosenbrock( int dim , const double* x );
tFitness Shifted_Rastrigin( int dim , const double * x );
tFitness Shifted_Griewank( int dim , const double * x );
tFitness Shifted_Ackley( int dim , const double* x );
tFitness f_Schwefel2_22(int dim, double *s);
tFitness f_Schwefel1_2(int dim, double *s);
tFitness Extended_f_10(int dim, double *x);
tFitness f_Bohachevsky(int dim, double *s);
tFitness f_Schaffer(int dim, double *s);
tFitness f_Hybrid_12(int dim, double *s); 
tFitness f_Hybrid_13(int dim, double *s); 
tFitness f_Hybrid_14(int dim, double *s); 
tFitness f_Hybrid_15(int dim, double *s); 
tFitness f_Hybrid_16new(int dim, double *s); 
tFitness f_Hybrid_17new(int dim, double *s); 
tFitness f_Hybrid_18new(int dim, double *s); 
tFitness f_Hybrid_19new(int dim, double *s); 

#endif
