#define CANTZONAS 15
#define CANTESPECIES 8
#define REPLICACIONES 1000

#include <stdio.h>
#include <math.h>
#include <time.h>


/**
 * Funcion que genera un subconjunto de tamaño m sin reemplazo a partir de un conjunto de tamaño CANTZONAS
 */
void generarSubconjunto(int subconjunto[],int m){
    int disponibles[CANTZONAS];
    for(int i=0 ; i<CANTZONAS ; i++){
        disponibles[i] = i;
    }
    int size = CANTZONAS;

    for(int i=0 ; i<m ; i++){
        int r = rand() % size;
        subconjunto[i] = disponibles[r];

        //elimino el elemento
        disponibles[r] = disponibles[size-1];
        size--;
    }
}

/**
 * Funcion que verifica si el subconjunto de zonas cubre todas las especies,
 * es decir, si para cada especie hay al menos una zona en el subconjunto que la contiene.
 */
bool cubreTodas(int matriz[CANTZONAS][CANTESPECIES],int subconjunto[],int m){
    for(int j = 0; j < CANTESPECIES; j++){
        bool cubierto = false;

        for(int i = 0; i < m; i++){
            if(matriz[subconjunto[i]][j] == 1){
                cubierto = true;
                break;
            }
        }
        if(!cubierto) return false;
    }

    return true;
}

/**
 * Funcion que calcula C(n, k) = n! / (k! * (n-k)!)
 */
double combinatoria(int n, int k){
    double res = 1.0;
    for(int i=1 ; i<=k ; i++){
        res *= (n - i + 1) / (double)i;
    }
    return res;
}

/*
* Intervalo de confianza usando el método de Agresti-Coull para una proporción de éxito p = S/n, con un nivel de confianza del 95%.
*/
void interValoAgrestiCoull(int S,int n,double total){
    double z = 1.96; // Valor z para un nivel de confianza del 95%
    double n_tilde = n + pow(z,2);
    double p_tilde = (S + (pow(z,2))/2.0) / n_tilde;

    double error = z * sqrt((p_tilde * (1 - p_tilde)) / n_tilde);

    double IC_inf = total * (p_tilde - error);
    double IC_sup = total * (p_tilde + error);

    printf("IC 95%%: [%.4g, %.4g]\n", IC_inf, IC_sup);
}

void montecarloConteo(int matriz[CANTZONAS][CANTESPECIES],int n,int m){
    int S = 0; //Cantidad de exitos
    int subconjunto[m];
    
    for(int i=0 ; i<n ; i++){
        generarSubconjunto(subconjunto,m);
        if(cubreTodas(matriz,subconjunto,m)){
            S++;
        }
    }

    // estimación de la probabilidad de éxito
    double pTecho = (double)S / n;
    double total = combinatoria(CANTZONAS, m);

    double NC = total * pTecho;// estimación del número de combinaciones que cubren todas las especies

    // desvío estándar
    double var = (pTecho * (1 - pTecho)) / n;
    double std = total * sqrt(var);

    printf("\nResultados para M = %d\n", m);
    printf("Replicaciones: %d\n", n);
    printf("NC estimado: %.4g\n", NC);
    printf("Desvio estandar: %.4g\n", std);
    // Agresti-Coull
    interValoAgrestiCoull(S,n,total);

}

/*
* Función que carga la matriz de presencia/ausencia de especies en cada zona.
*/
void cargarMatriz(int matriz[CANTZONAS][CANTESPECIES]){
    //Primero inicializo la matriz con ceros
    for(int i=0 ; i<CANTZONAS ; i++){
        for(int j=0 ; j<CANTESPECIES ; j++){
            matriz[i][j] = 0;
        }
    }
        
    // Zona 1
    matriz[0][0] = 1;
    matriz[0][1] = 1;
    matriz[0][2] = 1;
    matriz[0][5] = 1;
    matriz[0][7] = 1;

    // Zona 2
    matriz[1][0] = 1;
    matriz[1][2] = 1;
    matriz[1][3] = 1;
    matriz[1][5] = 1;
    matriz[1][6] = 1;

    // Zona 3
    matriz[2][0] = 1;
    matriz[2][3] = 1;
    matriz[2][4] = 1;
    matriz[2][6] = 1;
    matriz[2][7] = 1;

    // Zona 4
    matriz[3][0] = 1;
    matriz[3][1] = 1;
    matriz[3][3] = 1;
    matriz[3][5] = 1;
    matriz[3][6] = 1;
    matriz[3][7] = 1;

    // Zona 5
    matriz[4][0] = 1;
    matriz[4][2] = 1;
    matriz[4][5] = 1;
    matriz[4][7] = 1;

    // Zona 6
    matriz[5][1] = 1;
    matriz[5][3] = 1;
    matriz[5][6] = 1;
    matriz[5][7] = 1;

    // Zona 7
    matriz[6][2] = 1;
    matriz[6][4] = 1;
    matriz[6][7] = 1;

    // Zona 8
    matriz[7][1] = 1;
    matriz[7][2] = 1;
    matriz[7][4] = 1;
    matriz[7][6] = 1;
    matriz[7][7] = 1;

    // Zona 9
    matriz[8][1] = 1;
    matriz[8][4] = 1;
    matriz[8][5] = 1;
    matriz[8][7] = 1;

    // Zona 10
    matriz[9][2] = 1;
    matriz[9][3] = 1;
    matriz[9][5] = 1;
    matriz[9][6] = 1;
    matriz[9][7] = 1;

    // Zona 11
    matriz[10][1] = 1;
    matriz[10][4] = 1;
    matriz[10][5] = 1;
    matriz[10][6] = 1;

    // Zona 12
    matriz[11][2] = 1;
    matriz[11][3] = 1;
    matriz[11][4] = 1;
    matriz[11][5] = 1;
    matriz[11][6] = 1;

    // Zona 13
    matriz[12][0] = 1;
    matriz[12][1] = 1;
    matriz[12][5] = 1;
    matriz[12][6] = 1;
    matriz[12][7] = 1;

    // Zona 14
    matriz[13][0] = 1;
    matriz[13][1] = 1;
    matriz[13][3] = 1;
    matriz[13][5] = 1;
    matriz[13][6] = 1;

    // Zona 15
    matriz[14][1] = 1;
    matriz[14][2] = 1;
    matriz[14][4] = 1;
    matriz[14][5] = 1;
    matriz[14][6] = 1;
    matriz[14][7] = 1;
}


int main(){
    srand(time(NULL));
    system("clear");

    int n = REPLICACIONES;//n = cantidad de replicaciones
    int matriz[CANTZONAS][CANTESPECIES] = {0};
    cargarMatriz(matriz);

    for(int m=5; m<=6; m++){

        clock_t inicio = clock();
        
        montecarloConteo(matriz,n,m);

        // Calculo el tiempo de ejecucion
        clock_t fin = clock();
        double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
        printf("Tiempo de ejecucion: %.4f segundos\n\n", tiempo);
    }

    return 0;
}