#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <array>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Cabeza.h"
#include "UI.h"
#include "Pickup.h"
#include "Checkpoint.h"
#include "pause.h"
#include "Button.h"
#include "Spring.h"
#include "MovBlock.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#include <irrKlang.h>
#include <conio.h>
#include <stdio.h>

using namespace irrklang;


class Scene
{

public:
	Scene();
	~Scene();

	static Scene &instance()
	{
		static Scene S;

		return S;
	}

	void init(int scenenum);
	void screen(int scenenum);
	void AssetsInit();
	void update(int deltaTime);
	void UpdateUIPosition();
	void Death();
	bool checkCollisions(glm::ivec2 otherPosition, glm::ivec2 otherhitbox, glm::ivec2 entPosition, glm::ivec2 entHitbox);
	bool checkCollisionsmovblock(glm::ivec2 otherPosition, glm::ivec2 otherhitbox, glm::ivec2 entPosition, glm::ivec2 entHitbox);
	void movblockcollision();
	void render();
	void damage();
	void volumeengine(bool up);


	
	ISound* soundeffect(const char* song);
	ISoundEngine* engine;
	ISound* soundmap = 0;
	float volume = 1.0f;

	bool enter = true;


private:
	void initShaders();
	void databasescenes();
	void changescene(int mode); //N,S,E,0
	bool checkscenechange(int lastroom, int actualroom);

	glm::ivec2 getinitscreen();

	void chargescene();

	

private:

	int actualscreen = 0;

	struct Zones
	{
		UI *key, *gem1, *gem2;
		Pickup *keypick, *gem1pick, *gem2pick;
	};

	Zones arrayscenes[4];
	int actualscene, actualroom;
	TileMap *map;
	Player *player;
	UI *ui, *key, *gem1, *gem2, *hpui, *menuoptions,*options,*uimenu;
	Pickup *keypick, *gem1pick, *gem2pick;
	vector<Pickup*> pickups;
	bool notsecurepickups;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 initscreen;
	int movecamera; //0 nope 1 left 2 right 3 up 4 down
	glm::vec2 auxposcamera;

	vector<Button*> buttons;
	vector<Spring*> springs;

	vector<Checkpoint*> checkpoints;
	Checkpoint* activecheck;

	vector<MovBlock*> movblocs;

	bool auxchangescene,death;
	array<int,24> roomscenes; //NUMERO HABITACIONES

	bool paused;
	pause *p;
	UI *gameend;
	bool auxp;

	int auxdamagepl;

	bool sfxcheckpoint = true;
	bool checkpointsfxactive = true;
	int sfxbutton = -1;
	int sfxspring = -1;

};


#endif // _SCENE_INCLUDE

