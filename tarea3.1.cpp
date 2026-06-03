#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void generarPuntos(double punto[6]){
    for(int i = 0; i < 6; i++){
        punto[i] = (double)rand() / RAND_MAX;
    }
}

bool perteneceRegion(double punto[6], double centro[6], double radio){
    double suma = 0;
    for(int i = 0; i < 6; i++){
        suma += pow(punto[i] - centro[i], 2);
    }
    return suma <= pow(radio, 2);
}

bool restricciones(double punto[6]){
    bool r1 = (3 * punto[0] + 7 * punto[3]) <= 5;
    bool r2 = (punto[2] + punto[3]) <= 1;
    bool r3 = (punto[0] - punto[1] - punto[4] + punto[5]) >= 0;
    return r1 && r2 && r3;
}


void estimarDatos(int replicaciones){

    double puntos[6];
    double centro[6] = {0.45, 0.5, 0.6, 0.6, 0.5, 0.45};
    double radio = 0.35;
    double suma=0;
    for(int i = 0; i < replicaciones; i++){
        generarPuntos(puntos);
        if(perteneceRegion(puntos, centro, radio) && restricciones(puntos)){
            suma++;
        }
    }

    double estimador = (double) suma / replicaciones;
    double varianza = estimador * (1 - estimador) / (replicaciones - 1);
    double desvio = sqrt(varianza);

    printf("Estimacion del volumen de R: %f\n", estimador);
    printf("Desviacion estandar del estimador: %f\n", desvio);

}

int main(){
    int replicaciones;
    printf("Ingrese el numero de replicaciones: ");
    scanf("%d", &replicaciones);
    system("clear");
    
    clock_t inicio = clock();
    srand(time(NULL));

    
    printf("Numero de replicaciones: %d\n", replicaciones);
    estimarDatos(replicaciones);

    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f segundos\n\n", tiempo);

   return 0;
}