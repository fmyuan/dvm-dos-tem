#ifndef ONEENV_H_
#define ONEENV_H_

#include "../inc/diagnostics.h"
#include "../inc/fluxes.h"
#include "../inc/states.h"

#include "../inc/errorcode.h"
#include "../inc/layerconst.h"
#include "../inc/timeconst.h"
#include "../inc/physicalconst.h"

#include <deque>
using namespace std;

class oneenv{
  public:
  	oneenv();
  	~oneenv();

  	void clear();

  	snwstate_dim dim_snow;

    atmstate_env env_atms;  // last 's' - state variable
    vegstate_env env_vegs;
    snwstate_env env_snws;
    soistate_env env_sois;
    
    atmdiag_env env_atmd;   // last 'd' - diagnostic variable
    vegdiag_env env_vegd;
    snwdiag_env env_snwd;
    soidiag_env env_soid;
    
    lnd2atm_env env_l2a;    // 'l' - land, '2' - 'to', 'a' - atm
    atm2lnd_env env_a2l;
    atm2veg_env env_a2v;    // 'v' - veg
    veg2atm_env env_v2a;
    veg2gnd_env env_v2g;    // 'g' - ground
    soi2lnd_env env_soi2l;  // 'soi' - soil
    soi2atm_env env_soi2a;
    snw2atm_env env_snw2a;  // 'snw' - snow
    snw2soi_env env_snw2soi;
    
    double monthsfrozen;      // months since bottom soil frozen started - 24 months is the criterial for permafrost
	int rtfrozendays;         // soil top rootzone continously frozen days
	int rtunfrozendays;       // soil top rootzone continously unfrozen days
     
    void atmstates_zeroing();
    void atmfluxes_zeroing();

    void vegstates_zeroing();
    void vegfluxes_zeroing();

    void grndstates_zeroing();
    void grndfluxes_zeroing();

  private:
	
};

#endif /*ONEENV_H_*/
