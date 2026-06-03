#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/**
 * La función generarPuntos genera un punto aleatorio en un espacio de 6 dimensiones, donde cada coordenada es un número decimal entre 0 y 1.
 */
void generarPuntos(double punto[6]){
    for(int i = 0; i < 6; i++){
        punto[i] = (double)rand() / RAND_MAX;
    }
}

/*
 * La función perteneceRegion verifica si un punto dado está dentro de una región esférica definida por un centro y un radio.
 * Un punto pertenece si la suma de las diferencias al cuadrado entre cada coordenada del punto y el centro es menor o igual al radio al cuadrado.
*/
bool perteneceRegion(double punto[6], double centro[6], double radio){ 
    double suma = 0;
    for(int i = 0; i < 6; i++){
        double diff = punto[i] - centro[i];
        suma += diff * diff;
    }
    return suma <= pow(radio, 2);
}

/**
 * 3x1 + 7x4 ≤ 5;
 * x3 +x4 ≤ 1; 
 * x1 −x2 −x5 +x6 ≥ 0.
*/
bool restricciones(double punto[6]){
    bool r1 = (3 * punto[0] + 7 * punto[3]) <= 5;
    bool r2 = (punto[2] + punto[3]) <= 1;
    bool r3 = (punto[0] - punto[1] - punto[4] + punto[5]) >= 0;
    return r1 && r2 && r3;
}

/**
 * 
 */
void estimarVolumen(int replicaciones,double &estimador, double &desvio){

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

    estimador = (double) suma / replicaciones;
    double varianza = estimador * (1 - estimador) / replicaciones;
    desvio = sqrt(varianza);

    printf("Estimacion del volumen de R: %.4e\n", estimador);
    printf("Desviacion estandar del estimador: %.4e\n", desvio);

}

/**
 * Cantidad de replicaciones a realizar para garantizar un error menor a epsilon
 * con probabilidad delta, utilizando el criterio de peor caso de Hoeffding.
 */
double cantidadReplicaciones(double epsilon,double delta){

    // Formula de Hoeffding
    double nH_real = log(2.0 / delta) / (2 * epsilon * epsilon);

    // Redondeo hacia arriba
    long long int nH = (long long int) ceil(nH_real);

    printf("epsilon = %.10f\n", epsilon);
    printf("delta = %.2f\n", delta);
    printf("Cantidad de replicaciones (Hoeffding): %lld\n\n", nH);
    return nH;
}

/**
 * Intervalo de confianza usando Chevishev para un nivel de confianza del 95%
 */
void intervaloChebyshev(double estimador, double desvio, double delta){
    double k = sqrt(1.0/delta);
    double margenError = k * desvio;
    double limiteInferior = estimador - margenError;
    double limiteSuperior = estimador + margenError;
    double ancho = limiteSuperior - limiteInferior;

    printf("Intervalo de confianza (Chebyshev) al 95%%: [%.4e, %.4e]\n", limiteInferior, limiteSuperior);
    printf("Ancho del intervalo: %.4e\n\n", ancho);
}

void intervaloAgrestiCoull(double estimador,int replicaciones,double z){

    double z2 = z * z;

    double X = replicaciones * estimador;

    double n_tilde = replicaciones + z2;
    double X_tilde = X + z2 / 2.0;

    double p_tilde = X_tilde / n_tilde;

    double margen_agresti = z * sqrt((p_tilde * (1 - p_tilde)) / n_tilde);

    double limiteInferior = p_tilde - margen_agresti;
    double limiteSuperior = p_tilde + margen_agresti;
    double ancho = limiteSuperior - limiteInferior;

    printf("Intervalo de confianza (Agresti-Coull) al 95%%: [%.4e, %.4e]\n", limiteInferior, limiteSuperior);
    printf("Ancho del intervalo: %.4e\n\n", ancho);
}

int main(){
    double confianza = 0.95;
    double epsilon = 0.0001; 
    double delta = 1 - confianza;

    double estimador,desvio;

    double z = 1.96; // Valor z para un nivel de confianza del 95% 

    int replicaciones = 1000000;
    system("clear");
    
    clock_t inicio = clock();
    srand(time(NULL));

    
    printf("Numero de replicaciones: %d\n", replicaciones);

    //Obtengo datos estimados para el volumen de R y su desviacion estandar
    estimarVolumen(replicaciones, estimador, desvio);

    // Calculo el tiempo de ejecucion
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %.4e segundos\n\n", tiempo);

    
    double cantRep = cantidadReplicaciones(epsilon,delta);
    intervaloChebyshev(estimador, desvio, delta);
    intervaloAgrestiCoull(estimador, replicaciones, z);

    return 0;
}