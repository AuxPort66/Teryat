#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Cabeza.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player {

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void MovementStair(int deltaTime);
	void JumpingStair();

	void Movement();
	void Jumping();
	void Idleupdate();
	void Soundfx();

	bool Jumphead();
	void Throwhead();
	bool damagehead();

	void Appliedmovblocks();
	void addMovblockvec(glm::ivec2 movement);
	void addMovblockvechead(glm::ivec2 movement);

	void initInvincibility();

	void setTileMap(TileMap *tileMap);

	glm::ivec2 getPosition();
	void setPosition(const glm::vec2 &pos);
	bool getLeft();
	bool getFalling();
	bool getStair();

	int checkDamage();
	void setDamage(bool d);
	int getHp();
	void substractHp();

	void newHead();
	bool isHeadActive();
	void setActivehead(bool a);
	glm::ivec2 getHeadPosition();

	bool getCollBlockhead(int pos);
	void setCollBlockhead(int pos, bool coll);
	
	bool downmovblock, leftmovblock, rightmovblock, upmovblock;
	glm::ivec2 movofblock;
	glm::ivec2 posPlayer;
	Cabeza *head;

private:
	glm::ivec2 tileMapDispl;
	TileMap *map;
	Texture spritesheetcabeza, spritesheetsincabeza;
	Sprite *sprite, *spritecabeza, *spritesincabeza;

	bool left;
	bool falling;
	bool stair;

	bool idle;
	int idletime, idlecount;

	bool throwhead, throwreleased;

	bool bJumping, jumpreleased, upreleased;
	int jumpAngle, startY, countjump;

	bool damage; //If you recieve damage in that frame
	int hp;
	int invincibility;

	int cdstepsfx;
	int cdstepstairsfx;
};
#endif // _PLAYER_INCLUDE


