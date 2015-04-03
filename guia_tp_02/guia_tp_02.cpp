// guia_tp_02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cimg_library;

void ejercicio_1(){
	static unsigned char color[] = { 255 };
	std::vector<unsigned char> &LUT = CImgUtils::createLut255(); // por defecto, la identidad
	
	CImg<unsigned char> originalImage("../guia_tp_01/img/earth.png");

	CImg<unsigned char> curva = CImgUtils::new2DImageUchar(255, 255);
	CImgDisplay ventanaCurva(255, 255, "Curva");
	CImgDisplay ventanaImagen(originalImage);
	curva.draw_line(0, 255, 255, 0, color);
	ventanaCurva.display(curva);
	ventanaCurva.wait();
	while (!ventanaCurva.is_closed()) {
		if (ventanaCurva.button() & 1) { // boton izquierdo
			unsigned char x = ventanaCurva.mouse_x();
			unsigned char y = ventanaCurva.mouse_y();
			y = 255 - y;
			Punto<unsigned char> puntoCliqueado(x, y);
			CImg<unsigned char> editedImage(originalImage); // copia?
			CImgUtils::addRange(puntoCliqueado, editedImage, LUT, curva);
			ventanaCurva.display(curva);
			ventanaImagen.display(editedImage);
			//ventanaCurva.wait(50);
			ventanaCurva.set_button(); //resetea el mouse.
		}
		else {
			ventanaCurva.wait();
		}
	}
	
}

void ejercicio_2(){
	CImg<unsigned char> originalImage("../guia_tp_01/img/rmn.jpg");

	CImg<unsigned char> powImage(originalImage);
	CImg<unsigned char> logImage(originalImage);
	CImgUtils::logarithmicTransformation(logImage, 1024.0f);
	CImgUtils::powTransformation(powImage, 3.0f);
	CImgList<unsigned char> imagesList;
	imagesList.push_back(originalImage);
	imagesList.push_back(powImage);
	imagesList.push_back(logImage);

	CImgDisplay ventana;
	ventana.display(imagesList);
	CImgUtils::waitForWindow(ventana);

}

void ejercicio_3(){

	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");
	std::vector<CImg<unsigned char>> images;
	images.reserve(100);
	for (unsigned counter = 0; counter < 100; ++counter){
		images.push_back(CImgUtils::addNoise(originalImage, 0.0f, 255.0f*0.05f));
	}

	CImg<unsigned char> &modifiedImage = CImgUtils::reduceNoise(images);

	CImgDisplay originalImageWindow(originalImage);
	CImgDisplay modifiedImageWindow(modifiedImage);

	originalImageWindow.display(originalImage);
	modifiedImageWindow.display(modifiedImage);

	while (!originalImageWindow.is_closed() && !modifiedImageWindow.is_closed()){
		originalImageWindow.wait();
		modifiedImageWindow.wait();
	}

}

/*Filtro Emboss*/
void ejercicio_4(){
	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");
	CImg<unsigned char> embossFilterImage = CImgUtils::embossFilter(originalImage, 1, 1);

	CImgDisplay displayOriginal = CImgUtils::showImage(originalImage, "Original");
	CImgDisplay displayFiltered = CImgUtils::showImage(embossFilterImage, "Filtrada");
	
	CImgUtils::waitForWindow(displayOriginal);
}

void ejercicio_5() {
	const unsigned nImages = 8;
	unsigned char umbrales[nImages] = { 0 };
	CImgDisplay displays[nImages];
	CImg<unsigned char> images[nImages];
	//Detemino los valores de los umbrales
	for (int indx = 0; indx < nImages; ++indx){
		umbrales[indx] = indx * (255 / nImages);
		TjpLogger::getInstance().log("ejercicio_5() - Umbral Calculado: ", umbrales[indx]);
	}
	
	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");

	//solo por orden: (se puede hacer todo en el bucle de arriba.
	//Binarizo las imagenes
	for (int indx = 0; indx < nImages; ++indx) {
		images[indx] = CImgUtils::toBinary(originalImage, umbrales[indx]);
	}
	//Las muestro
	for (int indx = 0; indx < nImages; ++indx) {
		displays[indx] = CImgUtils::showImage(images[indx]);
	}
	//Espero que se cierre la ultima ventana.
	CImgUtils::waitForWindow(displays[nImages - 1]);


}

