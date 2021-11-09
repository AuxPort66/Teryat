#ifndef _SPRING_INCLUDE
#define _SPRING_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Spring
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram,int hab);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);

	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
	int getRoom();

	void setPressed(bool t);
	void setCdpressed(int t);
	bool isPressed();

private:
	glm::ivec2 tileMapDispl;
	TileMap* map;
	Texture spritesheet;
	Sprite* sprite;

	glm::ivec2 posSpring;

	bool pressed;
	int cdpressed;

	int room;
};

#endif //_SPRING_INCLUDE
