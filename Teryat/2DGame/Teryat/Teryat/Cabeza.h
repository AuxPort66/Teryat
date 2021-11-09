#ifndef _CABEZA_INCLUDE
#define _CABEZA_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

#pragma once
class Cabeza
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void Movement();
	void Jumping();
	void Checkmovblocks();
	void Soundfx();
	void render();

	void setTileMap(TileMap *tileMap);
	void setActive(bool a);
	void setPosition(const glm::vec2 &pos);


	glm::ivec2 getPosition();
	bool isActive();

	glm::vec2 getSize();

	void setDamage(bool d);
	bool isDamage();

	bool downmovblockhead, leftmovblockhead, rightmovblockhead, upmovblockhead;
	glm::ivec2 movofblock;

private:
	glm::ivec2 tileMapDispl;
	TileMap *map;
	Texture spritesheet;
	Sprite *sprite;

	glm::ivec2 posHead;
	bool bJumping;
	int jumpAngle, startY, countjump;
	
	bool active;

	bool damage;

	int cdstepsfx; //cooldown for steps sfx sound
	int checkstepsfx; //Auxiliary variable to avoid activating the sfx sound in the air
};

#endif // _CABEZA_INCLUDE