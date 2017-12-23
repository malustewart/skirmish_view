#pragma once

#include "allegro5\allegro.h"
#include <string>


//TODO: idea. bool frameless??

struct boxPalette_t {
	ALLEGRO_COLOR background;
	ALLEGRO_COLOR frameBase;
	ALLEGRO_COLOR frameHighlight;
};

/******************************
*
*
*
*/
class contentBox
{
public:
	contentBox(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height);
	~contentBox();
	void setPalette(boxPalette_t palette);
	boxPalette_t getPalette();
	void draw();
	void acknowledgeResize();

protected:

	ALLEGRO_DISPLAY * display;

	unsigned int displayWidth;
	unsigned int displayHeight;

	double relativeBoxStartX;	//esquina superior derecha, proporcional al tamanio del display (ej.: 0 indica el inicio del display, 0.5 la mitad, 1 el final)
	double relativeBoxStartY;	//esquina superior izquierda, proporcional al tamanio del display
	double relativeBoxWidth;	//ancho proporcional al tamanio del display
	double relativeBoxHeight;	//ancho proporcional al tamanio del display

	double contentStartX;	//esquina superior derecha, coordenada X EN PIXELES
	double contentStartY;	//esquina superior derecha, coordenada Y EN PIXELES
	double contentWidth;	//ancho EN PIXELES
	double contentHeight;	//ancho EN PIXELES

	void drawBox();						//dibuja el fondo y el marco del contentBox
	virtual void drawContent() = 0;		//dibujar el contenido del Box.
	virtual void resizeContent() = 0;	//reformatear el contenido en respuesta a un cambio de tamanio de display

	void setDimensions();
	boxPalette_t palette;
};