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
				float valor = imagen(x, y);
				std::stringstream ss;
				ss << "x=" << x << " y=" << y << " valor= " << valor << "\n";
				ventana.set_title(ss.str().c_str());
			}
		}
		else if (ventana.button() & 2) {// click derecho
			float color[] = { 255.0f };
			CImg<float> display_img(600, 600, 1, 1, 0);
			CImgDisplay display(display_img, "Perfil de Intensidad", 0, false, true); // para que inicie cerrada.
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
				int x = 0;
				while (pointIterator != puntos.end()){
					std::pair<int, int> &point = *pointIterator++;
					display_img(x++, 0) = imagen(point.first, point.second);
				}
				//display_img = display_img.crop(0, x);
				//display_img.save_ascii("intensidadSobreLinea.txt");
				display_img.draw_graph(display_img, color, 1.0f, 1, 1, 0, 0);
				display_img.display(display);
			}
			ventana.set_button();
			display.show();
			while (!display.is_closed()){}
		}
		else if ( ventana.button() & 4 || ventana.is_keyPADADD() ) {// click del medio o boton '+'
			modo = (modo + 1)%3;
		}
		ventana.wait();
	}
}

/*
Generar matriz de 256x256 y llenarla de valores aleatorios entre 0 y 255. Visualizar.
*/
void ejercicio_4() {
	CImg<unsigned char> matriz256x256(256, 256, 1, 1, 0); // imagen de 256x256x1 llena de 0.
	CImgDisplay display;

	for (int x = 0; x < 256; ++x){
		unsigned char valr = CImgUtils::generateRandomChar(0, 255);
		for (int y = 0; y < 256; ++y){
			matriz256x256(x, y) = valr;
		}
	}
	display.display(matriz256x256);
	display.set_title("Item 1");
	//while (!display.is_closed()) {}
	CImgUtils::waitForWindow(display);

	CImg<unsigned char> matriz100x100(100, 100, 1, 1, 0);
	for (int x = 0; x < 100; ++x){
		unsigned char valr = CImgUtils::generateRandomChar(0, 100);
		for (int y = 0; y < 100; ++y){
			matriz100x100(x, y) = valr;
		}
	}
	display.resize(matriz100x100);
	display.display(matriz100x100);
	display.set_title("Item 2");
	display.show();
	while (!display.is_closed()){
		if (display.is_keyPADADD()){
			for (int x = 0; x < 100; ++x){
				for (int y = 0; y < 100; ++y){
					matriz100x100(x, y) += 1;
				}
			}
			display.display(matriz100x100);
		}
		else if (display.is_keyPADSUB()){
			for (int x = 0; x < 100; ++x){
				for (int y = 0; y < 100; ++y){
					matriz100x100(x, y) -= 1;
				}
			}
			display.display(matriz100x100);
		}
		else {
		display.wait(); //para que no consuma recursos al pp
		}
	}

	CImg<unsigned char> matriz100x100Binaria(100, 100, 1, 1,0);
	for (int x = 0; x < matriz100x100Binaria.width(); x += 2){
		for (int y = 0; y < matriz100x100Binaria.height(); y++){
			matriz100x100Binaria(x, y) = 255;
		}
	}
	display.set_title("Item 3");
	display.resize(matriz100x100Binaria);
	display.display(matriz100x100Binaria); // se ve todo negro porque el rango va de 0 a 255 y la imagen tiene valores de 0 y 1
	display.show();
	//while (!display.is_closed()) { display.wait(); }
	CImgUtils::waitForWindow(display); 

	unsigned int width = 400, height = 400, circleRadius = 100;
	CImg<unsigned char> &blackBackGroundWhiteCircle = CImgUtils::drawCircle(width, height, circleRadius);
	display.set_title("Item 4");
	display.resize(blackBackGroundWhiteCircle);
	display.display(blackBackGroundWhiteCircle);
	display.show();
	//CImgUtils::waitForWindow(display);
	while (!display.is_closed()) {
		if (display.is_keyARROWRIGHT()){ //increases width
			++width;// = (width + 1)/* % 640*/; //ojo que puede da overflow
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else if (display.is_keyARROWLEFT()) { // decreases width
			--width;// = width - 1; // no llegar a 0.
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else if (display.is_keyARROWUP()){ //increases height
			--height;
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else if (display.is_keyARROWDOWN()){ //decreases height
			++height;
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else if (display.is_keyPADADD()){ //increases circle radius
			++circleRadius;
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else if (display.is_keyPADSUB()){ //decreases circle radius
			--circleRadius;
			CImg<unsigned char> &resized = CImgUtils::drawCircle(width, height, circleRadius);
			display.resize(resized);
			display.display(resized);
		}
		else {
			display.wait();
		}
	}
}

void ejercicio_5(){
	CImg<unsigned char> rmn("img/rmn.jpg");
	CImgList<unsigned char> subsamplingList;
	subsamplingList.push_back(rmn);
	int width = rmn.width();
	int heigth = rmn.height();
	while (width > 1 && heigth > 1){
		CImg<unsigned char> &subsampled = CImgUtils::subSampleBy2(subsamplingList.back());
		//subsampled.resize(rmn.width(), rmn.height());
		subsamplingList.push_back(subsampled);
		width = subsampled.width();
		heigth = subsampled.height();
	}

	auto iterator = subsamplingList.begin();
	while (iterator != subsamplingList.end()){
		auto &imagen = *iterator++;
		imagen.resize(rmn.width(), rmn.height());
	}

	CImgDisplay display;
	display.display(subsamplingList);
	
	CImgUtils::waitForWindow(display);

}

void ejercicio_6(){
	CImg<unsigned char> huang2("img/huang2.jpg");
	CImgList<unsigned char> huangs;
	huangs.push_back(huang2);
	int quantize = 256;
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));
	huangs.push_back(huang2.get_quantize(quantize/=2));

	CImgDisplay display;
	display.display(huangs);
	CImgUtils::waitForWindow(display);
}

/*Half-Toning*/
void ejercicio_7(){
	CImg<unsigned char> huang1("img/huang1.jpg"), huang2("img/huang2.jpg"), huang3("img/huang3.jpg");

	CImgUtils::halfToning(huang1);
	CImgUtils::halfToning(huang2);
	CImgUtils::halfToning(huang3);

}

/*Determinar el llenado de la botella*/
void ejercicio_8(){
	CImg<unsigned char> botellasRaw("img/botellas.tif");
	CImgList<unsigned char> botellas;

	/*[numeroBoterlla][x-y][inicio-fin]*/
	int posicionBotellas[5][2][2] = { 
	{ {0, 35}, {10,193} },    //primera botella
	{ {50,105}, {10,193} },   // segunda botella 
	{ {117, 171}, {10,193} }, //tercera
	{ {185, 240}, {10,193} }, //cuarta
	{ {252, 283}, {10,193} }  // quinta
	};

	unsigned char riseStart = 128;
	unsigned char riseEnd = botellasRaw.max();
	CImgUtils::transform(botellasRaw, riseStart, riseEnd);
	botellasRaw.quantize(4, false); //0 = negro, 1, 2, 3 = blanco

	//auto d = CImgUtils::showImage(botellasRaw);
	//CImgUtils::waitForWindow(d);

	/*x0, y0, x1, y1*/
	unsigned nroBotella = 0;
	while (nroBotella < 5){
		botellas.push_back(botellasRaw.get_crop(posicionBotellas[nroBotella][0][0],
			posicionBotellas[nroBotella][1][0],
			posicionBotellas[nroBotella][0][1],
			posicionBotellas[nroBotella][1][1]));
		nroBotella++;
	}
	
	float lessLoadedPercent = 100.0f;
	int lessLoadedIndex = -1;
	for (unsigned i = 0; i < nroBotella; ++i){
		CImg<unsigned char> &image = botellas[i];
		
		int x_pos = image._width/2;
		
		int y_lleno = 0;
		for (int y = image.height()-1; y >= 0; --y) {
			unsigned char &val = image(x_pos, y);
			if (val > 2) {
				y_lleno = y;
				break;
			}
		}

		//hardcoded:
		float loadedPercent = float(183-y_lleno)*100.0f / 183.0f;
		if (loadedPercent < lessLoadedPercent){
			lessLoadedPercent = loadedPercent;
			lessLoadedIndex = i;
		}
		std::stringstream ss;
		ss << "Botella n�:" << i + 1 << "\t% llena: " << loadedPercent;
		TjpLogger::getInstance().log(std::string("ejercicio_8()\t"), ss.str());
	}
	std::stringstream ss;
	ss << "Botella menos llena: " << lessLoadedIndex + 1 << " Porcentaje de llenado: " << lessLoadedPercent << "%";
	TjpLogger::getInstance().log(std::string("ejercicio_8()\t"), ss.str());

}

int _tmain(int argc, _TCHAR* argv[])
{
	int ejercicio = -1;
		std::cout << "GUIA DE EJERCICIOS NRO 1" << std::endl;
	do{
		std::cout << "Ingrese el numero de ejercicio [1-8], 0 = salir" << std::endl;
		std::cin >> ejercicio;

		while (ejercicio < 0 || ejercicio > 9){
			std::cout << "Ejercicio incorrecto, ingrese un valor entre 1 y 8, 0 = salir"<<std::endl;
			std::cin >> ejercicio;
		}

		switch (ejercicio){
		case 1:
			ejercicio_1();
			break;
		case 2:
			ejercicio_2();
			break;
		case 3:
			ejercicio_3();
			break;
		case 4:
			ejercicio_4();
			break;
		case 5:
			ejercicio_5();
			break;
		case 6:
			ejercicio_6();
			break;
		case 7:
			ejercicio_7();
			break;
		case 8:
			ejercicio_8();
			break;
		}
	} while (ejercicio != 0);
	
	return 0;
}