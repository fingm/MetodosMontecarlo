#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define iteraciones 1000000
#define estratos 5

/*
* Retorna un número aleatorio uniforme en [0,1]
*/
double U(){
    return (double)rand()/RAND_MAX;
}

/**
 * Función a integrar
 */
double g(double x1, double x2, double x3, double x4, double x5){
    return x1 * pow(x2,2) * pow(x3,3) * pow(x4,4) * pow(x5,5);
}

void estratificado(int opcion, double *a, double *b){

    // Acumuladores por estrato
    double medias[estratos] = {0};
    double varianzas[estratos] = {0};
    int iters[estratos];

    // Probabilidades de estratos
    double p[estratos];
    for(int i=0;i<estratos;i++){
        p[i] = b[i] - a[i];
    }

    // Asignación de muestras
    for(int i=0;i<estratos;i++){
        if(opcion == 0){
            iters[i] = iteraciones/estratos;
        }else{
            iters[i] = (int)(iteraciones * p[i]);
        }
    }

    // Acumuladores por estrato
    for(int i=0;i<estratos;i++){

        double suma = 0.0;
        double suma2 = 0.0;

        for(int j=0;j<iters[i];j++){

            double x1 = U();
            double x2 = U();
            double x3 = U();
            double x4 = U();

            // Estratificación en x5
            double x5 = a[i] + (b[i]-a[i]) * U();

            // Evaluación de la función
            double val = g(x1,x2,x3,x4,x5);

            // Acumulación de sumas para media y varianza
            suma += val;
            suma2 += pow(val,2);
        }

        medias[i] = suma / iters[i];
        varianzas[i] = (suma2 - iters[i]*pow(medias[i],2)) / (iters[i] - 1);
    }

    // Estimadores
    double theta = 0.0;
    double var_total = 0.0;

    for(int i=0;i<estratos;i++){
        theta += p[i] * medias[i];
        var_total += (pow(p[i],2)*varianzas[i]) / iters[i];
    }

    // Error estándar e intervalo de confianza
    double se = sqrt(var_total);
    double z = 1.96;

    // Resultados

    printf("\n");

    if(opcion == 0){
        printf("Estratificación uniforme:\n");
    }else{
        printf("Estratificación proporcional a la probabilidad:\n");
    }

    printf("\nEstimacion: %.10f\n", theta);
    printf("\nDesvio estandar: %.10f\n", se);
    printf("\nIC 95%%: [%.10f , %.10f]\n",theta - z*se, theta + z*se); 

    printf("\n");

}


int main(){
    system("clear");
    srand(time(NULL));

    //Estratos en funcion de x5 (sugerencia)
    double a[estratos] = {0.0, 0.72, 0.83, 0.90, 0.95};
    double b[estratos] = {0.72, 0.83, 0.90, 0.95, 1.0};

    // Caso con 10^6/5 muestras por estrato (uniforme)
    estratificado(0,a,b); 

    // Proporcional a la probabilidad de cada estrato
    estratificado(1,a,b); 

    return 0;
}