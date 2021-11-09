#include "Changeblocks.h"

void Changeblocks::init(glm::vec2 c, int newblock, int oldblock) {
	coord = c;
	blockactiv = newblock;
	blocknoactiv = oldblock;
}