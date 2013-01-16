#include "..\include\llinactivateallvertices.h"
#include "..\include\llmaplist.h"


llInactivateAllVertices::llInactivateAllVertices() : llMapWorker() {
	SetCommandName("InactivateAllVertices");
}

int llInactivateAllVertices::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;
	return 1;
}

int llInactivateAllVertices::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!points) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no point list", command_name, mapname);
		return 0;
	}

	for (int j=0; j<points->GetN(); j++) {
		points->SetInactive(j);
	}

	return 1;
}
