#include "contentBox.h"

#include <allegro5\allegro_primitives.h>
#include <algorithm>

using namespace std;


//TODO: decidir donde va la informacion que esta en los defines de aca abajo
#define CONTENT_MARGIN	10.0		//margen entre el verdadero inicio de la caja del textbox y los limites del texto
#define CORNER_ROUNDNESS 0.0

#define DISPLAY_SECTION_MARGIN 10.0	//margen para la "caja" que contiene todos los elementos del display
									//EJ.: en un display de ancho 600, si dice que la ubicacion de un elemento
									//del display va entre 300 a 600, en realidad, va entre:
									//(300 + DISPLAY_SECTION_MARGIN) y (600 - DISPLAY_SECTION_MARGIN)

void drawFunkyRectangle(int x1, int y1, int x2, int y2); //TODO: no entregar una funcion con este nombre



contentBox::contentBox(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height)
{
	this->display = display;

	palette.background = al_map_rgb(0, 0, 0);			//TODO: estaria bueno hacerlo de manera menos chota. Con archivo de configuraciones?

	palette.frameBase = al_map_rgb_f(0.5, 0.5, 0.5);
	palette.frameHighlight = al_map_rgba_f(1, 1, 1, 0.5);

	relativeBoxStartX = startX;
	relativeBoxStartY = startY;
	relativeBoxWidth = width;
	relativeBoxHeight = height;

	setDimensions();
}

contentBox::~contentBox()
{
}

void contentBox::setPalette(boxPalette_t palette)
{
	this->palette = palette;
}

boxPalette_t contentBox::getPalette()
{
	return palette;
}

void contentBox::draw()
{
	drawBox();

	al_set_clipping_rectangle(contentStartX, contentStartY, contentWidth, contentHeight);
	drawContent();
	al_reset_clipping_rectangle();
	
	al_flip_display();	//TODO: decidir si hacemos al_flip_display o nos dedicamos a resolver lo de flipear por sectores
}

void contentBox::acknowledgeResize()
{
	setDimensions();
	resizeContent();
	//TODO: draw()? o no lo ponemos aca?
}

void contentBox::drawBox()
{
	{
		//int op, src, dst, aop, asrc, adst;
		//al_get_separate_blender(&op, &src, &dst, &aop, &asrc, &adst);

		//ALLEGRO_BITMAP * clearbmp = al_create_bitmap(relativeBoxWidth * displayWidth - 2 * DISPLAY_SECTION_MARGIN,
		//											 relativeBoxHeight * displayHeight - 2 * DISPLAY_SECTION_MARGIN);
		//al_set_target_bitmap(clearbmp);
		//al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
		//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);	
		//al_draw_filled_rounded_rectangle(0, 0, al_get_bitmap_width(clearbmp), al_get_bitmap_height(clearbmp),
		//								 CORNER_ROUNDNESS, CORNER_ROUNDNESS, al_map_rgba_f(0,0,0,0));

		////setear blender para que solo se mantenga la parte de intereseting bmp que se va a dibujar sobre el circulo. lo demas es transparente.
		//al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
		//drawFunkyRectangle(0, 0, al_get_bitmap_width(clearbmp), al_get_bitmap_height(clearbmp));


		//al_set_separate_blender(op, src, dst, aop, asrc, adst);	//volver al blender inicial
		//al_set_target_backbuffer(display);	//resetear el backbuffer porque antes estaba en clearbmp
		//al_draw_bitmap(clearbmp, relativeBoxStartX * displayWidth + DISPLAY_SECTION_MARGIN, relativeBoxStartY * displayHeight + DISPLAY_SECTION_MARGIN, 0);	
		//al_flip_display();
	}


	//Dibujar el fondo
	al_draw_filled_rounded_rectangle(relativeBoxStartX * displayWidth + DISPLAY_SECTION_MARGIN, 
									 relativeBoxStartY * displayHeight + DISPLAY_SECTION_MARGIN, 
									(relativeBoxStartX + relativeBoxWidth) * displayWidth - DISPLAY_SECTION_MARGIN, 
									(relativeBoxStartY + relativeBoxHeight) * displayHeight - DISPLAY_SECTION_MARGIN, 
									 CORNER_ROUNDNESS, CORNER_ROUNDNESS, palette.background);	//TODO: borrar magic number (mismo que abajo)

	//Dibujar frame
	al_draw_rounded_rectangle(relativeBoxStartX * displayWidth + DISPLAY_SECTION_MARGIN,
							  relativeBoxStartY * displayHeight + DISPLAY_SECTION_MARGIN,
							 (relativeBoxStartX + relativeBoxWidth) * displayWidth - DISPLAY_SECTION_MARGIN,
							 (relativeBoxStartY + relativeBoxHeight) * displayHeight - DISPLAY_SECTION_MARGIN,
							  CORNER_ROUNDNESS, CORNER_ROUNDNESS, palette.frameBase, 5);	//TODO: borra magic numbers
	
	//Dibujar highlight del frame
	al_draw_rounded_rectangle(relativeBoxStartX * displayWidth + DISPLAY_SECTION_MARGIN,
							  relativeBoxStartY * displayHeight + DISPLAY_SECTION_MARGIN,
							 (relativeBoxStartX + relativeBoxWidth) * displayWidth - DISPLAY_SECTION_MARGIN,
							 (relativeBoxStartY + relativeBoxHeight) * displayHeight - DISPLAY_SECTION_MARGIN,
							  CORNER_ROUNDNESS, CORNER_ROUNDNESS, palette.frameHighlight, 1);
}

void contentBox::setDimensions()
{
	displayHeight = al_get_display_height(display);
	displayWidth = al_get_display_width(display);

	////calculo dimensiones para los limites de la box
	//boxStartX = displayWidth * startX + DISPLAY_SECTION_MARGIN;
	//boxStartY = displayHeight * TEXTLOG_H_START + DISPLAY_SECTION_MARGIN;
	//boxWidth = displayWidth * TEXTLOG_W_END - boxStartX - DISPLAY_SECTION_MARGIN;
	//boxHeight = displayHeight * TEXTLOG_H_END - boxStartY - DISPLAY_SECTION_MARGIN;

	//calculo dimensiones para los limites del contenido
	contentStartX = relativeBoxStartX * displayWidth + DISPLAY_SECTION_MARGIN + CONTENT_MARGIN;
	contentStartY = relativeBoxStartY * displayHeight + DISPLAY_SECTION_MARGIN + CONTENT_MARGIN;
	contentWidth = relativeBoxWidth * displayWidth - 2 * (CONTENT_MARGIN + DISPLAY_SECTION_MARGIN);
	contentHeight = relativeBoxHeight * displayHeight - 2 * (CONTENT_MARGIN + DISPLAY_SECTION_MARGIN);
}

void drawFunkyRectangle(int x1, int y1, int x2, int y2)
{
	ALLEGRO_VERTEX v[] = {
		{ x1, y1, 0, 128, 0, al_map_rgb_f(1, 0, 0) },
		{ x2, y1, 0, 256, 256, al_map_rgb_f(0, 0, 1) },
		{ x2, y2, 0, 0, 256, al_map_rgb_f(0, 1, 0) },
		{ x1, y2, 0, 256, 256, al_map_rgb_f(0, 1, 1) } };
	al_draw_prim(v, nullptr, al_load_bitmap("paperTexture.png"), 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN); //TODO: corregir esta cabeceada del al_load_bitmap
}