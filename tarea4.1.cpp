#include <stdio.h>
#include <math.h>

int main(){
    double epsilon = 0.01;
    double deltas[] = {0.05,0.01,0.001}; //Datos de la letra del ejercicio
    double zetas[] = {1.96,2.576,3.29}; //Datos obtenidos de la tabla de la distribución normal estándar 

    for(int i=0; i<3; i++){
        printf("\n\n");
        double delta = deltas[i];
        double zeta = zetas[i];

        int nC = ceil(1.0 / (4 * delta * epsilon * epsilon));//Chebyshev
        int nH = ceil(log(2.0 / delta) / (2 * epsilon * epsilon));//Hoeffding
        int nN = ceil(pow(zeta / (2 * epsilon), 2));//TCL

        printf("delta = %.3f\n", delta);
        printf("Chebyshev: nC = %d, TCL: nN = %d, Hoeffding: nH = %d\n\n", nC, nN, nH);
    }
    return 0;
}