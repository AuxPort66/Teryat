#include <GL/glew.h>
#include <GL/glut.h>
#include "Checkpoint.h"
#include "Game.h"


#define I_TILE 0.5
#define J_TILE 0.5

enum CheckAnims
{
	LEFT_HEAD,LEFT_NO_HEAD,RIGHT_HEAD,RIGHT_NO_HEAD
};

void Checkpoint::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool mode, int r) {
	active = false;
	room = r;

	spritesheet.loadFromFile("images/Checkpointsheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(J_TILE, I_TILE), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

		sprite->setAnimationSpeed(RIGHT_HEAD, 1);
		sprite->addKeyframe(RIGHT_HEAD, glm::vec2(0 * J_TILE, 0 * I_TILE));

		sprite->setAnimationSpeed(RIGHT_NO_HEAD, 1);
		sprite->addKeyframe(RIGHT_NO_HEAD, glm::vec2(1 * J_TILE, 0 * I_TILE));

		sprite->setAnimationSpeed(LEFT_HEAD, 1);
		sprite->addKeyframe(LEFT_HEAD, glm::vec2(0 * J_TILE, 1 * I_TILE));

		sprite->setAnimationSpeed(LEFT_NO_HEAD, 1);
		sprite->addKeyframe(LEFT_NO_HEAD, glm::vec2(1 * J_TILE, 1 * I_TILE));

	if (mode) sprite->changeAnimation(0);
	else sprite->changeAnimation(2);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCheckpoint.x), float(tileMapDispl.y + posCheckpoint.y)));
};

void Checkpoint::render() {
	sprite->render(false);
}

void Checkpoint::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Checkpoint::setPosition(const glm::vec2& pos) {
	posCheckpoint = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCheckpoint.x), float(tileMapDispl.y + posCheckpoint.y)));
}

glm::ivec2 Checkpoint::getPosition() {
	return posCheckpoint;
}

bool Checkpoint::isActive() {
	return active;
}

void Checkpoint::Active() {
	if (!active) 
		Game::instance().scene.soundeffect("songs/Halftonesfx/3. Movement/Jump_20.wav"); //SFX only in the first activation

	active = true;
	if (sprite->animation() == LEFT_HEAD) sprite->changeAnimation(LEFT_NO_HEAD);
	else if (sprite->animation() == RIGHT_HEAD) sprite->changeAnimation(RIGHT_NO_HEAD);
	render();
}

int Checkpoint::getRoom() {
	return room;
}