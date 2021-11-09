#ifndef  _PAUSE_INCLUDE
#define  _PAUSE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class pause {

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void render();

	void setActive(glm::ivec2 pos);

private:
	Texture spritesheet;
	Sprite* sprite;
};
#endif //  _PAUSE_INCLUDE