/*output One Site Cht Dimension variables */


#ifndef CHTOUTPUTER_H_
#define CHTOUTPUTER_H_
#include <iostream>
#include <netcdfcpp.h>

#include "../inc/errorcode.h"
#include "../data/CohortData.h"

using namespace std;

class ChtOutputer{
	public:
		ChtOutputer();
		~ChtOutputer();
				
		void init(string & dirfile);
		void outputCohortDimVars(const int & recordcnt);
		void setOutData(const int &outtstep, CohortData *cdp);

		string ncfname;

	private :

		CohortData *dimod;
	 	vegstate_dim *vegs;
	 	vegdiag_dim *vegd;
	 	snwstate_dim *snws;
	 	soistate_dim *sois;

	 	NcFile * ncfile;

		NcDim * timeD;
		NcDim * pftD;
		NcDim * snwlayerD;
		NcDim * soilayerD;
		NcDim * rotlayerD;

		NcVar* yearV;
		NcVar* monV;
		NcVar* dayV;
		NcVar* chtidV;

		NcVar* cmttypeV;

		NcVar* vegageV;
		NcVar* ifwoodyV;
		NcVar* ifdeciwoodyV;
		NcVar* ifperenialV;
		NcVar* nonvascularV;
		NcVar* aerenchymaV;
		NcVar* vegcovV;
	   	NcVar* laiV;
	   	NcVar* fpcV;
	   	NcVar* rtfracV;
	   	NcVar* fleafV;
	   	NcVar* ffoliageV;

	   	//
	   	NcVar* snwlynumV;
	   	NcVar* snwlydzV;
	   	NcVar* snwlyrhoV;
	   	NcVar* snwlyporV;
	   	NcVar* snwthickV;
		NcVar* snwdenseV;
		NcVar* snwextramassV;
		NcVar* snwageV;

		//
		NcVar* soillayernumV;
		NcVar* soilmossnumV;
		NcVar* soilshlwnumV;
		NcVar* soildeepnumV;
		NcVar* soilminenumV;

		NcVar* soilthickV;
		NcVar* mossthickV;
		NcVar* shlwthickV;
		NcVar* deepthickV;
		NcVar* mineathickV;
		NcVar* minebthickV;
		NcVar* minecthickV;

		NcVar* soilzV;
		NcVar* soildzV;
		NcVar* soiltypeV;
		NcVar* soilporoV;
		NcVar* soilrtfracV;
		NcVar* soiltextureV;

};


#endif /*CHTOUTPUER_H_*/
