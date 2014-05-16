#ifndef REGNOUTPUTER_H_
#define REGNOUTPUTER_H_
/*! this class is used to output modeling results in the netcdf format
 * when running TEM at regional-mode (i.e., multiple sites)
 * 
 */

#include "netcdfcpp.h"
#include <iostream>
#include <string>
using namespace std;

#include "../inc/errorcode.h"
#include "../data/OutDataRegn.h"

class RegnOutputer{
	
	public:
		RegnOutputer();
		~RegnOutputer();
	
 		void init(string& outputdir, string& stage, const int& startyr, const int& endyr);
  
 		void outputCohortVars(const int & yrind, const int & chtid, const int & status);
  
  		void outputCohortMissing(const int & yrind, const int &yrtsteps, const int & chtid);
  		void setOutData(OutDataRegn *regnodp);
  		  
    	NcFile* rFile;

	 	NcVar* chtidV;
	  	NcVar* statusV;
		NcVar* yearV;
		NcVar* yrmonV;
		NcVar* yrdoyV;
  		NcVar* ysfV;

   		//
   		NcVar* growstartV;
   		NcVar* growendV;

   		NcVar* vegcovV;
   		NcVar* vegageV;
   		NcVar* laiV;
   		NcVar* vegcV;
   		NcVar* leafcV;
   		NcVar* stemcV;
   		NcVar* rootcV;
   		NcVar* vegnV;
   		NcVar* labnV;
   		NcVar* leafnV;
   		NcVar* stemnV;
   		NcVar* rootnV;

   		NcVar* gppV;
   		NcVar* nppV;
        NcVar* ltrfalcV;
        NcVar* ltrfalnV;

        NcVar* nuptakeV;

        //
   		NcVar* permafrostV;

   		NcVar* mossdzV;
   		NcVar* oshlwdzV;
   		NcVar* odeepdzV;
   		NcVar* mineadzV;
   		NcVar* minebdzV;
   		NcVar* minecdzV;

   		NcVar* oshlwcV;
   		NcVar* odeepcV;
   		NcVar* mineacV;
   		NcVar* minebcV;
   		NcVar* mineccV;
   		NcVar* orgnV;
   		NcVar* avlnV;

   		NcVar* ch4V;

   		NcVar* rhV;
   		NcVar* ch4fluxV;
   		NcVar* ch4flux2aV;
   		NcVar* ch4flux2pV;
   		NcVar* ch4flux2bV;
   		NcVar* netnminV;

      	NcVar* orgninputV;
   		NcVar* avlninputV;
   		NcVar* orgnlostV;
   		NcVar* avlnlostV;
      	NcVar* doclostV;

      	//
      	NcVar* eetV;
   		NcVar* petV;
   		NcVar* qinflV;
   		NcVar* qdrainV;
      	NcVar* qrunoffV;

      	NcVar* snwthickV;
      	NcVar* sweV;

   		NcVar* wtdV;
   		NcVar* alcV;
      	NcVar* aldV;

   		NcVar* vwcshlwV;
   		NcVar* vwcdeepV;
   		NcVar* vwcmineaV;
      	NcVar* vwcminebV;
      	NcVar* vwcminecV;

   		NcVar* tshlwV;
   		NcVar* tdeepV;
   		NcVar* tmineaV;
   		NcVar* tminebV;
   		NcVar* tminecV;

   		NcVar* hkshlwV;
   		NcVar* hkdeepV;
   		NcVar* hkmineaV;
   		NcVar* hkminebV;
   		NcVar* hkminecV;

   		NcVar* tcshlwV;
   		NcVar* tcdeepV;
   		NcVar* tcmineaV;
   		NcVar* tcminebV;
   		NcVar* tcminecV;

   		NcVar* tbotrockV;

   		//
   		NcVar* burnthickV;
   		NcVar* burnsoicV;
   		NcVar* burnvegcV;
   		NcVar* burnsoinV;
   		NcVar* burnvegnV;
   		NcVar* burnretaincV;
   		NcVar* burnretainnV;

   		OutDataRegn *regnod;
   		
};

#endif /*REGNOUTPUTER_H_*/
