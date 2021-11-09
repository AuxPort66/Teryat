#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Spring.h"
#include "Game.h"


void Spring::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int hab) {
	pressed = false;
	room = hab;

	spritesheet.loadFromFile("images/Spring.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 8); //NO_PRESSED
	sprite->addKeyframe(0, glm::vec2(0, 0));

	sprite->setAnimationSpeed(1, 8); //PRESSED
	sprite->addKeyframe(1, glm::vec2(0.5, 0));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpring.x), float(tileMapDispl.y + posSpring.y)));
}

void Spring::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Spring::render() {
	sprite->render(false);
}

void Spring::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Spring::setPosition(const glm::vec2& pos) {
	posSpring = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpring.x), float(tileMapDispl.y + posSpring.y)));
}

glm::ivec2 Spring::getPosition() {
	return posSpring;
}

void Spring::setPressed(bool t) {
	pressed = t;
	if (pressed) {
		if (sprite->animation() != 1) sprite->changeAnimation(1);
		cdpressed = -1;
	}
	else
		if (sprite->animation() != 0) sprite->changeAnimation(0);
}

void Spring::setCdpressed(int t) {
	if (cdpressed < 0) cdpressed = t;

	if (cdpressed > 0)
		cdpressed--;
	else if (cdpressed == 0)
		setPressed(false);
}

bool Spring::isPressed() {
	return pressed;
}

int Spring::getRoom() {
	return room;
}