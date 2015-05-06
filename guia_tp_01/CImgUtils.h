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

using namespace cimg_library;

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

	template <class T> static inline T min(CImg<T> &image){
		T minimo = T(255);
		cimg_forXY(image, x, y){
			if (image(x, y) < minimo) minimo = (image(x, y));
		}
		return minimo;
	}

	template <class T> static inline CImg<T> new2DImage(unsigned int width, unsigned int height, unsigned spectrum){
		return CImg<T>(width, height, 1, spectrum, T(0));
	}

	static inline CImg<unsigned char> new2DImageUchar(unsigned int width, unsigned int height){
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
	static inline void waitForWindow(CImgDisplay &imagen){
		static std::string callFrom("waitForWindow()");
		TjpLogger::getInstance().log(callFrom, "Esperando Evento...");
		while (!imagen.is_closed()) { imagen.wait(); }
	}

	/*Crea una CImg y le dibuja un círculo en el medio*/
	template <typename T> 
	inline static CImg<T> drawCircle(int width, int height, int circleRadius, T &tipo = T(0) ){
		static T white[] = { T(255) };
		if (width*height <= 0) throw std::logic_error("Alto o ancho icorrecto/s, revisar los valores");
		CImg<T> image(width, height, 1, 1, 0);
		image.draw_circle(width / 2, height / 2, circleRadius, white);
		return image;
	}

	/*Reduce el tamaño de la imagen a la mitad. Descarta pixeles intermedios*/
	template <class T = unsigned char>
	static inline CImg<T> subSampleBy2(CImg<T> &image) {
		CImg<T> subsampled(image.width() / 2, image.height() / 2, image.depth(), image.spectrum(), 0);
		for (int x = 0; x < image.width(); x += 2){
			for (int y = 0; y < image.height(); y += 2){
				subsampled(x / 2, y / 2) = image(x, y);
			}
		}
		return subsampled;
	}
	/*Reemplaza una sub region de la imagen por otra. Ojo que no chequea limites*/
	template <class T>
	static inline void replaceSubRegion(CImg<T> &destiny, CImg<T> &source, int x_from, int y_from) {
		for (int x = x_from; x < min(source.width() + x_from, destiny.width()); ++x){
			for (int y = y_from; y < min(source.height() + y_from, destiny.height()); ++y){
				destiny(x, y) = source(x - x_from, y - y_from);
			}
		}
	}

	/*Crea una imagen de medios tonos. No modifica la original*/
	template<class T>
	static CImg<T> halfToning(CImg<T> &imagen){
		CImg<T> &halfToned = imagen.get_resize(imagen._width / 3, imagen._height / 3);
		//transform(imagen, T(100), T(200));
		imagen.log();
		halfToned.quantize(10, false);
		std::map<T, CImg<T>> mapa;
		CImg<T> img(3, 3, 1, 1, 0);

		mapa.insert(std::pair<T, CImg<T>>(T(0), img));

		img(1, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(1), img));

		img(2, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(2), img));

		img(0, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(3), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(4), img));

		img(2, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(5), img));

		img(2, 1) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(6), img));

		img(1, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(7), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(8), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(9), img));

		halfToned.resize(imagen.width(), imagen.height());

		for (int x = 0; x < halfToned.width(); x += 3){
			for (int y = 0; y < halfToned.height(); y += 3){
				T &key = halfToned(x, y);
				CImg<T> &value = mapa.at(key);
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
	template <class T>
	CImg<T> brightness(CImg<T> &image, float scale){
		return image * scale;
	}

	/*Transforma una imagen, a partir de un valor de inicio (aplica 0 hasta dicho valor) y un valor final(todo lo superior va a 255)*/
	template <class T>
	static void transform(CImg<T> &image, T riseStart, T riseEnd){
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
	template <class T> static CImgDisplay showImage(CImg<T> &image, const char* title = ""){
		CImgDisplay display;
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
	template <class T> static CImg<T> linearTransformation(CImg<T> &source, float gain = 1.0f, float offset = 0.0f){
		CImg<T> copy = source; //copia?
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
	template <class T> static inline CImg<T> logarithmicTransformation(CImg<T> &image){
		CImg <float> floatImage(image);
		floatImage.normalize(0.0f, 1.0f);
		cimg_forXY(floatImage, x, y){
			floatImage(x, y) = log(floatImage(x, y) + 1.0f);
		}
		floatImage.normalize(0.0f, 255.0f);
		/*image = floatImage;*/
		cimg_forXY(floatImage, x, y){
			image(x, y) = T(redondea(floatImage(x, y)));
		}
		return image;
	}

	template <class T> static inline CImg<T> powTransformation(CImg<T> &image, float exp){
		CImg <float> floatImage(image);
		floatImage.normalize(0.0f, 1.0f);
		cimg_forXY(floatImage, x, y){
			floatImage(x, y) = pow(floatImage(x, y), exp);
		}
		floatImage.normalize(0.0f, 255.0f);
		/*image = floatImage;*/
		cimg_forXY(floatImage, x, y){
			image(x, y) = T(redondea(floatImage(x, y)));
		}
		return image;
	}

	template <class T> static inline CImg<T> XOR(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (righ.size() != left.size()) {
			right.resize(left);
		}
		cimg_forxy(result, x, y){
			result(x, y) = (left(x, y) & 1) ^ (right(x, y) & 1) ? T(255), T(0);
		}
		return result;
	}

	template <class T> static inline CImg<T> AND(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (righ.size() != left.size()) {
			right.resize(left);
		}
		cimg_forxy(result, x, y){
			result(x, y) = left(x, y) && right(x, y) ? T(255), T(0);
		}
		return result;
	}

	template <class T> static inline CImg<T> OR(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (righ.size() != left.size()) {
			right.resize(left);
		}
		cimg_forxy(result, x, y){
			result(x, y) = left(x, y) || right(x, y) ? T(255), T(0);
		}
		return result;
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
	template <class T = unsigned char> static inline CImg<T> mapLUT(CImg<T> &source, std::vector<T> &LUT) {
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
	template <class T = unsigned char> static inline void addRange(Punto<T> &valorActual, CImg<T> &image, std::vector<T> &LUT, CImg<T> &curve){
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
	template <class T> static inline void redrawCurve(std::set<Punto<T>> &rangos, CImg<T> &image) {
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
	template<class T> static inline CImg<T> addImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val += rightCopy(x, y);
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("addImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? val - 255.0f : 255.0f - val; // doy la vuelta.
			}
			val /= 2;
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Resta pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template <class T> static inline CImg<T> substractImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

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
	template <class T> static inline CImg<T> multiplyImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val *= rightCopy(x, y);
			//val /= 255.0f; //normalization
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
	template <class T> static inline CImg<T> divideImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		//CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (leftSideImage.width() != rightCopy.width() || leftSideImage.height() != rightCopy.height()){
			rightCopy.resize(leftSideImage);
		}

		std::vector<unsigned char> LUT = createLut255(-1.0f, 255.0f); //r = -1*s-255 : inversa.
		mapLUT(rightCopy, LUT);
		return multiplyImages(leftSideImage, rightCopy);
	}

	/*Convierte una imagen en binaria a partir de un valor de umbral. No altera la imagen original*/
	template <class T> static inline CImg<T> toBinary(const CImg<T> &image, T umbral = T(127), T min = T(0), T max = T(255)){
		CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) > umbral ? max : min;
		}
		return copy;
	}

	/*Agrega ruido de distribución normal, con media = mean y desviación = deviation, no altera la imagen original*/
	template <class T> static inline CImg<T> addNoise(const CImg<T> &image, float mean, float deviation) {
		static std::default_random_engine generator;
		std::normal_distribution<float> distribution(mean, deviation);
		CImg<float> noiseMap(image.width(), image.height(), 1, 1, 0.0f);
		cimg_forXY(noiseMap, x, y){
			float noise = distribution(generator) + float(image(x, y));
			if (noise < 0.0f) noise = 0.0f;
			if (noise > 255.0f) noise = 255.0f;
			noiseMap(x, y) = noise - image(x, y);
		}

		CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) + T(noiseMap(x, y));
		}
		return copy;
	}

	/*TODO add doc, supone que todas la imagenes tienen el mismo tamaño*/
	template <class T> static inline CImg<T> reduceNoise(std::vector<CImg<T> > &images) {
		int width = images.begin()->width();
		int height = images.begin()->height();
		CImg<double> sum(width, height, 1, 1, 0.0);
		if (images.size() > 0){

			for (auto &image : images){
				sum += image;
			}

			sum /= images.size();
#ifdef _DEBUG
			sum.save_bmp("sum.bmp");
#endif
		}
		return CImg<T>(sum);
	}

	template <class T> static inline CImg<T> moverImagen(CImg<T> &image, int deltaX = 1, int deltaY = 0){
		CImg<T> &result = image.get_fill(0); // para que tenga el mismo tamaño

		for (int x = deltaX; x < image.width(); ++x){
			for (int y = deltaY; y < image.height(); ++y){
				result(x, y) = image(x - deltaX, y - deltaY);
			}
		}

		return result;
	}

	template <class T> static inline CImg<T> embossFilter(CImg<T> &image, int deltaX = 1, int deltaY = 0) {
		std::vector<unsigned char> &LUT = createLut255(-1.0f, 255.0f);
		CImg<T> semi_inverse = mapLUT(image, LUT);
		semi_inverse = moverImagen(semi_inverse, deltaX, deltaY);
		return addImages(image, semi_inverse);
	}
	/*
	bit 7 => 1000 0000 = 128
	bit 6 => 0100 0000 = 64
	...
	bit 0 = > 0000 0001 = 1
	*/
	template <class T> static inline CImg<T> toBit(CImg<T> &image, unsigned char bit = 0){
		bit = bit % 8; // para que no se pase de largo.
		CImg<T> result(image);
		unsigned char mascara = (1 << bit);
		result &= mascara;
		return result;
	}

	template <class T> static inline float meanSquaredError(const CImg<T> &actual, const CImg<T> &aprox) {
		CImg<T> aproxCorrected = aprox.size() == actual.size() ? aprox.get_resize(actual) : aprox;
		float mse = 0.0;
		cimg_forXY(actual, x, y) {
			float aux = fabs(float(actual(x, y)) - float(aproxCorrected(x, y)));
			mse += aux*aux;
		}
		return (mse / actual.size()) / actual.spectrum();
	}

	template <class T> static inline void showImageAndWait(CImg<T> &image, char* title = ""){
		CImgUtils::waitForWindow(CImgUtils::showImage(image, title));
	}

	/*pasar por parametro los valores donde se quiere retornar el calculo*/
	static void getCentroid(int x0, int y0, int x1, int y1, int &cx, int &cy) {
		if (x0 > x1) intercambia(x0, x1);
		if (y0 > y1) intercambia(y0, y1);
		cx = x0 + (x1 - x0) / 2;
		cy = y0 + (y1 - y0) / 2;
	}

	template < class T > static float getDistance(CImg<T> &left, CImg<T> &right) {
		float result = 0.0f;

		cimg_forXY(left, x, y){
			float aux = float(left(x, y)) - float(right(x, y));
			result += aux*aux;
		}

		return result;
	}
#define M_PI 3.14159265358979323846f

	template<typename T>
	static inline CImg<T> gaussFilter(T sigma = T(1.0f), T deriv = T(0)) {
		T width = 3 * sigma;               // may be less width?
		T sigma2 = sigma*sigma;
		CImg<T> filter;
		filter.assign(int(2 * width) + 1);

		T i = 0;
		for (T x = -width; x <= width; x += 1.0f) {
			T g = T(exp(-0.5*x*x / sigma2)) / sqrt(2 * M_PI) / sigma;
			if (deriv == 1) g *= -x / sigma2;
			if (deriv == 2) g *= (x*x / sigma2 - 1.0f) / sigma2;
			filter(i) = T(g);
			i++;
		}
		return filter;
	}

	/**
	* Copiado desde las funciones de la catedra, modificado para funcionar con double. Optimizado ligeramente
	*/
	inline static void draw_3D_image(const CImg<double> &imagen, const double sigma = 1.0f, const double ratioz = 1.0, const unsigned int di = 10){
		// Init data
		CImg<double> &img = imagen.get_blur(sigma).get_resize(-100, -100, 1, 3);
		CImg<double> &norm = img.get_norm().get_normalize(0, 255);

		CImgList<unsigned int> primitives;
		CImgList<double> colors;
		const CImg<> &points = img.get_elevation3d(primitives, colors, norm*-ratioz);

		// Compute image isophotes.
		std::fprintf(stderr, "\n- Compute image isophotes"); std::fflush(stderr);
		CImgList<unsigned int> isoprimitives;
		CImgList<double> isocolors;
		CImg<> isopoints;

		for (unsigned int i = 0; i<255; i += di) {
			CImgList<> prims;
			const CImg<> &pts = norm.get_isoline3d(prims, (float)i);
			isopoints.append_object3d(isoprimitives, pts, prims);
		}
		cimglist_for(isoprimitives, l) {
			const unsigned int i0 = isoprimitives(l, 0);
			const float x0 = isopoints(i0, 0), y0 = isopoints(i0, 1);
			const double
				r = (double)img.linear_atXY(x0, y0, 0),
				g = (double)img.linear_atXY(x0, y0, 1),
				b = (double)img.linear_atXY(x0, y0, 2);
			isocolors.insert(CImg<double>::vector(r, g, b));
		}
		cimg_forX(isopoints, ll) isopoints(ll, 2) = -ratioz*norm.linear_atXY(isopoints(ll, 0), isopoints(ll, 1));

		// Enter event loop
		std::fprintf(stderr, "\n- Enter interactive loop.\n\n"
			"Reminder : \n"
			" + Use mouse to rotate and zoom object\n"
			" + key 'F' : Toggle fullscreen\n"
			" + key 'Q' or 'ESC' : Quit\n"
			" + Any other key : Change rendering type\n\n"); std::fflush(stderr);
		const char *const title = "Image viewed as a surface";
		CImgDisplay disp(800, 600, title, 0);
		unsigned int rtype = 2;
		CImg<float> &pose = CImg<float>::identity_matrix(4);

		while (!disp.is_closed()) {
			const double white[3] = { 255, 255, 255 };
			CImg<double> visu(disp.width(), disp.height(), 1, 3, 0);
			visu.draw_text(10, 10, "Render : %s", white, 0, 1, 19, rtype == 0 ? "Points" : (rtype == 1 ? "Lines" : (rtype == 2 ? "Faces" : (rtype == 3 ? "Flat-shaded faces" :
				(rtype == 4 ? "Gouraud-shaded faces" : (rtype == 5 ? "Phong-shaded faces" : "Isophotes"))))));
			if (rtype == 6){
				visu.display_object3d(disp, isopoints, isoprimitives, isocolors, true, 1, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
			}
			else {
				visu.display_object3d(disp, points, primitives, colors, true, rtype, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
			}
			switch (disp.key()) {
			case 0: break;
			case cimg::keyBACKSPACE: rtype = (7 + rtype - 1) % 7; break;
			case cimg::keyQ:
			case cimg::keyESC: disp.close(); break;
			case cimg::keyF:
				if (disp.is_fullscreen()) disp.resize(800, 600); else disp.resize(disp.screen_width(), disp.screen_height());
				disp.toggle_fullscreen();
				break;
			default: rtype = (rtype + 1) % 7; break;
			}
		}
	}



	template <class T = double>
	inline static CImg<T> getSpectrum(CImgList<T> &fft) {
		if (fft.size() != 2) {
			throw std::runtime_error("fft debe poseer dos componentes");
		}
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int ancho = real.width();
		int alto = real.height();
		CImg<T> &magnitud = real.get_fill(T(0));
		cimg_forXY(magnitud, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			magnitud(x, y) = r*r + i*i;
		}
		return magnitud.sqrt();
	}


	template <class T = double> 
	inline static void showSpectrum(CImgList<T> &fft, boolean center = true, boolean save = false) {
		CImg<T> &magnitud = CImgUtils::getSpectrum(fft);
		int alto = magnitud.height();
		int ancho = magnitud.width();
		magnitud += 1;
		magnitud.log();
		magnitud.normalize(0, 255);
		if (center) {
			magnitud.shift(ancho / 2, alto / 2, 0, 0, 2);
		}
		if (save) {
			magnitud.save_bmp("magnitud.bmp");
		}
		magnitud.display();
		
	}

	template <class T> inline static void showPhase(CImgList<T> &fft) {
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int alto = real.width();
		int ancho = real.height();
		CImg<T> phase(real.get_fill(T(0)));
		cimg_forXY(phase, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			phase(x, y) = atan2( i , r );
		}
		phase.display();
	}

	template <class T>
	inline static CImg<T> getPhase(CImgList<T> &fft){
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int alto = real.width();
		int ancho = real.height();
		CImg<T> phase(real.get_fill(T(0)));
		cimg_forXY(phase, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			phase(x, y) = atan2(i, r);
		}
		return phase;
	}

};