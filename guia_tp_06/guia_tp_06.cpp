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
	cimgd imagen("img/sangre.jpg");
	
	cimgd& imagenConRuidoGassiano = CImgUtils::agregarRuidoGaussiano(imagen, 11.0);
	cimgd& imagenConRuidoImpulsivoGaussiano = CImgUtils::agregarRuidoImpulsivo(imagenConRuidoGassiano, 11.0);

	(imagen, imagenConRuidoGassiano, imagenConRuidoImpulsivoGaussiano).display("original, gaussiano, gaussiano+impulsivo");

	cimgd& filtradaConGeometrica = CImgUtils::filtroMediaGeometrica(imagenConRuidoImpulsivoGaussiano, 3);
	cimgd& filtradaConContraArmonicaPositivo = CImgUtils::filtroMediaContraArmonica(imagenConRuidoImpulsivoGaussiano, 3, 0.5);
	cimgd& filtradaConContraArmonicaNegativo = CImgUtils::filtroMediaContraArmonica(imagenConRuidoImpulsivoGaussiano, 3, -0.5);

	(filtradaConGeometrica, filtradaConContraArmonicaPositivo, filtradaConContraArmonicaNegativo).display("Geometrica - ContraArmonica 0.5 - Contra Armonica -0.5");
	TjpLogger& log = TjpLogger::getInstance();

	std::stringstream ss;
	ss << "ECM Geometrica " << filtradaConGeometrica.MSE(imagen) << "\n"
		<< "ECM ContraArmonica Q = 0.5" << filtradaConContraArmonicaPositivo.MSE(imagen) << "\n"
		<< "ECM ContraArmonica Q = -0.5" << filtradaConContraArmonicaNegativo.MSE(imagen) << "\n";

	log.log("Ejercicio_2_1()",ss.str());
}

void ejercicio_3(){
	cimgd imagen("img/sangre.jpg");

	cimgd& imagenConRuidoGassiano = CImgUtils::agregarRuidoGaussiano(imagen, 11.0);
	cimgd& imagenConRuidoImpulsivoGaussiano = CImgUtils::agregarRuidoImpulsivo(imagenConRuidoGassiano, 11.0);

	(imagen, imagenConRuidoGassiano, imagenConRuidoImpulsivoGaussiano).display("original, gaussiano, gaussiano+impulsivo");

	// E MUY GUENO
	cimgd& filtradaMediaAlfaRecortado = CImgUtils::filtroMediaAlfaRecortado(imagenConRuidoImpulsivoGaussiano, 3, 6);
	cimgd& filtradaPuntoMedio = CImgUtils::filtroPuntoMedio(imagenConRuidoImpulsivoGaussiano, 3);
	cimgd& gaussConPuntoMedio = CImgUtils::filtroPuntoMedio(imagenConRuidoGassiano, 3);

	(filtradaMediaAlfaRecortado, filtradaPuntoMedio, gaussConPuntoMedio).display("MediaAlfaRecotrado, Sxy = 3, d = 2 -- Punto Medio");
}

void ejercicio_4() {
	cimgd imagen("img/img_degradada.tif");
	//tamaño = 256;
	//cimgld& fft = imagen.get_FFT();
	//CImgUtils::showSpectrum(fft);
	//PICO x = 58, y = 38
	//PICO x = 58, y = 218;
	int filas[] = {38, 218},
		columnas[] = {58, 198},
		anchos[] = {3, 3},
		altos[] = {3, 3};
	cimgld& fft = CImgUtils::filtroFrecuenciaFilaColumna(imagen, filas, columnas, anchos, altos, 2);
	cimgld& filtradaInversa = fft.get_FFT(true);
	filtradaInversa[0].display();
	//CImgUtils::showSpectrum(fft);

	//fft.display("FFT");
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

void prueba_parcial_2() {
	cimgf coins("../guia_tp_01/img/coins.tif");
	cimgf mascara = CImgUtils::new3x3SharpKernel(true, true);

	cimgf& primeroEqualizado = coins.get_equalize(255);
	primeroEqualizado.convolve(mascara);

	cimgf& primeroAcentuado = coins.get_convolve(mascara);
	primeroAcentuado.equalize(255);

	primeroAcentuado.normalize(0,255);
	primeroEqualizado.normalize(0,255);
	


	cimglf imagenes;
	imagenes.push_back(primeroAcentuado).push_back(primeroEqualizado);

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
	ejercicio_4();
	return 0;
}