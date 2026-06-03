#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*
*Recibe  un punto(x,y) y devuelve el valor de la función f(x,y). Es la altura de la montaña en ese punto.
* Si el punto está fuera , devuelve 0.
*/
double f(double x, double y){
    double radio = 0.4;
    double dx = pow(x - 0.5,2);
    double dy = pow(y - 0.5,2);
    double d = sqrt(dx + dy);

    if(d <= radio){//Si el punto está dentro del círculo de radio 0.4
        return 8.0 - (8.0/0.4)*d;
    }
    return 0.0;
}

/**
 * Realiza la estimación de la integral de f(x,y) sobre el cuadrado unitario [0,1]x[0,1] usando Monte Carlo.
 */
void estimarIntegral(int n, double &estimador, double &varianza){

    double suma = 0.0;
    double suma2 = 0.0;

    for(int i = 0; i < n; i++){
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        double val = f(x,y);

        suma += val;
        suma2 += pow(val,2);
    }

    estimador = suma / n;

    // Varianza muestral de f(X)
    double var_muestra = (suma2 - n * pow(estimador,2)) / (n - 1);

    // Varianza del estimador
    varianza = var_muestra / n;
}

double parteA(int replicaciones, double z){
    srand(time(NULL));

    system("clear");
    printf("Parte a:\n\n");


    double est; //Estimación de la integral
    double var; //Varianza del estimador

    estimarIntegral(replicaciones, est, var);


    double error = z * sqrt(var);

    printf("Estimacion: %.4g\n", est);
    printf("IC 95%%: [%.4g, %.4g]\n", est - error, est + error);

    printf("\n\nParte b:\n\n");
    return var*replicaciones;//Estimación de sigma^2
}

int parteB(double sigma2, double z){
    double epsilon = 1e-3;
    int n_requerido = ceil((z*z * sigma2) / (epsilon*epsilon));

    printf("n requerido: %d\n", n_requerido);
    return n_requerido;
}

void parteC(int n_requerido, double z){
    printf("\n\nParte c:\n\n");

    double est2, var2;

    estimarIntegral(n_requerido, est2, var2);

    double error2 = z * sqrt(var2);

    printf("Replicaciones realizadas: %d\n", n_requerido);
    printf("Estimacion: %.4g\n", est2);
    printf("IC 95%%: [%.4g, %.4g]\n", est2 - error2, est2 + error2);

    printf("\n\n");
}

int main(){

    clock_t inicio = clock();
    srand(time(NULL));

    int replicaciones = 1000000;
    double z = 1.96;

    // Parte a

    double sigma2 = parteA(replicaciones, z);

        // Calculo el tiempo de ejecucion
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %.4e segundos\n\n", tiempo);

    // Parte b

    int n_requerido = parteB(sigma2, z);

    // Parte c

    parteC(n_requerido, z);



    return 0;
}