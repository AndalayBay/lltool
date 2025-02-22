#ifndef _PLLALGSTRIPE_H_
#define _PLLALGSTRIPE_H_

#include "../include/llmap.h"
#include "../include/llalglist.h"

class llAlgStripe : public llAlg {

private:

	float lowest, highest, value_at_lowest, value_at_highest;

public:

	//constructor
	llAlgStripe(char *_alg_list, char *_map);

	double GetCeiling(double *_ceiling = NULL); 
	double GetValue(float _x, float _y, double *_value = NULL); 

	virtual llWorker * Clone() {
		return new llAlgStripe(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
