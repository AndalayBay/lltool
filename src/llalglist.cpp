//a list of algorithms

#include "../include/llalglist.h"


llAlgCollection::llAlgCollection() {
	alg_list.resize(0);	
}

llAlgList& _fllAlgList() {
    static llAlgList* ans = new llAlgList();
    return *ans;
}

llAlgList * _llAlgList() {
    return &_fllAlgList();
}

llAlgList::llAlgList() {
	collections.resize(0);
	collections.resize(0);
}

int llAlgList::AddAlgCollection(char *_name, llAlgCollection *_col) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, col_name[i]) == 0) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"AddAlgCollection::AddAlgCollection: collection %s exists already", _name);
			return 0;
		}
	}
	collections.resize(size + 1);
	collections[size]      = _col;
	col_name.resize(size + 1);
	col_name[size]         = _name;
	return 1;
}

llAlgCollection *llAlgList::GetAlgCollection(char* _name) {
	for (int i=0; i<GetSize(); i++) {
		if (_stricmp(_name, col_name[i]) == 0)
			return collections[i];
	}
	return NULL;
}

int llAlgCollection::UpdateMaps() {
	for (int i=0; i<GetSize(); i++) {
		if (!alg_list[i]->UpdateMap()) return 0;
	}
	return 1;
}

llCreateAlgCollection::llCreateAlgCollection() : llWorker() {
	SetCommandName("CreateAlgCollection");
	name = NULL;
}

int llCreateAlgCollection::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-name", &name, LLWORKER_OBL_OPTION);

	return 1;
}

int llCreateAlgCollection::Exec(void) {
	if (!llWorker::Exec()) return 0;

	_llAlgList()->AddAlgCollection(name, new llAlgCollection());

	return 1;
}

