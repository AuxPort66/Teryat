#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very simple format. 
// With this information it builds a single VBO that contains all tiles. 
// The render method draws the whole map independently of what is visible.


class TileMap {

public:
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	// Collision tests for axis aligned bounding boxes.
	// Method collisionMoveDown also corrects Y coordinate if the box is already intersecting a tile below.
	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int *posX,bool stair) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	int collisionBehind(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int *posX, bool stair) const; 
	bool collisionUpstair(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionDownstair(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	void changeTile(glm::ivec2 coord, int newblock);
	int getTile(glm::ivec2 coord);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	bool loadLevel(const string &levelFile);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
};
#endif // _TILE_MAP_INCLUDE


