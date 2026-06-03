#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define R 0.4
#define H 8.0
#define PI 3.141592

/*
 * Genera normal N(0,1) usando Box-Muller
 * 
 */
double gauss() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;

    return sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2); // Box-Muller
}

/*
 * Altura de la montaña
 */
double f(double x, double y) {
    double dx = x - 0.5;
    double dy = y - 0.5;
    double d = sqrt(dx*dx + dy*dy);

    if (d <= R) {
        return H - (H / R) * d;
    }
    return 0.0;
}

/*
 * Genera punto uniforme en el círculo usando el método del curso:
 * r ~ F(r)=r^2  => r = sqrt(U)
 * dirección uniforme con normales
 */
void generar_punto(double &x, double &y) {

    double z1 = gauss();
    double z2 = gauss();

    double norm = sqrt(z1*z1 + z2*z2);

    double u = (double)rand() / RAND_MAX;
    double r = sqrt(u); // F(r)=r^2 . Inversa 

    x = 0.5 + R * r * (z1 / norm);
    y = 0.5 + R * r * (z2 / norm);
}

/*
 * Estimación Monte Carlo en el círculo
 */
void estimarIntegral(int n, double &estimador, double &varianza) {

    double suma = 0.0;
    double suma2 = 0.0;

    double area = PI * R * R;

    for (int i = 0; i < n; i++) {

        double x, y;

        generar_punto(x, y);

        double val = f(x, y);

        suma += val;
        suma2 += val * val;
    }

    double media = suma / n;

    estimador = area * media;

    double var_muestra = (suma2 / n) - (pow(media,2));
    varianza = (pow(area,2) * var_muestra) / n;
}

/*
 * Parte a. Estimación y varianza con n=1,000,000 y z=1.96 (95% IC)
 */
double parteA(int n, double z) {

    printf("Parte a:\n\n");

    double est, var;

    estimarIntegral(n, est, var);

    double error = z * sqrt(var);

    printf("Estimacion: %.6f\n", est);
    printf("IC 95%%: [%.6f, %.6f]\n", est - error, est + error);

    return var * n;
}

/*
 * Parte b. Cálculo de n requerido para error < 0.001 con z=1.96
 */
int parteB(double sigma2, double z) {

    double epsilon = 1e-3;

    int n = ceil((z * z * sigma2) / (epsilon * epsilon));

    printf("\nParte b:\n");
    printf("n requerido: %d\n", n);

    return n;
}

/*
 * Parte c. Estimación y varianza con n requerido para error < 0.001 y z=1.96
 */
void parteC(int n, double z) {

    printf("\nParte c:\n\n");

    double est, var;

    estimarIntegral(n, est, var);

    double error = z * sqrt(var);

    printf("Replicaciones: %d\n", n);
    printf("Estimacion: %.6f\n", est);
    printf("IC 95%%: [%.6f, %.6f]\n", est - error, est + error);
}


int main() {
    printf("\n");
    srand(time(NULL));

    clock_t inicio = clock();

    int n = 1000000;
    double z = 1.96;

    double sigma2 = parteA(n, z);

    clock_t fin = clock();

    printf("\nTiempo: %.4f s\n", (double)(fin - inicio) / CLOCKS_PER_SEC);

    int n_req = parteB(sigma2, z);

    parteC(n_req, z);
    printf("\n");
    return 0;
}