void ejercicio_6(){
	const unsigned nImages = 8;
	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");
	CImg<unsigned char> images[nImages];
	CImgDisplay displays[nImages];
	for (unsigned char indx = 0; indx < nImages; ++indx) {
		images[indx] = CImgUtils::toBinary(CImgUtils::toBit(originalImage, indx), unsigned char((1 << indx) - 1));
		displays[indx] = CImgUtils::showImage(images[indx]);
	}
	
	CImgUtils::waitForWindow(displays[nImages - 1]);

	//parte 2:
	TjpLogger &log = TjpLogger::getInstance();
	log.log("ejercicio_6()", "Comenzando parte 2");

	std::vector<CImg<unsigned char>> imagenes;
	imagenes.reserve(nImages);
	int mask = 128, aux = 128;
	for (unsigned i = 0; i < nImages; ++i) {
		std::stringstream ss;
		CImg<unsigned char> &masked = originalImage & (mask);
		mask += (aux /= 2);// = 128+64+32+16+8+4+2+1
		float mse = CImgUtils::meanSquaredError(originalImage, masked);
		ss << "Cantidad de bits:" << i + 1 << " mse=" << mse;
		log.log("meanSquaredError()", ss.str());
	}
}

void ejercicio_7(){
	TjpLogger &log = TjpLogger::getInstance();
	log.log("ejercicio_7() - ", "Item 1. Procesando imagen earth.bmp");
	CImg<unsigned char> originalImage("../guia_tp_01/img/earth.png");
	originalImage.log2().normalize(0, 255);

	log.log("ejercicio_7() - ", "Item 1. Mostrando imagen procesada earth.bmp");
	CImgUtils::showImageAndWait(originalImage);
	

	//no se que item es: blisters
	CImg<unsigned char> blisterCompleto("../guia_tp_01/img/blister_completo.jpg");
	CImg<unsigned char> blisterIncompleto("../guia_tp_01/img/blister_incompleto.jpg");

	CImg<unsigned char> mascara = CImgUtils::toBinary(blisterCompleto, unsigned char(100));
	CImgUtils::showImageAndWait(mascara, "Máscara");

	std::vector<std::pair<int,int>> puntosSuperiores;
	std::vector<std::pair<int, int>> puntosInferiores;

	std::stringstream ss;
	for (int y = 0; y < mascara.height(); y+=2){
		for (int x = 0; x < mascara.width(); x += 2){
			if (mascara(x, y) == 255) { // identifico el primer pixel blanco de la mascara.
				ss << "Punto superior izquierdo encontrado: x=" << x << " y=" << y;
				log.log("ejercicio_7() : Blisters", ss.str());
				ss.str(""); // limpio el buffer
				puntosSuperiores.push_back(std::pair<int, int>(x, y));
				for (int yy = y; yy < mascara.height(); ++yy) { // busco el limite inferior izquierdo del comprimido.
					if (mascara(x, yy + 1) == 0) {
						for (int xx = x; xx < mascara.width(); ++xx) { // busco el limite inferior derecho del comprimido.
							if (mascara(xx + 1, yy) == 0){
								ss << "Punto inferior derecho encontrado: x=" << xx << " y=" << yy;
								log.log("ejercicio_7() : Blisters", ss.str());
								ss.str(""); // limpio el buffer
								puntosInferiores.push_back(std::pair<int, int>(xx, yy));
								x = xx; y = yy; break;
							}
						}
						break;
					}
				}
			}
		}
	}

	

}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_7();
	//ejercicio_7();
	return 0;
}

