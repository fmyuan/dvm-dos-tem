#ifndef VEGETATION_BGC_H_
#define VEGETATION_BGC_H_
#include "../lookup/CohortLookup.h"
#include "../../include/ModelData.h"

#include "../data/CohortData.h"
#include "../data/EnvData.h"
#include "../data/FirData.h"
#include "../data/BgcData.h"
#include "../data/RestartData.h"

#include "../inc/parameters.h"

#include "../snowsoil/Soil_Bgc.h"

#include "../ecodomain/Vegetation.h"

#include <cmath>

<<<<<<< HEAD
	class Vegetation_Bgc{
  		public:
   			Vegetation_Bgc();
   			~Vegetation_Bgc();
  	
  			int tstepmode;
   			int ipft;
    		bool nfeed;
    		bool avlnflg;
    		bool baseline;
=======
class Vegetation_Bgc {
public:
  Vegetation_Bgc();
  ~Vegetation_Bgc();
>>>>>>> master

  int ipft;

  vegpar_cal calpar;
  vegpar_bgc bgcpar;

  vegstate_bgc tmp_vegs;

<<<<<<< HEAD
   			void initializeParameter();
    		void initializeState();
    		void initializeState5restart(RestartData *resin);
    
			void prepareIntegration();
  			void delta();
  			void deltanfeed();
  			void deltastate();
			void afterIntegration();
=======
  atm2veg_bgc del_a2v;
  veg2atm_bgc del_v2a;
  veg2soi_bgc del_v2soi;
  soi2veg_bgc del_soi2v;
  veg2veg_bgc del_v2v;
  vegstate_bgc del_vegs;
>>>>>>> master

  void initializeParameter();

  void initializeState();
  void set_state_from_restartdata(const RestartData & rdata);

  void prepareIntegration(const bool &nfeedback);
  void delta();
  void deltanfeed();
  void deltastate();
  void afterIntegration();

  //void adapt();
  void adapt_c2n_ratio_with_co2(const double & yreet, const double & yrpet,
      const double & initco2, const double & currentco2);

<<<<<<< HEAD
			double fracnuptake[MAX_SOI_LAY];  //fraction of N extraction in each soil layer for current PFT
			double fltrfall;                  //season fraction of max. monthly litter-falling fraction
			double dleafc;                    // C requirement of foliage growth at current time-step
		    double d2wdebrisc;
		    double d2wdebrisn;
			double totrzavln;     			  // root zone available N content for N uptake
=======
  void setCohortLookup(CohortLookup* chtlup);
>>>>>>> master

  void setCohortData(CohortData* cdp);
  void setEnvData(EnvData* edp);
  void setBgcData(BgcData* bdp);

<<<<<<< HEAD
  			CohortData *cd;
  			RegionData * rd;
  			vegstate_dim *cd_vegs;
  			vegdiag_dim *cd_vegd;
  			soistate_dim *cd_sois;

  			atmstate_env *ed_atms;
  			atmstate_env *edyly_atms;
  			atm2lnd_env *ed_a2l;
  			lnd2atm_env *ed_l2a;
  			lnd2atm_env *edyly_l2a;
  			soistate_env *ed_sois;
  			soidiag_env *ed_soid;

  			vegstate_bgc *bd_vegs;
  			vegdiag_bgc *bd_vegd;
  			atm2veg_bgc *bd_a2v;
  			veg2atm_bgc *bd_v2a;
  			veg2veg_bgc *bd_v2v;
  			soistate_bgc *bd_sois;
  			soidiag_bgc *bd_soid;
  			veg2soi_bgc *bd_v2soi;
  			soi2veg_bgc *bd_soi2v;
=======
  bool get_nfeed();
  void set_nfeed(bool);
  void set_nfeed(const std::string &value);
>>>>>>> master

private:

  bool nfeed; // NOTE: It seems like this is probably supposed to shadow
  // ModelData.nfeed. Not sure if this should be strictly
  // enforced somehow?


  //fraction of N extraction in each soil layer for current PFT
  double fracnuptake[MAX_SOI_LAY];

  double fltrfall; //season fraction of max. monthly litterfalling fraction
  double dleafc; // C requirement of foliage growth at current timestep
  double d2wdebrisc;
  double d2wdebrisn;

  CohortLookup * chtlu;

  CohortData * cd;
  EnvData * ed;
  BgcData * bd;

  void updateCNeven(const double & yreet,const double & yrpet,
                    const double & initco2,const double & currentco2 );

  double getGPP(const double &co2, const double & par,
                const double &leaf, const double & foliage,
                const double &ftemp, const double & gv);

  double getTempFactor4GPP(const double & tair, const double & tgppopt);
  double getGV(const double & eet,const double & pet );

  double getRm(const double & vegc,const double & raq10, const double &kr);

  /*!  rq10: effect of temperature on plant respiration, updated every month */
  double getRaq10(const double & tair);

  /*! kr: for calculating plant maintanence respiration*/
  double getKr(const double & vegc, const int & ipart);

  double getNuptake(const double & foliage, const double & raq10,
                    const double & kn1, const double & nmax);

};

#endif /*VEGETATION_BGC_H_*/
