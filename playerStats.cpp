#include "playerStats.h"

#include <algorithm>

#define COIN_SPRITE_FILE "moneySprite.png"
#define ARMY_SPRITE_FILE "armySprite.png"
#define TIME_SPRITE_FILE "timeSprite.png"

#define STATS_FONT_FILE	"ttf.ttf"

using namespace std;

playerStats::playerStats(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height) 
	: contentBox(display, startX, startY, width, height)
{
	ALLEGRO_BITMAP * timeSprite = al_load_bitmap(TIME_SPRITE_FILE);

	myStats[MONEY].bitmap = al_load_bitmap(COIN_SPRITE_FILE);	//TODO: control de error
	myStats[ARMIES].bitmap = al_load_bitmap(ARMY_SPRITE_FILE);

	myStats[MONEY].statValue = string("2456");
	myStats[ARMIES].statValue = string("367,5");

	font = al_load_font(STATS_FONT_FILE, 20, 0);	//TODO: magic numbers

	lineHeight = al_get_font_line_height(font);
}

playerStats::~playerStats()
{
	al_destroy_font(font);
}

void playerStats::drawStat(statLine stat, unsigned int x, unsigned int y)
{	
	//TODO: dejar al_draw_scaled_bitmap o hacer un nuevo bitmap del tamanio indicado?

	unsigned int bmpWidth = al_get_bitmap_width(stat.bitmap);
	unsigned int bmpHeight = al_get_bitmap_height(stat.bitmap);

	double longestEdge = (double)max(bmpWidth, bmpHeight);
	
	double resizingFactor = ((double)lineHeight) / longestEdge;

//	al_draw_bitmap(stat.bitmap, contentStartX, contentStartY, 0);

	al_draw_scaled_bitmap(stat.bitmap, 0, 0,
						  bmpWidth, bmpHeight,
						  x, y, resizingFactor * bmpWidth, resizingFactor * bmpHeight, 0);
	al_draw_text(font, al_map_rgba_f(0, 0, 0, 0.6), x + lineHeight + 2, y + 2, 0, stat.statValue.c_str());
	al_draw_text(font, al_map_rgb_f(1, 1, 1), x + lineHeight, y, 0, stat.statValue.c_str());
}

void playerStats::drawContent()
{
	for (int i = 0; i < AMOUNT_OF_STATS; i++)
	{
		drawStat(myStats[i], contentStartX, contentStartY + lineHeight * i);
	}
}

void playerStats::resizeContent()
{

}
