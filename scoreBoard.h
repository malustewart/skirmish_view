#pragma once

#include "avatarRoundHP.h"

class scoreBoard
{
public:
	scoreBoard(ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * myAvatar, ALLEGRO_BITMAP * theirAvatar, unsigned int maxHP, ALLEGRO_COLOR scoreColor);
	
	//max height es width/4 para los dos constructores.
	scoreBoard(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, ALLEGRO_BITMAP * myAvatar, ALLEGRO_BITMAP * theirAvatar, unsigned int maxHP, ALLEGRO_COLOR scoreColor);

	~scoreBoard();
private:
	avatarRoundHP * myHP;
	avatarRoundHP * theirHP;
};

