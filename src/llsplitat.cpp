#include "../include/llsplitat.h"

llSplitAt::llSplitAt() : llTriMod() {
	SetCommandName("SplitAt");
}

int llSplitAt::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	x = y = 0;

	return 1;
}

int llSplitAt::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x", &x);
	RegisterValue("-y", &y);

	return 1;
}


int llSplitAt::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	if (!Used("-x") && !Used("-y")) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: either -x or -y has to be defined", command_name);
		return 0;
	}

	if (Used("-x")) {
		triangles->DivideAt(true, x, map);    	    
	}
	if (Used("-y")) {
		triangles->DivideAt(false, y, map);    	    
	}

	_llLogger()->WriteNextLine(LOG_COMMAND, "%s: done", command_name);

	return 1;
}