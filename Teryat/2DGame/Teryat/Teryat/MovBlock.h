#ifndef _MOVBLOCK_INCLUDE
#define _MOVBLOCK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class MovBlock {
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram,bool horizontal);
	void update(int deltaTime);
	void render();

	void Movement();

	void setTileMap(TileMap *tileMap);

	void setPosition(const glm::vec2 &pos, const glm::vec2 &posf);
	glm::ivec2 getPosition();

	bool leftcoll, rightcoll, downcoll, upcoll;
	bool leftcollhead, rightcollhead, downcollhead, upcollhead;
	glm::ivec2 movtoplayer, movtohead;

private:
	glm::ivec2 tileMapDispl;
	TileMap *map;
	Texture spritesheet;
	Sprite *sprite;

	glm::ivec2 posBlocks, posfinal, posinit;

	bool horizontal;
	bool ir;
};
#endif // _MOVBLOCK_INCLUDE

