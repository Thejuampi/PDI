#pragma once

#include <cmath> // fabs
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <sstream>
#include <random>
#include "CImg.h"
#include "TjpLogger.h"

template <class T = unsigned char> class Punto{
private:
	T _x;
	T _y;
public:
	Punto(T x, T y) :_x(x), _y(y){}
	Punto() :_x(-1), _y(-1){}
	T x(){ return this->_x; }
	T y(){ return this->_y; }

	template <class T>
	friend inline bool operator<(const Punto<T> &p0, const Punto<T> &p1) {
		return p0._x < p1._x;
	}
#if 0
	bool operator ==(const Punto<T> &p0){ // simil java equals, p nunca es nulo
		return _x == p0.x() && _y == p.y();
	}
	bool operator !=(const Punto<T> &p0){
		return _x != p0.x() || _y != p.y();
	}

	bool operator<(const Punto<T> &p0) {
		return _x < p0.x();
	}
	bool operator<=(const Punto<T> &p0) {
		return _x <= p0.x();
	}
#endif
};
//template <typename T>
//struct PuntoComparator{
//	bool operator< (const Punto<T> &p0, const Punto<T> &p1) {
//		return p0.x() < p1.x();
//	}
//};
//
//bool std::operator<(const Punto<unsigned char> &p0, const Punto<unsigned char> &p1){
//	return p0.x() < p1.x();
//}

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
		return (value > 0) ? (int(value + .5)) : (int(value - .5));
	}

	template <class T> static inline void intercambia(T&t1, T&t2) //swap en conflicto con stl
	{
		T t0 = t1; t1 = t2; t2 = t0;
	} // cambia el contenido y no la direccion (lento pero seguro)

	template <class T> static inline cimg_library::CImg<T> new2DImage(unsigned int width, unsigned int height, unsigned spectrum){
		return cimg_library::CImg<T>(width, height, 1, spectrum, T(0));
	}

	static inline cimg_library::CImg<unsigned char> new2DImageUchar(unsigned int width, unsigned int height){
		return new2DImage<unsigned char>(width, height, 1);
	}

	void static lineaBresenham(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>> &puntos) { //no tiene divisiones comparado con DDA, y no tiene redondeos, es mas eficiente
		int dx = x2 - x1, dy = y2 - y1, x, y, NE, E, D, xstep, ystep;
		puntos.clear();
		if (!(dx || dy)) {
			puntos.push_back(std::pair<int, int>(x1, y1));
			return;
		}

		if (abs(dx) > abs(dy)) { //|dx|>|dy| => tendencia horizontal
			if (dx < 0) { intercambia(x1, x2); intercambia(y1, y2); dx = -dx; dy = -dy; } // de P2 a P1, intercambia para ir de P1 a P2
			x = x1; y = y1; NE = dx << 1; // (<<1 = *2, shift de un bit)
			ystep = 1; if (dy < 0) { ystep = -1; dy = -dy; } //ystep = 1 sino doy vuelta; a y le sumo 1, sino le resto 1 para los siguientes puntos
			E = dy << 1; D = E - dx; //= 2*dy-dx
			while (x < x2) {
				agregarPunto(x, y, puntos);
				if (D > 0) { y += ystep; D -= NE;/*D=2*dx*di+2*dy-dx; le resto 2dx siD>0*/ } // determina su subo(bajo) "y", x siempre avanza en 1
				x++; D += E; //E = 2dy
			}
		}
		else{//tendencia vertical
			if (dy < 0) { intercambia(y1, y2); intercambia(x1, x2); dy = -dy; dx = -dx; } //de P2 a P1, blabla
			y = y1; x = x1; NE = dy << 1; // (<<1 = *2, shift de un bit)
			xstep = 1; if (dx < 0) { xstep = -1; dx = -dx; }
			E = dx << 1; D = E - dy;
			while (y < y2) {
				agregarPunto(x, y, puntos);
				if (D > 0) { x += xstep; D -= NE; } //determina si avanzo(retroceso en x, "y" siempre avanza en 1.
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
		image.draw_circle(width / 2, height / 2, circleRadius, white);
		return image;
	}

	/*Reduce el tamaño de la imagen a la mitad. Descarta pixeles intermedios*/
	template <class T = unsigned char> static inline cimg_library::CImg<T> subSampleBy2(cimg_library::CImg<T> &image) {
		cimg_library::CImg<T> subsampled(image.width() / 2, image.height() / 2, image.depth(), image.spectrum(), 0);
		for (int x = 0; x < image.width(); x += 2){
			for (int y = 0; y < image.height(); y += 2){
				subsampled(x / 2, y / 2) = image(x, y);
			}
		}
		return subsampled;
	}
	/*Reemplaza una sub region de la imagen por otra. Ojo que no chequea limites*/
	template <class T> static inline void replaceSubRegion(cimg_library::CImg<T> &destiny, cimg_library::CImg<T> &source, int x_from, int y_from) {
		for (int x = x_from; x < min(source.width() + x_from, destiny.width()); ++x){
			for (int y = y_from; y < min(source.height() + y_from, destiny.height()); ++y){
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

		img(0, 0) = T(255);
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

		for (int x = 0; x < halfToned.width(); x += 3){
			for (int y = 0; y < halfToned.height(); y += 3){
				T &key = halfToned(x, y);
				cimg_library::CImg<T> &value = mapa.at(key);
				replaceSubRegion(halfToned, value, x, y);
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
	template <class T> static T linearTransformation(T &value, float gain = 1.0f, float offset = 0.0f){
		T val = value;
		float fVal = float(val);
		fVal = gain*fVal + offset;
		if (fVal < 0.0f) fVal = 0.0f;
		if (fVal > 255.0f) fVal = 255.0f;
		val = T(redondea(fVal));
		return val;
	}

	/*Transforma linealmente una imagen. No modifica la imagen original*/
	template <class T> static cimg_library::CImg<T> linearTransformation(cimg_library::CImg<T> &source, float gain = 1.0f, float offset = 0.0f){
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
	/*Aplica la transformacion logarítimica a una imagen*/
	template <class T> static inline cimg_library::CImg<T> logarithmicTransformation(cimg_library::CImg<T> &image, float r){
#ifdef _DEBUG
		static std::string callFrom("logarithmicTransformation()");
		std::stringstream ss;
		ss << "image:";
#endif
			cimg_forXY(image, x, y){
			float val = log(1.0f + float(image(x, y)));
#ifdef _DEBUG
			ss << val << ",";
#endif
			image(x, y) = T(val);
		}
#ifdef _DEBUG
		TjpLogger::getInstance().log(callFrom, ss.str());
#endif
		return image;
	}

	template <class T> static inline cimg_library::CImg<T> powTransformation(cimg_library::CImg<T> &image, float exp){
		cimg_forXY(image, x, y){
			float val = float(image(x, y));
			val = pow(val, exp);
			if (val < 0.0f) val = 0.0f; // posible?, por las dudas lo dejo.
			if (val > 255.0f) val = 255.0f;
			image(x, y) = T(val);
		}
		return image;
	}

	/*Modifica una LUT*/
	template <class T = unsigned char> static void alterLUTLinear(std::vector<T> &LUT, const float gain = 1.0f, const float offset = 0.0f, T start = T(0), T end = T(255)){
		if (start == end) return;
		if (end < start) intercambia(start, end);
		if (start < T(0)) start = T(0);
		if (end > T(255)) end = T(255);
		for (T x = start; x < end; ++x){
			LUT[x] = linearTransformation(x, gain, offset);
		}
		//No se puede hacer el <= porque se pasa de rango al hacer 255+1 = 0
		LUT[end] = linearTransformation(end, gain, offset);
#ifdef _DEBUG
		static std::string c("alterLut()");
		std::stringstream ss;
		for (auto i : LUT){
			ss << int(i) << ", ";
		}
		TjpLogger::getInstance().log(c, ss.str());
#endif
		//return LUT;
	}

	/*Crea una LUT, con ganancia, offset, inicio del rango y fin del rango */
	template <class T = unsigned char> static inline std::vector<T> createLut255(float gain = 1.0f, float offset = 0.0f, T start = T(0), T end = T(255)) {
		if (end < start) intercambia(start, end);
		if (start < T(0)) start = T(0);
		if (end > T(255)) end = T(255);
		int range = abs(end - start) + 1;
		std::vector<T> result(range);
		alterLUTLinear(result, gain, offset, start, end);
		return result;
	}

	/*Mapea una LUT sobre una imagen y retorna la imagen resultante. No altera la imagen Original*/
	template <class T = unsigned char> static inline cimg_library::CImg<T> mapLUT(cimg_library::CImg<T> &source, std::vector<T> &LUT) {
		CImg<T> copy = source;
		cimg_forXY(source, x, y){
			T &val = copy(x, y);
			val = LUT[val];
		}
		return copy;
	}
	/*Dados dos puntos, calcula la pendiente de la linea que los une*/
	template <class T = unsigned char> static inline float calcularGanancia(Punto<T> actual, Punto<T> next) {
		float pend = float(next.y() - actual.y()) / float(next.x() - actual.x());
		return pend;
	}

	/*Dados dos puntos, calcula la constante de la ecuación de la recta de los une*/
	template <class T = unsigned char> static inline float calcularOffset(Punto<T> actual, Punto<T> next){
		//c=(x2*y1 - x1*y2) / (x2 - x1)
		float x1 = float(actual.x());
		float x2 = float(next.x());
		float y1 = float(actual.y());
		float y2 = float(next.y());
		float offset = (x2*y1 - x1*y2) / (x2 - x1);
		return offset;
	}
	/*TODO add doc*/
	template <class T = unsigned char> static inline void addRange(Punto<T> &valorActual, cimg_library::CImg<T> &image, std::vector<T> &LUT, cimg_library::CImg<T> &curve){
		static std::set<Punto<T>> rangos = { Punto<T>(T(0), T(0)), Punto<T>(T(255), T(255)) }; // se crea una ves. Ojo, no es para nada thread safe.

		std::set<Punto<T>>::iterator rangoIterator = rangos.find(valorActual);

		if (rangoIterator == rangos.end()){ // no tiene el valor -> modificar la curva
			rangos.insert(valorActual); //ordena automaticamente de menor a mayor.
			std::set<Punto<T>>::iterator valorInsertadoIterator = rangos.find(valorActual);
			std::set<Punto<T>>::iterator copiaMas = valorInsertadoIterator;
			std::set<Punto<T>>::iterator copiaMenos = valorInsertadoIterator;

			//std::set<Punto<T> >::iterator valorInsertadoIterator = rangos.insert(punto); //ordena automaticamente de menor a mayor.

			//Punto<T> &valorActual = punto;
			const Punto<T> &valorAnterior = *(--copiaMenos);
			const Punto<T> &valorSiguiente = *(++copiaMas);
			/*Tengo que modificar la curva anterior y la siguiente al punto actual*/
			/*Curva anterior, no confundir con el valor que tenia previamente, sino con la curva que esta antes.*/
			float pendienteAnterior = calcularGanancia(valorAnterior, valorActual);
			float offsetAnterior = calcularOffset(valorAnterior, valorActual);
			T x_anterior = const_cast<Punto<T>&>(valorAnterior).x();
			T x_actual = const_cast<Punto<T>&>(valorActual).x();
			if (x_actual > x_anterior)
				alterLUTLinear(LUT, pendienteAnterior, offsetAnterior, x_anterior, x_actual);
			/*Curva siguiente*/
			float pendienteSiguiente = calcularGanancia(valorActual, valorSiguiente);
			float offsetSiguiente = calcularOffset(valorActual, valorSiguiente);
			T x_siguiente = const_cast<Punto<T>&>(valorSiguiente).x();
			if (x_siguiente > ++x_actual)
				alterLUTLinear(LUT, pendienteSiguiente, offsetSiguiente, x_actual, x_siguiente);

			/*Segunda etapa, mapear de nuevo el LUT a la imagen. TODO: optimizar para que solo mapee los segmentos nuevos*/
			image = mapLUT(image, LUT);

			/*Tercera etapa, redibujar la curva*/
			redrawCurve(rangos, curve);
#if 0
#endif
		} // else -> no hacer nada, porque el valor ya esta.

	}

protected:
	template <class T> static inline void redrawCurve(std::set<Punto<T>> &rangos, cimg_library::CImg<T> &image) {
		static unsigned char color[] = { 255 };
		image.fill(T(0));
		std::set<Punto<T>>::iterator actualPoint = rangos.begin();
		if (actualPoint == rangos.end()) return;
		std::set<Punto<T>>::iterator nextPoint = actualPoint;
		++nextPoint;
		if (nextPoint == rangos.end()) return;
		while (nextPoint != rangos.end()){
			const Punto<T> &p0 = (*actualPoint++);
			const Punto<T> &p1 = (*nextPoint++);
			image.draw_line(const_cast<Punto<T>&>(p0).x(),
				255 - const_cast<Punto<T>&>(p0).y(),
				const_cast<Punto<T>&>(p1).x(),
				255 - const_cast<Punto<T>&>(p1).y(),
				color);
		}
#ifdef _DEBUG
		static std::string c("redrawCurve()");
		TjpLogger::getInstance().log(c, "Guardando imagen de curva");
		image.save_bmp("click.bmp");
#endif
	}
public:
	/*Suma pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template<class T> static inline cimg_library::CImg<T> addImages(cimg_library::CImg<T> &leftSideImage, cimg_library::CImg<T> &rightSideImage){
		cimg_library::CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		cimg_library::CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val += rightCopy(x, y);
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("addImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? val-255.0f : 255.0f-val; // doy la vuelta.
			}
			val /= 2;
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Resta pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template <class T> static inline cimg_library::CImg<T> substractImages(cimg_library::CImg<T> &leftSideImage, cimg_library::CImg<T> &rightSideImage){
		cimg_library::CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		cimg_library::CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val -= rightCopy(x, y);
			val /= 2;
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("addImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? val - 255.0f : 255.0 - val;
			}
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Multiplica pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template <class T> static inline cimg_library::CImg<T> multiplyImages(cimg_library::CImg<T> &leftSideImage, cimg_library::CImg<T> &rightSideImage){
		cimg_library::CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		cimg_library::CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val *= rightCopy(x, y);
			val /= 255.0f; //normalization
#ifdef _DEBUG
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("multiplyImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? 255.0f : 0.0f;
			}
#endif
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Multiplica la inversa de la segunda imagen (derecha) sobre la primera (izquierda).*/
	template <class T> static inline cimg_library::CImg<T> divideImages(cimg_library::CImg<T> &leftSideImage, cimg_library::CImg<T> &rightSideImage){
		//cimg_library::CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		cimg_library::CImg<T> rightCopy(rightSideImage);

		if (leftSideImage.width() != rightCopy.width() || leftSideImage.height() != rightCopy.height()){
			rightCopy.resize(leftSideImage);
		}

		std::vector<unsigned char> LUT = createLut255(-1.0f, 255.0f); //r = -1*s-255 : inversa.
		mapLUT(rightCopy, LUT);
		return multiplyImages(leftSideImage, rightCopy);
	}

	template <class T> static inline cimg_library::CImg<T> toBinary(cimg_library::CImg<T> &image){
		cimg_library::CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) > T(127) ? T(1) : T(0);
		}
		return copy;
	}

	/*Agrega ruido de distribución normal, con media = mean y desviación = deviation*/
	template <class T> static inline cimg_library::CImg<T> addNoise(cimg_library::CImg<T> &image, float mean, float deviation) {
		static std::default_random_engine generator;
		std::normal_distribution<float> distribution(mean, deviation);
		cimg_library::CImg<float> noiseMap(image.width(), image.height(), 1, 1, 0.0f);
		cimg_forXY(noiseMap, x, y){
			float noise = distribution(generator) + float(image(x,y));
			if (noise < 0.0f) noise = 0.0f;
			if (noise > 255.0f) noise = 255.0f;
			noiseMap(x, y) = noise - image(x,y);
		}

		cimg_library::CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) + T(noiseMap(x, y));
		}
		return copy;
	}

	/*TODO add doc, supone que todas la imagenes tienen el mismo tamaño*/
	template <class T> static inline cimg_library::CImg<T> reduceNoise(std::vector<cimg_library::CImg<T> > &images) {
		int width = images.begin()->width();
		int height = images.begin()->height();
		cimg_library::CImg<double> sum(width, height, 1, 1, 0.0 );
		if (images.size() > 0){

			for (auto &image : images){
				sum += image;
			}

			sum /= images.size();
#ifdef _DEBUG
			sum.save_bmp("sum.bmp");
#endif
		}
		return cimg_library::CImg<T>(sum);
	}

	template <class T> static inline cimg_library::CImg<T> moverImagen(cimg_library::CImg<T> &image, int deltaX = 1, int deltaY = 0){
		cimg_library::CImg<T> &result = image.get_fill(0); // para que tenga el mismo tamaño

		for (int x = deltaX; x < image.width(); ++x){
			for (int y = deltaY; y < image.height(); ++y){
				result(x, y) = image(x - deltaX, y - deltaY);
			}
		}

		return result;
	}

	template <class T> static inline cimg_library::CImg<T> embossFilter(cimg_library::CImg<T> &image, int deltaX = 1, int deltaY = 0) {
		std::vector<unsigned char> &LUT = createLut255(-1.0f, 255.0f);
		cimg_library::CImg<T> semi_inverse = mapLUT(image, LUT);
		semi_inverse = moverImagen(semi_inverse, deltaX, deltaY);
		return addImages(image, semi_inverse);
	}

};