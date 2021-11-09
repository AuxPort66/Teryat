#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Changeblocks.h"

class Button {

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, vector<Changeblocks*> arraychanges);
	void update(int deltaTime);
	void render();

	void ActiveChangeblocks();

	void setTileMap(TileMap* tileMap);

	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();

	void setPressed(bool t);
	bool isPressed();
	void setCdpressed(int t);

	int getAnimation();
	
private:
	glm::ivec2 tileMapDispl;
	TileMap* map;
	Texture spritesheet;
	Sprite* sprite;
	
	glm::ivec2 posButton;

	vector<Changeblocks*> changedblocks;
	bool pressed;
	int cdpressed;
};
#endif //_BUTTON_INCLUDE
