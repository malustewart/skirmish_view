#pragma once

#include "Gif.h"
#include "EDAlist.h"
#include "Tile.h"

class Folder
{
public:
	Folder();
	~Folder();

	void draw();

private:
	EDAlist<Tile> tiles;
	gif selectAnimation;

};

