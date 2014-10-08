/*output One Site Env-Module variables */


#ifndef ENVOUTPUTER_H_
#define ENVOUTPUTER_H_
#include <iostream>
#include <cstdlib>
#include <netcdfcpp.h>

#include "../data/EnvData.h"
#include "../inc/errorcode.h"
#include "../inc/timeconst.h"
#include "../inc/layerconst.h"

using namespace std;

class EnvOutputer{
	public:
		EnvOutputer();
		~EnvOutputer();
				
		void init(string & dirfile, const int &numpft);
		void outputCohortEnvVars(const int &outtstep, const int &ipft,
				snwstate_dim *dsnow, EnvData * envod,
				const int&iy, const int&im, const int &id,
				const int & tstepcnt);

		string ncfname;
		bool pftintegrated;

	private :

		atmstate_env *atms;
		atmdiag_env *atmd;
		atm2lnd_env *a2l;
		atm2veg_env *a2v;
		lnd2atm_env *l2a;

		vegstate_env *vegs;
		vegdiag_env *vegd;
		veg2atm_env *v2a;
		veg2gnd_env *v2g;

		snwstate_env *snws;
		snw2atm_env *snw2a;
	    soistate_env *sois;
	    soidiag_env *soid;
	    soi2atm_env *soi2a;
	    soi2lnd_env *soi2l;

		NcFile * ncfileenv;

		NcDim * timeD;
		NcDim * pftD;
		NcDim * snwlayerD;
		NcDim * soilayerD;
		NcDim * frontD;

		NcVar* chtidV;
		NcVar* errorV;    //error code
		NcVar* yearV;
		NcVar* monV;
		NcVar* dayV;

	   	NcVar* tairV;
	   	NcVar* co2V;
	   	NcVar* vpV;
	   	NcVar* svpV;
	   	NcVar* vpdV;
	   	NcVar* nirrV;
	   	NcVar* parV;
	   	NcVar* precV;
	   	NcVar* rnflV;
	   	NcVar* snflV;

	   	NcVar* pardownV;
	   	NcVar* parabsorbV;
	   	NcVar* swdownV;
	   	NcVar* swinterV;
	   	NcVar* rinterV;
	   	NcVar* sinterV;
	   	NcVar* eetV;
	   	NcVar* petV;

	   	NcVar* vegwaterV;
	   	NcVar* vegsnowV;
	   	NcVar* vegrcV;
	   	NcVar* vegccV;
	   	NcVar* vegbtranV;
	   	NcVar* vegm_ppfdV;
	   	NcVar* vegm_vpdV;

	   	NcVar* vegswreflV;
	   	NcVar* vegevapV;
	   	NcVar* vegtranV;
	   	NcVar* vegevap_pV;
	   	NcVar* vegtran_pV;
	   	NcVar* vegsublimV;

	   	NcVar* vegswthflV;
	   	NcVar* vegrdripV;
	   	NcVar* vegrthflV;
	   	NcVar* vegsdripV;
	   	NcVar* vegsthflV;

		//snow
	   	NcVar* snwthickV;
		NcVar* snwdenseV;
		NcVar* snwextramassV;
		NcVar* snwageV;
		NcVar* sweV;
	   	NcVar* tsnwV;
	   	NcVar* swesumV;
	   	NcVar* tsnwaveV;
	   	NcVar* snwswreflV;
	   	NcVar* snwsublimV;

	 	//soil
	   	NcVar* soilicesumV;
	   	NcVar* soilliqsumV;
	   	NcVar* soilvwcshlwV;
	   	NcVar* soilvwcdeepV;
	   	NcVar* soilvwcmineaV;
	   	NcVar* soilvwcminebV;
	   	NcVar* soilvwcminecV;

	   	NcVar* soiltaveV;
	   	NcVar* soiltshlwV;
	   	NcVar* soiltdeepV;
	   	NcVar* soiltmineaV;
	   	NcVar* soiltminebV;
	   	NcVar* soiltminecV;

	   	NcVar* soiltsV;
		NcVar* soilliqV;
		NcVar* soiliceV;
		NcVar* soilvwcV;
		NcVar* soillwcV;
		NcVar* soiliwcV;
		NcVar* soilfrontzV;
		NcVar* soilfronttypeV;

		NcVar* soilwatertabV;
		NcVar* permafrostV;
		NcVar* soilaldV;
		NcVar* soilalcV;

		NcVar* soilgrowstartV;
		NcVar* soilgrowendV;
		NcVar* soiltsrtdpV;
		NcVar* soiltsdegdayV;
		NcVar* soilrtthawpctV;

		//
		NcVar* soilswreflV;
		NcVar* soilevapV;
		NcVar* soilevap_pV;

		NcVar* qoverV;
		NcVar* qdrainV;

};


#endif /*SNOWSOILOUTPUER_H_*/
