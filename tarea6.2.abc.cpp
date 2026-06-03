#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*
* g(x1,x2,x3,x4,x5) = x1 * x2^2 * x3^3 * x4^4 * x5^5
*/
double g(double x1, double x2, double x3, double x4, double x5){
    return x1 * pow(x2,2) * pow(x3,3) * pow(x4,4) * pow(x5,5);
}

void estimarIntegral(int n, double &estimador, double &varianza){

    double suma = 0.0;
    double suma2 = 0.0;

    for(int i = 0; i < n; i++){

        double x1 = (double)rand()/RAND_MAX;
        double x2 = (double)rand()/RAND_MAX;
        double x3 = (double)rand()/RAND_MAX;
        double x4 = (double)rand()/RAND_MAX;
        double x5 = (double)rand()/RAND_MAX;

        double val = g(x1,x2,x3,x4,x5);

        suma += val;
        suma2 += pow(val,2);
    }

    estimador = suma / n;

    double var_muestra = (suma2 - n * pow(estimador,2)) / (n - 1);

    varianza = var_muestra / n;
}

int parteB(double var_estimador, int replicaciones, double z){
    printf("\nParte b\n\n");

    double epsilon = 1e-4;//error maximo permitido

    double sigma2 = var_estimador * replicaciones; //varianza de la muestra

    // usar varianza del estimador -> multiplicar por n
    int n_req = ceil((pow(z,2) * sigma2) / (pow(epsilon,2)));

    //n requerido para obtener un error menor a epsilon con 95% de confianza
    printf("n requerido: %d\n\n", n_req);
    return n_req;
}

void parteC(int n_req, double exacto){
    printf("\nParte c\n\n");

    int L = 500;

    int c90 = 0, c95 = 0, c99 = 0;

    for(int i = 0; i < L; i++){

        srand(time(NULL) + i); // semillas distintas

        double est_i, var_i;
        estimarIntegral(n_req, est_i, var_i);

        double e90 = 1.645 * sqrt(var_i);
        double e95 = 1.96  * sqrt(var_i);
        double e99 = 2.576 * sqrt(var_i);

        if(est_i - e90 <= exacto && exacto <= est_i + e90) c90++;
        if(est_i - e95 <= exacto && exacto <= est_i + e95) c95++;
        if(est_i - e99 <= exacto && exacto <= est_i + e99) c99++;
    }

    printf("Cobertura 90%%: %.6g\n", (double)c90 / L);
    printf("Cobertura 95%%: %.6g\n", (double)c95 / L);
    printf("Cobertura 99%%: %.6g\n\n", (double)c99 / L);
}


int main(){
    system("clear");
    printf("Parte a\n\n");

    srand(time(NULL));

    int replicaciones = 1000000; //Por letra se pide n=10^6
    double est, var;

    estimarIntegral(replicaciones, est, var);

    double exacto = 1.0/720.0;//valor real (teorico) de la integral

    printf("Estimacion: %.4g\n", est);
    printf("Valor exacto: %.4g\n", exacto);
    double se = sqrt(var);

    printf("Desvio estandar: %.10f\n", se);

    double z = 1.96;
    double error = z * sqrt(var);
    
    printf("IC 95%%: [%.4g, %.4g]\n", est - error, est + error);//intervalo

    int n_req = parteB(var, replicaciones, z);

    parteC(n_req, exacto);

    return 0;
}