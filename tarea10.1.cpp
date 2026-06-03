#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>



/*
* La función X genera un número aleatorio entre 0 y 1 utilizando la función rand() de la biblioteca estándar de C.
*/
double X(){
    return (double)rand() / (RAND_MAX + 1.0);
}

/*
*  La función Y también genera un número aleatorio entre 0 y 1 utilizando la función rand() de la biblioteca estándar de C.
*/
double Y(){
    return (double)rand() / (RAND_MAX + 1.0);
}

/*
*Inicializar Buffer
*/
void iniciarT(double T[],int tope){
    for(int i = 0; i < tope; i++){
        T[i] = X(); 
    }
}

/**
 * La función shuffle toma un arreglo T de números aleatorios y un entero K, y devuelve un número aleatorio del arreglo T.
 */
double shuffle(double T[],int K){
    double y = Y(); // y ∈  [0,1)

    int j = (int)(K * y);

    double z = T[j];

    T[j] = X();// El buffer se actualiza con un nuevo número aleatorio generado por X()

    return z;
}


/**
 * Genera un punto aleatorio en R^6 utilizando el método de shuffle.
 */
void generarPunto(double punto[6],double T[], int K){
    for(int i = 0; i < 6; i++){
        punto[i] = shuffle(T, K); 
    }
}

/**
 * La función f calcula el valor de la función f en un punto dado en R^6.
 */
double f(double punto[6]){
    double suma = 0.0;
    for(int i = 0; i < 6; i++){
        suma += pow(punto[i],2);
    }
    return 1.0/sqrt(suma);
}

/**
 * Genera un punto aleatorio en R^6 utilizando el método tradicional sin shuffle.
 */
void generarPuntoSinShuffle(double punto[6]){
    for(int i = 0; i < 6; i++){
        punto[i] = X(); 
    }
}

/**
 * Método de Monte Carlo para estimar la media, la desviación estándar y el intervalo de confianza al 99% de la función f en R^6 utilizando el método de shuffle.
 */
void monteCarlo(int replicaciones, int K,bool shuffle){

    double T[K]; 
    iniciarT(T, K);

    double punto[6];

    double suma = 0.0;
    double sumaCuadrados = 0.0;

    for(int i = 0; i < replicaciones; i++){

        if(shuffle){
            generarPunto(punto, T, K);
        }else{
            generarPuntoSinShuffle(punto);
        }

        double valorF = f(punto);

        suma += valorF;
        sumaCuadrados += pow(valorF,2);
    }

    //Media
    double media = suma / replicaciones;

    //Varianza muestral
    double varianza = (sumaCuadrados - replicaciones * pow(media,2)) / (replicaciones - 1);

    //Desviación estándar
    double desviacionEstandar = sqrt(varianza);

    //Intervalo de confianza al 99%. TCL = media ± z * (desviación estándar / sqrt(n))
    double z = 2.576; // Valor z para un nivel de confianza del 99%
    double margenError = z * (desviacionEstandar / sqrt(replicaciones));

    double limiteInferior = media - margenError;
    double limiteSuperior = media + margenError;

    //Resultados
    printf("Media: %.4g\n", media);
    printf("Desviación estándar: %.4g\n", desviacionEstandar);
    printf("Intervalo de confianza al 99%%: [%.4g, %.4g]\n", limiteInferior, limiteSuperior);

}



int main(){
    // Diferentes valores de K para comparar los resultados
    int K[7] = {10,100,1000, 5000,10000, 50000};
    int replicaciones = 100000;

    srand(time(NULL));
    system("clear");
    printf("Estimación de la media, desviación estándar e intervalo de confianza al 99%% de la función f en R^6 utilizando el método de Monte Carlo con shuffle:\n\n");

    // Se ejecuta el método de Monte Carlo con shuffle para cada valor de K y se mide el tiempo de ejecución
    for(int i = 0; i < 6; i++){
        clock_t inicio = clock();
        printf("K = %d\n", K[i]);
        monteCarlo(replicaciones, K[i], true);
        clock_t fin = clock();
        double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
        printf("Tiempo de ejecucion: %f segundos\n\n", tiempo);
        printf("\n");
    }

    // Se ejecuta el método de Monte Carlo sin shuffle para el valor de K más grande y se mide el tiempo de ejecución
    clock_t inicio = clock();

    printf("Estimación de la media, desviación estándar e intervalo de confianza al 99%% de la función f en R^6 utilizando el método de Monte Carlo sin shuffle:\n\n");
    monteCarlo(replicaciones, K[5], false);
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f segundos\n\n", tiempo);
    printf("\n");

    return 0;
}