#include "avatarRoundHP.h"

#include <algorithm>
#include <allegro5\allegro_primitives.h>

using namespace std;

avatarRoundHP::avatarRoundHP(ALLEGRO_BITMAP * avatar, unsigned int radius, unsigned int maxHP, ALLEGRO_COLOR scoreColor)
{
}

avatarRoundHP::avatarRoundHP(ALLEGRO_BITMAP * avatar, unsigned int radius, unsigned int maxHP, unsigned int topX, unsigned int leftY, ALLEGRO_COLOR scoreColor)
{
	if (avatar == nullptr)
	{
		valid = false;
		return;
	}

	this->radius = radius;
	offset = (double)radius / 8;	//TODO: magic number
	centerX = topX + radius;
	centerY = leftY + radius;
	this->maxHP = maxHP;
	currentHP = maxHP;
	this->scoreColor = scoreColor;
	transparency = true;

	
	//hacer backup del blender y del target bitmap actuales
	int op, src, dst;
	al_get_blender(&op, &src, &dst);	//TODO: hacer backup sin el separate blender del alpha
	ALLEGRO_BITMAP * targetBmpBackup = al_get_target_bitmap();

	roundAvatar = al_create_bitmap((radius - 3 * offset) * 2, (radius - 3 * offset) * 2);
	al_set_target_bitmap(roundAvatar);

	//Esta configuracion de colores sirve como "mascara" para despues quedarse solo con un circulo de lo que sea que se le dibuje arriba
	al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);	//sirve para que se mantenga la transparencia del circulo dibujado
	al_draw_filled_circle(radius - 3 * offset, radius - 3 * offset, radius - 3 * offset, al_map_rgba_f(0, 0, 0, 0));	//TODO: magic number

	int w = al_get_bitmap_width(avatar), h = al_get_bitmap_height(avatar);
	double scalingFactor = (double)(radius - 3 * offset) * 2 / min(w, h);

	//setear blender para que solo se mantenga la parte del avatar que se va a dibujar sobre el circulo. Lo demas es transparente.
	al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
	//dibujar de manera que entre toda la imagen en el bitmap sin estirarse
	al_draw_scaled_bitmap(avatar, 0, 0, w, h, 0, 0, w * scalingFactor, h * scalingFactor, 0);


	al_set_blender(op, src, dst);
	al_set_target_bitmap(targetBmpBackup);

	//TODO: chequear que con png10 no me anda bien el blending. es demasiado rectangular, y ademas me queda descentrado del irculo
}

avatarRoundHP::~avatarRoundHP()
{
	al_destroy_bitmap(roundAvatar);
}

void avatarRoundHP::draw()
{
	al_draw_filled_circle(centerX, centerY, radius, al_map_rgba_f(0, 0, 0, 0.5));
		
	float currentAngle = 0;
	for (int i = 0; i < currentHP; i++)
	{
		al_draw_arc(centerX, centerY, radius - offset, currentAngle, 2 * ALLEGRO_PI / maxHP - 0.1, scoreColor, offset / 2);	//TODO: magic numbers
		currentAngle += (2 * ALLEGRO_PI / maxHP);
	}
	al_draw_filled_circle(centerX, centerY, radius - 2 * offset, al_map_rgba_f(0.5, 0.5, 0.5, 0.5));
	al_draw_bitmap(roundAvatar, centerX - (radius - 3 * offset), centerY - (radius - 3 * offset), 0);	//TODO: magic numbers
}

void avatarRoundHP::setCurrentHP(int currentHP)
{
	this->currentHP = currentHP;
}

void avatarRoundHP::changeHPby(signed int HPchange)
{
	currentHP += HPchange;
	if (currentHP > maxHP)
	{
		currentHP = maxHP;
	}
	else if(currentHP < 0)
	{
		currentHP = 0;
	}
}

void avatarRoundHP::setPosition(unsigned int topX, unsigned int leftY)
{
	centerX = topX + radius;
	centerY = leftY + radius;
}

ALLEGRO_COLOR avatarRoundHP::getScoreColor()
{
	return ALLEGRO_COLOR();
}

bool avatarRoundHP::isValid()
{
	return valid;
}

int avatarRoundHP::getCurrentHP()
{
	return 0;
}

void avatarRoundHP::setScoreColor(ALLEGRO_COLOR scoreColor)
{
}

bool avatarRoundHP::getTransparency()
{
	return transparency;
}

void avatarRoundHP::setTransparency(bool transparency)
{
	this->transparency = transparency;
}