#ifndef RESTARTOUTPUTER_H_
#define RESTARTOUTPUTER_H_

/*! this class is used to output the state in the netcdf format
 */

#include <netcdfcpp.h>

#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

#include "../inc/layerconst.h"
#include "../inc/timeconst.h"
#include "../inc/errorcode.h"

#include "../data/RestartData.h"

class RestartOutputer {
	public:
		RestartOutputer();
		~RestartOutputer();

		void init(string& dir, string& stage);

		void outputVariables(const int & chtcount);
		void setRestartOutData(RestartData * resodp);
		int errorChecking();

		NcFile* restartFile;
		string restartfname;

	private:
		RestartData * resod;

   		NcDim * chtD;
   		NcDim * pftD;
   		NcDim * pftpartD;
   		NcDim * rootlayerD;
   		NcDim * snowlayerD;
   		NcDim * soillayerD;
   		NcDim * rocklayerD;
   		NcDim * frontD;
   		NcDim * prvyearD;
   		NcDim * prvmonthD;

		NcVar* chtidV;
		NcVar* errcodeV;

		//atm
		NcVar* dsrV;
		NcVar* firea2sorgnV;

		NcVar* ysfV;

	    //veg
	    NcVar* ifwoodyV;
	    NcVar* ifdeciwoodyV;
	    NcVar* ifperenialV;
	    NcVar* nonvascularV;

	    NcVar* vegageV;
	    NcVar* vegcovV;
	    NcVar* laiV;
	    NcVar* aerenchymaV;
	    NcVar* rootfracV;

		NcVar* vegwaterV;
		NcVar* vegsnowV;

		NcVar* vegcV;
		NcVar* labnV;
		NcVar* strnV;
		NcVar* deadcV;
		NcVar* deadnV;

		NcVar* toptV;
	    NcVar* eetmxV;
	    NcVar* growingttimeV;
	    NcVar* unnormleafmxV;

		NcVar* toptAV;
	    NcVar* eetmxAV;
	    NcVar* growingttimeAV;
	    NcVar* unnormleafmxAV;
	    NcVar* foliagemxV;

	    //snow
	    NcVar* numsnwlV;
	    NcVar* snwextramassV;
	    NcVar* DZsnowV;
	    NcVar* TSsnowV;
	    NcVar* LIQsnowV;
	    NcVar* RHOsnowV;
	    NcVar* ICEsnowV;
	    NcVar* AGEsnowV;

	    //ground-soil
	    NcVar* numslV;
	    NcVar* monthsfrozenV;
	    NcVar* rtfrozendaysV;
	    NcVar* rtunfrozendaysV;

	    NcVar* watertabV;

	    NcVar* DZsoilV;
	    NcVar* AGEsoilV;
	    NcVar* TYPEsoilV;
	    NcVar* TSsoilV;
	    NcVar* LIQsoilV;
	    NcVar* ICEsoilV;
	    NcVar* FROZENsoilV;
	    NcVar* FROZENFRACsoilV;
	    NcVar* TEXTUREsoilV;

	    NcVar* TSrockV;
	    NcVar* DZrockV;

	    NcVar* frontZV;
	    NcVar* frontFTV;

	    NcVar* wdebriscV;
	    NcVar* dmosscV;
	    NcVar* rawcV;
	    NcVar* somaV;
	    NcVar* somprV;
	    NcVar* somcrV;

	    NcVar* wdebrisnV;
	    NcVar* dmossnV;
	    NcVar* solnV;
	    NcVar* avlnV;

	    NcVar* prvltrfcnAV;


};

#endif /*RESTARTOUTPUTER_H_*/
