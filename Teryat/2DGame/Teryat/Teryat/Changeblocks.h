#ifndef _CHANGEBLOCKS_INCLUDE
#define _CHANGEBLOCKS_INCLUDE

#include "Sprite.h"

class Changeblocks {

public:
	void init(glm::vec2 c, int newblock, int oldblock);

	glm::ivec2 coord;
	int blockactiv, blocknoactiv;
};
#endif //_CHANGEBLOCKS_INCLUDE