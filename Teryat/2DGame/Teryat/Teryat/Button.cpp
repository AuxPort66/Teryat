#include "Button.h"


void Button::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, vector<Changeblocks*> arraychanges) {
	pressed = false;
	cdpressed = -1;
	changedblocks = arraychanges;
	spritesheet.loadFromFile("images/Button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 8); //NO_PRESSED
	sprite->addKeyframe(0, glm::vec2(0, 0));

	sprite->setAnimationSpeed(1, 8); //PRESSED
	sprite->addKeyframe(1, glm::vec2(0.5, 0));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posButton.x), float(tileMapDispl.y + posButton.y)));
}

void Button::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Button::render() {
	sprite->render(false);
}

void Button::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Button::setPosition(const glm::vec2& pos) {
	posButton = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posButton.x), float(tileMapDispl.y + posButton.y)));

}

glm::ivec2 Button::getPosition() {
	return posButton;
}

void Button::setPressed(bool t) {
	pressed = t;
	if (pressed) {
		if (sprite->animation() != 1) 
			sprite->changeAnimation(1);
		cdpressed = -1;
		ActiveChangeblocks();
	}
	else {
		if (sprite->animation() != 0)
			sprite->changeAnimation(0);
		ActiveChangeblocks();
	}
}

void Button::ActiveChangeblocks() {
	for (int i = 0; i < changedblocks.size(); i++) {
		int blocktype;
		if (pressed) 
			blocktype = changedblocks[i]->blockactiv;
		else 
			blocktype = changedblocks[i]->blocknoactiv;
		map->changeTile(changedblocks[i]->coord, blocktype);
	}
}

void Button::setCdpressed(int t) {
	if(cdpressed < 0) cdpressed = t;

	if (cdpressed > 0) cdpressed--;
	else if (cdpressed == 0) setPressed(false);
}

bool Button::isPressed() {
	return pressed;
}

int Button::getAnimation() {
	return sprite->animation();
}