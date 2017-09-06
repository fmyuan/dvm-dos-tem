#ifndef COHORT_H_
#define COHORT_H_

#include "../../include/Climate.h"

#include "../ecodomain/Ground.h"
#include "../ecodomain/Vegetation.h"

#include "../vegetation/Vegetation_Env.h"
#include "../vegetation/Vegetation_Bgc.h"

#include "../snowsoil/Snow_Env.h"
#include "../snowsoil/Soil_Env.h"
#include "../snowsoil/SoilParent_Env.h"
#include "../snowsoil/Soil_Bgc.h"

#include "../disturb/WildFire.h"

#include "../data/CohortData.h"

#include "../data/EnvData.h"
#include "../data/BgcData.h"
#include "../data/FirData.h"

#include "../data/RestartData.h"

#include "../lookup/CohortLookup.h"

#include "Integrator.h"

// headers for run
<<<<<<< HEAD
	#include "Timer.h"
	#include "ModelData.h"
	#include "OutRetrive.h"

	class Cohort{
		public :
			Cohort();
			~Cohort();
	
	        // model running status
			int errorid;
			bool failed;    // when an exception is caught, set failed to be true
	
 			//
 			Timer * timer;

 			//inputs
			CohortLookup chtlu;

			// domain
            Atmosphere atm;
            Vegetation veg;
            Ground ground;

            // processes
            Vegetation_Env vegenv[NUM_PFT];
            Snow_Env snowenv;
            Soil_Env soilenv;
            SoilParent_Env solprntenv;

            Vegetation_Bgc vegbgc[NUM_PFT];
            Soil_Bgc soilbgc;

            WildFire fire;

            // output data buffer
            OutRetrive outbuffer;

		// data
            EnvData ed[NUM_PFT];
            BgcData bd[NUM_PFT];
            EnvData * edall;
            BgcData * bdall;

            FirData * fd;   // this for all PFTs and their soil
    
            ModelData * md;
            RegionData * rd;
            GridData * gd;

            CohortData cd;
            RestartData resid;    //for input

 		    void initSubmodules();
 		    void setTime(Timer * timerp);

 		    void setModelData(ModelData* md);
 		    void setInputData(RegionData * rd, GridData * gd);
 		    void setProcessData(EnvData * alledp, BgcData * allbdp, FirData *fdp);

 		    void initStatePar();
 		    void prepareAllDrivingData();
 	        void prepareDayDrivingData(const int & yrcnt, const int &usedatmyr);
 		    void updateOneTimestep(const int & yrcnt, const int & currmind, const int & dinmcurr);

	private:

            Integrator vegintegrator[NUM_PFT];
            Integrator solintegrator;

     	    void updateDIMveg(const int & currmind, const int & currdinm, const bool & dvmmodule);
     	    void updateDIMgrd(const int & currmind, const int & currdinm, const bool & dslmodule);

     	    void updateEnv(const int & currmind, const int & currdinm);
 	 	    void updateBgc(const int & currmind, const int & currdinm);
     	    void updateFir(const int & yrcnt, const int & currmind);

		    // update root distribution
		    void getSoilFineRootFrac(vegstate_dim *cd_veg, soistate_dim *cd_soil);
		    double assignSoilLayerRootFrac(const double & topz, const double & botz,
		           const double csumrootfrac[MAX_ROT_LAY], const double dzrotlay[MAX_ROT_LAY]);

		   //
     	   void assignAtmEd2pfts_daily();
     	   void assignGroundEd2pfts_daily();
		   void getSoilTransfactor4all_daily();
		   void getEd4allveg_daily();
     	   void getEd4land_daily();

     	   //
     	   void assignSoilBd2pfts();
     	   void getBd4allveg();
=======
#include "../../include/ModelData.h"

class Cohort {
public :
  Cohort();
  Cohort(int y, int x, ModelData* modeldatapointer);
  ~Cohort();
  
  int y;
  int x;

  float lon;
  float lat;

  // model running status
  int errorid;
  bool failed;    // when an exception is caught, set failed to be true

  /*
    Note: FRI is a member of CohortData because it is checked in
    Soil_Bgc::prepareintegration(...), and at that point there is no access to
    the members/fields of a Cohort...
  */

  // old? can I deprecate these??
  //double pfsize[NUM_FSIZE];
  //double pfseason[NUM_FSEASON];
  
  //inputs
  CohortLookup chtlu;

  // domain
  Vegetation veg;
  Ground ground;
  
  // new domain
  Climate climate;

  // processes
  Vegetation_Env vegenv[NUM_PFT];
  Snow_Env snowenv;
  Soil_Env soilenv;
  SoilParent_Env solprntenv;

  Vegetation_Bgc vegbgc[NUM_PFT];
  Soil_Bgc soilbgc;

  WildFire fire;

  // data
  EnvData ed[NUM_PFT];
  BgcData bd[NUM_PFT];
  EnvData * edall;
  BgcData * bdall;

  FirData year_fd[12]; //Monthly fire data, for all PFTs and soil
  FirData * fd;   //Fire data for an individual month 

  ModelData * md;

  CohortData cd;
  RestartData restartdata;
  

//  void NEW_load_climate_from_file(int y, int x);
//  void NEW_load_veg_class_from_file(int y, int x);
//  void NEW_load_fire_from_file(int y, int x);

  void initialize_internal_pointers();

  void setModelData(ModelData* md);
  void setProcessData(EnvData * alledp, BgcData * allbdp, FirData *fdp);

  void initialize_state_parameters();
  //void prepareAllDrivingData();
  //void prepareDayDrivingData(const int & yrcnt, const int &usedatmyr);
  void updateMonthly(const int & yrcnt, const int & currmind,
                     const int & dinmcurr, std::string stage);
  
  void set_state_from_restartdata();
  void set_restartdata_from_state();

  void load_proj_climate(std::string&);//Provides data to Climate for loading proj data

private:

  Integrator vegintegrator[NUM_PFT];
  Integrator solintegrator;


  void updateMonthly_DIMveg(const int & currmind, const bool & dvmmodule);
  void updateMonthly_DIMgrd(const int & currmind, const bool & dslmodule);

  void updateMonthly_Env(const int & currmind, const int & dinmcurr);
  void updateMonthly_Bgc(const int & currmind);
  void updateMonthly_Dsb(const int & yrcnt, const int & currmind, std::string stage);

  // Fire is a type of disturbance
  void updateMonthly_Fir(const int & year, const int & midx, std::string stage);

  // update root distribution
  void getSoilFineRootFrac_Monthly();
  double assignSoilLayerRootFrac(const double & topz, const double & botz,
                                 const double csumrootfrac[MAX_ROT_LAY],
                                 const double dzrotlay[MAX_ROT_LAY]);

  //
  void assignAtmEd2pfts_daily();
  void assignGroundEd2pfts_daily();
  void getSoilTransfactor4all_daily();
  void getEd4allveg_daily();
  void getEd4land_daily();

  void assignSoilBd2pfts_monthly();
  void getBd4allveg_monthly();
>>>>>>> master

};
#endif /*COHORT_H_*/
