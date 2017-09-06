#ifndef VEGETATION_H_
#define VEGETATION_H_
#include "../lookup/CohortLookup.h"

#include "../data/EnvData.h"
#include "../data/BgcData.h"
#include "../data/CohortData.h"
#include "../data/RestartData.h"

#include "../../include/ModelData.h"

#include "../inc/errorcode.h"
#include "../inc/parameters.h"

#include <cmath>

<<<<<<< HEAD
   			bool updateLAI5vegc;
   			int tstepmode;

   			void initializeParameter();
    		void initializeState();
    		void initializeState5restart(RestartData *resin);
    
    		void updateLai(const int & currmind, const int &currdinm);
    		void updateFpc();
    		void updateVegcov();
			void updateFrootfrac();

			void phenology(const int &currmind, const int &currdinm);

			void setCohortLookup(CohortLookup* chtlup);
			void setCohortData(CohortData * cdp);
			void setEnvData(const int &ip, EnvData * edp);
			void setBgcData(const int &ip, BgcData * bdp);
=======
class Vegetation {
public:
  Vegetation();
  
  /** */
  Vegetation(int cmtnum, const ModelData* mdp);

  ~Vegetation();

  vegpar_dim vegdimpar;

  bool updateLAI5vegc;

  void initializeParameter();
  void initializeState();
>>>>>>> master

  void set_state_from_restartdata(const RestartData & rd);

<<<<<<< HEAD
  			vegstate_dim *cd_vegs;
  			vegdiag_dim *cd_vegd;

  			vegstate_env *ed_vegs[NUM_PFT];
  			vegdiag_env *ed_vegd[NUM_PFT];
  			soidiag_env *ed_soid[NUM_PFT];
            veg2atm_env *ed_v2a[NUM_PFT];
            atmstate_env *ed_atms[NUM_PFT];

  			vegstate_bgc *bd_vegs[NUM_PFT];
  			vegdiag_bgc *bd_vegd[NUM_PFT];
=======
  void updateLai(const int & currmind);
  void updateFpc();
  void updateVegcov();
  void updateFrootfrac();
>>>>>>> master

  void phenology(const int &currmind);

  void setCohortLookup(CohortLookup* chtlup);
  void setCohortData(CohortData * cdp);

  void setEnvData(const int &ip, EnvData * edp);
  void setBgcData(const int &ip, BgcData * bdp);

private:
  CohortLookup * chtlu;
  CohortData * cd;

  EnvData * ed[NUM_PFT];
  BgcData * bd[NUM_PFT];

  double getFleaf(const int &ipft, const double & unnormleaf,
                  const double &prvunnormleafmx);
  double getUnnormleaf(const int& ipft, double &prveetmx,
                       const double & eet, const double & prvunleaf);
  double getFfoliage(const int &ipft, const bool & ifwoody,
                     const bool &ifperenial, const double &vegc);
  double getYearlyMaxLAI(const int &ipft);

};

#endif /*VEGETATION_H_*/
