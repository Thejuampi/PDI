#pragma once

#include <cmath> // fabs
#include <vector>
#include <map>
#include <sstream>
#include "CImg.h"
#include "TjpLogger.h"

class CImgUtils{

private:

protected:
	static inline void agregarPunto(int x, int y, std::vector<std::pair<int, int>> &vector){
		vector.push_back(std::pair<int, int>(x, y));
#ifdef _DEBUG
		static std::string callfrom = "agregarPunto()";
		std::stringstream ss;
		ss << "x=" << x << " y=" << y;
		TjpLogger::getInstance().log(callfrom, ss.str());
#endif
	}

public:

	template <class T> static inline T min(const T &v1, const T &v2){
		return v2 < v1 ? v2 : v1;
	}

	template <class T> static inline T max(const T &v1, const T &v2){
		return v2 > v1 ? v2 : v1;
	}

	template <class T> static inline int redondea(const T& value)
	{
		return (value>0) ? (int(value + .5)) : (int(value - .5));
	}

	template <class T> static inline void intercambia(T&t1, T&t2) //swap en conflicto con stl
	{
		T t0 = t1; t1 = t2; t2 = t0;
	} // cambia el contenido y no la direccion (lento pero seguro)

	void static lineaBresenham(int x1, int y1, int x2, int y2, std::vector<std::pair<int,int>> &puntos) { //no tiene divisiones comparado con DDA, y no tiene redondeos, es mas eficiente
		int dx = x2 - x1, dy = y2 - y1, x, y, NE, E, D, xstep, ystep;
		puntos.clear();
		if (!(dx || dy)) { 
			puntos.push_back(std::pair<int, int>(x1, y1));
			return;
		}

		if (abs(dx)>abs(dy)) { //|dx|>|dy| => tendencia horizontal
			if (dx<0) { intercambia(x1, x2); intercambia(y1, y2); dx = -dx; dy = -dy; } // de P2 a P1, intercambia para ir de P1 a P2
			x = x1; y = y1; NE = dx << 1; // (<<1 = *2, shift de un bit)
			ystep = 1; if (dy<0) { ystep = -1; dy = -dy; } //ystep = 1 sino doy vuelta; a y le sumo 1, sino le resto 1 para los siguientes puntos
			E = dy << 1; D = E - dx; //= 2*dy-dx
			while (x<x2) {
				agregarPunto(x, y, puntos);
				if (D>0) { y += ystep; D -= NE;/*D=2*dx*di+2*dy-dx; le resto 2dx siD>0*/ } // determina su subo(bajo) "y", x siempre avanza en 1
				x++; D += E; //E = 2dy
			}
		}
		else{//tendencia vertical
			if (dy<0) { intercambia(y1, y2); intercambia(x1, x2); dy = -dy; dx = -dx; } //de P2 a P1, blabla
			y = y1; x = x1; NE = dy << 1; // (<<1 = *2, shift de un bit)
			xstep = 1; if (dx<0) { xstep = -1; dx = -dx; }
			E = dx << 1; D = E - dy;
			while (y<y2) {
				agregarPunto(x, y, puntos);
				if (D>0) { x += xstep; D -= NE; } //determina si avanzo(retroceso en x, "y" siempre avanza en 1.
				y++; D += E;
			}
		}
		agregarPunto(x, y, puntos); // punto final
	}

	/*Retorna un unsigned char entre /param min y /param max
	Ojo que no chequea que max > min*/
	static inline unsigned char generateRandomChar(unsigned char min, unsigned char max) {
		//srand(time(NULL));
		unsigned char valr = min + rand() % (max - min);
#ifdef _DEBUG
		static std::string callFrom = "generateRandomChar()";
		unsigned v(valr);
		TjpLogger::getInstance().log(callFrom, v);
#endif
		return valr;
	}

	/*Deja una ventana ociosa a la espera de un evento. Sirve principalmente para que no se cierre.*/
	static inline void waitForWindow(cimg_library::CImgDisplay &imagen){
		static std::string callFrom("waitForWindow()");
		TjpLogger::getInstance().log(callFrom, "Esperando Evento...");
		while (!imagen.is_closed()) { imagen.wait(); }
	}

	/*Crea una CImg y le dibuja un círculo en el medio*/
	static cimg_library::CImg<unsigned char> drawCircle(int width, int height, int circleRadius){
		static unsigned char white[] = { 255 };
		cimg_library::CImg<unsigned char> image(width, height, 1, 1, 0);
		image.draw_circle(width/2, height/2, circleRadius, white);
		return image;
	}

	/*Reduce el tamaño de la imagen a la mitad. Descarta pixeles intermedios*/
	template <class T = unsigned char> static inline cimg_library::CImg<T> subSampleBy2(cimg_library::CImg<T> &image ) {
		cimg_library::CImg<T> subsampled(image.width()/2, image.height()/2, image.depth(), image.spectrum(), 0);
		for (int x = 0; x < image.width(); x += 2){
			for (int y = 0; y < image.height(); y += 2){
				subsampled(x/2, y/2) = image(x, y);
			}
		}
		return subsampled;
	}
	/*Reemplaza una sub region de la imagen por otra. Ojo que no chequea limites*/
	template <class T> static inline void replaceSubRegion(cimg_library::CImg<T> &destiny, cimg_library::CImg<T> &source, int x_from, int y_from) {
		for (int x = x_from; x < min(source.width() + x_from, destiny.width()); ++x){
			for (int y = y_from; y < min(source.height() + y_from, destiny.height()) ; ++y){
				destiny(x, y) = source(x - x_from, y - y_from);
			}
		}
	}

