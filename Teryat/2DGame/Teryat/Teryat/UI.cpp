#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "UI.h"
#include "Game.h"

#define CD_ANIM 15
#define LIMIT_VOLUME_ANIMFRAME 10

void UI::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int mode)
{
	anim = 0;
	cdanim = 0;
	vol = 10;
	type = mode;

	switch (mode) {
	case 0:
		active = true;
		spritesheet.loadFromFile("images/UIwithoutlifes.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2(1, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1, 1));
		break;
	case 1:
		active = false;
		spritesheet.loadFromFile("images/UItilesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.333), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.5, 0));
		break;
	case 2:
		active = false;
		spritesheet.loadFromFile("images/UItilesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.333), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.5, 0.333));
		break;
	case 3:
		active = true;
		spritesheet.loadFromFile("images/Life1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 128), glm::vec2((float)1/6, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(7);

		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2((float)5/6, 0.f));
		sprite->setAnimationSpeed(1, 1);
		sprite->addKeyframe(1, glm::vec2((float)5/6, 0.f));
		sprite->setAnimationSpeed(2, 1);
		sprite->addKeyframe(2, glm::vec2((float)4/6, 0.f));
		sprite->setAnimationSpeed(3, 1);
		sprite->addKeyframe(3, glm::vec2((float)3/6, 0.f));
		sprite->setAnimationSpeed(4, 1);
		sprite->addKeyframe(4, glm::vec2((float)2/6, 0.f));
		sprite->setAnimationSpeed(5, 1);
		sprite->addKeyframe(5, glm::vec2((float)1/6, 0.f));
		sprite->setAnimationSpeed(6, 1);
		sprite->addKeyframe(6, glm::vec2(0.f, 0.f));
		break;
	case 4:
		active = true;
		spritesheet.loadFromFile("images/Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2((float)1/7,(float)1/6), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 10);

		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)1/7, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)2/7, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)3/7, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)4/7, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)5/7, 0.f));
		sprite->addKeyframe(0, glm::vec2((float)6/7, 0.f));
		
		sprite->addKeyframe(0, glm::vec2(0.f, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)1/7, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)2/7, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)3/7, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)4/7, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)5/7, (float)1/6));
		sprite->addKeyframe(0, glm::vec2((float)6/7, (float)1/6));
		
		sprite->addKeyframe(0, glm::vec2(0.f, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)1/7, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)2/7, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)3/7, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)4/7, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)5/7, (float)2/6));
		sprite->addKeyframe(0, glm::vec2((float)6/7, (float)2/6));

		sprite->addKeyframe(0, glm::vec2(0.f, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)1/7, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)2/7, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)3/7, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)4/7, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)5/7, (float)3/6));
		sprite->addKeyframe(0, glm::vec2((float)6/7, (float)3/6));

		sprite->addKeyframe(0, glm::vec2(0.f, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)1/7, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)2/7, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)3/7, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)4/7, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)5/7, (float)4/6));
		sprite->addKeyframe(0, glm::vec2((float)6/7, (float)4/6));

		sprite->addKeyframe(0, glm::vec2(0.f, (float)5/6));
		sprite->addKeyframe(0, glm::vec2((float)1/7, (float)5/6));
		sprite->addKeyframe(0, glm::vec2((float)2/7, (float)5/6));
		sprite->addKeyframe(0, glm::vec2((float)3/7, (float)5/6));
		sprite->addKeyframe(0, glm::vec2((float)4/7, (float)5/6));
		break;
	case 5:
		active = true;
		spritesheet.loadFromFile("images/menubutt.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2((float)1 / 3,(float) 1/2), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(3);
		sprite->setAnimationSpeed(0, 3);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.f, (float)1/2));
		sprite->setAnimationSpeed(1, 3);
		sprite->addKeyframe(1, glm::vec2((float)1 / 3, 0.f));
		sprite->addKeyframe(1, glm::vec2((float)1 / 3, (float)1 / 2));
		sprite->setAnimationSpeed(2, 3);
		sprite->addKeyframe(2, glm::vec2((float)2 / 3, 0.f));
		sprite->addKeyframe(2, glm::vec2((float)2 / 3, (float)1 / 2));
		break;

	case 6:
		active = true;
		spritesheet.loadFromFile("images/optionsbutt.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2((float)1 / 22, (float)1 / 2), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(22);

		sprite->setAnimationSpeed(0, 3);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.f, (float)1 / 2));
		sprite->setAnimationSpeed(1, 3);
		sprite->addKeyframe(1, glm::vec2((float)1 / 22, 0.f));
		sprite->addKeyframe(1, glm::vec2((float)1 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(2, 3);
		sprite->addKeyframe(2, glm::vec2((float)2 / 22, 0.f));
		sprite->addKeyframe(2, glm::vec2((float)2 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(3, 3);
		sprite->addKeyframe(3, glm::vec2((float)3 / 22, 0.f));
		sprite->addKeyframe(3, glm::vec2((float)3 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(4, 3);
		sprite->addKeyframe(4, glm::vec2((float)4 / 22, 0.f));
		sprite->addKeyframe(4, glm::vec2((float)4 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(5, 3);
		sprite->addKeyframe(5, glm::vec2((float)5 / 22, 0.f));
		sprite->addKeyframe(5, glm::vec2((float)5 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(6, 3);
		sprite->addKeyframe(6, glm::vec2((float)6 / 22, 0.f));
		sprite->addKeyframe(6, glm::vec2((float)6 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(7, 3);
		sprite->addKeyframe(7, glm::vec2((float)7 / 22, 0.f));
		sprite->addKeyframe(7, glm::vec2((float)7 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(8, 3);
		sprite->addKeyframe(8, glm::vec2((float)8 / 22, 0.f));
		sprite->addKeyframe(8, glm::vec2((float)8 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(9, 3);
		sprite->addKeyframe(9, glm::vec2((float)9 / 22, 0.f));
		sprite->addKeyframe(9, glm::vec2((float)9 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(10, 3);
		sprite->addKeyframe(10, glm::vec2((float)10 / 22, 0.f));
		sprite->addKeyframe(10, glm::vec2((float)10 / 22, (float)1 / 2));

		sprite->setAnimationSpeed(11, 3);
		sprite->addKeyframe(11, glm::vec2((float)11 / 22, 0.f));
		sprite->addKeyframe(11, glm::vec2((float)11 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(12, 3);
		sprite->addKeyframe(12, glm::vec2((float)12 / 22, 0.f));
		sprite->addKeyframe(12, glm::vec2((float)12 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(13, 3);
		sprite->addKeyframe(13, glm::vec2((float)13 / 22, 0.f));
		sprite->addKeyframe(13, glm::vec2((float)13 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(14, 3);
		sprite->addKeyframe(14, glm::vec2((float)14 / 22, 0.f));
		sprite->addKeyframe(14, glm::vec2((float)14 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(15, 3);
		sprite->addKeyframe(15, glm::vec2((float)15 / 22, 0.f));
		sprite->addKeyframe(15, glm::vec2((float)15 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(16, 3);
		sprite->addKeyframe(16, glm::vec2((float)16 / 22, 0.f));
		sprite->addKeyframe(16, glm::vec2((float)16 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(17, 3);
		sprite->addKeyframe(17, glm::vec2((float)17 / 22, 0.f));
		sprite->addKeyframe(17, glm::vec2((float)17 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(18, 3);
		sprite->addKeyframe(18, glm::vec2((float)18 / 22, 0.f));
		sprite->addKeyframe(18, glm::vec2((float)18 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(19, 3);
		sprite->addKeyframe(19, glm::vec2((float)19 / 22, 0.f));
		sprite->addKeyframe(19, glm::vec2((float)19 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(20, 3);
		sprite->addKeyframe(20, glm::vec2((float)20 / 22, 0.f));
		sprite->addKeyframe(20, glm::vec2((float)20 / 22, (float)1 / 2));
		sprite->setAnimationSpeed(21, 3);
		sprite->addKeyframe(21, glm::vec2((float)21 / 22, 0.f));
		sprite->addKeyframe(21, glm::vec2((float)21 / 22, (float)1 / 2));

		break;

	case 7: 
		active = true;
		spritesheet.loadFromFile("images/endgame.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(576, 576), glm::vec2(1, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0, 0));
		break;
	}
	
	
	if (mode == 6) {
		sprite->changeAnimation(10);
		anim = 10;
	}
	else sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posUI.x), float(tileMapDispl.y + posUI.y)));
}

void UI::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (type == 5 && active) { //Initial Menu
		if (cdanim == 0) {
			if ((Game::instance().getKey('W') || Game::instance().getKey('w') || Game::instance().getSpecialKey(GLUT_KEY_UP)) && anim > 0) {
				anim--;
				sprite->changeAnimation(anim);
				cdanim = CD_ANIM;
			}
			else if ((Game::instance().getKey('S') || Game::instance().getKey('s') || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) && anim < 2) {
				anim++;
				sprite->changeAnimation(anim);
				cdanim = CD_ANIM;
			}
		}
		else cdanim--;

		if (Game::instance().getKey(' ') && Game::instance().scene.enter) {
			switch (anim)
			{
			case 0:
				Game::instance().changescene(2);
				break;
			case 1: 
				Game::instance().changescene(1);
				break;
			case 2:
				exit(1);
				break;
			}
		}
	}
	else if (type == 6 && active) {

		if (cdanim == 0) {
			if ((Game::instance().getKey('W') || Game::instance().getKey('w') || Game::instance().getSpecialKey(GLUT_KEY_UP)) && anim > 10) {
				anim = vol;
				sprite->changeAnimation(anim);
				cdanim = CD_ANIM;
			}
			else if ((Game::instance().getKey('S') || Game::instance().getKey('s') || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) && anim < 11) {
				anim = 11 + vol;
				sprite->changeAnimation(anim);
				cdanim = CD_ANIM;
			}
			else if(anim < LIMIT_VOLUME_ANIMFRAME && (Game::instance().getKey('D') || Game::instance().getKey('d') || Game::instance().getSpecialKey(GLUT_KEY_RIGHT)))
			{
				vol++;
				anim = vol;
				sprite->changeAnimation(vol);
				Game::instance().scene.volumeengine(true);
				cdanim = CD_ANIM;
			}
			else if (anim > 0 && anim <= LIMIT_VOLUME_ANIMFRAME && (Game::instance().getKey('A') || Game::instance().getKey('a') || Game::instance().getSpecialKey(GLUT_KEY_LEFT))) {
				vol--;
				anim = vol;
				sprite->changeAnimation(vol);
				Game::instance().scene.volumeengine(false);
				cdanim = CD_ANIM;
			}
		}
		else cdanim--;

		if (Game::instance().getKey(' ') && anim >= 11 && Game::instance().scene.enter) {
			anim = vol;
			Game::instance().changescene(0);
			sprite->changeAnimation(vol);
		}
	}
}

void UI::changeAnimation(int anim){
	if(sprite->animation() != anim) sprite->changeAnimation(anim);
}

void UI::render() {
	if(active) sprite->render(false);
}

void UI::setTileMap(TileMap * tileMap) {
	map = tileMap;
}

void UI::setPosition(const glm::vec2 & pos) {
	posUI = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posUI.x), float(tileMapDispl.y + posUI.y)));
}

bool UI::isActive() {
	return active;
}

void UI::Active(bool a) {
	active = a;
}