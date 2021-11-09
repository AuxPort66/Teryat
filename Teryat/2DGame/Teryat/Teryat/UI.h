#ifndef _UI_INCLUDE
#define _UI_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class UI
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int mode);
	void update(int deltaTime);
	void render();

	void changeAnimation(int anim);

	void setTileMap(TileMap *tileMap);

	void setPosition(const glm::vec2 &pos);

	bool isActive();
	void Active(bool a);

private:
	glm::ivec2 tileMapDispl, posUI;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	int type;
	int anim, cdanim;
	int vol;
	
	bool active;
};
#endif // _UI_INCLUDE