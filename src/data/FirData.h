#ifndef FIRDATA_H_
#define FIRDATA_H_
/*! this class contains the fire at annually time steps.
*/
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>

#include "../inc/diagnostics.h"
#include "../inc/fluxes.h"
#include "../inc/states.h"
#include "../inc/timeconst.h"

class FirData {
public:

<<<<<<< HEAD
        bool useseverity;
=======
  FirData();
  ~FirData();

  void clear();
>>>>>>> master

  bool useseverity;

  soidiag_fir fire_soid;

  veg2atm_fir fire_v2a;
  veg2soi_fir fire_v2soi;
  veg2dead_fir fire_v2dead;

  soi2atm_fir fire_soi2a;
  atm2soi_fir fire_a2soi;

  void init();
  void beginOfYear();
  void endOfYear();
  void beginOfMonth();
  void burn();
  
<<<<<<< HEAD
        void clear();
        void beginOfYear();
    
=======
  std::string report_to_string(const std::string& msg);


>>>>>>> master
};

#endif /*FIRDATA_H_*/
