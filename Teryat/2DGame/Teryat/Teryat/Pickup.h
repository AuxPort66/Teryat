#ifndef _PICKUP_INCLUDE
#define _PICKUP_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "UI.h"

class Pickup {

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int mode);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);

	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();

	void setTaked(bool t);
	bool istaked();

	bool secure;
	UI *pickupui;

private:
	glm::ivec2 tileMapDispl;
	TileMap *map;
	Texture spritesheet;
	Sprite *sprite;

	glm::ivec2 posPickup;

	bool taked;
};
#endif //_PICKUP_INCLUDE