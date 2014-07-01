#ifndef REGNOUTDATA_H_
#define REGNOUTDATA_H_
/* 
 * define output variables for regional TEM run
 * memory consumption too much, so modified as output year by year
 *  rather than cohort by cohort - Yuan
 */

#include <vector>
using namespace std;

#include "../inc/errorcode.h"
#include "../inc/timeconst.h"
#include "../inc/cohortconst.h"

// the following index MUST be exactly match up with the var list in "outvarlist.txt" located in /config/
// this list of index is used in 'OutTrieve::OutRegnBuffer.ccp' and 'RegnOutputer.cpp'
// (it's for easy code maintainence)
enum outvarlistkey {
   		     I_growstart = 0, I_growend, I_vegcov, I_vegage,
   		     I_lai,  I_vegc, I_leafc, I_stemc, I_rootc,
   		     I_vegn, I_labn, I_leafn, I_stemn, I_rootn,
   		     I_gpp,  I_npp,  I_ltrfalc, I_ltrfaln, I_nuptake,

   		     I_permafrost, I_mossdz, I_oshlwdz, I_odeepdz, I_mineadz, I_minebdz, I_minecdz,
   		     I_oshlwc, I_odeepc, I_mineac, I_minebc, I_minecc, I_orgn, I_avln, I_ch4,
   		     I_rh, I_ch4prod, I_ch4oxid, I_ch4flx, I_ch4flx2a, I_ch4flx2p, I_ch4flx2b, I_netnmin,

   		     I_orgninput, I_avlninput, I_doclost, I_orgnlost, I_avlnlost,

   		     I_eet,	I_pet, I_qinfl,	I_qdrain, I_qrunoff,
      	     I_snwthick, I_swe,	I_wtd, I_alc, I_ald,
   		     I_vwcshlw,	I_vwcdeep, I_vwcminea, I_vwcmineb, I_vwcminec,
   		     I_tshlw,I_tdeep,I_tminea, I_tmineb, I_tminec, I_tbotrock,
   		     I_hkshlw, I_hkdeep, I_hkminea, I_hkmineb, I_hkminec,
   		     I_tcshlw, I_tcdeep, I_tcminea, I_tcmineb, I_tcminec,

			I_burnthick, I_burnsoic, I_burnvegc, I_burnsoin, I_burnvegn,
			I_burnretainc, I_burnretainn,

			I_outvarno

};

class OutDataRegn{

	public:

		OutDataRegn();
		~OutDataRegn();

	 	int chtid;
		int year;
   		int yrsdist;
   		int outvarlist[I_outvarno];

   		vector<int> status;
		vector<int> month;
		vector<int> doy;

   		// after update the following vars, please update the list and enum above
   		vector<int> growstart[NUM_PFT];
   		vector<int> growend[NUM_PFT];
   		vector<double> vegcov[NUM_PFT];
   		vector<double> vegage[NUM_PFT];

   		vector<double> lai[NUM_PFT];
   		vector<double> vegc[NUM_PFT];
   		vector<double> leafc[NUM_PFT];
   		vector<double> stemc[NUM_PFT];
   		vector<double> rootc[NUM_PFT];
   		vector<double> vegn[NUM_PFT];
   		vector<double> labn[NUM_PFT];
   		vector<double> leafn[NUM_PFT];
  		vector<double> stemn[NUM_PFT];
  		vector<double> rootn[NUM_PFT];

   		vector<double> gpp[NUM_PFT];
   		vector<double> npp[NUM_PFT];

        vector<double> ltrfalc[NUM_PFT];
        vector<double> ltrfaln[NUM_PFT];

        vector<double> nuptake[NUM_PFT];

        //
   		vector<int> permafrost;

   		vector<double> mossdz;
   		vector<double> oshlwdz;
   		vector<double> odeepdz;
   		vector<double> mineadz;
   		vector<double> minebdz;
   		vector<double> minecdz;

   		vector<double> oshlwc;
   		vector<double> odeepc;
   		vector<double> mineac;
   		vector<double> minebc;
   		vector<double> minecc;

   		vector<double> orgn;
   		vector<double> avln;
   		vector<double> ch4;

   		vector<double> rh;
   		vector<double> ch4prod;
   		vector<double> ch4oxid;
   		vector<double> ch4flux;
   		vector<double> ch4flux2a;
   		vector<double> ch4flux2p;
   		vector<double> ch4flux2b;
   		vector<double> netnmin;

   		vector<double> orgninput;
   		vector<double> avlninput;

      	vector<double> doclost;
      	vector<double> orgnlost;
      	vector<double> avlnlost;

      	//
      	vector<double> eet;
   		vector<double> pet;
   		vector<double> qinfl;
   		vector<double> qdrain;
      	vector<double> qrunoff;

      	vector<double> snwthick;
      	vector<double> swe;

   		vector<double> wtd;
   		vector<double> alc;
      	vector<double> ald;

   		vector<double> vwcshlw;
   		vector<double> vwcdeep;
   		vector<double> vwcminea;
      	vector<double> vwcmineb;
      	vector<double> vwcminec;

   		vector<double> tshlw;
   		vector<double> tdeep;
   		vector<double> tminea;
   		vector<double> tmineb;
   		vector<double> tminec;

   		vector<double> hkshlw;
   		vector<double> hkdeep;
   		vector<double> hkminea;
   		vector<double> hkmineb;
   		vector<double> hkminec;

   		vector<double> tcshlw;
   		vector<double> tcdeep;
   		vector<double> tcminea;
   		vector<double> tcmineb;
   		vector<double> tcminec;

   		vector<double> tbotrock;

   		//
   		vector<double> burnthick;
   		vector<double> burnsoic;
   		vector<double> burnvegc;
   		vector<double> burnsoin;
   		vector<double> burnvegn;
   		vector<double> burnretainc;
   		vector<double> burnretainn;
	 
};

#endif /*REGNOUTDATA_H_*/
