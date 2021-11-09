#ifndef _CHECKPOINT_INCLUDE
#define _CHECKPOINT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Checkpoint
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool mode, int room);
	void render();

	void setTileMap(TileMap *tileMap);

	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();

	bool isActive();
	void Active();

	int getRoom();

private:
	glm::ivec2 tileMapDispl;
	TileMap *map;
	Texture spritesheet;
	Sprite *sprite;
	
	glm::ivec2 posCheckpoint;

	bool active;

	int room;
};

#endif // _CHECKPOINT_INCLUDE