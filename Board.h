#pragma once
#include "contentBox.h"

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 12

enum{TERRENO, EJERCITO, CANT_DE_ELEMENTOS};
enum{TIERRA, AGUA, PLANTA, CANT_DE_TERRENOS};
enum{NADA, HOMBRE, MUJER, BEBE, CANT_DE_EJERCITOS};


class Board :
	public contentBox
{
public:
	Board(ALLEGRO_DISPLAY * display);
	virtual ~Board();
private:
	virtual void drawContent();
	virtual void resizeContent();
	unsigned int map[BOARD_WIDTH][BOARD_HEIGHT][CANT_DE_ELEMENTOS];	
	unsigned int tileSide;
	ALLEGRO_BITMAP * imgTerrenos[CANT_DE_TERRENOS];	//con esto mas allegro config podemos cargar las imagees con un for.
	ALLEGRO_BITMAP * imgEjercito[CANT_DE_EJERCITOS];
};