	/*Crea una imagen de medios tonos. No modifica la original*/
	template<class T> static cimg_library::CImg<T> halfToning(cimg_library::CImg<T> &imagen){
		cimg_library::CImg<T> &halfToned = imagen.get_resize(imagen._width / 3, imagen._height / 3);
		//transform(imagen, T(100), T(200));
		imagen.log();
		halfToned.quantize(10, false);
		std::map<T, cimg_library::CImg<T>> mapa;
		cimg_library::CImg<T> img(3, 3, 1, 1, 0);

		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(0), img));
		
		img(1, 0) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(1), img));

		img(2, 2) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(2), img));

		img(0,0) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(3), img));
		
		img(0, 2) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(4), img));
		
		img(2, 0) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(5), img));

		img(2, 1) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(6), img));

		img(1, 2) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(7), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(8), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, cimg_library::CImg<T>>(T(9), img));

		halfToned.resize(imagen.width(), imagen.height());

		for (int x = 0; x < halfToned.width(); x+=3){
			for (int y = 0; y < halfToned.height(); y += 3){
				T &key = halfToned(x, y);
				cimg_library::CImg<T> &value = mapa.at(key);
				replaceSubRegion(halfToned,value , x, y);
			}
		}

		CImgDisplay display;
		display.display(halfToned);
		display.show();

		waitForWindow(display);

		return halfToned;

	}

	/*NO USAR*/
	template <class T> cimg_library::CImg<T> brightness(cimg_library::CImg<T> &image, float scale){
		return image * scale;
	}

	/*Transforma una imagen, a partir de un valor de inicio (aplica 0 hasta dicho valor) y un valor final(todo lo superior va a 255)*/
	template <class T> static void transform(cimg_library::CImg<T> &image, T riseStart, T riseEnd){
		if (riseEnd < riseStart){
			throw std::logic_error("Error: riseEnd < riseStart");
		}
		float m = 255.0f / (riseEnd - riseStart);
		float b = 255.0f*riseStart / (riseStart - riseEnd);
		cimg_forXY(image, x, y){
			T &val = image(x, y);
			if (val < riseStart){
				val = T(0);
			}
			else if (val > riseEnd){
				val = ~T(0);
			}
			else {
				val = T(b + m*val); //transform
			}
		}
	}

	/*Crea una ventana y muestra la imagen que se pasa como parametro. EL titulo es opcional. Retorna la ventana*/
	template <class T> static cimg_library::CImgDisplay showImage(cimg_library::CImg<T> &image, const char* title = ""){
		cimg_library::CImgDisplay display;
		display.display(image);
		display.set_title(title);
		return display;
	}

	/*Transforma linealmente un pixel, no modifica el original*/
	template <class T> static T linearTransform(T &value, float gain = 1.0f, float offset = 0.0f){
		T val = value;
		float fVal = float(val);
		fVal = gain*fVal + offset;
		if (fVal < 0.0f) fVal = 0.0f;
		if (fVal > 255.0f) fVal = 255.0f;
		val = T(redondea(fVal));
		return val;
	}

	/*Transforma linealmente una imagen. No modifica la imagen original*/
	template <class T> static cimg_library::CImg<T> linearTransform(cimg_library::CImg<T> &source, float gain = 1.0f, float offset=0.0f){
		cimg_library::CImg<T> copy = source; //copia?
		cimg_forXY(copy, x, y){
			T &val = copy(x, y);
			float fVal = float(val);
			fVal = gain*fVal + offset;
			if (fVal < 0.0f) fVal = 0.0f;
			if (fVal > 255.0f) fVal = 255.0f;
			val = T(redondea(fVal));
		}
		return copy;
	}

	/*Crea una LUT, con ganancia, offset, inicio del rango y fin del rango */
	template <class T = unsigned char> static inline std::vector<T> createLut255(float gain = 1.0f, float offset = 0.0f, T start = T(0), T end = T(255)) {
		if (end < start) intercambia(start, end);
		if (start < T(0)) start = T(0);
		if (end > T(255)) end = T(255);
		int range = abs(end - start);
		std::vector<T> result(range);
		for (T x = start; x < end; ++x){
			result[x] = linearTransform(x, gain, offset);
		}
		return result;
	}

	/*Mapea una LUT sobre una imagen y retorna la imagen resultante*/
		template <class T = unsigned char> static inline cimg_library::CImg<T> mapLUT(cimg_library::CImg<T> &source, std::vector<T> &LUT) {
		CImg<T> copy = source;
		cimg_forXY(source, x, y){
			T &val = copy(X, y);
			val = LUT[val];
		}
		return copy;
	}
};