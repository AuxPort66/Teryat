#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SIZEX 32
#define SIZEY 32
#define SPEED 2
#define I_TILE 0.10
#define J_TILE 0.25
#define COUNT_JUMP_INIT 3
#define INVICIBILITY_TIME 20

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, IDLE_LEFT, IDLE_RIGHT, LARGE_IDLE_LEFT, LARGE_IDLE_RIGHT,
	STAIR_IDLE, STAIR_MOVE, SIT_LEFT, SIT_RIGHT, SIT_IDLE_LEFT, SIT_IDLE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	invincibility = idletime = 0;
	cdstepsfx = cdstepstairsfx = 0;
	hp = 1;
	bJumping = throwhead = idle = stair = false;
	left = false;
	damage = false;
	jumpreleased = false;
	countjump = COUNT_JUMP_INIT;

	head = new Cabeza();
	head->init(tileMapPos, shaderProgram);
	throwreleased = true;

	movofblock = glm::ivec2(0, 0);
	rightmovblock = leftmovblock = downmovblock = upmovblock = false;

	spritesheetsincabeza.loadFromFile("images/Zombiecharactersheetnohead.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesincabeza = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(J_TILE, I_TILE), &spritesheetsincabeza, &shaderProgram);
	spritesincabeza->setNumberAnimations(17);

		spritesincabeza->setAnimationSpeed(STAND_LEFT, 8);
		spritesincabeza->addKeyframe(STAND_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(STAND_RIGHT, 8);
		spritesincabeza->addKeyframe(STAND_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(MOVE_LEFT, 8);
		spritesincabeza->addKeyframe(MOVE_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_LEFT, glm::vec2(1 * J_TILE, 1 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_LEFT, glm::vec2(2 * J_TILE, 1 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_LEFT, glm::vec2(3 * J_TILE, 1 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(MOVE_RIGHT, 8);
		spritesincabeza->addKeyframe(MOVE_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_RIGHT, glm::vec2(1 * J_TILE, 0 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_RIGHT, glm::vec2(2 * J_TILE, 0 * I_TILE));
		spritesincabeza->addKeyframe(MOVE_RIGHT, glm::vec2(3 * J_TILE, 0 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(JUMP_LEFT, 8);
		spritesincabeza->addKeyframe(JUMP_LEFT, glm::vec2(0 * J_TILE, 8 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(JUMP_RIGHT, 8);
		spritesincabeza->addKeyframe(JUMP_RIGHT, glm::vec2(0 * J_TILE, 7 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(FALL_LEFT, 8);
		spritesincabeza->addKeyframe(FALL_LEFT, glm::vec2(1 * J_TILE, 8 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(FALL_RIGHT, 8);
		spritesincabeza->addKeyframe(FALL_RIGHT, glm::vec2(1 * J_TILE, 7 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(IDLE_LEFT, 10);
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(2 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(IDLE_RIGHT, 10);
		spritesincabeza->addKeyframe(IDLE_RIGHT, glm::vec2(0 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_RIGHT, glm::vec2(1 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_RIGHT, glm::vec2(1 * J_TILE, 2 * I_TILE));
	
		spritesincabeza->setAnimationSpeed(IDLE_LEFT, 10);
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(2 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		spritesincabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));

		spritesincabeza->setAnimationSpeed(LARGE_IDLE_LEFT, 5);
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(2 * J_TILE, 5 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(3 * J_TILE, 5 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(0 * J_TILE, 6 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(2 * J_TILE, 6 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(3 * J_TILE, 6 * I_TILE));

		spritesincabeza->setAnimationSpeed(LARGE_IDLE_RIGHT, 5);
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(2 * J_TILE, 3 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(3 * J_TILE, 3 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(0 * J_TILE, 4 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(2 * J_TILE, 4 * I_TILE));
		spritesincabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(3 * J_TILE, 4 * I_TILE));

		spritesincabeza->setAnimationSpeed(STAIR_IDLE, 1);
		spritesincabeza->addKeyframe(STAIR_IDLE, glm::vec2(0 * J_TILE, 9 * I_TILE));

		spritesincabeza->setAnimationSpeed(STAIR_MOVE, 6);
		spritesincabeza->addKeyframe(STAIR_MOVE, glm::vec2(0 * J_TILE, 9 * I_TILE));
		spritesincabeza->addKeyframe(STAIR_MOVE, glm::vec2(1 * J_TILE, 9 * I_TILE));
		spritesincabeza->addKeyframe(STAIR_MOVE, glm::vec2(2 * J_TILE, 9 * I_TILE));
		spritesincabeza->addKeyframe(STAIR_MOVE, glm::vec2(3 * J_TILE, 9 * I_TILE));

		spritesincabeza->setAnimationSpeed(SIT_RIGHT, 1);
		spritesincabeza->addKeyframe(SIT_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));

		spritesincabeza->setAnimationSpeed(SIT_LEFT, 1);
		spritesincabeza->addKeyframe(SIT_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));

		spritesincabeza->setAnimationSpeed(SIT_IDLE_RIGHT, 10);
		spritesincabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));
		spritesincabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(2 * J_TILE, 3 * I_TILE));
		spritesincabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(3 * J_TILE, 3 * I_TILE));

		spritesincabeza->setAnimationSpeed(SIT_IDLE_LEFT, 10);
		spritesincabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));
		spritesincabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(2 * J_TILE, 5 * I_TILE));
		spritesincabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(3 * J_TILE, 5 * I_TILE));

	spritesheetcabeza.loadFromFile("images/Zombiecharactersheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritecabeza = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(J_TILE, I_TILE), &spritesheetcabeza, &shaderProgram);
	spritecabeza->setNumberAnimations(19);
	
		spritecabeza->setAnimationSpeed(STAND_LEFT, 8);
		spritecabeza->addKeyframe(STAND_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));
		
		spritecabeza->setAnimationSpeed(STAND_RIGHT, 8);
		spritecabeza->addKeyframe(STAND_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));
		
		spritecabeza->setAnimationSpeed(MOVE_LEFT, 8);
		spritecabeza->addKeyframe(MOVE_LEFT, glm::vec2(0 * J_TILE, 1 * I_TILE));
		spritecabeza->addKeyframe(MOVE_LEFT, glm::vec2(1 * J_TILE, 1 * I_TILE));
		spritecabeza->addKeyframe(MOVE_LEFT, glm::vec2(2 * J_TILE, 1 * I_TILE));
		spritecabeza->addKeyframe(MOVE_LEFT, glm::vec2(3 * J_TILE, 1 * I_TILE));
		
		spritecabeza->setAnimationSpeed(MOVE_RIGHT, 8);
		spritecabeza->addKeyframe(MOVE_RIGHT, glm::vec2(0 * J_TILE, 0 * I_TILE));
		spritecabeza->addKeyframe(MOVE_RIGHT, glm::vec2(1 * J_TILE, 0 * I_TILE));
		spritecabeza->addKeyframe(MOVE_RIGHT, glm::vec2(2 * J_TILE, 0 * I_TILE));
		spritecabeza->addKeyframe(MOVE_RIGHT, glm::vec2(3 * J_TILE, 0 * I_TILE));
		
		spritecabeza->setAnimationSpeed(JUMP_LEFT, 8);
		spritecabeza->addKeyframe(JUMP_LEFT, glm::vec2(0 * J_TILE, 8 * I_TILE));
		
		spritecabeza->setAnimationSpeed(JUMP_RIGHT, 8);
		spritecabeza->addKeyframe(JUMP_RIGHT, glm::vec2(0 * J_TILE, 7 * I_TILE));
		
		spritecabeza->setAnimationSpeed(FALL_LEFT, 8);
		spritecabeza->addKeyframe(FALL_LEFT, glm::vec2(1 * J_TILE, 8 * I_TILE));
		
		spritecabeza->setAnimationSpeed(FALL_RIGHT, 8);
		spritecabeza->addKeyframe(FALL_RIGHT, glm::vec2(1 * J_TILE, 7 * I_TILE));
		
		spritecabeza->setAnimationSpeed(IDLE_LEFT, 10);
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(2 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		
		spritecabeza->setAnimationSpeed(IDLE_RIGHT, 10);
		spritecabeza->addKeyframe(IDLE_RIGHT, glm::vec2(0 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_RIGHT, glm::vec2(1 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_RIGHT, glm::vec2(1 * J_TILE, 2 * I_TILE));
		
		spritecabeza->setAnimationSpeed(IDLE_LEFT, 10);
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(2 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		spritecabeza->addKeyframe(IDLE_LEFT, glm::vec2(3 * J_TILE, 2 * I_TILE));
		
		spritecabeza->setAnimationSpeed(LARGE_IDLE_LEFT, 5);
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(2 * J_TILE, 5 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(3 * J_TILE, 5 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(0 * J_TILE, 6 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(1 * J_TILE, 6 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(2 * J_TILE, 6 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_LEFT, glm::vec2(3 * J_TILE, 6 * I_TILE));
		
		spritecabeza->setAnimationSpeed(LARGE_IDLE_RIGHT, 5);
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(2 * J_TILE, 3 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(3 * J_TILE, 3 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(0 * J_TILE, 4 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(1 * J_TILE, 4 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(2 * J_TILE, 4 * I_TILE));
		spritecabeza->addKeyframe(LARGE_IDLE_RIGHT, glm::vec2(3 * J_TILE, 4 * I_TILE));
		
		spritecabeza->setAnimationSpeed(STAIR_IDLE, 1);
		spritecabeza->addKeyframe(STAIR_IDLE, glm::vec2(0 * J_TILE, 9 * I_TILE));
		
		spritecabeza->setAnimationSpeed(STAIR_MOVE, 6);
		spritecabeza->addKeyframe(STAIR_MOVE, glm::vec2(0 * J_TILE, 9 * I_TILE));
		spritecabeza->addKeyframe(STAIR_MOVE, glm::vec2(1 * J_TILE, 9 * I_TILE));
		spritecabeza->addKeyframe(STAIR_MOVE, glm::vec2(2 * J_TILE, 9 * I_TILE));
		spritecabeza->addKeyframe(STAIR_MOVE, glm::vec2(3 * J_TILE, 9 * I_TILE));

		spritecabeza->setAnimationSpeed(SIT_RIGHT, 1);
		spritecabeza->addKeyframe(SIT_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));

		spritecabeza->setAnimationSpeed(SIT_LEFT, 1);
		spritecabeza->addKeyframe(SIT_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));

		spritecabeza->setAnimationSpeed(SIT_IDLE_RIGHT, 10);
		spritecabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(1 * J_TILE, 3 * I_TILE));
		spritecabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(2 * J_TILE, 3 * I_TILE));
		spritecabeza->addKeyframe(SIT_IDLE_RIGHT, glm::vec2(3 * J_TILE, 3 * I_TILE));

		spritecabeza->setAnimationSpeed(SIT_IDLE_LEFT, 10);
		spritecabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(1 * J_TILE, 5 * I_TILE));
		spritecabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(2 * J_TILE, 5 * I_TILE));
		spritecabeza->addKeyframe(SIT_IDLE_LEFT, glm::vec2(3 * J_TILE, 5 * I_TILE));

	sprite = spritecabeza;
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime) {
	if (!stair){
		Throwhead();
		sprite->update(deltaTime);
		Idleupdate();
		if (invincibility > 0) invincibility--;
		Movement();
		if (invincibility == 0 && map->collisionBehind(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y, &posPlayer.x, false) == 1)
			damage = true;
		Jumping();
	}
	else {
		MovementStair(deltaTime);
		JumpingStair();
	}

	jumpreleased = !Game::instance().getKey(' ');
	upreleased = !Game::instance().getKey('W') || Game::instance().getKey('w');
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	Appliedmovblocks();
	movofblock = glm::ivec2(0, 0);
	downmovblock = upmovblock = rightmovblock = leftmovblock = false;

	if (head->isActive()) head->update(deltaTime);
	Soundfx();
}

void Player::Soundfx() {
	if (cdstepsfx == 0 && (sprite->animation() == MOVE_RIGHT || sprite->animation() == MOVE_LEFT)) {
		Game::instance().scene.soundeffect("songs/Impactaudio/footstep_grass_004.ogg");
		cdstepsfx = 16;
	}
	else if (cdstepsfx > 0) cdstepsfx--;
}

void Player::JumpingStair() {
	if (Game::instance().getKey(' ') && jumpreleased) {
		cdstepstairsfx = 0;
		stair = false;
		idlecount = 0;
		jumpreleased = false;

		Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav");//Saltar

		if (left)
			if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
		else if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
		countjump = 0;
		bJumping = true;
		jumpAngle = 0;
		startY = posPlayer.y;
	}
}

void Player::MovementStair(int deltaTime) {

	bool collisionDown = map->collisionMoveDown(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y, &posPlayer.x, false) != 0 || map->collisionDownstair(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y);

	if (!map->collisionUpstair(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y) && (Game::instance().getKey('W') || Game::instance().getKey('w'))) {
		stair = false;
		posPlayer.y -= 26;
		bJumping = false;
		countjump = 0;
		jumpreleased = false;
		if (sprite->animation() != IDLE_RIGHT) sprite->changeAnimation(IDLE_RIGHT);
		falling = false;
		cdstepstairsfx = 0;
	}
	else if ((collisionDown || downmovblock) && (Game::instance().getKey('S') || Game::instance().getKey('s'))) {
		stair = false;
		bJumping = false;
		countjump = 0;
		jumpreleased = false;
		if (sprite->animation() != IDLE_RIGHT) sprite->changeAnimation(IDLE_RIGHT);
		falling = false;
		cdstepstairsfx = 0;
	}
	else { 
		if (Game::instance().getKey('W') || Game::instance().getKey('w')) {
			if (sprite->animation() != STAIR_MOVE)
				sprite->changeAnimation(STAIR_MOVE);
			posPlayer.y -= SPEED;
			sprite->update(deltaTime);
			falling = false;
			if (cdstepstairsfx == 0) {
				Game::instance().scene.soundeffect("songs/Impactaudio/impactSoft_medium_004.ogg"); //Escalera
				cdstepstairsfx = 16;
			}
			else cdstepstairsfx--;
		}
		else if (Game::instance().getKey('S') || Game::instance().getKey('s')) {
			if (sprite->animation() != STAIR_MOVE) sprite->changeAnimation(STAIR_MOVE);
			posPlayer.y += SPEED;
			falling = true;
			sprite->update(deltaTime);
			if (cdstepstairsfx == 0) {
				Game::instance().scene.soundeffect("songs/Impactaudio/impactSoft_medium_004.ogg"); //Escalera
				cdstepstairsfx = 16;
			}
			else cdstepstairsfx--;
		}
	}
}

void Player::Jumping() {
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180) {
			bJumping = false;
			posPlayer.y = startY;
		}
		else {
			if (downmovblock && (sprite->animation() == FALL_RIGHT || sprite->animation() == FALL_LEFT))
				bJumping = false;
			else posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y, &posPlayer.x, false) == 0 && !downmovblock) 
					bJumping = true;
				else bJumping = false;

				if (bJumping) {
					idlecount = 0;
					if (left) {
						falling = true;
						if (sprite->animation() != FALL_LEFT) sprite->changeAnimation(FALL_LEFT);
					}
					else {
						if (sprite->animation() != FALL_RIGHT) sprite->changeAnimation(FALL_RIGHT);
						falling = true;
					}
				}
			}
			else if (map->collisionMoveUp(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y) || upmovblock) {
				jumpAngle = 180 - jumpAngle;
			}
			if (jumpreleased && Jumphead() && Game::instance().getKey(' ')) {
				jumpreleased = false;
				idlecount = 0;
				Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav"); //Saltar en cabeza
				if (left)
					if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
				else if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
				countjump = 0;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	else {
		int collisionDown = 0;

		if (downmovblock) collisionDown = 1;
		else {
			posPlayer.y += FALL_STEP;
			collisionDown = map->collisionMoveDown(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y, &posPlayer.x, Game::instance().getKey('S') || Game::instance().getKey('s'));
		}

		if (collisionDown != 0) {
			falling = false;
			if (sprite->animation() == FALL_LEFT) {
				sprite->changeAnimation(STAND_LEFT);
				Game::instance().scene.soundeffect("songs/Impactaudio/impactSoft_medium_001.ogg"); //Caer
			}
			else if (sprite->animation() == FALL_RIGHT) {
				sprite->changeAnimation(STAND_RIGHT);
				Game::instance().scene.soundeffect("songs/Impactaudio/impactSoft_medium_001.ogg"); //Caer
			}
			countjump = COUNT_JUMP_INIT;
			if (Game::instance().getKey(' ') && jumpreleased) {
				idlecount = 0;
				jumpreleased = false;
				Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav"); //Saltar
				if (left) {
					if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
				}
				else if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
				countjump = 0;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			else if (collisionDown == 2) {
				stair = true;
				if (sprite->animation() != STAIR_MOVE) sprite->changeAnimation(STAIR_MOVE);
				posPlayer.y += 16;
			}
		}
		else if (countjump > 0) {
			if (Game::instance().getKey(' ') && jumpreleased) {
				jumpreleased = false;
				idlecount = 0;
				Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav");//Saltar
				if (left)
					if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
				else if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
				countjump = 0;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			countjump--;
		}
		else if (jumpreleased && Jumphead() && Game::instance().getKey(' ')) {
			jumpreleased = false;
			idlecount = 0;
			Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav");//Saltar
			if (left)
				if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			countjump = 0;
			bJumping = true;
			jumpAngle = 0;
			startY = posPlayer.y;
		}
		else {
			idlecount = 0;
			falling = true;
			if (left)
				if (sprite->animation() != FALL_LEFT) sprite->changeAnimation(FALL_LEFT);
			else
				if (sprite->animation() != FALL_RIGHT) sprite->changeAnimation(FALL_RIGHT);
		}
	}
}

void Player::Movement() {
	if (Game::instance().getKey('A') || Game::instance().getKey('a')) {
		idlecount = 0;
		left = true;
		if (sprite->animation() != MOVE_LEFT && sprite->animation() != FALL_LEFT && sprite->animation() != JUMP_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(SIZEX, SIZEY)) || leftmovblock)
			sprite->changeAnimation(STAND_LEFT);
		else posPlayer.x -= SPEED;
	}
	else if (Game::instance().getKey('D') || Game::instance().getKey('d')) {
		idlecount = 0;
		left = false;
		if (sprite->animation() != MOVE_RIGHT && sprite->animation() != FALL_RIGHT && sprite->animation() != JUMP_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		if (map->collisionMoveRight(posPlayer, glm::ivec2(SIZEX, SIZEY)) || rightmovblock)
			sprite->changeAnimation(STAND_RIGHT);
		else posPlayer.x += SPEED;
	}
	else {
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (map->collisionBehind(posPlayer, glm::ivec2(SIZEX, SIZEY), &posPlayer.y, &posPlayer.x, ((Game::instance().getKey('W') || Game::instance().getKey('w')) && upreleased)) == 2) {
		stair = true;
		if (sprite->animation() != STAIR_MOVE) sprite->changeAnimation(STAIR_MOVE);
		posPlayer.y -= 16;
		jumpreleased = false;
	}
}

void Player::Idleupdate() {
	int random = rand() % 6 + 4;
	if (sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT) {
		idletime++;
		if (idlecount > 4 && random == 4) {
			idle = false;
			idlecount = 0;
			idletime = 0;
			if (left)
				sprite->changeAnimation(SIT_LEFT);
			else
				sprite->changeAnimation(SIT_RIGHT);
		}
		if (idletime > 100 && random == 4) {
			idletime = 0;
			idlecount++;
			if (left)
				sprite->changeAnimation(IDLE_LEFT);
			else
				sprite->changeAnimation(IDLE_RIGHT);
		}
	}
	else if (sprite->animation() == IDLE_RIGHT || sprite->animation() == IDLE_LEFT) {
		idletime++;
		if (idletime == 22) {
			idletime = 0;
			if (left)
				sprite->changeAnimation(STAND_LEFT);
			else 
				sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (sprite->animation() == SIT_RIGHT || sprite->animation() == SIT_LEFT) {
		idletime++;
		if (idlecount > 4 && random == 4) {
			idle = false;
			idlecount = 0;
			idletime = 0;
			if (left)
				sprite->changeAnimation(LARGE_IDLE_LEFT);
			else 
				sprite->changeAnimation(LARGE_IDLE_RIGHT);
		}
		if (idletime > 100 && random == 4) {
			idletime = 0;
			idlecount++;
			if (left)
				sprite->changeAnimation(SIT_IDLE_LEFT);
			else 
				sprite->changeAnimation(SIT_IDLE_RIGHT);
		}
	}
	else if (sprite->animation() == SIT_IDLE_RIGHT || sprite->animation() == SIT_IDLE_LEFT) {
		idletime++;
		if (idletime == 22) {
			idletime = 0;
			if (left)
				sprite->changeAnimation(SIT_LEFT);
			else
				sprite->changeAnimation(SIT_RIGHT);
		}
	}
	else if (sprite->animation() == LARGE_IDLE_RIGHT || sprite->animation() == LARGE_IDLE_LEFT) {
		idletime++;
		if (idletime == 140) {
			idletime = 0;
			idlecount = 0;
			if (left)
				sprite->changeAnimation(SIT_LEFT);
			else
				sprite->changeAnimation(SIT_RIGHT);
		}
	}
}

void Player::Throwhead() {
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !throwhead && throwreleased) {
		throwhead = true;
		head->setPosition(posPlayer);
		head->setActive(true);
		int aux = sprite->animation();
		sprite = spritesincabeza;
		sprite->changeAnimation(aux);
		idlecount = 0;
		Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Blop_2.wav");
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && throwhead && Jumphead()) {
		newHead();
		idlecount = 0;
		throwreleased = false;
		Game::instance().scene.soundeffect("songs/Halftonesfx/6. Bullets & Powerups/Bullet_6.wav");
	}
	else if (!Game::instance().getSpecialKey(GLUT_KEY_UP)) throwreleased = true;
}

bool Player::damagehead() {
	bool aux = head->isDamage();
	head->setDamage(false);
	return aux;
}

void Player::Appliedmovblocks() {
	if (movofblock.x != 0) {
		if(movofblock.x < 0) {
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(SIZEX, SIZEY)) || leftmovblock) {
				damage = true;
				hp = 1;
			}
			else
				posPlayer.x += movofblock.x;
		}
		else {
			if (map->collisionMoveRight(posPlayer, glm::ivec2(SIZEX, SIZEY)) || rightmovblock) {
				damage = true;
				hp = 1;
			}
			else
				posPlayer.x += movofblock.x;
		}
	}
}

void Player::addMovblockvec(glm::ivec2 movement) {
	movofblock.x += movement.x;
	movofblock.y += movement.y;
}

void Player::addMovblockvechead(glm::ivec2 movement) {
	head->movofblock.x += movement.x;
	head->movofblock.y += movement.y;
}

void Player::render() {
	if (head->isActive()) head->render();
	sprite->render(false);
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
	head->setTileMap(tileMap);
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

bool Player::getLeft() {
	return left;
}

bool Player::getFalling() {
	return falling;
}

bool Player::getStair() {
	return stair;
}

int Player::checkDamage() {
	return damage;
}

void Player::setDamage(bool d) {
	damage = d;
}

int Player::getHp() {
	return hp;
}

void Player::substractHp() {
	hp--;
}

void Player::initInvincibility() {
	invincibility = INVICIBILITY_TIME;
}

bool Player::Jumphead() {
	glm::vec2 headpos = head->getPosition();
	glm::vec2 headsize = head->getSize();

	bool PlayerinHeadAxisX = (posPlayer.x + SIZEX) >= headpos.x && (headpos.x + headsize.x) >= posPlayer.x;
	bool PlayerinHeadAxisY = (posPlayer.y + SIZEY) >= headpos.y && (headpos.y + headsize.y / 2) >= (posPlayer.y + SIZEY / 2);

	if (PlayerinHeadAxisX && PlayerinHeadAxisY) return true;
	return false;
}

void Player::newHead() {
	int aux = sprite->animation();
	sprite = spritecabeza;
	sprite->changeAnimation(aux);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	head->setActive(false);
	throwhead = false;
}

bool Player::isHeadActive() {
	return head->isActive();
}

void Player::setActivehead(bool a) {
	head->setActive(a);
}

glm::ivec2 Player::getHeadPosition() {
	return head->getPosition();
}

bool Player::getCollBlockhead(int pos) {
	
	if (head->isActive()) {
		switch (pos)
		{
			case 0:
				return head->upmovblockhead;
				break;
			case 1:
				return head->downmovblockhead;
				break;
			case 2:
				return head->leftmovblockhead;
				break;
			case 3:
				return head->rightmovblockhead;
				break;
		}
	}
	return true;
}

void Player::setCollBlockhead(int pos, bool coll) {
	switch (pos)
	{
	case 0:
		head->upmovblockhead = coll;
		break;
	case 1:
		head->downmovblockhead = coll;
		break;
	case 2:
		head->leftmovblockhead = coll;
		break;
	case 3:
		head->rightmovblockhead = coll;
		break;
	}
}



