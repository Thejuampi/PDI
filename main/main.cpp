// main.cpp : Defines the entry point for the console application.
//

/*PROCECAMIENTO DIGITAL DE IMAGENES*/
/*GUIA DE TRABAJOS PRÁCTICOS Nº 0*/

#include "stdafx.h"

using namespace cimg_library;

int _tmain(int argc, char **argv) {
	const char* filename = cimg_option("-i", "cameraman.tif", "Image file\n");
	CImg<float> imagen(filename); //Carga de disco
	int recorte_x = 50;
	int recorte_y = 60;
	CImg<float> imagenRecortada(recorte_x, recorte_y);
	for (int fila = 0; fila < recorte_x; ++fila) {
		for (int columna = 0; columna < recorte_y; ++columna) {
			imagenRecortada(fila, columna) = imagen(fila, columna);
		}
	}

	imagenRecortada.normalize(200.0f,255.0f);

	int imagenSize = imagenRecortada.size();
	CImgDisplay imageDisplayWindow(imagenRecortada, "recortada");

	imagenRecortada.save_ascii("imagenRecortada.txt");
	imagenRecortada.save_bmp("imagen.bmp");
	return 0;
}
/**
*
* Crea una imagen vacia con CImg::CImg(size x,size y,size z,size c,value/s);
*/
void ejercicio_1(){

	int size_x		= 3;	// ancho
	int size_y		= 3;	// alto
	int size_z		= 1;	// una sola "capa"
	int n_channels	= 1;	// un solo canal (escala de grises)
	unsigned char v = 255;  // valor inicial de toda la imagen (blanco)

	CImg<unsigned char> imagen(size_x, size_y, size_x, n_channels, v);
}