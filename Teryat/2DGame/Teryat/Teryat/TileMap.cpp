#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "TileMap.h"
using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap() {
	if(map != NULL)
		delete map;
}


void TileMap::render() const {
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free() {
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile) {
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++) {
		for(int i=0; i<mapSize.x; i++) {
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::changeTile(glm::ivec2 coord, int newblock) {
	map[coord.x * mapSize.x + coord.y] = newblock;
}

int TileMap::getTile(glm::ivec2 coord) {
	int aux = map[coord.x * mapSize.x + coord.y];
	return aux;
}
void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program) {
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}


bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const {
	int x, y0, y1;
	
	x = (pos.x + 3) / tileSize ;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] >= 32 && map[y*mapSize.x + x] != 63)
			return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const {
	int x, y0, y1;
	
	x = (pos.x + size.x - 4) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] >= 32 && map[y*mapSize.x + x] != 63)
			
			return true;
	}
	return false;
}

int TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int *posX,bool stair) const {
	int x0, x1, y;
	
	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x + size.x - 5) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if((map[y*mapSize.x + x] >= 32 && map[y*mapSize.x+x] != 63) || map[y*mapSize.x + x] == 29)
		{
			if(*posY - tileSize * y + size.y <= 8)
			{
				*posY = tileSize * y - size.y;
				if (map[y*mapSize.x + x] == 29 && stair) {
					*posX = tileSize * x;
					return 2; // stairs
				}
				return 1; // floor
			}
		}
	}
	
	return 0; // nothing
}


bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const {
	int x0, x1, y;

	x0 = (pos.x+4) / tileSize;
	x1 = (pos.x + size.x-5) / tileSize;
	y = (pos.y + 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] >= 32)
		{
			if ((tileSize * (y + 1)) - *posY  <= 8)
			{
				*posY = tileSize * (y+1);
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionUpstair(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const {
	int x0, x1, y;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x + size.x - 5) / tileSize;
	y = (pos.y + 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] > 28)
		{
				return true;
		}
	}

	return false;
}

bool TileMap::collisionDownstair(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	int x0, x1, y;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x + size.x - 5) / tileSize;
	y = (pos.y + 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 30)
		{
			return true;
		}
	}

	return false;
}

int TileMap::collisionBehind(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int *posX, bool stair) const {

	int x0, x1, y0, y1;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x + size.x - 5) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++) {
			if (map[y*mapSize.x + x] == 23 || map[y*mapSize.x + x] == 63) return 1; // spikes 
			else if (map[y * mapSize.x + x] == 31 && stair && abs(((*posX + size.x/2) - (tileSize * x + tileSize/2))) <= 12) {
				*posX = tileSize * x;
				return 2; // stairs
			}

				
		}
	}
	return 0; // nothing

}






























