// guia_tp_06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../guia_tp_01/CImg.h"
#include "../guia_tp_01/CImgUtils.h"

typedef void(*ejercicio)(void);
const char* camPath = "../guia_tp_01/img/cameraman.tif";
typedef CImg<double> cimgd;
typedef CImg<float > cimgf;
typedef CImgList<float> cimglf;
typedef CImgList<double> cimgld;
typedef CImgDisplay cimgdis;
const float color[] = { 255.0f };

/*TODO agregar desvio interactivamente*/
void ejercicio_1_1() {
	//cimgf imagen(256,256,1,1,128.0f);
	cimgf imagen(camPath);

	cimgf& ruidoGauss = imagen.get_noise(11.f, 0);
	cimgf& ruidoUniforme = imagen.get_noise(11.f, 1);
	cimgf& ruidoImpulsivo = imagen.get_noise(11.f, 2);

	cimglf imagenes;
	imagenes.push_back(ruidoGauss);
	imagenes.push_back(ruidoUniforme);
	imagenes.push_back(ruidoImpulsivo);

	//imagenes.display();

	cimglf histogramas;

	cimgf ruidoGaussHisto = ruidoGauss.get_histogram(255, 0.0, 255.0);
	cimgf ruidoUniformeHisto = ruidoUniforme.get_histogram(255, 0.0, 255.0f);
	cimgf ruidoImpulsivoHisto = ruidoImpulsivo.get_histogram(255, 0.0, 255.0f);

	ruidoGaussHisto.display_graph("ruido gaussiano");
	ruidoUniformeHisto.display_graph("ruido uniforme");
	ruidoImpulsivoHisto.display_graph("ruido impulsivo");
}

/*TODO: agregar posibilidad de elegir el desvio interactivamente*/
void ejercicio_1_2() {
	cimgf imagen(256, 256, 1, 1, 128.0f);

	cimgf& ruidoGauss = imagen.get_noise(11.f, 0);
	cimgf& ruidoUniforme = imagen.get_noise(11.f, 1);
	cimgf& ruidoImpulsivo = imagen.get_noise(11.f, 2);

	cimgf ruidoGaussHisto = ruidoGauss.get_histogram(255, 0.0, 255.0);
	cimgf ruidoUniformeHisto = ruidoUniforme.get_histogram(255, 0.0, 255.0f);
	cimgf ruidoImpulsivoHisto = ruidoImpulsivo.get_histogram(255, 0.0, 255.0f);

	ruidoGaussHisto.display_graph("ruido gaussiano");
	ruidoUniformeHisto.display_graph("ruido uniforme");
	ruidoImpulsivoHisto.display_graph("ruido impulsivo");

}


void ejercicio_2_1() {
	cimgf imagen(camPath);
	imagen.resize(512, 512, -100, -100, 6);
	imagen.get_noise(20, 2);
	cimgf imagenFiltrada = CImgUtils::filtroMediana(imagen, 2);
	imagenFiltrada.display();
}

void prueba_parcial_1() {
	cimgf coins("../guia_tp_01/img/coins.tif");
	cimgf &coins_threshold = coins.get_threshold(171.0f, true);
	cimgf mascara(10, 10, 1, 1, 1.f / (10.f*10.f));
	coins_threshold.convolve(mascara);
	std::vector<float> &LUT = CImgUtils::createLut255(-1.0f, 255.0f, 0.0f, 255.0f);
	coins_threshold = CImgUtils::mapLUT(coins_threshold, LUT);
	coins_threshold = CImgUtils::toBinary(coins_threshold, 253.0f, 0.0f, 1.0f);
	coins = CImgUtils::multiplyImages(coins, coins_threshold);
	coins.equalize(255,60,160);
	
	mascara = cimgf(3, 3,
		0.0f, -1.f, 0.0f,
		-1.0f, 4.0f, -.0f,
		0.0f, -1.f, 0.0f
		);
	
	cimgf &sinsharp = coins.get_convolve(mascara);
	cimglf imagenes;
	imagenes.push_back(coins).push_back(sinsharp);
	imagenes.display();
}

const ejercicio ejercicios[] = {
	ejercicio_1_1,
	ejercicio_1_2,
	ejercicio_2_1
};

const int nEjercicios = 3;

int _tmain(int argc, _TCHAR* argv[])
{
	//ejercicios[nEjercicios - 1]();
	prueba_parcial_1();
	return 0;
}