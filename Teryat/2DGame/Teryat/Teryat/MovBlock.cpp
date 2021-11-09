#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MovBlock.h"

#define SIZEX 32
#define SIZEY 32
#define I_TILE 1
#define J_TILE 1
#define SPEED 2

void MovBlock::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, bool h) {
	horizontal = h;

	spritesheet.loadFromFile("images/movblocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 32), glm::vec2(J_TILE, I_TILE), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2(0 * J_TILE, 0 * I_TILE));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlocks.x), float(tileMapDispl.y + posBlocks.y)));
}

void MovBlock::update(int deltaTime) {
	movtoplayer.x = 0;
	movtoplayer.y = 0;
	movtohead.x = 0;
	movtohead.y = 0;

	Movement();

	rightcoll = leftcoll = downcoll = upcoll = false;
	rightcollhead = leftcollhead = downcollhead = upcollhead = false;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlocks.x), float(tileMapDispl.y + posBlocks.y)));
}

void MovBlock::Movement() {
	glm::vec2 posdestin;
	if (ir) posdestin = posfinal;
	else posdestin = posinit;

	if (horizontal) {
		if (posBlocks.x - posdestin.x > 0.05) {
			posBlocks.x -= SPEED;
			if (leftcoll || downcoll) movtoplayer.x -= SPEED;
			if (leftcollhead || downcollhead) movtohead.x -= SPEED;
		}
		else if (posBlocks.x - posdestin.x < -0.05) {
			posBlocks.x += SPEED;
			if (rightcoll || downcoll) movtoplayer.x += SPEED;
			if (rightcollhead || downcollhead) movtohead.x += SPEED;
		}
		else {
			posBlocks = posdestin;
			ir = !ir;
		}
	}
	else {
		if (posBlocks.y - posdestin.y > 0.05) {
			posBlocks.y -= SPEED;
			if (downcoll) movtoplayer.y -= SPEED;
			if (downcollhead) movtohead.y -= SPEED;
		}
		else if (posBlocks.y - posdestin.y < -0.05) {
			posBlocks.y += SPEED;
			if (downcoll) movtoplayer.y += SPEED;
			if (downcollhead) movtohead.y += SPEED;
		}
		else {
			posBlocks = posdestin;
			ir = !ir;
		}
	}
}

void MovBlock::render() {
	sprite->render(false);
}

void MovBlock::setTileMap(TileMap * tileMap) {
	map = tileMap;
}

void MovBlock::setPosition(const glm::vec2 & pos, const glm::vec2 & posf) {
	posBlocks = pos;
	posinit = pos;
	posfinal = posf;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlocks.x), float(tileMapDispl.y + posBlocks.y)));
	ir = true;
}

glm::ivec2 MovBlock::getPosition() {
	return posBlocks;
}

