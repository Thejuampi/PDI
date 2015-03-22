// guia_tp_01.cpp : Defines the entry point for the console application.
//

/*PROCECAMIENTO DIGITAL DE IMAGENES*/
/*GUIA DE TRABAJOS PR�CTICOS N� 01*/

#include "stdafx.h"

using namespace cimg_library;

/* Carga la imagen cameraman.tif y recorta la parte de la cara del camarografo con un bucle for()*/
void ejercicio_1(){
	CImg<float> imagen("img/cameraman.tif");

	int inicio_x	= 95;
	int inicio_y	= 35;
	int fin_x = inicio_x + 45;
	int fin_y = inicio_y + 50;
	/* la imagen tiene 45x50 = 2250 pixeles, 3 canales de color de 8 bits cada uno = 24 bits = 3 bytes,
	*  entonces son 2250*3 bytes = 6750 bytes + cabecera.
	*/
	CImg<float> imagenRecortada(45,50);

	for (int x = inicio_x; x < fin_x; ++x){
		for (int y = inicio_y; y < fin_y; ++y){
			imagenRecortada(x - inicio_x, y - inicio_y) = imagen(x, y);
		}
	}

	CImgDisplay ventana(imagenRecortada,"Imagen Recortada");

	imagenRecortada.save_bmp("img/imagenRecortada.bmp");

	while (!ventana.is_closed()){}

}
/*Carga 3 imagenes de dos formas, con una std::list y con CImgList. Con std::list se muestran individualmente,
mientras que con CImgList se muestran todas en la misma ventana.
*/
void ejercicio_2() {
	std::list<CImg<float>> imageList;

	imageList.push_back(CImg<float>("img/cameraman.tif"));
	imageList.push_back(CImg<float>("img/a7v600-X.gif"));
	imageList.push_back(CImg<float>("img/billete.jpg"));

	std::list<CImg<float> >::iterator imagesIterator = imageList.begin();
	while (imagesIterator != imageList.end()){
		CImg<float> &image = *imagesIterator;
		image.print();
		image.display();
		++imagesIterator;
	}

	CImgList<float> imageList2;

	imageList2.push_back(CImg<float>("img/cameraman.tif"));
	imageList2.push_back(CImg<float>("img/a7v600-X.gif"));
	imageList2.push_back(CImg<float>("img/billete.jpg"));
	CImgDisplay ventana;
	ventana.display(imageList2, 'x', 0.1f);

	while (!ventana.is_closed()){};
}

void ejercicio_3() {
	CImg<float> imagen("img/cameraman.tif");
	CImgDisplay ventana;
	ventana.display(imagen);
	int modo = 0;
	while (!ventana.is_closed()) {
			int x = ventana.mouse_x();
			int y = ventana.mouse_y();
		if (ventana.button() & 1){ //click izquierdo
			if (x != -1 && y != -1){
				int valor = imagen(x, y);
				std::stringstream ss;
				ss << "x=" << x << " y=" << y << " valor= " << valor << "\n";
				ventana.set_title(ss.str().c_str());
			}
		}
		else if (ventana.button() & 2) {// click derecho
			float color[] = { 255.0f };
			CImg<float> display_img(600, 600, 1, 1, 0);
			CImgDisplay display(display_img, "Perfil de Intensidad");
			if (modo == 0){ // fila
				int image_width = imagen.width();
				display_img.draw_graph(imagen.get_crop(0, y, image_width, y), color, 1.0f, 1, 1, 0, 255, 0);
				display_img.display(display);
			}
			else if(modo == 1){ //columna
				int image_height = imagen.height();
				display_img.draw_graph(imagen.get_crop(x, 0, x, image_height), color, 1.0f, 1,1, 0, 255, 0);
				display_img.display(display);
			}
			else if (modo == 2) { //linea
				int image_height = imagen.height();
				int image_width = imagen.width();
				std::vector<std::pair<int, int>> puntos(512);
				int x1 = ventana.mouse_x(); //primer punto
				int y1 = ventana.mouse_y();
				ventana.set_button(); // limpia el boton presionado del mouse.
				while (!(ventana.button() & 2)){} // freno la ventana hasta que se presione el boton derecho del mouse.
				int x2 = ventana.mouse_x(); // segundo punto
				int y2 = ventana.mouse_y();
				CImgUtils::lineaBresenham(x1, y1, x2, y2, puntos); //algoritmo de Nestor
				std::vector<std::pair<int, int>>::iterator pointIterator = puntos.begin();
				while (pointIterator != puntos.end()){
					int x = 0;
					std::pair<int, int> &point = *pointIterator++;
					display_img(x++, 0) = imagen(point.first, point.second);
				}
				display_img.draw_graph(display_img, color, 1.0f, 1, 1, 0, 255);
				display_img.display(display);
			}
			ventana.set_button();
			while (!display.is_closed()){}
		}
		else if ( ventana.button() & 4 || ventana.is_keyPADADD() ) {// click del medio o boton '+'
			modo = (modo + 1)%3;
		}
		ventana.wait();
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	//ejercicio_1();
	//ejercicio_2();
	ejercicio_3();
	return 0;
}