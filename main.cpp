#include <iostream>
#include <vector>
#include <math.h>

float TrapecioSucesivo(float pasoH, int iteracion, std::vector<float>& subintervalo, float estimacionAnterior);
float EvaluacionFuncion(float a);
std::vector<float> CalcularNuevosSubintervalos(std::vector<float>& valoresX);
float CalcularSumaFunciones(const std::vector<float>& subintervalo);
float ExtrapolacionRomberg(std::vector<float>& estimacionExtrapolacion, int j);


int main() {
	int intervaloA, intervaloB, precision;
	float pasoH;
	std::vector<float> trapecioEstimaciones, valoresX;

	std::cout<<"Integracion por el metodo de Romberg\n"
	         <<"usando la regla sucesiva del trapecio\n"<<std::endl;

    std::cout<<"Funcion a integrar sqrt(1 + e^(2x))"<<std::endl;
	std::cout<<"Ingresa el limite de integracion inferior a: ";
	std::cin>>intervaloA;
	std::cout<<"Ingresa el limite de integracion superior b: ";
	std::cin>>intervaloB;
	std::cout<<"Ingresa el valor de la precision: ";
	std::cin>>precision;

	valoresX.push_back(intervaloA);
	valoresX.push_back(intervaloB);

	for(int i = 0; i < precision; i++) {
		std::vector<float> subintervalo = (i > 0) ? CalcularNuevosSubintervalos(valoresX) : valoresX;
		float estimacionAnterior = (i == 0) ? 0 : trapecioEstimaciones.at(i-1);

		pasoH = (intervaloB - intervaloA) / pow(2, i);
		trapecioEstimaciones.push_back(TrapecioSucesivo(pasoH, i, subintervalo, estimacionAnterior));
	}

    std::cout<<"\nEstimaciones con la regla del trapecio"<<std::endl;
	for(int i = 0; i < trapecioEstimaciones.size(); i++)
		printf("%.6f\n", trapecioEstimaciones.at(i));

    std::cout<<std::endl;
    float estimacionRomberg = ExtrapolacionRomberg(trapecioEstimaciones, 1);
    printf("Estimacion de la integral por extrapolacion de Romberg: %.6f \n\n", estimacionRomberg);

	return 0;
}

float EvaluacionFuncion(float a) {
	/* funcion: sqrt(1 + (e^x)^2) */
	return sqrt(1 + exp(2*a));
}

float TrapecioSucesivo(float pasoH, int iteracion, std::vector<float>& subintervalo, float estimacionAnterior) {
	if(iteracion == 0)
		return (pasoH/2.0) * (CalcularSumaFunciones(subintervalo));

	return (estimacionAnterior/2.0) + (pasoH * CalcularSumaFunciones(subintervalo));

}

std::vector<float> CalcularNuevosSubintervalos(std::vector<float>& valoresX) {
	std::vector<float> subintervalos;
	std::vector<float>::iterator itr = valoresX.begin() + 1;
	int tamanioActual = valoresX.size(); // evita la recursividad infinita
	int desplazamientoInf = 0, desplazamientoSup = 1;

	for(int i = 0; i < tamanioActual - 1; i++) {
		float nuevoSubintervalo = valoresX.at(desplazamientoInf) + ((valoresX.at(desplazamientoSup) - valoresX.at(desplazamientoInf))/2.0);
		subintervalos.push_back(nuevoSubintervalo);
		itr = valoresX.insert(itr, nuevoSubintervalo);
		itr += 2; desplazamientoInf += 2; desplazamientoSup = desplazamientoInf + 1;
	}

	return subintervalos;
}

float CalcularSumaFunciones(const std::vector<float>& subintervalo) {
	float suma = 0;
	for(int i = 0; i < subintervalo.size(); i++)
		suma += EvaluacionFuncion(subintervalo.at(i));

	return suma;
}

float ExtrapolacionRomberg(std::vector<float>& estimacionExtrapolacion, int j){
    std::vector<float> extrapolacion;

    if(estimacionExtrapolacion.size() == 1)
        return estimacionExtrapolacion.at(0);

    std::cout<<"Extrapolacion de Romberg"<<std::endl;
    for(int i=0; i<estimacionExtrapolacion.size() - 1; i++){
        extrapolacion.push_back(estimacionExtrapolacion.at(i+1) + (estimacionExtrapolacion.at(i+1) - estimacionExtrapolacion.at(i)) / (pow(4, j) - 1));
        printf("%.6f\n", extrapolacion.at(i));
    }

    std::cout<<std::endl;

    j++;
    ExtrapolacionRomberg(extrapolacion, j);
}
