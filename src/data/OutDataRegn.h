#ifndef REGNOUTDATA_H_
#define REGNOUTDATA_H_
/* 
 * define output variables for regional TEM run
 * memory consumption too much, so modified as output year by year
 *  rather than cohort by cohort - Yuan
 */

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
	  	int status[DINY];
		int year;
		int month[MINY];
		int doy[DINY];
   		int yrsdist;

   		int outvarlist[I_outvarno];

   		// after update the following vars, please update the list and enum above
   		int growstart[DINY][NUM_PFT];
   		int growend[DINY][NUM_PFT];
   		double vegcov[DINY][NUM_PFT];
   		double vegage[DINY][NUM_PFT];

   		double lai[DINY][NUM_PFT];
   		double vegc[DINY][NUM_PFT];
   		double leafc[DINY][NUM_PFT];
   		double stemc[DINY][NUM_PFT];
   		double rootc[DINY][NUM_PFT];
   		double vegn[DINY][NUM_PFT];
   		double labn[DINY][NUM_PFT];
   		double leafn[DINY][NUM_PFT];
  		double stemn[DINY][NUM_PFT];
  		double rootn[DINY][NUM_PFT];

   		double gpp[DINY][NUM_PFT];
   		double npp[DINY][NUM_PFT];

        double ltrfalc[DINY][NUM_PFT];
        double ltrfaln[DINY][NUM_PFT];

        double nuptake[DINY][NUM_PFT];

        //
   		int permafrost[DINY];

   		double mossdz[DINY];
   		double oshlwdz[DINY];
   		double odeepdz[DINY];
   		double mineadz[DINY];
   		double minebdz[DINY];
   		double minecdz[DINY];

   		double oshlwc[DINY];
   		double odeepc[DINY];
   		double mineac[DINY];
   		double minebc[DINY];
   		double minecc[DINY];

   		double orgn[DINY];
   		double avln[DINY];
   		double ch4[DINY];

   		double rh[DINY];
   		double ch4prod[DINY];
   		double ch4oxid[DINY];
   		double ch4flux[DINY];
   		double ch4flux2a[DINY];
   		double ch4flux2p[DINY];
   		double ch4flux2b[DINY];
   		double netnmin[DINY];

   		double orgninput[DINY];
   		double avlninput[DINY];

      	double doclost[DINY];
      	double orgnlost[DINY];
      	double avlnlost[DINY];

      	//
      	double eet[DINY];
   		double pet[DINY];
   		double qinfl[DINY];
   		double qdrain[DINY];
      	double qrunoff[DINY];

      	double snwthick[DINY];
      	double swe[DINY];

   		double wtd[DINY];
   		double alc[DINY];
      	double ald[DINY];

   		double vwcshlw[DINY];
   		double vwcdeep[DINY];
   		double vwcminea[DINY];
      	double vwcmineb[DINY];
      	double vwcminec[DINY];

   		double tshlw[DINY];
   		double tdeep[DINY];
   		double tminea[DINY];
   		double tmineb[DINY];
   		double tminec[DINY];

   		double hkshlw[DINY];
   		double hkdeep[DINY];
   		double hkminea[DINY];
   		double hkmineb[DINY];
   		double hkminec[DINY];

   		double tcshlw[DINY];
   		double tcdeep[DINY];
   		double tcminea[DINY];
   		double tcmineb[DINY];
   		double tcminec[DINY];

   		double tbotrock[DINY];

   		//
   		double burnthick[DINY];
   		double burnsoic[DINY];
   		double burnvegc[DINY];
   		double burnsoin[DINY];
   		double burnvegn[DINY];
   		double burnretainc[DINY];
   		double burnretainn[DINY];
	 
};

#endif /*REGNOUTDATA_H_*/
