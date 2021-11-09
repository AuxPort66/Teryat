#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "Scene.h"
#include "Game.h"
#include "Changeblocks.h"
#pragma comment(lib, "irrKlang.lib") 

#define SCREEN_X 32	
#define SCREEN_Y 32

#define TRANSITION 14

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 94 

#define AUXSCENELARGE 4

#define INITROOM 0

#define NUM_ZONES 3


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


enum ScenesTypes {
	MENU, OPTIONS, LEVEL, END
};


void Scene::init(int scenenum) {
	initShaders();
	engine = createIrrKlangDevice();
	engine->setSoundVolume(volume);
	soundmap = engine->play2D("songs/8bitadventure-DavidRenda.wav", true, false, true);
	map = TileMap::createTileMap("levels/MapaDemo.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	initscreen.y = 2560.f;
	initscreen.x = 0.f;

	uimenu = new UI();
	uimenu->init(glm::ivec2(0, 0), texProgram, 4);
	uimenu->setPosition(glm::vec2(initscreen.x, initscreen.y));
	uimenu->setTileMap(map);

	menuoptions = new UI();
	menuoptions->init(glm::ivec2(0, 0), texProgram, 5);
	menuoptions->setPosition(glm::vec2(initscreen.x, initscreen.y));
	menuoptions->setTileMap(map);

	options = new UI();
	options->init(glm::ivec2(0, 0), texProgram, 6);
	options->setPosition(glm::vec2(initscreen.x, initscreen.y));
	options->setTileMap(map);

	gameend = new UI();
	gameend->init(glm::ivec2(0, 0), texProgram, 7);
	gameend->setPosition(glm::vec2(initscreen.x, initscreen.y));
	gameend->setTileMap(map);

	screen(scenenum);
}

void Scene::screen(int scenenum) {

	actualscreen = scenenum;

	switch (scenenum)
	{
	case MENU:
		menuoptions->Active(true);
		options->Active(false);
		break;

	case OPTIONS:
		menuoptions->Active(false);
		options->Active(true);
		break;

	case END:
		initscreen.y = 2560.f;
		initscreen.x = 0.f;
		projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y));
		uimenu->Active(true);
		gameend->Active(true);
		break;

	case LEVEL: //LEVEL
		paused = auxp = false;
		auxchangescene = true;
		death = false;
		movecamera = 0;

		actualroom = INITROOM;
		notsecurepickups = false;

		p = new pause();
		p->init(initscreen, texProgram);

		databasescenes();

		actualscene = roomscenes[actualroom];

		AssetsInit();

		activecheck = NULL;

		auxdamagepl = 0;
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);

		keypick = arrayscenes[actualscene].keypick;

		gem1pick = arrayscenes[actualscene].gem1pick;
		gem2pick = arrayscenes[actualscene].gem2pick;

		ui = new UI();
		ui->init(glm::ivec2(0, 0), texProgram, 0);
		ui->setPosition(glm::vec2(initscreen.x, initscreen.y));
		ui->setTileMap(map);

		key = arrayscenes[actualscene].key;
		gem1 = arrayscenes[actualscene].gem1;
		gem2 = arrayscenes[actualscene].gem2;
		break;
	}

	projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y));
	currentTime = 0.0f;
}

