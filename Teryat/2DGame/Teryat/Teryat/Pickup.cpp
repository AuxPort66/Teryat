
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Pickup.h"
#include "Game.h"

void Pickup::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int mode) {
	taked = false;
	secure = false;
	spritesheet.loadFromFile("images/Miscelaneasheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 8); //KEY
	sprite->addKeyframe(0, glm::vec2(0, 0.5));

	sprite->setAnimationSpeed(1,2); //GEM
	sprite->addKeyframe(1, glm::vec2(0.5, 0));
	sprite->addKeyframe(1, glm::vec2(0, 0));

	sprite->changeAnimation(mode);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPickup.x), float(tileMapDispl.y + posPickup.y)));
}

void Pickup::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Pickup::render() {
	sprite->render(false);
}

void Pickup::setTileMap(TileMap * tileMap) {
	map = tileMap;
}

void Pickup::setPosition(const glm::vec2 & pos) {
	posPickup = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPickup.x), float(tileMapDispl.y + posPickup.y)));
}

glm::ivec2 Pickup::getPosition() {
	return posPickup;
}

bool Pickup::istaked() {
	return taked;
}

void Pickup::setTaked(bool t) {
	taked = t;
}
