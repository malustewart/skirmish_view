#include "Board.h"
#include <iostream>
#include <algorithm>
#include <allegro5\allegro_primitives.h>

using namespace std;

Board::Board(ALLEGRO_DISPLAY * display):contentBox(display, 0.28, 0.25, 0.44, 0.5)
{
	srand(time(nullptr));

	tileSide = min(contentWidth / BOARD_WIDTH, contentHeight / BOARD_HEIGHT);

	imgTerrenos[TIERRA] = al_load_bitmap("dirtyTexture.png");
	imgTerrenos[AGUA] = al_load_bitmap("glassTexture.png");
	imgTerrenos[PLANTA] = al_load_bitmap("grassTexture2.png");

	imgEjercito[NADA] = nullptr;
	imgEjercito[HOMBRE] = al_load_bitmap("manSprite.png");
	imgEjercito[MUJER] = al_load_bitmap("womanSprite.png");
	imgEjercito[BEBE] = al_load_bitmap("babySprite.png");

	for (int k = 0; k < CANT_DE_ELEMENTOS; k++)
	{
		cout << (k ? "EJERCITOS" : "TERRENOS") << endl;
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				map[i][j][k] = (rand() % (k ? CANT_DE_EJERCITOS : CANT_DE_TERRENOS) );	//TODO: borrar esta cabeceada
				cout << map[i][j][k] << " ";
			}
			cout << endl;
		}
	}



	
}


Board::~Board()
{
}

void Board::drawContent()
{
	ALLEGRO_BITMAP ** imgArray = nullptr;
	for (int k = 0; k < CANT_DE_ELEMENTOS; k++)
	{
		if (k == 0)
		{
			imgArray = imgTerrenos;
		}
		else if (k == 1)
		{
			imgArray = imgEjercito;
		}
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				if (imgArray[map[i][j][k]] != nullptr)
				{
					al_draw_scaled_bitmap(imgArray[map[i][j][k]], 0, 0,
						al_get_bitmap_width(imgArray[map[i][j][k]]), al_get_bitmap_height(imgArray[map[i][j][k]]),
						contentStartX + i * tileSide, contentStartY + j * tileSide,
						tileSide, tileSide, 0);
				}
			}
		}
		al_flip_display();
	}
}

void Board::resizeContent()
{
}