void Scene::AssetsInit() {

	ifstream infile("levels/Assets.txt");

	vector<Changeblocks*> arraychanges;
	Changeblocks* auxchanges;
	glm::vec2 auxcoord;

	Button* auxbutt;

	Spring* auxspr;

	Checkpoint *auxcheckpoint;

	MovBlock *auxmov;

	int x, y, x2, y2, type, face, room;
	string line;
	while (getline(infile, line)) {
		istringstream iss(line);
		char c;
		iss >> c;

		switch (c) {
			case 'A':
				iss >> x >> y >> type;
				auxchanges = new Changeblocks();
				auxcoord = glm::vec2(x, y);
				auxchanges->init(auxcoord, type, map->getTile(auxcoord));
				arraychanges.push_back(auxchanges);
				break;
			case 'B':
				iss >> x >> y;
				auxbutt = new Button();
				auxbutt->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, arraychanges);
				auxbutt->setPosition(glm::vec2(map->getTileSize() * x, map->getTileSize() * y));
				auxbutt->setTileMap(map);
				buttons.push_back(auxbutt);
				break;
			case 'S':
				iss >> room >> x >> y;
				auxspr = new Spring();
				auxspr->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, room);
				auxspr->setPosition(glm::vec2(map->getTileSize() * x, map->getTileSize() * y));
				auxspr->setTileMap(map);
				springs.push_back(auxspr);
				break;
			case 'C':
				iss >> type >> room >> x >> y;
				auxcheckpoint = new Checkpoint();
				auxcheckpoint->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type, room);
				auxcheckpoint->setPosition(glm::vec2(map->getTileSize() * x, map->getTileSize() * y));
				auxcheckpoint->setTileMap(map);
				checkpoints.push_back(auxcheckpoint);
				break;
			case 'M':
				iss >> type >> x >> y >> x2 >> y2;
				auxmov = new MovBlock();
				auxmov->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
				auxmov->setPosition(glm::vec2(map->getTileSize() * x, map->getTileSize() * y), glm::vec2(map->getTileSize() * x2, map->getTileSize() * y2));
				auxmov->setTileMap(map);
				movblocs.push_back(auxmov);
				break;
		}

	}
}

