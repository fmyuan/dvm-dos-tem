#ifndef OUTRETRIVE_H_
	#define OUTRETRIVE_H_

    #include "Timer.h"
	#include "ModelData.h"

	#include "../data/EnvData.h"
	#include "../data/BgcData.h"
	#include "../data/FirData.h"
    #include "../data/RestartData.h"
    #include "../data/OutDataRegn.h"

	class OutRetrive{
		public :
			OutRetrive();
			~OutRetrive();

			int cohortcount;

	    	OutDataRegn * regnod;

	    	RestartData * resod;

	    	//
	    	void setDimensionData(CohortData *cdp);
	    	void setProcessData(const int &ip, EnvData *edp, BgcData *bdp);
	    	void setFireData(FirData *fdp);

	    	//
	    	void setRegnOutData(OutDataRegn * regnodp);
	    	void setRestartOutData(RestartData *resodp);

	    	//
	    	void updateRegnOutputBuffer(const int &im, const int &idoy);

	    	void updateRestartOutputBuffer();

	    private:
	    	//
	    	CohortData *cd;
	    	EnvData *edall;
	    	BgcData *bdall;
	    	EnvData *ed[NUM_PFT];
	    	BgcData *bd[NUM_PFT];
	    	FirData *fd;

};
#endif /*COHORT_H_*/
