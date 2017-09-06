/*
 * Vegetation.cpp
 *
 * Purpose: Defining vegetation structure
 *
 * History:
 *   June 28, 2011, by F.-M. Yuan:
 *     (1) added for constructing vegetation domain (plant community)
 *
 * Important:
 *   (1) Parameters are read from 'CohortLookup.cpp', and set to
 *       'vegdimpar' (struct::vegpar_dim)
 *   (2) Here, all functions are on ALL pfts for ONE community. In this way,
 *       some complicated PFT interaction and dynamics/structure changing
 *       may be put here in the future.
 *
 */

#include "Vegetation.h"

<<<<<<< HEAD
Vegetation::Vegetation(){
	updateLAI5vegc = false;
	tstepmode = DAILY;
	chtlu = NULL;
	cd = NULL;
	cd_vegd = NULL;
	cd_vegs = NULL;
};
=======
#include "../TEMUtilityFunctions.h"
>>>>>>> master

#include "../TEMLogger.h"
extern src::severity_logger< severity_level > glg;

Vegetation::Vegetation() {


}

/** New style constructor. Building the right thing.

   Since we have the modelData (for config directory/file) and the cmtnumber
   we can lookup the appropriate values from the configuration file.
 */
Vegetation::Vegetation(int cmtnum, const ModelData* mdp) {


  // This seems horribly brittle now as it really depends on the order and
  // presence of the lines in the parameter file...

  BOOST_LOG_SEV(glg, note) << "Vegetation constructor. Community type: " << cmtnum;

  BOOST_LOG_SEV(glg, note) << "Setting Vegetation internal values from file: "
                           << mdp->parameter_dir << "cmt_dimvegetation.txt";


  // MAYBE WE DON'T NEED TO SET PARAMETERS HERE? -
  // They are set later in CohortLookup, and then read into the veg structure.

  // get a list of data for the cmt number
  std::list<std::string> l = temutil::parse_parameter_file(
      mdp->parameter_dir + "cmt_dimvegetation.txt", cmtnum, 40
  );


  // FIX THIS?? Why are some parameters not being set?? (ifwoody, etc..)

  // pop each line off the front of the list
  // and assign to the right data member.
  temutil::pfll2data_pft(l, vegdimpar.cov);
  l.pop_front(); // ifwoody
  l.pop_front(); // ifdeciwoody
  l.pop_front(); // ifperenial
  l.pop_front(); // nonvascular
  temutil::pfll2data_pft(l, vegdimpar.sla);
  temutil::pfll2data_pft(l, vegdimpar.klai);
  temutil::pfll2data_pft(l, vegdimpar.minleaf);
  temutil::pfll2data_pft(l, vegdimpar.aleaf);
  temutil::pfll2data_pft(l, vegdimpar.bleaf);
  temutil::pfll2data_pft(l, vegdimpar.cleaf);
  temutil::pfll2data_pft(l, vegdimpar.kfoliage);
  l.front();
  temutil::pfll2data_pft(l, vegdimpar.cov);
  temutil::pfll2data_pft(l, vegdimpar.m1);
  temutil::pfll2data_pft(l, vegdimpar.m2);
  temutil::pfll2data_pft(l, vegdimpar.m3);
  temutil::pfll2data_pft(l, vegdimpar.m4);

//  for (int i = 0; i < MAX_ROT_LAY; i++) {
//    temutil::pfll2data_pft(l, vegdimpar.frootfrac[i]);
//  }
//
//  temutil::pfll2data_pft(l, vegdimpar.lai);
//
//  for (int im = 0; im < MINY; im++) {
//    temutil::pfll2data_pft( l, vegdimpar.envlai[im]);
//  }
}

Vegetation::~Vegetation() {
}

/**
 Set the bgc parameters from inputs stored in 'chtlu' - reuseable
 Note: here will remove those PFT with no greater than zero 'fpc'
       and initialize the total actual pft number
*/
void Vegetation::initializeParameter() {

  // This should probably be in the Ctor for a Vegetation object.
  
  for (int ip=0; ip<NUM_PFT; ip++) {
    // This will remove those PFTs with 0 ground coverage. So be cautious
    // the index consistent with 'state' variables!!
    if (chtlu->vegcov[ip] > 0.0) {
      vegdimpar.sla[ip] = chtlu->sla[ip];
      vegdimpar.klai[ip] = chtlu->klai[ip];
      vegdimpar.minleaf[ip] = chtlu->minleaf[ip];
      vegdimpar.aleaf[ip] = chtlu->aleaf[ip];
      vegdimpar.bleaf[ip] = chtlu->bleaf[ip];
      vegdimpar.cleaf[ip] = chtlu->cleaf[ip];
      vegdimpar.kfoliage[ip] = chtlu->kfoliage[ip];
      vegdimpar.cov[ip] = chtlu->cov[ip];
      vegdimpar.m1[ip] = chtlu->m1[ip];
      vegdimpar.m2[ip] = chtlu->m2[ip];
      vegdimpar.m3[ip] = chtlu->m3[ip];
      vegdimpar.m4[ip] = chtlu->m4[ip];
    }
  }
}

