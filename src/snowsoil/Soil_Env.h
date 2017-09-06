#ifndef SOIL_ENV_H_
#define SOIL_ENV_H_

#include "Stefan.h"
#include "Richards.h"
#include "TemperatureUpdator.h"

#include "../data/CohortData.h"
#include "../data/EnvData.h"
#include "../data/FirData.h"
#include "../data/RestartData.h"

#include "../inc/errorcode.h"
#include "../inc/parameters.h"
#include "../inc/layerconst.h"
#include "../lookup/CohortLookup.h"

#include "../ecodomain/Ground.h"

class Soil_Env {
public:

  Soil_Env();
  ~Soil_Env();

<<<<<<< HEAD
		int tstepmode;

		soipar_env envpar;
	
		Richards richards;
		Stefan stefan;
		TemperatureUpdator tempupdator;
=======
  soipar_env envpar;
>>>>>>> master

  Richards richards;
  Stefan stefan;
  TemperatureUpdator tempupdator;

  void setGround(Ground* grndp);
  void setCohortData(CohortData* cdp);
  void setEnvData(EnvData* edp);
  void setCohortLookup(CohortLookup * chtlup);

  void resetDiagnostic();   /*! reset diagnostic variables to initial values */

  void initializeParameter();
  void initializeState();
  void set_state_from_restartdata(const RestartData & rdata);

  void updateDailyGroundT(const double & tdrv, const double & dayl);
  void updateDailySM();

<<<<<<< HEAD
		void retrieveDailyTM(const double & tdrv, Layer* toplayer, Layer* lstsoill);
=======
  void getSoilTransFactor(double btran[MAX_SOI_LAY], Layer* fstsoill,
                          const double vrootfr[MAX_SOI_LAY]);
>>>>>>> master

  void retrieveDailyTM(Layer* toplayer, Layer* lstsoill);

<<<<<<< HEAD
		 CohortLookup* chtlu;
		 Ground * ground;

		 GridData *gd;
		 vegdiag_dim *cd_vegd;
		 soistate_dim *cd_soil;

		 atmstate_env *ed_atms;
		 atmdiag_env *ed_atmd;
		 atm2lnd_env *ed_a2l;
		 veg2atm_env *ed_v2a;
		 veg2gnd_env *ed_v2g;
		 snwstate_env *ed_snws;
		 snwdiag_env *ed_snwd;
		 snw2atm_env *ed_snw2a;
		 snw2soi_env *ed_snw2soi;
		 soistate_env *ed_sois;
		 soidiag_env *ed_soid;
		 soi2atm_env *ed_soi2a;
		 soi2lnd_env *ed_soi2l;
  
		 void updateDailySurfFlux(Layer* frontl, const double & dayl);
		 void updateDailySoilThermal4Growth(Layer* fstsoill, const double &tsurface);
		 void updateLayerStateAfterThermal(Layer* fstsoill, Layer *lstsoill, Layer* botlayer);
=======
private:
>>>>>>> master

  Ground * ground;
  CohortData * cd;
  EnvData * ed;
  CohortLookup* chtlu;

  void updateDailySurfFlux(Layer* frontl, const double & dayl);
  void updateDailySoilThermal4Growth(Layer* fstsoill, const double &tsurface);
  void updateLayerStateAfterThermal(Layer* fstsoill, Layer *lstsoill,
                                    Layer* botlayer);

<<<<<<< HEAD
=======
  void retrieveDailyFronts();

  double getEvaporation(const double & dayl, const double &rad);
  double getPenMonET(const double & ta, const double& vpd, const double &irad,
                     const double &rv, const double & rh);
  double getWaterTable(Layer* fstsoil);
  double getRunoff(Layer* fstsoill, Layer* drainl,
                   const double & rnth,
                   const double & melt);

  // the following codes not used anymore
  double getInflFrozen(Layer *fstminl, const double &  rnth,
                       const double & melt);
  double updateLayerTemp5Lat(Layer* currl, const double & infil);
>>>>>>> master

};

#endif /*SOIL_ENV_H_*/
