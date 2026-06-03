#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define CANT_NODOS 10

/* Representacion de nodo del grafo.
    Cada nodo tiene una cota inferior, una cota superior y un valor actual.
 */
typedef struct{
    double cotaInf;
    double cotaSup;
    double valorActual;
} Nodo;

/*Representacion del grafo como lista de adyacencia.
  Un arreglo de nodos de tipo Nodo.
*/
typedef struct {
    Nodo arr[CANT_NODOS+1];
} Grafo;

/* 
    Funcion que genera el grafo. Asigna a cada nodo su cota inferior y su cota superior.
*/
void generarGrafo(Grafo* grafo){

    grafo->arr[1].cotaInf = 40;
    grafo->arr[1].cotaSup = 56;

    grafo->arr[2].cotaInf = 24;
    grafo->arr[2].cotaSup = 32;

    grafo->arr[3].cotaInf = 20;
    grafo->arr[3].cotaSup = 40;

    grafo->arr[4].cotaInf = 16;
    grafo->arr[4].cotaSup = 48;

    grafo->arr[5].cotaInf = 10;
    grafo->arr[5].cotaSup = 30;

    grafo->arr[6].cotaInf = 15;
    grafo->arr[6].cotaSup = 30;

    grafo->arr[7].cotaInf = 20;
    grafo->arr[7].cotaSup = 25;

    grafo->arr[8].cotaInf = 30;
    grafo->arr[8].cotaSup = 50;

    grafo->arr[9].cotaInf = 40;
    grafo->arr[9].cotaSup = 60;

    grafo->arr[10].cotaInf = 8;
    grafo->arr[10].cotaSup = 16;

}

/*
    Funcion que genera el valor actual de cada nodo. Dicho valor consiste en un numero aleatorio entre la cota inferior y la cota superior de cada nodo.
*/
void generarValorActual(Grafo* grafo){
    for(int i = 1; i <= CANT_NODOS; i++){
        double cotaInf = grafo->arr[i].cotaInf;
        double cotaSup = grafo->arr[i].cotaSup;

        double r = (double) rand() / RAND_MAX;
        grafo->arr[i].valorActual = cotaInf + r * (cotaSup - cotaInf);
    }
}

/*
    Funcion que obtiene el maximo valor entre dos numero.
*/
double max(double a, double b){
    return (a > b) ? a : b;
}

/*
    Funcion que obtiene el maximo valor entre cuatro numeros.
*/
double max2(double a, double b, double c, double d){
    return max(max(a,b), max(c,d));
}

/**
    Funcion que obtiene el maximo valor entre tres numeros.
 */
double max3(double a, double b, double c){
    return max(max(a,b), c);
}

/*
    Funcion que obtiene el tiempo total del proyecto en una iteracion dada.
    Para esto, se obtiene el valor actual de cada nodo y se calcula el tiempo total del proyecto utilizando el orden de presedencia de cada nodo .
*/
double calcularTiempoProyecto(Grafo* grafo){
    double tiempoTotal = 0;

    double t1 = grafo->arr[1].valorActual;
    double t2 = grafo->arr[2].valorActual;
    double t3 = grafo->arr[3].valorActual;
    double t4 = grafo->arr[4].valorActual;
    double t5 = grafo->arr[5].valorActual;
    double t6 = grafo->arr[6].valorActual;
    double t7 = grafo->arr[7].valorActual;
    double t8 = grafo->arr[8].valorActual;
    double t9 = grafo->arr[9].valorActual;
    double t10 = grafo->arr[10].valorActual;

    double f1 = t1;
    double f2 = f1+t2;
    double f3 = f1+t3;
    double f4 = t4 + max(f2,f3);
    double f5 = t5+ max(f2,f3);
    double f6 = t6+ f3;
    double f7 = t7 + f3;
    double f8 = t8 + max2(f4, f5 , f6,f7);
    double f9 = t9 + f5;
    double f10 = t10 + max3(f7,f8,f9);

    tiempoTotal = f10;

    return tiempoTotal;
}

/*
    Funcion que estima el tiempo esperado del proyecto, la desviacion estandar y el tiempo de CPU utilizado para realizar las replicaciones.
    Para esto, se generan un numero de replicaciones dado por el usuario. En cada replicacion, se genera un valor actual para cada nodo y se calcula el tiempo total del proyecto.
*/
void estimarTiempo(int replicaciones){
    
    Grafo grafo;
    generarGrafo(&grafo);

    double suma = 0;
    double sumaDesviacionEstandar = 0;
    double promedio = 0;
    double suma2 = 0;
    double varianza =0;
    double desvioEstimador = 0;
    double tiempoCPU = 0;

    clock_t inicio = clock();

    for(int i = 0; i < replicaciones; i++){
        generarValorActual(&grafo);
        double t = calcularTiempoProyecto(&grafo);
        suma += t;
        suma2 += t*t;
    }

    promedio = suma/replicaciones;
    varianza = (suma2 - replicaciones * promedio * promedio) / (replicaciones - 1);
    if(varianza < 0){
        varianza = 0;
    }
    sumaDesviacionEstandar = sqrt(varianza);

    desvioEstimador = sumaDesviacionEstandar / sqrt(replicaciones);

    clock_t fin = clock();
    tiempoCPU = (double)(fin-inicio)/CLOCKS_PER_SEC;

    system("clear");
    printf("Numero de replicaciones: %d\n", replicaciones);
    printf("Tiempo esperado = %f\n", promedio);
    printf("Desvio estandar del estimador = %f\n", desvioEstimador);
    printf("Tiempo CPU = %f\n\n", tiempoCPU);
}

/*
    Funcion principal que solicita al usuario el numero de replicaciones a realizar y llama a la funcion estimarTiempo para obtener el tiempo esperado del proyecto, la desviacion estandar y el tiempo de CPU utilizado.
*/
int main(){
    system("clear");
    //Utilizo como semilla la hora actual para obtener resultados distintos
    srand(time(NULL));
    int replicaciones;
    printf("Ingrese el numero de replicaciones: ");
    scanf("%d", &replicaciones);
    if(replicaciones < 2){
        printf("n debe ser >= 2\n");
        return 0;
    }else{
        estimarTiempo(replicaciones);
    }

    return 0;
}