// set the initial states from inputs
void Vegetation::initializeState() {
  //
  for (int i=0; i<NUM_PFT; i++) {
    cd->m_veg.vegage[i]  = 0;
  }

  // from 'lookup'
  cd->hasnonvascular = false;

  for (int i=0; i<NUM_PFT; i++) {
    if (chtlu->vegcov[i] > 0.) {
      cd->m_veg.vegcov[i]      = chtlu->vegcov[i];
      cd->m_veg.ifwoody[i]     = chtlu->ifwoody[i];
      cd->m_veg.ifdeciwoody[i] = chtlu->ifdeciwoody[i];
      cd->m_veg.ifperenial[i]  = chtlu->ifperenial[i];
      cd->m_veg.nonvascular[i] = chtlu->nonvascular[i];

      if (cd->m_veg.nonvascular[i]>0) {  //checking and resetting
        cd->m_veg.ifwoody[i]     = 0;
        cd->m_veg.ifdeciwoody[i] = 0;
        cd->m_veg.ifperenial[i]  = 0;
      }

      if (cd->m_veg.nonvascular[i] > 0) {
        cd->hasnonvascular = true;
      }

      cd->m_veg.lai[i] = chtlu->lai[i];

      for (int il=0; il<MAX_ROT_LAY; il++) {
        cd->m_veg.frootfrac[il][i] = chtlu->frootfrac[il][i]/100.0; // chtlu - in %
      }
    }
  }

  updateFpc();
  updateFrootfrac();
};