void Scene::update(int deltaTime) {	
	if (actualscreen == MENU || actualscreen == OPTIONS) {
		menuoptions->update(deltaTime);
		uimenu->update(deltaTime);
		options->update(deltaTime);
		if (Game::instance().getKey(' ')) enter = false;
		else enter = true;
		
	}
	else if (actualscreen == END) {
		uimenu->update(deltaTime);
		if (Game::instance().getKey(' ') || Game::instance().getKey(27))
			exit(0);
	}

	else if (actualscreen == LEVEL) {

		if (player->posPlayer.x == 62 * 32 && player->posPlayer.y < 17 * 32) 
			Game::instance().changescene(END);
		if (movecamera != 0) {
			switch (movecamera) {
			case 1:
				if (auxchangescene) {
					changescene(MENU);
					auxchangescene = false;
				}

				if (initscreen.x > (auxposcamera.x - (SCREEN_WIDTH - map->getTileSize() * 2) + TRANSITION)) {
					initscreen.x -= TRANSITION;
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else if (initscreen.x != (auxposcamera.x - (SCREEN_WIDTH - map->getTileSize() * 2))) {
					initscreen.x = (auxposcamera.x - (SCREEN_WIDTH - map->getTileSize() * 2));
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else {
					auxchangescene = true;
					movecamera = 0;
				}
				break;

			case 2:
				if (auxchangescene) {
					changescene(OPTIONS);
					auxchangescene = false;
				}

				if (initscreen.x < (auxposcamera.x + SCREEN_WIDTH - map->getTileSize() * 2) - TRANSITION) {
					initscreen.x += TRANSITION;
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else if (initscreen.x != (auxposcamera.x + SCREEN_WIDTH - map->getTileSize() * 2)) {
					initscreen.x = (auxposcamera.x + SCREEN_WIDTH - map->getTileSize() * 2);
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else {
					auxchangescene = true;
					movecamera = 0;
				}
				break;

			case 3:
				if (auxchangescene) {
					changescene(LEVEL);
					auxchangescene = false;
				}

				if (initscreen.y > (auxposcamera.y - (SCREEN_HEIGHT - map->getTileSize() * 2) + TRANSITION)) {
					initscreen.y -= TRANSITION;
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else if (initscreen.y != (auxposcamera.y - (SCREEN_HEIGHT - map->getTileSize() * 2))) {
					initscreen.y = (auxposcamera.y - (SCREEN_WIDTH - map->getTileSize() * 2));
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else {
					auxchangescene = true;
					movecamera = 0;
				}
				break;

			case 4:
				if (auxchangescene) {
					changescene(END);
					auxchangescene = false;
				}

				if (initscreen.y < (auxposcamera.y + SCREEN_HEIGHT - map->getTileSize() * 2) - TRANSITION) {
					initscreen.y += TRANSITION;
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else if (initscreen.y != (auxposcamera.y + SCREEN_HEIGHT - map->getTileSize() * 2)) {
					initscreen.y = (auxposcamera.y + SCREEN_HEIGHT - map->getTileSize() * 2);
					projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y)); //AÑADIR TRANSICIÓN EN VEZ DE GOLPE
					UpdateUIPosition();
				}
				else {
					auxchangescene = true;
					movecamera = 0;
				}
				break;
			}
		}
		else {

			if (Game::instance().getKey(27) && !auxp) {
				paused = !paused;
				auxp = true;
			}
			else if (!Game::instance().getKey(27)) 
				auxp = false;

			if (paused)
				p->setActive(initscreen);
			else {
				currentTime += deltaTime;

				movblockcollision();

				for (int i = 0; i < movblocs.size(); i++) {
					movblocs[i]->update(deltaTime);
					player->addMovblockvec(movblocs[i]->movtoplayer);
					player->addMovblockvechead(movblocs[i]->movtohead);
				}

				player->update(deltaTime);

				if (!keypick->istaked() && checkCollisions(keypick->getPosition(), glm::ivec2(32, 32), player->getPosition(), glm::ivec2(32, 32))) {
					Game::instance().scene.soundeffect("songs/Rpgaudio/beltHandle1.ogg");
					keypick->setTaked(true);
					key->Active(true);
					keypick->secure = false;
					notsecurepickups = true;
				}
				if (!gem1pick->istaked() && checkCollisions(gem1pick->getPosition(), glm::ivec2(32, 32), player->getPosition(), glm::ivec2(32, 32))) {
					Game::instance().scene.soundeffect("songs/Impactaudio/impactGlass_light_003.ogg");
					gem1pick->setTaked(true);
					gem1->Active(true);
					gem1pick->secure = false;
					notsecurepickups = true;
				}
				if (!gem2pick->istaked() && checkCollisions(gem2pick->getPosition(), glm::ivec2(32, 32), player->getPosition(), glm::ivec2(32, 32))) {
					Game::instance().scene.soundeffect("songs/Impactaudio/impactGlass_light_003.ogg");
					gem2pick->setTaked(true);
					gem2->Active(true);
					gem2pick->secure = false;
					notsecurepickups = true;
				}

				for (int i = 0; i < checkpoints.size(); i++) {
					if (checkpoints[i]->getRoom() == actualroom && checkCollisions(checkpoints[i]->getPosition(), glm::ivec2(32, 32), player->getPosition(), glm::ivec2(32, 32))) {
						player->newHead();
						activecheck = checkpoints[i];
						checkpoints[i]->Active();
						if(notsecurepickups){
							for (int i = 0; i < pickups.size(); i++) {
								if (pickups[i]->istaked() && !pickups[i]->secure) pickups[i]->secure = true;
							}
							notsecurepickups = false;
						}
					}
				}

				bool maprefresh = false;
				for (int i = 0; i < buttons.size(); i++) {

					bool playerinbutton = (checkCollisions(buttons[i]->getPosition(), glm::ivec2(32, 16), player->getHeadPosition(), glm::ivec2(24, 24)) && player->isHeadActive()) || checkCollisions(buttons[i]->getPosition(), glm::ivec2(16, 32), player->getPosition(), glm::ivec2(32, 32));
					
					if (playerinbutton && !buttons[i]->isPressed()) {
						if (i != sfxbutton) {
							Game::instance().scene.soundeffect("songs/Rpgaudio/metalClick.ogg");
							sfxbutton = i;
						}
						maprefresh = true;
						buttons[i]->setPressed(true);
					}
					else if (!playerinbutton && buttons[i]->isPressed()) {
						if (sfxbutton == i) sfxbutton = -1;
						buttons[i]->setCdpressed(35);
						if (buttons[i]->getAnimation() == 0) maprefresh = true;
					}
				}
				if (maprefresh) map->prepareArrays(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


				for (int i = 0; i < springs.size(); i++) {

					bool headpresspring = checkCollisions(springs[i]->getPosition(), glm::ivec2(32, 16), player->getHeadPosition(), glm::ivec2(24, 24)) && player->isHeadActive();
					bool playerpresspring = checkCollisions(springs[i]->getPosition(), glm::ivec2(16, 32), player->getPosition(), glm::ivec2(32, 32));
					
					if ((headpresspring || playerpresspring) && !springs[i]->isPressed()) {
						springs[i]->setPressed(true);
						if (headpresspring) player->setActivehead(true);
						if (i != sfxspring) {
							if(actualroom == springs[i]->getRoom()) Game::instance().scene.soundeffect("songs/Halftonesfx/7. Blip, Blop and Pops/Pop_3.wav");
							sfxspring = i;
						}
					}
					else if (!playerpresspring && !headpresspring && springs[i]->isPressed()) {
						if (sfxspring == i) sfxspring = -1;
						springs[i]->setCdpressed(5);
					}
				}

				bool checkheadamage = player->damagehead();
				if (player->checkDamage() || checkheadamage) {
					player->substractHp();
					Game::instance().scene.soundeffect("songs/Halftonesfx/4. Impact/Hit_2.wav");
					player->setDamage(false);
					player->initInvincibility();
					if (player->getHp() == 0 || checkheadamage) {
						Death();
					}
					auxdamagepl = 15;
				}

				if (!keypick->istaked())
					keypick->update(deltaTime);
				if (!gem1pick->istaked())
					gem1pick->update(deltaTime);
				if (!gem2pick->istaked())
					gem2pick->update(deltaTime);

				if (!death) {
					if ((player->getPosition().x < (initscreen.x - 16) && player->getLeft())) {
						auxposcamera.x = initscreen.x;
						movecamera = 1;
					}
					else if ((player->getPosition().x > (initscreen.x + SCREEN_WIDTH - 80) && !player->getLeft())) { //AÑADIR X0 E Y0 DE LA PANTALLA  || (player->getPosition().x < (initscreen - 16) && player->left)
						auxposcamera.x = initscreen.x;
						movecamera = 2;
					}
					else if ((player->getPosition().y < (initscreen.y - 16) && !player->getFalling())) {
						auxposcamera.y = initscreen.y;
						movecamera = 3;
					}
					else if ((player->getPosition().y > (initscreen.y + SCREEN_HEIGHT - 80) && player->getFalling())) {
						auxposcamera.y = initscreen.y;
						movecamera = 4;
					}
				}
				else death = false;
			}
		}
	}
}

void Scene::UpdateUIPosition() {
	ui->setPosition(glm::vec2(initscreen.x, initscreen.y));
	key->setPosition(glm::vec2(initscreen.x + map->getTileSize() * 7, initscreen.y + map->getTileSize() * 17));
	gem1->setPosition(glm::vec2(initscreen.x + map->getTileSize() * 9, initscreen.y + map->getTileSize() * 17));
	gem2->setPosition(glm::vec2(initscreen.x + map->getTileSize() * 10, initscreen.y + map->getTileSize() * 17));
}

void Scene::Death() {

	glm::ivec2 revive;
	if (activecheck != NULL) 
		revive = activecheck->getPosition();
	else revive = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize());

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(revive);
	player->setTileMap(map);

	initscreen.x = revive.x - ((int(revive.x)/ map->getTileSize()) % 16) * map->getTileSize();
	initscreen.y = revive.y - ((int(revive.y) / map->getTileSize()) % 16) * map->getTileSize();
	projection = glm::ortho(float(initscreen.x), float(initscreen.x) + float(SCREEN_WIDTH), float(initscreen.y) + float(SCREEN_HEIGHT), float(initscreen.y));
	
	if (notsecurepickups) {
		for (int i = 0; i < pickups.size(); i++) {
			if (pickups[i]->istaked() && !pickups[i]->secure) {
				pickups[i]->setTaked(false);
				pickups[i]->pickupui->Active(false);
			}
		}
		notsecurepickups = false;
	}

	int auxroom = INITROOM;
	if (activecheck != NULL) auxroom = activecheck->getRoom();
	if (checkscenechange(actualroom, auxroom)) {
		actualroom = auxroom;
		actualscene = roomscenes[actualroom];
		chargescene();
	}
	else actualroom = auxroom;
	UpdateUIPosition();
}

bool Scene::checkCollisions(glm::ivec2 otherPosition, glm::ivec2 otherhitbox, glm::ivec2 entPosition, glm::ivec2 entHitbox) {
		bool Overlap = ((entPosition.x + entHitbox.x - 2) > otherPosition.x + 2
			&& entPosition.x + 2 < (otherPosition.x + otherhitbox.x - 2)
			&& (entPosition.y + entHitbox.y) > otherPosition.y
			&& entPosition.y < (otherPosition.y + otherhitbox.y));
		if (Overlap)
			return true;
		return false;
}


void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (actualscreen == MENU || actualscreen == OPTIONS) {
		uimenu->render();
		menuoptions->render();
		options->render();
	}
	else if (actualscreen == END) {
		uimenu->render();
		gameend->render();

	}
	else if (actualscreen == LEVEL) {
		map->render();
		if (!keypick->istaked()) keypick->render();
		if (!gem1pick->istaked()) gem1pick->render();
		if (!gem2pick->istaked()) gem2pick->render();

		for (int i = 0; i < checkpoints.size(); i++)
			checkpoints[i]->render();
		for (int i = 0; i < buttons.size(); i++)
			buttons[i]->render();
		for (int i = 0; i < springs.size(); i++)
			springs[i]->render();
		for (int i = 0; i < movblocs.size(); i++)
			movblocs[i]->render();

		if (auxdamagepl > 0) {
			texProgram.setUniform4f("color", 0.803, 0.835, 0.525, 1.0f);
			player->render();
			texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			auxdamagepl--;
		}
		else
			player->render();

		ui->render();
		key->render();
		gem1->render();
		gem2->render();
		if (paused) p->render();
	}
}

void Scene::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}

	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}

	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked()) {
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::databasescenes() {

	roomscenes = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2 };

	UI *auxuikey, *auxuigem1, *auxuigem2;
	Pickup *auxpickkey, *auxgem1pick, *auxgem2pick;
	Zones tmpzones;

	ifstream infile("levels/Pickups.txt");

	int x, y, x2, y2, type;
	string line;
	int countzone = 0;
	while (getline(infile, line)) {
		istringstream iss(line);
		iss >> type;

		switch (type) {
		case 0:
			iss >> x >> y >> x2 >> y2;
			auxpickkey = new Pickup();
			auxpickkey->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
			auxpickkey->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
			auxpickkey->setTileMap(map);
			tmpzones.keypick = auxpickkey;
			pickups.push_back(auxpickkey);

			auxuikey = new UI();
			auxuikey->init(glm::ivec2(0, 0), texProgram, type + 1);
			auxuikey->setPosition(glm::vec2(map->getTileSize() * x2, map->getTileSize() * y2));
			auxuikey->setTileMap(map);
			auxpickkey->pickupui = auxuikey;
			tmpzones.key = auxuikey;
			break;

		case 1:
			iss >> x >> y >> x2 >> y2;
			auxgem1pick = new Pickup();
			auxgem1pick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
			auxgem1pick->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
			auxgem1pick->setTileMap(map);
			tmpzones.gem1pick = auxgem1pick;
			pickups.push_back(auxgem1pick);

			auxuigem1 = new UI();
			auxuigem1->init(glm::ivec2(0, 0), texProgram, type + 1);
			auxuigem1->setPosition(glm::vec2(map->getTileSize() * x2, map->getTileSize() * y2));
			auxuigem1->setTileMap(map);
			auxgem1pick->pickupui = auxuigem1;
			tmpzones.gem1 = auxuigem1;
			break;

		case 2:
			iss >> x >> y >> x2 >> y2;
			auxgem2pick = new Pickup();
			auxgem2pick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
			auxgem2pick->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
			auxgem2pick->setTileMap(map);
			tmpzones.gem2pick = auxgem2pick;
			pickups.push_back(auxgem2pick);

			auxuigem2 = new UI();
			auxuigem2->init(glm::ivec2(0, 0), texProgram, type);
			auxuigem2->setPosition(glm::vec2(map->getTileSize() * x2, map->getTileSize() * y2));
			auxuigem2->setTileMap(map);
			auxgem2pick->pickupui = auxuigem2;
			tmpzones.gem2 = auxuigem2;

			arrayscenes[countzone++] = tmpzones;
			break;

		}
	}
}

void Scene::changescene(int mode) {
	int lastroom = actualroom;
	switch (mode)
	{
	case 0:
		actualroom -= 1;
		break;
	case 1:
		actualroom += 1;
		break;
	case 2:
		actualroom += AUXSCENELARGE;
		break;
	case 3:
		actualroom -= AUXSCENELARGE;
		break;
	default:
		break;
	}

	if (checkscenechange(lastroom, actualroom)) {
		actualscene = roomscenes[actualroom];
		chargescene();
	}
}

bool Scene::checkscenechange(int lastroom, int actualroom) {

	if (roomscenes[lastroom] != roomscenes[actualroom]) return true;
	else return false;
}

glm::ivec2 Scene::getinitscreen() {
	return initscreen;
}

void Scene::chargescene() {
	keypick = arrayscenes[actualscene].keypick;
	gem1pick = arrayscenes[actualscene].gem1pick;
	gem2pick = arrayscenes[actualscene].gem2pick;
	key = arrayscenes[actualscene].key;
	gem1 = arrayscenes[actualscene].gem1;
	gem2 = arrayscenes[actualscene].gem2;
	if (keypick != NULL) keypick->render();
	if (gem1pick != NULL) gem1pick->render();
	if (gem2pick != NULL) gem2pick->render();
	player->render();
	ui->render();
	key->render();
	gem1->render();
	gem2->render();
}

bool Scene::checkCollisionsmovblock(glm::ivec2 otherPosition, glm::ivec2 otherhitbox, glm::ivec2 entPosition, glm::ivec2 entHitbox) {
	bool Overlap = ((entPosition.x + entHitbox.x) > otherPosition.x
		&& entPosition.x < (otherPosition.x + otherhitbox.x)
		&& (entPosition.y + entHitbox.y) > otherPosition.y
		&& entPosition.y < (otherPosition.y + otherhitbox.y));
	if (Overlap)
		return true;
	return false;
}

void Scene::movblockcollision() {
	for (int i = 0; i < movblocs.size(); i++) {
		if (!player->upmovblock) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getPosition().x, player->getPosition().y - 8), glm::ivec2(32, 8)))
			{
				player->upmovblock = true; 
				movblocs[i]->upcoll = true;
			}
			else player->upmovblock = false;
		}
		if (!player->leftmovblock) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getPosition().x + 3, player->getPosition().y), glm::ivec2(0, 32)))
			{
				player->leftmovblock = true;
				movblocs[i]->rightcoll = true;
			}
			else player->leftmovblock = false;
		}
		if (!player->rightmovblock) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getPosition().x + 29, player->getPosition().y), glm::ivec2(0, 32)))
			{
				player->rightmovblock = true;
				movblocs[i]->leftcoll = true;
			}
			else player->rightmovblock = false;
		}
		if (!player->downmovblock && !player->rightmovblock && !player->leftmovblock) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getPosition().x + 4, player->getPosition().y + 32), glm::ivec2(24, 4)))
			{
				player->downmovblock = true;
				player->posPlayer.y = movblocs[i]->getPosition().y - 32;
				movblocs[i]->downcoll = true;
			}
			else player->downmovblock = false;
		}
		if (!player->getCollBlockhead(0)) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getHeadPosition().x, player->getHeadPosition().y - 8), glm::ivec2(24, 8)))
			{
				player->setCollBlockhead(0, true);
				movblocs[i]->upcollhead = true;
			}
			else player->setCollBlockhead(0, false);
		}
		if (!player->getCollBlockhead(1)) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getHeadPosition().x + 4, player->getHeadPosition().y + 26), glm::ivec2(16, 2)))
			{
				player->setCollBlockhead(1,true);
				movblocs[i]->downcollhead = true;
			}
			else player->setCollBlockhead(1, false);
		}
		if (!player->getCollBlockhead(2)) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getHeadPosition().x + 3, player->getHeadPosition().y), glm::ivec2(0, 24)))
			{
				player->setCollBlockhead(2, true);
				movblocs[i]->rightcollhead = true;
			}
			else player->setCollBlockhead(2, false);
		}
		if (!player->getCollBlockhead(3)) {
			if (checkCollisionsmovblock(movblocs[i]->getPosition(), glm::ivec2(64, 32), glm::ivec2(player->getHeadPosition().x + 21, player->getHeadPosition().y), glm::ivec2(0, 24)))
			{
				player->setCollBlockhead(3, true);
				movblocs[i]->leftcollhead = true;
			}
			else player->setCollBlockhead(3, false);
		}
	}
}

ISound* Scene::soundeffect(const char* sound) {
	ISound* aux = NULL;
	aux = engine->play2D(sound);
	return aux;
}

void Scene::volumeengine(bool up) {
	float dif = 0.0f;
	if (up) dif = 0.1f;
	else dif = -0.1f;

	volume = volume + dif;
	engine->setSoundVolume(volume);
	engine->update();
}


