/*! this class contains the bgc at certain time steps.
 * 
 */
#ifndef ONEBGC_H_
#define ONEBGC_H_

#include "../inc/errorcode.h"
#include "../inc/diagnostics.h"
#include "../inc/fluxes.h"
#include "../inc/states.h"

#include <math.h>

#include <deque>
using namespace std;

class onebgc{
 	public:
  		onebgc();
  		~onebgc();

  		void clear();
	
		vegstate_bgc bgc_vegs;
		soistate_bgc bgc_sois;
		vegdiag_bgc bgc_vegd;
		soidiag_bgc bgc_soid;
		lnd2atm_bgc bgc_l2a;
		atm2veg_bgc bgc_a2v;
		veg2atm_bgc bgc_v2a;
		veg2soi_bgc bgc_v2soi;
		soi2veg_bgc bgc_soi2v;
		veg2veg_bgc bgc_v2v;
		soi2lnd_bgc bgc_soi2l;
		soi2atm_bgc bgc_soi2a;
		atm2soi_bgc bgc_a2soi;
		soi2soi_bgc bgc_soi2soi;

		deque <double> prvltrfcnque[MAX_SOI_LAY];

    	void landstates_zeroing();
    	void landfluxes_zeroing();

    	void vegstates_zeroing();
    	void vegfluxes_zeroing();

    	void soilstates_zeroing();
    	void soilfluxes_zeroing();
    
    private:

};

#endif /*ONEBGC_H_*/
