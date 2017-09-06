#ifndef SNOW_ENV_H_
#define SNOW_ENV_H_

#include "../inc/errorcode.h"
#include "../data/CohortData.h"
#include "../data/EnvData.h"
#include "../data/RestartData.h"
#include "../ecodomain/Ground.h"
#include "../lookup/CohortLookup.h"

class Snow_Env {
public:
  Snow_Env();
  ~Snow_Env();

<<<<<<< HEAD
		int tstepmode;
=======
  double wind;     /*! the wind speed class (0, 1)? of a vegetation type*/
>>>>>>> master

  // parameters for snow physics
  snwpar_env snowenvpar;

  void setGround(Ground* grndp);

  void setCohortLookup(CohortLookup* chtlup);
  void setCohortData(CohortData* cdp);
  void setEnvData(EnvData* edp);

  void initializeParameter();
  void initializeNewSnowState();
  void set_state_from_restartdata(const RestartData & rdata);

  void updateDailyM(const double & tdrv);
  void checkSnowLayersT(Layer* frontl);

private:

<<<<<<< HEAD
	    CohortLookup * chtlu;

	    vegdiag_dim *cd_vegd;

	    atm2lnd_env *ed_a2l;
		veg2gnd_env *ed_v2g;
		snwstate_env *ed_snws;
		snwdiag_env *ed_snwd;
		snw2atm_env *ed_snw2a;
		snw2soi_env *ed_snw2soi;
		soidiag_env *ed_soid;
		soi2atm_env *ed_soi2a;
		soi2lnd_env *ed_soi2l;
=======
  CohortLookup * chtlu;
  CohortData * cd;
  EnvData * ed;
>>>>>>> master

  Ground * ground;

  void updateSnowEd(Layer * frontl);
  double meltSnowLayersAfterT(Layer * frontl);

  void updateDailySurfFlux( Layer* frontl, const double & tdrv);

  double getSublimation(double const & rn, double const & swe,
                        double const & ta);

  double getAlbedoVis(const double & tem); //get albedo of visible
                                           //  radition of snow
  double getAlbedoNir(const double & tem); //get albedo of Nir radition of snow

};

#endif /*SNOW_ENV_H_*/