<<<<<<< HEAD
// set the initial states from inputs
void Vegetation::initializeState(){

	//
	for (int i=0; i<NUM_PFT; i++){
		cd_vegs->vegage[i]  = 0;
	}
=======
//set the initial states from restart inputs:
void Vegetation::set_state_from_restartdata(const RestartData & rd) {
  for (int ip=0; ip<NUM_PFT; ip++) {
    cd->m_veg.vegage[ip]      = rd.vegage[ip];
    cd->m_veg.vegcov[ip]      = rd.vegcov[ip];
    cd->m_veg.ifwoody[ip]     = rd.ifwoody[ip];
    cd->m_veg.ifdeciwoody[ip] = rd.ifdeciwoody[ip];
    cd->m_veg.ifperenial[ip]  = rd.ifperenial[ip];
    cd->m_veg.nonvascular[ip] = rd.nonvascular[ip];
    cd->m_veg.lai[ip]         = rd.lai[ip];

    for (int il=0; il<MAX_ROT_LAY; il++) {
      cd->m_veg.frootfrac[il][ip] = rd.rootfrac[il][ip];
    }
>>>>>>> master

    cd->m_vegd.eetmx[ip]        = rd.eetmx[ip];
    cd->m_vegd.unnormleafmx[ip] = rd.unnormleafmx[ip];
    cd->m_vegd.growingttime[ip] = rd.growingttime[ip];
    cd->m_vegd.topt[ip]         = rd.topt[ip];
    cd->m_vegd.foliagemx[ip]    = rd.foliagemx[ip];
    cd->prveetmxque[ip].clear();

<<<<<<< HEAD
    for (int i=0; i<NUM_PFT; i++) {
    	if (chtlu->vegcov[i] > 0.){
    		cd_vegs->vegcov[i]      = chtlu->vegcov[i];

    		cd_vegs->ifwoody[i]     = chtlu->ifwoody[i];
    		cd_vegs->ifdeciwoody[i] = chtlu->ifdeciwoody[i];
    		cd_vegs->ifperenial[i]  = chtlu->ifperenial[i];

    		cd_vegs->nonvascular[i] = chtlu->nonvascular[i];
    		if (cd_vegs->nonvascular[i]>0) {  //checking and resetting
        		cd_vegs->ifwoody[i]     = 0;
        		cd_vegs->ifdeciwoody[i] = 0;
        		cd_vegs->ifperenial[i]  = 0;
    		}

    		if (cd_vegs->nonvascular[i] > 0) cd->hasnonvascular = true;

    		cd_vegs->lai[i]         = chtlu->lai[i];

    		cd_vegs->aerenchyma[i]  = chtlu->aerenchyma[i];

    		for (int il=0; il<MAX_ROT_LAY; il++) {
    			cd_vegs->frootfrac[il][i] = chtlu->frootfrac[il][i]/100.;   //chtlu - in %
    		}
=======
    for(int i=0; i<10; i++) {
      double eetmxa = rd.eetmxA[i][ip];
      // note: older value is in the lower position in the deque

      if(eetmxa!=MISSING_D) {
        cd->prveetmxque[ip].push_back(eetmxa);
      }
    }

    cd->prvunnormleafmxque[ip].clear();

    for(int i=0; i<10; i++) {
      double unleafmxa = rd.unnormleafmxA[i][ip];
      // note: older value is in the lower position in the deque

      if(unleafmxa!=MISSING_D) {
        cd->prvunnormleafmxque[ip].push_back(unleafmxa);
      }
    }

    cd->prvgrowingttimeque[ip].clear();
>>>>>>> master

    for(int i=0; i<10; i++) {
      double growingttimea = rd.growingttimeA[i][ip];
      //note: older value is lower in the deque

      if(growingttimea!=MISSING_D) {
        cd->prvgrowingttimeque[ip].push_back(growingttimea);
      }
    }

    cd->toptque[ip].clear();

<<<<<<< HEAD
//set the initial states from restart inputs:
void Vegetation::initializeState5restart(RestartData *resin){

    for (int ip=0; ip<NUM_PFT; ip++) {
   		cd_vegs->vegage[ip]      = resin->vegage[ip];

    	cd_vegs->vegcov[ip]      = resin->vegcov[ip];
    	cd_vegs->ifwoody[ip]     = resin->ifwoody[ip];
    	cd_vegs->ifdeciwoody[ip] = resin->ifdeciwoody[ip];
    	cd_vegs->ifperenial[ip]  = resin->ifperenial[ip];
    	cd_vegs->nonvascular[ip] = resin->nonvascular[ip];
    	cd_vegs->lai[ip]         = resin->lai[ip];
    	cd_vegs->aerenchyma[ip]  = resin->aerenchyma[ip];

    	for (int il=0; il<MAX_ROT_LAY; il++) {
          cd_vegs->frootfrac[il][ip] = resin->rootfrac[il][ip];
    	}

    	cd_vegd->eetmx[ip]        = resin->eetmx[ip];
    	cd_vegd->unnormleafmx[ip] = resin->unnormleafmx[ip];
    	cd_vegd->growingttime[ip] = resin->growingttime[ip];
    	cd_vegd->topt[ip]         = resin->topt[ip];
    	cd_vegd->foliagemx[ip]    = resin->foliagemx[ip];

   		cd->prveetmxque[ip].clear();
       	for(int i=0; i<10; i++){
    		double eetmxa = resin->eetmxA[i][ip];    //note: older value is in the lower position in the deque
    		if(eetmxa!=MISSING_D){
    			cd->prveetmxque[ip].push_back(eetmxa);
    		}
    	}

		cd->prvunnormleafmxque[ip].clear();
    	for(int i=0; i<10; i++){
    		double unleafmxa = resin->unnormleafmxA[i][ip];    //note: older value is in the lower position in the deque
    		if(unleafmxa!=MISSING_D){
    			cd->prvunnormleafmxque[ip].push_back(unleafmxa);
    		}
    	}

		cd->prvgrowingttimeque[ip].clear();
    	for(int i=0; i<10; i++){
    		double growingttimea = resin->growingttimeA[i][ip];    //note: older value is, lower in the deque
    		if(growingttimea!=MISSING_D){
    			cd->prvgrowingttimeque[ip].push_back(growingttimea);
    		}
    	}

		cd->toptque[ip].clear();
    	for(int i=0; i<10; i++){
    		double topta = resin->toptA[i][ip];    //note: older value is, lower in the deque
    		if(topta!=MISSING_D){
    			cd->toptque[ip].push_back(topta);
    		}

    	}
=======
    for(int i=0; i<10; i++) {
      double topta = rd.toptA[i][ip];
      //note: older value is, lower in the deque
>>>>>>> master

      if(topta!=MISSING_D) {
        cd->toptque[ip].push_back(topta);
      }
    }
  }

  updateFpc();
  updateFrootfrac();
};

// must be called after 'foliage' and 'leaf' updated
<<<<<<< HEAD
void Vegetation::updateLai(const int &currmind, const int &currdinm){
	for(int ip=0; ip<NUM_PFT; ip++)	{
    	if (cd_vegs->vegcov[ip]>0.){
    		if(!updateLAI5vegc){
    			cd_vegs->lai[ip] = chtlu->envlai[currmind][ip];     //So, this will give a portal for input LAI
                if (tstepmode==DAILY) {   // need to interpolate for daily simulation
                	double curlai = chtlu->envlai[currmind][ip];
                	double prelai = 0.;
                	if(currmind>0) {
                		prelai=chtlu->envlai[currmind-1][ip];
                	} else {
                		prelai=chtlu->envlai[MINY-1][ip];
                	}
                	// assuming 'prelai'at the first day and 'curlai' at the last day of month
                	cd_vegs->lai[ip] = prelai + (curlai-prelai)/DINM[currmind]*(currdinm+1);
                }
    		}else {
    			if (bd_vegs[ip]->c[I_leaf] > 0.) {
    				cd_vegs->lai[ip] = vegdimpar.sla[ip] * bd_vegs[ip]->c[I_leaf];
    			} else {
    				if (cd_vegd->growingttime[ip] == 0.0) {
    					cd_vegs->lai[ip] = 0.001;   // this is needed for leaf emerging of annual or deciduous species
    				}
    			}
    		}
    	}
	}
};

// sum of all PFTs' fpc must be not greater than 1.0
void Vegetation::updateFpc(){
	double fpcmx = 0.;
	double fpcsum = 0.;
	double fpc[NUM_PFT];
	for(int ip=0; ip<NUM_PFT; ip++)	{

		if (cd_vegs->vegcov[ip]>0.){

			double ilai = cd_vegs->lai[ip];
			fpc[ip] = 1.0 - exp(-vegdimpar.klai[ip] * ilai);
			if (fpc[ip]>fpcmx) {
				fpcmx = fpc[ip];
			}
			fpcsum +=fpc[ip];
			cd_vegs->fpc[ip] = fpc[ip];
    	}

	}

	if (fpcsum > 1.0) {
		for(int ip=0; ip<NUM_PFT; ip++)	{
	    	if (cd_vegs->vegcov[ip]>0.){
	    		cd_vegs->fpc[ip] /= fpcsum;
	    	}
		}
		fpcsum = 1.0;
	}
	cd_vegd->fpcsum = fpcsum;
=======
void Vegetation::updateLai(const int &currmind) {
  for(int ip=0; ip<NUM_PFT; ip++) {
    if (cd->m_veg.vegcov[ip]>0.) {
      if(!updateLAI5vegc) {
        cd->m_veg.lai[ip] = chtlu->envlai[currmind][ip];//So, this will give a
                                                        //  portal for input LAI
      } else {
        if (bd[ip]->m_vegs.c[I_leaf] > 0.) {
          cd->m_veg.lai[ip] = vegdimpar.sla[ip] * bd[ip]->m_vegs.c[I_leaf];
        } else {
          if (ed[ip]->m_soid.rtdpgrowstart>0 && ed[ip]->m_soid.rtdpgrowend<0) {
            cd->m_veg.lai[ip] = 0.001; // this is needed for leaf emerging
          }
        }
      }
    }
  }
};

// sum of all PFTs' fpc must be not greater than 1.0
void Vegetation::updateFpc() {
  double fpcmx = 0.;
  double fpcsum = 0.;
  double fpc[NUM_PFT];

  for(int ip=0; ip<NUM_PFT; ip++) {
    if (cd->m_veg.vegcov[ip]>0.) {
      double ilai = cd->m_veg.lai[ip];
      fpc[ip] = 1.0 - exp(-vegdimpar.klai[ip] * ilai);

      if (fpc[ip]>fpcmx) {
        fpcmx = fpc[ip];
      }

      fpcsum +=fpc[ip];
      cd->m_veg.fpc[ip] = fpc[ip];
    }
  }

  if (fpcsum > 1.0) {
    for(int ip=0; ip<NUM_PFT; ip++) {
      if (cd->m_veg.vegcov[ip]>0.) {
        cd->m_veg.fpc[ip] /= fpcsum;
      }
    }

    fpcsum = 1.0;
  }
>>>>>>> master

  cd->m_vegd.fpcsum = fpcsum;
};

// vegetation coverage update (note - this is not same as FPC)
<<<<<<< HEAD
// and Here it's simply assumed as the max. foliage coverage projected on ground throughout the whole plant lift-time
// shall be more working on this in future
void Vegetation::updateVegcov(){
	double foliagecov = 0.;
	cd->hasnonvascular = false;
	for(int ip=0; ip<NUM_PFT; ip++)	{
		double ilai = cd_vegs->lai[ip];
		foliagecov = 1.0 - exp(-vegdimpar.klai[ip] * ilai);
		if (cd_vegs->vegcov[ip]<foliagecov) {
			cd_vegs->vegcov[ip]=foliagecov;
		}

		if (cd_vegs->vegcov[ip]>1.e-5) {
			cd_vegs->ifwoody[ip]     = chtlu->ifwoody[ip];
			cd_vegs->ifdeciwoody[ip] = chtlu->ifdeciwoody[ip];
			cd_vegs->ifperenial[ip]  = chtlu->ifperenial[ip];
			cd_vegs->nonvascular[ip] = chtlu->nonvascular[ip];

			if (cd_vegs->nonvascular[ip] > 0) cd->hasnonvascular = true;
		}
	}
=======
// and Here it's simply assumed as the max. foliage coverage projected on
//   ground throughout the whole plant lift-time shall be more working on
//   this in future
void Vegetation::updateVegcov() {
  double foliagecov = 0.;
  cd->hasnonvascular = false;

  for(int ip=0; ip<NUM_PFT; ip++) {
    double ilai = cd->m_veg.lai[ip];
    foliagecov = 1.0 - exp(-vegdimpar.klai[ip] * ilai);

    if (cd->m_veg.vegcov[ip]<foliagecov) {
      cd->m_veg.vegcov[ip]=foliagecov;
    }

    if (cd->m_veg.vegcov[ip]>1.e-5) {
      cd->m_veg.ifwoody[ip]     = chtlu->ifwoody[ip];
      cd->m_veg.ifdeciwoody[ip] = chtlu->ifdeciwoody[ip];
      cd->m_veg.ifperenial[ip]  = chtlu->ifperenial[ip];
      cd->m_veg.nonvascular[ip] = chtlu->nonvascular[ip];
>>>>>>> master

      if (cd->m_veg.nonvascular[ip] > 0) {
        cd->hasnonvascular = true;
      }
    }
  }
};

<<<<<<< HEAD
//leaf phenology - moved from 'Vegetation_Bgc.cpp' for easy modification, if needed in the future
void Vegetation::phenology(const int &currmind, const int &currdinm){

	for(int ip=0; ip<NUM_PFT; ip++)	{

		if (cd_vegs->vegcov[ip]>0.){

			// vegetation standing age
			// tentatively set to a common age from 'ysf' - year since fire
			// (TODO) should have more variability based on PFT types
			if (currmind == MINY-1 && currdinm == DINM[currmind]) { // at the last-day of last month
				cd_vegs->vegage[ip] = cd->yrsdist;
				if (cd_vegs->vegage[ip]<=0) cd_vegd->foliagemx[ip] = 0.;
			}

			double prvunnormleafmx = 0.;   // previous 10 years' average as below
			deque <double> prvdeque = cd->prvunnormleafmxque[ip];
			int dequeno = prvdeque.size();
			for (int i=0; i<dequeno; i++) {
				prvunnormleafmx +=prvdeque[i]/dequeno;
			}

			double prveetmx=0;
			prvdeque = cd->prveetmxque[ip];
			dequeno = prvdeque.size();
			for (int i=0; i<dequeno; i++) {
				prveetmx +=prvdeque[i]/dequeno;
			}

			// 1) current EET and previous max. EET controlled
			double tempunnormleaf = 0.;;
			double eet = ed_v2a[ip]->tran;  //originally it's using 'l2a.eet', which includes soil/veg evaporation - that may not relate to leaf phenology
			tempunnormleaf = getUnnormleaf(ip, prveetmx, eet, cd_vegd->unnormleaf[ip]);
			cd_vegd->unnormleaf[ip] = tempunnormleaf;  // prior to here, the 'unnormleaf[ip]' is from the previous month

			double fleaf = getFleaf(ip, tempunnormleaf, prvunnormleafmx);
			if (cd_vegs->lai[ip]<=0.) fleaf = 0.;
			cd_vegd->fleaf[ip] = fleaf;

			// set the phenological variables of the year
			if (currmind == 0 && currdinm == 0) {
				cd_vegd->eetmx[ip] = eet;
				cd_vegd->unnormleafmx[ip] = tempunnormleaf;
				cd_vegd->growingttime[ip] = ed_soid[ip]->rtdpgdd;
				cd_vegd->topt[ip] = ed_atms[ip]->ta;

				cd_vegd->maxleafc[ip] = getYearlyMaxLAI(ip)/vegdimpar.sla[ip];
			} else {
		    	if (cd_vegd->eetmx[ip] < eet) {
		    		cd_vegd->eetmx[ip] = eet;
		    	}

		    	if (cd_vegd->unnormleafmx[ip] < tempunnormleaf) {
					cd_vegd->unnormleafmx[ip] = tempunnormleaf;
					cd_vegd->topt[ip] = ed_atms[ip]->ta;   // it's updating for current year and then update the 'deque', but not used in 'GPP' estimation
				}

				if (cd_vegd->growingttime[ip]<ed_soid[ip]->rtdpgdd) {
					// here, we take the top root zone degree-days since growing started
					// can be modified when such an algorithm available
					cd_vegd->growingttime[ip]=ed_soid[ip]->rtdpgdd;
				}
			}

			// 2) plant size (biomass C) or age controlled foliage fraction relative to the max. leaf C
			cd_vegd->ffoliage[ip] = getFfoliage(ip, cd_vegs->ifwoody[ip],
					cd_vegs->ifperenial[ip], bd_vegs[ip]->call);

		} else { // 'vegcov' is 0
			cd_vegd->unnormleaf[ip] = MISSING_D;
			cd_vegd->fleaf[ip]      = MISSING_D;

			cd_vegd->eetmx[ip]        = MISSING_D;
			cd_vegd->unnormleafmx[ip] = MISSING_D;
			cd_vegd->topt[ip]         = MISSING_D;
			cd_vegd->maxleafc[ip]     = MISSING_D;

			cd_vegd->growingttime[ip] = MISSING_D;

			cd_vegd->ffoliage[ip] = MISSING_D;
		}

	}
=======
//leaf phenology - moved from 'Vegetation_Bgc.cpp' for easy modification,
//  if needed in the future
void Vegetation::phenology(const int &currmind) {
  for(int ip=0; ip<NUM_PFT; ip++) {
    if (cd->m_veg.vegcov[ip]>0.) {
      double prvunnormleafmx = 0.;   // previous 10 years' average as below
      deque <double> prvdeque = cd->prvunnormleafmxque[ip];
      int dequeno = prvdeque.size();

      for (int i=0; i<dequeno; i++) {
        prvunnormleafmx +=prvdeque[i]/dequeno;
      }

      double prveetmx=0;
      prvdeque = cd->prveetmxque[ip];
      dequeno = prvdeque.size();

      for (int i=0; i<dequeno; i++) {
        prveetmx +=prvdeque[i]/dequeno;
      }

      // 1) current EET and previous max. EET controlled
      double tempunnormleaf = 0.;;
      double eet = ed[ip]->m_v2a.tran;//originally it's using 'l2a.eet', which
                                      //  includes soil/veg evaporation - that
                                      //  may not relate to leaf phenology
      tempunnormleaf = getUnnormleaf(ip, prveetmx, eet, cd->m_vegd.unnormleaf[ip]);
      cd->m_vegd.unnormleaf[ip] = tempunnormleaf;//prior to here, the
                                                 //  'unnormleaf[ip]' is from
                                                 //  the previous month
      double fleaf = getFleaf(ip, tempunnormleaf, prvunnormleafmx);

      if (cd->m_veg.lai[ip]<=0.) {
        fleaf = 0.;
      }

      cd->m_vegd.fleaf[ip] = fleaf;

      // set the phenological variables of the year
      if (currmind == 0) {
        cd->m_vegd.eetmx[ip] = eet;
        cd->m_vegd.unnormleafmx[ip] = tempunnormleaf;
        cd->m_vegd.growingttime[ip] = ed[ip]->m_soid.rtdpgdd;
        cd->m_vegd.topt[ip] = ed[ip]->m_atms.ta;
        cd->m_vegd.maxleafc[ip] = getYearlyMaxLAI(ip)/vegdimpar.sla[ip];
      } else {
        if (eet>cd->m_vegd.eetmx[ip]) {
          cd->m_vegd.eetmx[ip] = eet;
        }

        if (cd->m_vegd.unnormleafmx[ip] < tempunnormleaf) {
          cd->m_vegd.unnormleafmx[ip] = tempunnormleaf;
          // it's updating monthly for current year and then update the 'deque',
          // but not used in 'GPP' estimation
          cd->m_vegd.topt[ip] = ed[ip]->m_atms.ta;
        }

        if (cd->m_vegd.growingttime[ip]<ed[ip]->m_soid.rtdpgdd) {
          //here, we take the top root zone degree-days since growing started
          cd->m_vegd.growingttime[ip]=ed[ip]->m_soid.rtdpgdd;
        }
      }

      //2) plant size (biomass C) or age controlled foliage fraction rative
      //   to the max. leaf C
      cd->m_vegd.ffoliage[ip] = getFfoliage(ip, cd->m_veg.ifwoody[ip],
                                            cd->m_veg.ifperenial[ip],
                                            bd[ip]->m_vegs.call);
    } else { // 'vegcov' is 0
      cd->m_vegd.unnormleaf[ip] = MISSING_D;
      cd->m_vegd.fleaf[ip] = MISSING_D;
      cd->m_vegd.eetmx[ip] = MISSING_D;
      cd->m_vegd.unnormleafmx[ip] = MISSING_D;
      cd->m_vegd.topt[ip] = MISSING_D;
      cd->m_vegd.maxleafc[ip] = MISSING_D;
      cd->m_vegd.growingttime[ip] = MISSING_D;
      cd->m_vegd.ffoliage[ip] = MISSING_D;
    }
  }
>>>>>>> master
};

// functions for eet adjusted foliage growth index
// 'prvunleaf' is the unnormalized leaf from last time period
// 'prveetmx' is monthly eetmx of previous simulation period (year)

double Vegetation::getUnnormleaf(const int& ipft, double &prveetmx,
                                 const double & eet,
                                 const double & prvunleaf) {
  double normeet;
  double unnormleaf;

  if (prveetmx <= 0.0) {
    prveetmx = 1.0;
  }

  normeet = eet/prveetmx;

  if(normeet>1) {
    normeet =1;
  }

  unnormleaf = (vegdimpar.aleaf[ipft] * normeet)
               +(vegdimpar.bleaf[ipft] * prvunleaf)
               +vegdimpar.cleaf[ipft];

  if (unnormleaf < (0.5 * vegdimpar.minleaf[ipft])) {
    unnormleaf = 0.5 * vegdimpar.minleaf[ipft];
  }

<<<<<<< HEAD
  	return (unnormleaf);
=======
  return unnormleaf;
>>>>>>> master
};

//fleaf is normalized EET and previous EET determined phenology index 0~1
//i.e., f(phenology) in gpp calculation
<<<<<<< HEAD
double Vegetation::getFleaf(const int &ipft, const double & unnormleaf, const double &prvunnormleafmx){
  	double fleaf;

  	if (prvunnormleafmx <= 0.0) {
  	 	fleaf = 0.0;
  	} else {
  	 	fleaf= unnormleaf/prvunnormleafmx;
   	}

  	if (fleaf < vegdimpar.minleaf[ipft] ){
    	fleaf = vegdimpar.minleaf[ipft];
  	} else  if (fleaf > 1.0 ) {
   		fleaf = 1.0;
   	}

  	return (fleaf);
};

// function for biomass C adjusted foliage growth index (0 - 1.0)
double Vegetation::getFfoliage(const int &ipft, const bool & ifwoody, const bool &ifperenial, const double &vegc){

	double ffoliage =0;

  	if(!ifwoody){
  		if (!ifperenial) {
  			ffoliage = 1.0;    //annual: yearly max. not controlled by current plant C biomass (because it dies every year)
  		} else {
  			ffoliage = 1.0/(1.0 + vegdimpar.kfoliage[ipft] * exp(vegdimpar.cov[ipft]* vegc));
  		}

 	} else {

 		//from Zhuang et al., 2003
 		double m1 = vegdimpar.m1[ipft];
 		double m2 = vegdimpar.m2[ipft];
 		double m3 = vegdimpar.m3[ipft];
 		double m4 = vegdimpar.m4[ipft];

 		double fcv = m3*vegc /(1+m4*vegc);
 		ffoliage =  1./(1+m1*exp(m2*sqrt(fcv)));
	}

    //it is assumed that foliage will not go down during a growing life cycle
  	if(ffoliage>cd_vegd->foliagemx[ipft]){
  		cd_vegd->foliagemx[ipft] = ffoliage;
  	}else{
 		ffoliage = cd_vegd->foliagemx[ipft];
  	}

  	return (ffoliage);
};

// plant max. LAI function
double Vegetation::getYearlyMaxLAI(const int &ipft){

	double laimax = 0.;

	for (int im=0; im<MINY; im++) {   // taking the max. of input 'envlai[12]' adjusted by 'vegcov'
		double covlai = chtlu->envlai[im][ipft]*cd_vegs->vegcov[ipft];
		if (laimax<=covlai) laimax = covlai;
	}
	laimax *= cd_vegd->foliagemx[ipft];

  	return (laimax);
};
=======
double Vegetation::getFleaf(const int &ipft, const double & unnormleaf,
                            const double &prvunnormleafmx) {
  double fleaf;

  if (prvunnormleafmx <= 0.0) {
    fleaf = 0.0;
  } else {
    fleaf= unnormleaf/prvunnormleafmx;
  }

  if (fleaf < vegdimpar.minleaf[ipft] ) {
    fleaf = vegdimpar.minleaf[ipft];
  } else  if (fleaf > 1.0 ) {
    fleaf = 1.0;
  }

  return fleaf;
};

// function for biomass C adjusted foliage growth index (0 - 1.0)
double Vegetation::getFfoliage(const int &ipft, const bool & ifwoody,
                               const bool &ifperenial, const double &vegc) {
  double ffoliage =0;

  //if(!ifwoody) {
    if (!ifperenial) {
      ffoliage = 1.0; //annual: yearly max. not controlled by current plant
                      //  C biomass (because it dies every year)
    } else {
      ffoliage = 1.0 / (1.0 + vegdimpar.kfoliage[ipft] * exp(vegdimpar.cov[ipft] * vegc));
    }
  //  } else {
  //    //from Zhuang et al., 2003
  //    double m1 = vegdimpar.m1[ipft];
  //    double m2 = vegdimpar.m2[ipft];
  //    double m3 = vegdimpar.m3[ipft];
  //    double m4 = vegdimpar.m4[ipft];
  //    double fcv = m3*vegc /(1+m4*vegc);
  //    ffoliage =  1./(1+m1*exp(m2*sqrt(fcv)));
  //  }

  //it is assumed that foliage will not go down during a growth cycle
  if(ffoliage>cd->m_vegd.foliagemx[ipft]) {
    cd->m_vegd.foliagemx[ipft] = ffoliage;
  } else {
    ffoliage = cd->m_vegd.foliagemx[ipft];
  }

  return ffoliage;
};

// plant max. LAI function
double Vegetation::getYearlyMaxLAI(const int &ipft) {
  double laimax = 0.;

  for (int im=0; im<12; im++) {//taking the max. of input 'envlai[12]'
                               //  adjusted by 'vegcov'
//    double covlai = chtlu->envlai[im][ipft]*cd->m_veg.vegcov[ipft];
      double covlai = chtlu->envlai[im][ipft];
    if (laimax <= covlai) {
      laimax = covlai;
    }
  }

  laimax *= cd->m_vegd.ffoliage[ipft];
  return laimax;
}
>>>>>>> master


// the following can be developed further for dynamic fine root distribution
// currently, it's only do some checking
<<<<<<< HEAD
void Vegetation::updateFrootfrac(){

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd_vegs->vegcov[ip]>0.){

    		double totrootfrac = 0.;
    		for (int il=0; il<MAX_ROT_LAY; il++){
    			if (cd_vegs->frootfrac[il][ip]>0.) {
    				totrootfrac+=cd_vegs->frootfrac[il][ip];
    			}
    		}

		//
    		if (totrootfrac>0.) {
    			for (int il=0; il<MAX_ROT_LAY; il++){
    				cd_vegs->frootfrac[il][ip] /= totrootfrac;
    			}

    		} else {
    			for (int il=0; il<MAX_ROT_LAY; il++){
    				cd_vegs->frootfrac[il][ip] = 0.;
    			}

    		}

    	} // end of 'vegcov[ip]>0'
	}

};


void Vegetation::setCohortLookup(CohortLookup* chtlup){
  	chtlu = chtlup;
};

void Vegetation::setCohortData(CohortData* cdp){
 	cd = cdp;
	if (tstepmode==MONTHLY) {
 		cd_vegs = &cdp->m_veg;
 		cd_vegd = &cdp->m_vegd;
 	}else if(tstepmode==DAILY){
 		cd_vegs = &cdp->d_veg;
 		cd_vegd = &cdp->d_vegd;
 	}
};

void Vegetation::setEnvData(const int &ip, EnvData* edp){
  	// ed[ip] = edp;
 	if (tstepmode==MONTHLY) {
  		ed_vegs[ip] = &edp->m_vegs;
  		ed_vegd[ip] = &edp->m_vegd;
  		ed_soid[ip] = &edp->m_soid;
        ed_v2a[ip]  = &edp->m_v2a;
        ed_atms[ip] = &edp->m_atms;
  	}else if(tstepmode==DAILY){
  		ed_vegs[ip] = &edp->d_vegs;
  		ed_vegd[ip] = &edp->d_vegd;
  		ed_soid[ip] = &edp->d_soid;
        ed_v2a[ip]  = &edp->d_v2a;
        ed_atms[ip] = &edp->d_atms;
  	}
};

void Vegetation::setBgcData(const int &ip, BgcData* bdp){
	 //bd[ip] = bdp;
 	if (tstepmode==MONTHLY) {
  		bd_vegs[ip] = &bdp->m_vegs;
  		bd_vegd[ip] = &bdp->m_vegd;
  	}else if(tstepmode==DAILY){
  		bd_vegs[ip] = &bdp->d_vegs;
  		bd_vegd[ip] = &bdp->d_vegd;
  	}
=======
void Vegetation::updateFrootfrac() {

  // Apparently loops over all PFTs and each PFT's 'artifical' soil layers
  // and then reset the frootfrac for the 'artifical' soil layers so that
  // the sum is up to 100%?
  // Not sure how useful this code is....

  for (int ip=0; ip<NUM_PFT; ip++) {
    if (cd->m_veg.vegcov[ip]>0.) {
      double totrootfrac = 0.;

      for (int il=0; il<MAX_ROT_LAY; il++) {
        if (cd->m_veg.frootfrac[il][ip]>0.) {
          totrootfrac += cd->m_veg.frootfrac[il][ip];
        }
      }

      if (totrootfrac > 0.0) {
        for (int il=0; il<MAX_ROT_LAY; il++) {
          cd->m_veg.frootfrac[il][ip] /= totrootfrac;
        }
      } else {
        for (int il=1; il<MAX_ROT_LAY; il++) {
          cd->m_veg.frootfrac[il][ip] = 0.0;
        }
      }
    } // end of 'vegcov[ip]>0'
  }
};


void Vegetation::setCohortLookup(CohortLookup* chtlup) {
  chtlu = chtlup;
};

void Vegetation::setCohortData(CohortData* cdp) {
  cd = cdp;
};

void Vegetation::setEnvData(const int &ip, EnvData* edp) {
  ed[ip] = edp;
};

void Vegetation::setBgcData(const int &ip, BgcData* bdp) {
  bd[ip] = bdp;
>>>>>>> master
};
