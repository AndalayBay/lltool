#include "../include/llalgconst.h"

llAlgConst::llAlgConst(char*_alg_list, char *_map) : llAlg(_alg_list, _map) {
	SetCommandName("AlgConst");
}

int llAlgConst::Prepare(void) {
	if (!llAlg::Prepare()) return 0;
	return 1;
}

double llAlgConst::GetCeiling(double *_ceiling) {

	double loc_ceiling=1.;

	if (_ceiling) {
		if (add)
			*_ceiling += loc_ceiling*add;
		if (multiply)
			*_ceiling *= loc_ceiling*multiply;
		return *_ceiling;
	} else {
		return loc_ceiling*add*multiply;
	}
}

double llAlgConst::GetValue(float _x, float _y, double *_value) {

	double loc_value=1.;

	if (_value) {
		if (add)
			*_value += loc_value*add;
		if (multiply)
			*_value *= loc_value*multiply;
		return *_value;
	} else {
		return loc_value*multiply + add*loc_value;
	}

	return loc_value;
}

int llAlgConst::Exec(void) {

	if (!llAlg::Exec()) return 0;

	if (alg_list) {
		llAlgCollection *algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection [%s] not found", command_name, alg_list);
			return 0;
		}
		algs->AddAlg(this);
	}

	return 1;
}
