#include "Cabeza.h"
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SIZEX 24
#define SIZEY 24
#define SPEED 2
#define I_TILE 0.5
#define J_TILE 0.125

enum CabezaAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Cabeza::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram) {
	movofblock = glm::ivec2(0, 0);
	cdstepsfx = 0;
	bJumping = active = false;
	rightmovblockhead = leftmovblockhead = downmovblockhead = upmovblockhead = false;
	damage = false;
	checkstepsfx = 0;

	spritesheet.loadFromFile("images/headsheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(J_TILE, I_TILE), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));

		sprite->setAnimationSpeed(MOVE_LEFT, 7);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * J_TILE, 1 * I_TILE));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(6 * J_TILE, 1 * I_TILE));

		sprite->setAnimationSpeed(MOVE_RIGHT, 7);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 * J_TILE, 0 * I_TILE));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 * J_TILE, 0 * I_TILE));

	sprite->changeAnimation(STAND_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHead.x), float(tileMapDispl.y + posHead.y)));
}

void Cabeza::update(int deltaTime) {
	if (active) {
		sprite->update(deltaTime);

		Movement();
		Jumping();
		Soundfx();
		Checkmovblocks();

		movofblock = glm::ivec2(0, 0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHead.x), float(tileMapDispl.y + posHead.y)));
	}
	rightmovblockhead = leftmovblockhead = downmovblockhead = upmovblockhead = false;
}

void Cabeza::Movement() {
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		if (!map->collisionMoveLeft(posHead, glm::ivec2(SIZEX, SIZEY)) && !leftmovblockhead) {
			posHead.x -= SPEED;
		}
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		if (!map->collisionMoveRight(posHead, glm::ivec2(SIZEX, SIZEY)) && !rightmovblockhead) {
			posHead.x += SPEED;
		}
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	}
	else {
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}
}

void Cabeza::Jumping() {
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posHead.y = startY;
		}
		else
		{
			if (downmovblockhead) {
				bJumping = false; posHead.y += 3;
			}
			else posHead.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posHead, glm::ivec2(SIZEX, SIZEY), &posHead.y, &posHead.x, false) == 0 && !downmovblockhead) bJumping = true;
				else bJumping = false;
			}
			else if (map->collisionMoveUp(posHead, glm::ivec2(SIZEX, SIZEY), &posHead.y) || upmovblockhead) {
				jumpAngle = 180 - jumpAngle;
			}
		}
	}
	else if (!downmovblockhead)
	{
		posHead.y += FALL_STEP;
		checkstepsfx = map->collisionMoveDown(posHead, glm::ivec2(SIZEX, SIZEY), &posHead.y, &posHead.x, false);
	}
}

void Cabeza::Checkmovblocks() {
	if (movofblock.x != 0) {
		if (movofblock.x < 0) {
			if (map->collisionMoveLeft(posHead, glm::ivec2(SIZEX, SIZEY)) || leftmovblockhead)
				damage = true;
			else
				posHead.x += movofblock.x;
		}
		else {
			if (map->collisionMoveRight(posHead, glm::ivec2(SIZEX, SIZEY)) || rightmovblockhead)
				damage = true;
			else
				posHead.x += movofblock.x;
		}
	}
}

void Cabeza::Soundfx() {
	bool PlayerisMoving = (sprite->animation() == MOVE_RIGHT || sprite->animation() == MOVE_LEFT);

	if (cdstepsfx == 0 && PlayerisMoving && !bJumping && !downmovblockhead && checkstepsfx != 0) {
		Game::instance().scene.soundeffect("songs/Impactaudio/footstep_grass_004.ogg");
		cdstepsfx = 16;
	}
	else if (cdstepsfx > 0) cdstepsfx--;
}

void Cabeza::render() {
	sprite->render(true);
}

void Cabeza::setTileMap(TileMap * tileMap) {
	map = tileMap;
}

void Cabeza::setActive(bool a) {
	active = a;
	if (active) {
		bJumping = true;
		jumpAngle = 0;
		startY = posHead.y;
	}
}

bool Cabeza::isActive() {
	return active;
}

void Cabeza::setPosition(const glm::vec2 & pos) {
	posHead = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHead.x), float(tileMapDispl.y + posHead.y)));
}

glm::ivec2 Cabeza::getPosition() {
	return posHead;
}

glm::vec2 Cabeza::getSize() {
	return glm::vec2(SIZEX, SIZEY);
}

void Cabeza::setDamage(bool d) {
	damage = d;
}

bool Cabeza::isDamage() {
	return damage;
}
