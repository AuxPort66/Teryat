#include "pause.h"

void pause::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2(0, 0));
	sprite->changeAnimation(0);
	sprite->setPosition(tileMapPos);
}

void pause::setActive(glm::ivec2 pos) {
	sprite->setPosition(pos);
}

void pause::render() {
	sprite->render(false);
}