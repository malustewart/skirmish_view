#pragma once
#include "contentBox.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"

#include <string>

typedef enum { MONEY, ARMIES, AMOUNT_OF_STATS } stats_t;


struct statLine {
	ALLEGRO_BITMAP * bitmap;
	std::string statValue;
};

class playerStats :
	public contentBox
{
public:
	playerStats(ALLEGRO_DISPLAY * display, float startX, float startY, float width, float height);
	~playerStats();
private:
	//TODO: los nombres de los sprites se reciben por constructor o hay un define y chau picho?

	statLine myStats[AMOUNT_OF_STATS];
	statLine theirStats[AMOUNT_OF_STATS];
	
	void drawStat(statLine stat, unsigned int x, unsigned int y);

	ALLEGRO_BITMAP * myAvatar;
	ALLEGRO_BITMAP * theirAvatar;

	ALLEGRO_FONT * font;

	unsigned int lineHeight;

	virtual void drawContent();
	virtual void resizeContent();
};

