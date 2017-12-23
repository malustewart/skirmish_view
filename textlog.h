#pragma once

#include "contentBox.h"
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include <list>
#include <vector>
#include <string>


class textlog : public contentBox
{
public:
	textlog(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height);
	~textlog();
	void append(std::string newMsg);
	void append(char* newMsg);
	void scroll(signed int offset);
	void clear();

private:
	std::list<std::vector<std::string>> log;
	unsigned int maxLinesDisplayed;	//maxima cantidad de lines que entra en el textlog
	signed int scrollOffset;		//cuantas lineas para arriba esta escrolleado el textlog
	double lineHeight;				//alto de cada renglon. Depende de la fuente elegida
	unsigned int totalLines;		//cuantas lineas estan guardadas. se actualiza cada vez ue se cambia de tamanio el display
	ALLEGRO_FONT * font;
	ALLEGRO_COLOR fontColor;

	virtual void drawContent();		//dibujar el contenido del Box.
	virtual void resizeContent();
};

