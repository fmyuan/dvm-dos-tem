/*
 *  Cohort.cpp
 *
 * Purpose: call TEM core processes at community (cohort)-level within a common grid
 *
 * History:
 *     June 28, 2011, by F.-M. Yuan:
 *          (1) Recoding based on DOS-TEM's code;
 *          (2) DVM concepts added
 *
 * Important Notes:
 *     (1) There are two sets of 'ed', 'bd': one set is for 'vegetation' with multiple PFTs;
 *     and another set is the integrated for all. These two is not same for PFTs, but same for 'ground'
 *
 *
 */

#include "Cohort.h"

Cohort::Cohort(){

};

Cohort::~Cohort(){

};

// initialization of pointers used in modules called here
void Cohort::initSubmodules(){

	// for controlling of error messaging in some subroutines
	ground.debugging = md->consoledebug;
	soilenv.tempupdator.debugging = md->consoledebug;
	soilenv.stefan.debugging      = md->consoledebug;
	soilenv.richards.debugging    = md->consoledebug;

 	//atmosphere module pointers
	atm.setCohortData(&cd);
	atm.setEnvData(edall);

 	// ecosystem domain (structure)
	veg.tstepmode = md->timestep;
	veg.setCohortData(&cd);
	veg.setCohortLookup(&chtlu);

	ground.setCohortLookup(&chtlu);

 	// vegetation module pointers
 	for (int i=0; i<NUM_PFT; i++){
 		veg.setEnvData(i, &ed[i]);
 		veg.setBgcData(i, &bd[i]);

 		vegenv[i].tstepmode = DAILY;
 		vegenv[i].setCohortLookup(&chtlu);
 		vegenv[i].setEnvData(&ed[i]);
 		vegenv[i].setCohortData(&cd);

 		vegbgc[i].tstepmode = md->timestep;
 		vegbgc[i].nfeed = md->nfeed;
 		vegbgc[i].setCohortLookup(&chtlu);
 		vegbgc[i].setCohortData(&cd);
 		vegbgc[i].setEnvData(&ed[i]);
 		vegbgc[i].setBgcData(&bd[i]);

 	}

	//snow-soil module pointers
 	snowenv.tstepmode = DAILY;
 	snowenv.setGround(&ground);
 	snowenv.setCohortLookup(&chtlu);
	snowenv.setCohortData(&cd);
 	snowenv.setEnvData(edall);

 	soilenv.tstepmode = DAILY;
 	soilenv.setGround(&ground);
 	soilenv.setCohortLookup(&chtlu);
	soilenv.setCohortData(&cd);
	soilenv.setEnvData(edall);

 	solprntenv.tstepmode = DAILY;
	solprntenv.setGround(&ground);
	solprntenv.setEnvData(edall);

 	soilbgc.tstepmode = md->timestep;
 	soilbgc.nfeed = md->nfeed;
 	soilbgc.avlnflag = md->avlnflg;
 	soilbgc.baseline = md->baseline;
 	soilbgc.setGround(&ground);
	soilbgc.setCohortLookup(&chtlu);
	soilbgc.setCohortData(&cd);
	soilbgc.setEnvData(edall);
	soilbgc.setBgcData(bdall);
	soilbgc.setFirData(fd);

    //fire module pointers
	fire.setCohortLookup(&chtlu);
	fire.setCohortData(&cd);
	fire.setAllEnvBgcData(edall, bdall);
 	for (int i=0; i<NUM_PFT; i++){
 		fire.setBgcData(&bd[i], i);
 	}
 	fire.setFirData(fd);

	//BGC states change integration module pointers (note that the calling order, otherwise segmentation fault)
 	solintegrator.setSoil_Bgc(&soilbgc);     // this setting do first
	solintegrator.setSoiBgcData(bdall);      // this setting do secondly
 	for (int i=0; i<NUM_PFT; i++){
 		vegintegrator[i].setVegetation_Bgc(&vegbgc[i]);    // this setting do first
 		vegintegrator[i].setVegBgcData(&bd[i]);            // this setting do secondly
 	}

 	// Output data pointers
 	outbuffer.setDimensionData(&cd);
	outbuffer.setProcessData(-1, edall, bdall);
	for (int ip=0; ip<NUM_PFT; ip++){
		outbuffer.setProcessData(ip, &ed[ip], &bd[ip]);
	}
	outbuffer.setFireData(fd);

};

// The following 'set...' functions allow initialized data pointers outside be used here
void Cohort::setTime(Timer * timerp){
  	timer = timerp;
};

void Cohort::setModelData(ModelData* mdp){
  	md = mdp;
};

void Cohort::setInputData(RegionData * rdp, GridData * gdp){

  	rd = rdp;
  	gd = gdp;

	cd.rd = rd;
	cd.gd = gd;

};

void Cohort::setProcessData(EnvData * alledp, BgcData * allbdp, FirData *fdp){
	edall = alledp;
	bdall = allbdp;

	fd = fdp;

 	bdall->cd = &cd;
 	edall->cd = &cd;

	for (int i=0; i<NUM_PFT; i++){
		bd[i].cd = &cd;
		ed[i].cd = &cd;
 	}

};

//re-initializing for a new community of all PFTs sharing same atm/snow-soil domains within a grid
void Cohort::initStatePar() {

	//
	if (md->initmode>=3){
		cd.yrsdist = resid.yrsdist;
	}

 	// FOR VEGETATION
	//vegetation dimension/structure
	veg.initializeParameter();
	if(md->initmode<3){      // from 'chtlu' or 'sitein'
		veg.initializeState();
	} else {     // initmode  >=3: restart
		veg.initializeState5restart(&resid);
	}

	// pft needs to be initialized individually for 'envmodule' and 'bgcmodule'
	for (int ip=0; ip<NUM_PFT; ip++) {

		vegenv[ip].ipft = ip;
		vegbgc[ip].ipft = ip;

		//set-up paramters for vegetation processes
		vegenv[ip].initializeParameter();
		vegbgc[ip].initializeParameter();

		if(md->initmode<3){
			vegbgc[ip].initializeState();
			vegenv[ip].initializeState();

		} else {
			vegbgc[ip].initializeState5restart(&resid);
			vegenv[ip].initializeState5restart(&resid);
		}

	}

 	 // initialize dimension/structure for snow-soil
	// first read in the default initial parameter for snow/soil
	ground.initParameter();

 	snowenv.initializeParameter();
  	soilenv.initializeParameter();
 	soilbgc.initializeParameter();

	if(md->initmode < 3){    //lookup or sitein

		ground.initDimension();   //read-in snow/soil structure from 'chtlu'

		// reset the soil texture data from grid-level soil.nc, rather than 'chtlu',
 	    // Note that the mineral layer structure is already defined above
		if (md->runmode==2 || md->runmode==3){  //region-TEM runmode
			float z=0;
			for (int i=0; i<ground.mineral.num; i++){
	 			 z+=ground.mineral.dz[i];
	 			 if (z<=0.30) {   //assuming the grid top-soil texture is for top 30 cm
	 				 ground.mineral.texture[i] = gd->topsoil;
	 			 } else {
	 				 ground.mineral.texture[i] = gd->botsoil;
	 			 }

			}
		}

 		 // then if we have sitein.nc, as specified. In this way, if sitein.nc may not provide
 		 // all data, then model will still be able to use the default.
	    if(md->initmode ==2){ //from sitein.nc specified as md->initialfile
//	    	setSiteStates(&sitein);
	    }

	    // set-up the snow-soil-soilparent structure
	    ground.initLayerStructure(&cd.d_snow, &cd.m_soil);   //snow updated daily, while soil dimension at monthly
	    cd.d_soil = cd.m_soil;

	    // initializing snow/soil/soilparent env state conditions after layerStructure done
	    snowenv.initializeNewSnowState();  //Note: ONE initial snow layer as new snow
	    soilenv.initializeState();
	    solprntenv.initializeState();

	    // initializing soil bgc state conditions
		soilbgc.initializeState();


	} else {    // initmode>=3: restart

		// set-up the snow-soil structure from restart data
 		ground.initLayerStructure5restart(&cd.d_snow, &cd.m_soil, &resid);   //snow updated daily, while soil dimension at monthly
	    cd.d_soil = cd.m_soil;

 		// initializing snow/soil env state conditions from restart data
 		snowenv.initializeState5restart(&resid);
 		soilenv.initializeState5restart(&resid);
 		solprntenv.initializeState5restart(&resid);

 		// initializing soil bgc state conditions from restart data
 		soilbgc.initializeState5restart(&resid);

	}

	//integrating the individual 'bd' initial conditions into 'bdall' initial conditions, if veg involved
	getBd4allveg();
 
	// fire processes
	if(md->initmode<3){
		fire.initializeState();
	} else {
		fire.initializeState5restart(&resid);
	}
	fire.initializeParameter();


};

void Cohort::prepareAllDrivingData(){

    // climate monthly data for all atm years
    atm.prepareMonthDrivingData();

    //fire driving data (if input) for all years
    if (!md->friderived && !md->runeq) {
        fire.prepareDrivingData();
    }
};

// climate daily data for one year
void Cohort::prepareDayDrivingData(const int & yrindx, const int & usedatmyr){

	//default climate/co2 setting
	bool changeclm = true;
	bool changeco2 = true;
	if (md->runeq) {
		changeclm = false;
		changeco2 = false;
	} else if (md->runsp) {
		changeco2 = false;
	}

	// preparing ONE year daily driving data
	if (timer->eqend) {
		// run the model after eq stage, climate and co2 driver controlled by setting in control file.
		if (md->changeclimate == 1) {
			changeclm = true;
		} else if (md->changeclimate == -1) {
			changeclm = false;
		}

		if (md->changeco2 == 1) {
			changeco2 = true;
		} else if (md->changeco2 == -1) {
			changeco2 = false;
		}

		if (md->act_clmstep == MINY) {
			atm.prepareDayDrivingData(yrindx, usedatmyr, changeclm, changeco2);
		} else if (md->act_clmstep == DINY) {
			atm.prepareDayDrivingData2(yrindx, changeco2);
		}

	} else {
		// run the model at eq stage, climate and co2 driver not controlled by setting in control file.
		if (md->act_clmstep == MINY) {
			atm.prepareDayDrivingData(yrindx, usedatmyr, false, false);
		} else if (md->act_clmstep == DINY) {
			atm.prepareDayDrivingData2(yrindx, false);
		}

	}
};

void Cohort::updateOneTimestep(const int & yrcnt, const int & currmind, const int & currdinm){

	// currmind: zero-based indexing, currdinm: day in the month (0 based-index)
	cd.day = currdinm;
    cd.month = currmind;
	int dinmcurr = DINM[currmind];
	int doy = timer->getDOYIndex(currmind, currdinm+1);

	// note: do the 'cd' initializing timely accumulators here,
	// because 'cd' are for both 'veg' (multiple-pfts) and 'soil',
	// except for 'cd_snow' (which are directly associated with 'ed').
	if(md->timestep==DAILY && currdinm==0) cd.beginOfMonth();
	if(currmind==0 && currdinm==0) cd.beginOfYear();

 	// first, update the current dimension/structure of veg-snow/soil column (domain)
   	updateDIMveg(currmind, currdinm, md->dvmmodule);

   	updateDIMgrd(currmind, currdinm, md->dslmodule);

   	// secondly, update the water/thermal process to get (bio)physical conditions
 	if(md->envmodule){
 		updateEnv(currmind, currdinm);
  	}

   	//thirdly, update the BGC process to get the C/N states and fluxes
  	if(md->bgcmodule){
  		updateBgc(currmind, currdinm);
  	}

  	// fourthly, run the disturbance module at monthly time-step only
   	if(md->dsbmodule && currdinm==dinmcurr-1){
   	   	updateFir(yrcnt, currmind);
   	}

	// note: do the 'cd' updating timely accumulators here,
	// because 'cd' are for both 'veg' (multiple-pfts) and 'soil',
	// except for 'cd_snow' (which are directly associated with 'ed').
   	if(md->timestep==DAILY) {
   		cd.endOfDay(dinmcurr);
   	} else if (md->timestep==MONTHLY){
   		cd.d_soil = cd.m_soil;    // just in case needed somewhere
   		cd.d_veg  = cd.m_veg;
   	}
	if(currdinm==dinmcurr-1) cd.endOfMonth();
	if(currmind==11 && currdinm==dinmcurr-1) cd.endOfYear();

	////////////////////////////
	// store all data for single/multiple cohorts
	if (md->outRegn) {
		outbuffer.updateRegnOutputBuffer(currmind, doy);
	}

	// always output the restart data (current time-step)
	outbuffer.updateRestartOutputBuffer(md);

};

/////////////////////////////////////////////////////////
//Environment Module Calling at one time-step (ALWAYS daily)
/////////////////////////////////////////////////////////
void Cohort::updateEnv(const int & currmind, const int & currdinm){

//Yuan: note that the Veg-Env module calling is for a few PFTs within ONE cohort
//      1) ed calling is done for each PFTs within the module
//      2) Env-module calling is done for one PFT, so needs loop for vegetation-relevant processes
//      3) 'currmind':  0 based-index, 'currdinm': day of the month (0 based-index)

    // (i) the n factor for soil temperature calculation from Tair

    edall->d_soid.nfactor =1;
    // Yuan: the following has temporarily commentted out - a lot of trouble
/*	if(currmind>=5 && currmind<=9){  //for warm season: Yuan: this will make a BIG jump of soil temperature at 5/9
		if(cd.ifdeciwoody){      //deciduous woody community type
			edall->d_soid.nfactor = 0.94;
		}
		if(cd.ifconiwoody) {
			if(fd->ysf <veg.vegdimpar.matureagemx){
				edall->d_soid.nfactor = 1.1 -(fd->ysf)/veg.vegdimpar.matureagemx * (1.1 -0.66);
			}else{
				edall->d_soid.nfactor =0.66;
		    }
		}
	}
*/

	// (ii)Initialize the yearly/monthly accumulators, which are accumulating at the end of month/day in 'ed'
	vegstate_dim *cd_veg;  // 'veg' may run at either daily/monthly time-step
	if (md->timestep==MONTHLY) {
		cd_veg = &cd.m_veg;
	} else if(md->timestep==DAILY) {
		cd_veg = &cd.d_veg;
	} else {
		cd_veg = &cd.y_veg;
	}

	for (int ip=0; ip<NUM_PFT; ip++){
		if (cd_veg->vegcov[ip]>0.){
			if(currmind==0){
				ed[ip].atm_beginOfYear();
				ed[ip].veg_beginOfYear();
				ed[ip].grnd_beginOfYear();
			}

			if (currdinm==0){
				ed[ip].atm_beginOfMonth();
				ed[ip].veg_beginOfMonth();
				ed[ip].grnd_beginOfMonth();
			}
		}
	}
	//
	if(currmind==0){
		edall->atm_beginOfYear();
		edall->veg_beginOfYear();
		edall->grnd_beginOfYear();
	}
	if (currdinm==0){
		edall->atm_beginOfMonth();
		edall->veg_beginOfMonth();
		edall->grnd_beginOfMonth();
	}

	// (iii) daily light/water processes at plant canopy
    int dinmcurr = DINM[currmind];
	int doy =timer->getDOYIndex(currmind, currdinm+1);  // currdinm indexing from 0
	double daylength = gd->alldaylengths[doy];

	//get the daily atm drivers and the data is in 'edall'
	atm.updateDailyAtm(currmind, currdinm);

	//'edall' in 'atm' must be assigned to that in 'ed' for each PFT
	assignAtmEd2pfts_daily();
	for (int ip=0; ip<NUM_PFT; ip++){
		if (cd_veg->vegcov[ip]>0.){
			if (cd_veg->nonvascular[ip]<=0) {   // for vascular plants
				// get the soil moisture controlling factor on plant transpiration
				double frootfr[MAX_SOI_LAY];
				for (int i=0; i<MAX_SOI_LAY; i++){
					frootfr[i] = cd.d_soil.frootfrac[i][ip];
				}

				soilenv.getSoilTransFactor(ed[ip].d_soid.fbtran, ground.fstsoill, frootfr);
				ed[ip].d_vegd.btran = 0.;
				for (int il=0; il<MAX_SOI_LAY; il++) {
					ed[ip].d_vegd.btran+=ed[ip].d_soid.fbtran[il];
				}

			} else {     // for non-vascular plants - needs further algorithm development
				double rh = ed[ip].d_atmd.vp/ed[ip].d_atmd.svp;
				if ( rh >= 0.60 || ed[ip].d_soid.sws[0]>0.60) {
					ed[ip].d_vegd.btran = 1.;
				} else {
					ed[ip].d_vegd.btran = 0.;
				}

			}

			// calculate vegetation light/water dynamics at daily timestep
			vegenv[ip].updateRadiation();
			vegenv[ip].updateWaterBalance(daylength);  //daylength in hours

		}
	}

	// integrating daily 'veg' portion in 'ed' of all PFTs for 'edall'
	getEd4allveg_daily();

	// Snow-soil Env-module: ground/soil temperatur e- moisture dynamics at daily timestep
	// note: hydrology is done separately for snow and soil, but thermal process is done as a continuous column
	//       so, thermal process (including phase changing) is carried out before hydrological process
	double tdrv = edall->d_atms.ta;

	soilenv.updateDailyGroundT(tdrv, daylength); // snow-soil temperature, including snow-melting and soil water phase changing
	snowenv.updateDailyM(tdrv);                  // snow water/thickness changing - must be done after 'T' because of melting

	// get the new bottom drainage layer and its depth, which needed for soil moisture calculation
	ground.setDrainL(ground.lstsoill, edall->d_sois.ald, edall->d_sois.watertab);
	soilenv.updateDailySM();  //soil moisture

	// save the variables to daily 'edall' (Note: not PFT specified)
	soilenv.retrieveDailyTM(tdrv, ground.toplayer, ground.lstsoill);
	solprntenv.retrieveDailyTM(ground.lstsoill);   //assuming rock layer's temperature equal to that of lstsoill

	assignGroundEd2pfts_daily();      //sharing the 'ground' portion in 'edall' with each pft 'ed'

	getEd4land_daily();  // integrating 'veg' and 'ground' into 'land'

	// update Snow structure at daily time-step due to its direct connection with T-M
	ground.retrieveSnowDimension(&cd.d_snow);
	cd.endOfDay_snow(dinmcurr);

	//accumulate daily vars into monthly for 'ed' of each PFT
	for (int ip=0; ip<NUM_PFT; ip++){
		if (cd_veg->vegcov[ip] > 0.0) {
			ed[ip].atm_endOfDay(dinmcurr);
			ed[ip].veg_endOfDay(dinmcurr);
			ed[ip].grnd_endOfDay(dinmcurr, doy);

			// accumulate yearly vars at the last day of a month
			if(currdinm==dinmcurr-1){
				ed[ip].atm_endOfMonth();
				ed[ip].veg_endOfMonth();
				ed[ip].grnd_endOfMonth();
			}
		}
	}

	//accumulate daily vars into monthly for 'ed' of all pfts
	edall->atm_endOfDay(dinmcurr);
	edall->veg_endOfDay(dinmcurr);              //be sure 'getEd4allpfts' called above
	edall->grnd_endOfDay(dinmcurr, doy);

	// accumulate yearly vars at the last day of a month for all pfts
	if(currdinm==dinmcurr-1){
		edall->atm_endOfMonth();
		edall->veg_endOfMonth();
		edall->grnd_endOfMonth();
	}

};

///////////////////////////////////////////////////////////////////////////////////////////
// Biogeochemical Module Calling at one time-step (DAILY or MONTHLY)
///////////////////////////////////////////////////////////////////////////////////////////
void Cohort::updateBgc(const int & currmind, const int & currdinm){

	// currmind: zero-based indexing, currdinm: day in the month (0 based-index)
	int dinmcurr = DINM[currmind];

	//
	vegstate_dim *cd_veg;  // 'veg' may run at either daily/monthly time-step
	if (md->timestep==MONTHLY) {
		cd_veg = &cd.m_veg;
	} else if(md->timestep==DAILY) {
		cd_veg = &cd.d_veg;
	} else {
		cd_veg = &cd.y_veg;
	}

	// initializing yearly accumulators if either daily or monthly time-step
	if((md->timestep==DAILY || md->timestep==MONTHLY) &&
	   (currmind==0 && currdinm==0)){
	    for (int ip=0; ip<NUM_PFT; ip++){
	    	if (cd_veg->vegcov[ip]>0.){
	    		bd[ip].veg_beginOfYear();
	    		bd[ip].soil_beginOfYear();
	    		bd[ip].land_beginOfYear();
	    	}
		}
		bdall->veg_beginOfYear();
		bdall->soil_beginOfYear();
		bdall->land_beginOfYear();
	}

	// initializing monthly accumulators ONLY if at daily time-step
    if (md->timestep==DAILY && currdinm==0) {
	    for (int ip=0; ip<NUM_PFT; ip++){
			if (cd_veg->vegcov[ip]>0.){
	    		bd[ip].veg_beginOfMonth();
	    		bd[ip].soil_beginOfMonth();
	    		bd[ip].land_beginOfMonth();
	    	}
		}
		bdall->veg_beginOfMonth();
		bdall->soil_beginOfMonth();
		bdall->land_beginOfMonth();
    }

    // BGC calling
	// it's called at the last day of the month if at monthly
    // so that monthly drivers/states are updated upon monthly-averaged 'ed' or other drivers
	if ((md->timestep==MONTHLY && currdinm == dinmcurr-1) ||    // monthly
	     md->timestep==DAILY) {                                 // daily

	///////////////////////////////////////////////////////////////////////////////////////////////////
		// vegetation BGC module calling
		for (int ip=0; ip<NUM_PFT; ip++){
			if (cd_veg->vegcov[ip]>0.){

				vegbgc[ip].prepareIntegration();
				vegintegrator[ip].updateVegBgc();
				vegbgc[ip].afterIntegration();

				// monthly data accumulation from daily simulation
				if (md->timestep == DAILY) {
					bd[ip].veg_endOfDay(dinmcurr);
				} else if (md->timestep==MONTHLY){
					bd[ip].d_vegs = bd[ip].m_vegs;    // just in case needed somewhere
				}

				// yearly data accumulation from either daily or monthly simulation
				if (currdinm == dinmcurr-1) bd[ip].veg_endOfMonth();

				// the following will evolve ideal C/N ratio for N requirement with changing CO2
				if(currmind==11 && currdinm == dinmcurr-1){
					vegbgc[ip].adapt();
				}
			}
		}

		getBd4allveg();             // integrating the pfts' 'bd' to allveg 'bdall'

		if(md->timestep==DAILY) {
			bdall->veg_endOfDay(dinmcurr); // monthly data accumulation
		} else if (md->timestep==MONTHLY){
	   		bdall->d_vegs = bdall->m_vegs;    // just in case needed somewhere
		}
		if(currdinm==dinmcurr-1) bdall->veg_endOfMonth();      // yearly data accumulation

	///////////////////////////////////////////////////////////////////////////////////////////////////
	    // soil BGC module calling

		// note: in soil bgc, edall is used (i.e., not pft individually)
        // call the ODE for soil bgc
		soilbgc.prepareIntegration();
		solintegrator.updateSoiBgc(MAX_SOI_LAY);
		soilbgc.afterIntegration();

		// monthly data updating
		if(md->timestep==DAILY) {
			bdall->soil_endOfDay(dinmcurr);
		} else if (md->timestep==MONTHLY){
			bdall->d_sois = bdall->m_sois;   //just in case needed somewhere
		}

		// yearly data accumulation
		if (currdinm==dinmcurr-1) {
			bdall->soil_endOfMonth();
			bdall->land_endOfMonth();
		}

		assignSoilBd2pfts();      //sharing the 'ground' portion in 'bdall' with each pft 'bd'

	} //monthly or daily block

};

//fire disturbance module calling at monthly time-step currently
/////////////////////////////////////////////////////////////////////////////////
void Cohort::updateFir(const int & yrind, const int & currmind){

	if(currmind ==0){
		fd->beginOfYear();

		fire.getOccur(yrind, md->friderived);
	}

   	if (yrind==fire.oneyear && currmind==fire.onemonth){
		//fire, C burning for each PFT, and C/N pools updated through 'bd', but not soil structure
   		// soil root fraction also updated through 'cd'
   		fire.burn();

   		// summarize burned veg C/N of individual 'bd' for each PFT above
   		for (int ip=0; ip<NUM_PFT; ip++){

   			if (cd.m_veg.vegcov[ip]>0.){

   				for (int i=0; i<NUM_PFT_PART; i++) {
   					bdall->m_vegs.c[i]    += bd[ip].m_vegs.c[i] * cd.m_veg.vegcov[ip];
   					bdall->m_vegs.strn[i] += bd[ip].m_vegs.strn[i] * cd.m_veg.vegcov[ip];
   				}

   				bdall->m_vegs.labn    += bd[ip].m_vegs.labn * cd.m_veg.vegcov[ip];
   				bdall->m_vegs.call    += bd[ip].m_vegs.call * cd.m_veg.vegcov[ip];
   				bdall->m_vegs.strnall += bd[ip].m_vegs.strnall * cd.m_veg.vegcov[ip];
   				bdall->m_vegs.nall    += bd[ip].m_vegs.nall * cd.m_veg.vegcov[ip];

   				bdall->m_vegs.deadc   += bd[ip].m_vegs.deadc * cd.m_veg.vegcov[ip];
   				bdall->m_vegs.deadn   += bd[ip].m_vegs.deadn * cd.m_veg.vegcov[ip];

   			}
   		}

   		// assign the updated soil C/N pools during firing to double-linked layer matrix in 'ground'
        if(md->timestep==DAILY) {
        	bdall->d_sois = bdall->m_sois;
        	bdall->d_vegs = bdall->m_vegs;
        }
   		soilbgc.assignCarbonBd2Layer();

		// then, adjusting soil structure after fire burning
   		// (Don't do this prior to the previous calling)
		ground.adjustSoilAfterburn();
		// and save the 'ground' C-data back to 'bdall'
		soilbgc.assignCarbonLayer2Bd();

		// update all other pft's 'bd'
		assignSoilBd2pfts();

		// update 'cd'
		cd.yrsdist = 0.;
		ground.retrieveSnowDimension(&cd.d_snow);
		ground.retrieveSoilDimension(&cd.m_soil);

 		getSoilFineRootFrac(&cd.m_veg, &cd.m_soil);

 		if(md->timestep==DAILY) {
 			cd.d_veg  = cd.m_veg;
 			cd.d_soil = cd.m_soil;
 		}
  	}

};

/////////////////////////////////////////////////////////////////////////////////
//   Dynamic Vegetation Module (DVM) calling
////////////////////////////////////////////////////////////////////////////////
void Cohort::updateDIMveg(const int & currmind, const int & currdinm, const bool & dvmmodule){
	// currmind: 0 based index, currdinm: day in the month (0 based index)

	int dinmcurr = DINM[currmind];

	//switch for using LAI read-in (false) or dynamically with vegC
	// the read-in LAI is through the 'chtlu->envlai[12]', i.e., a 12 monthly-LAI
	if (dvmmodule) {
		veg.updateLAI5vegc = md->updatelai;
	} else {
		veg.updateLAI5vegc = false;
	}

	// update phenological variables (factors used for GPP), and LAI
	veg.phenology(currmind, currdinm);
	veg.updateLai(currmind, currdinm);    // this must be done only after phenology

	// LAI updated above for each PFT, but FPC (foliage percent cover) may need adjustment
	veg.updateFpc();
	veg.updateVegcov();

	veg.updateFrootfrac();

	if (currdinm == dinmcurr-1){
		if (md->timestep==DAILY) cd.m_veg = cd.d_veg; // it will be updated again to get those mean values for monthly
		if (md->timestep==MONTHLY) cd.d_veg = cd.m_veg; // just in case we need it somewhere
	}
};

/////////////////////////////////////////////////////////////////////////////////
//   Dynamical Soil Layer Module (DSL)
////////////////////////////////////////////////////////////////////////////////
void Cohort::updateDIMgrd(const int & currmind, const int & currdinm, const bool & dslmodule){
	// currmind: 0 based index, currdinm: day in the month (0 based index)

	vegstate_dim *cd_veg;  // 'veg' may run at either daily/monthly time-step
	if (md->timestep==MONTHLY) {
		cd_veg = &cd.m_veg;
	} else if(md->timestep==DAILY) {
		cd_veg = &cd.d_veg;
	} else {
		cd_veg = &cd.y_veg;
	}

	// re-call the 'bdall' soil C contents
	// and assign them to the double-linked layer matrix each time-step
	if(md->bgcmodule) soilbgc.assignCarbonBd2Layer();

	// only update the thickness at begin of year, since it is a slow process
	// NOTE: the following performance IS on 'ground', which not associated with time-step
	if(dslmodule && currmind==0 && currdinm==0){
		// calculate the OSL layer thickness from C contents
		ground.updateOslThickness5Carbon(ground.fstsoill);

		// above callings didn't modify the layer matrix structure
		// in case that some layers may be getting too thick or too thin due to C content dynamics
		// then, re-do layer division or combination is necessary for better thermal/hydrological simulation
		if (cd.hasnonvascular && ground.moss.type<=0) {  //
			double prvpft = 0.;
			for (int ip=0; ip<NUM_PFT; ip++){
		    	if (cd_veg->nonvascular[ip]!=I_vascular){
		    		if (cd_veg->vegcov[ip]>prvpft)
		    			ground.moss.type = cd_veg->nonvascular[ip];
		    	}
			}

		}
	    ground.redivideSoilLayers();

		// and save the bgc data in double-linked structure back to 'bdall'
		soilbgc.assignCarbonLayer2Bd();

	}

	// update soil dimension,
	// especially here the root fraction which updated each time-step
	if (md->timestep==MONTHLY && currdinm == 0) { // first day of the month
		ground.retrieveSoilDimension(&cd.m_soil);
		getSoilFineRootFrac(&cd.m_veg, &cd.m_soil);
		cd.d_soil = cd.m_soil;      //just in case need 'd_soil' somewhere
	} else if (md->timestep==DAILY){
		ground.retrieveSoilDimension(&cd.d_soil);
		getSoilFineRootFrac(&cd.d_veg, &cd.d_soil);
	} else if (currmind==0 && currdinm==0){
		ground.retrieveSoilDimension(&cd.y_soil);
		getSoilFineRootFrac(&cd.y_veg, &cd.y_soil);
		cd.m_soil = cd.y_soil;
		cd.d_soil = cd.m_soil;
	}

	// update all soil 'bd' to each pft
	assignSoilBd2pfts();

}

//////////////////////////////////////////////////////////////////////////////////////
// adjusting fine root fraction in soil
void Cohort::getSoilFineRootFrac(vegstate_dim *cd_veg, soistate_dim *cd_soil){

	double mossthick = cd_soil->mossthick;
	double totfrootc = 0.;   //fine root C summed for all PFTs
	for (int ip=0; ip<NUM_PFT; ip++){

		if (cd_veg->vegcov[ip]>0.){

			double layertop, layerbot;
			// covert PFT 10-layer root fraction to accumulative ones for interpolating
			double cumrootfrac[MAX_ROT_LAY];
			cumrootfrac[0] = cd_veg->frootfrac[0][ip];
			for (int il=1; il<MAX_ROT_LAY; il++){
				cumrootfrac[il] = cumrootfrac[il-1]+cd_veg->frootfrac[il][ip];
			}

			// calculate soil fine root fraction from PFT's 10-root-layer structure
			// note: at this point, soil fine root fraction ACTUALLY is root biomass C distribution along soil profile
			for (int il=0; il<cd_soil->numsl; il++){
				if (cd_soil->type[il]>0) {   // non-moss soil layers only
					layertop = cd_soil->z[il] - mossthick;
					layerbot = cd_soil->z[il]+cd_soil->dz[il]-mossthick;

					cd_soil->frootfrac[il][ip] = assignSoilLayerRootFrac(layertop, layerbot, cumrootfrac, ROOTTHICK);  //fraction
					cd_soil->frootfrac[il][ip] *= cd_veg->fpc[ip];

					if(md->bgcmodule){
						if(md->timestep==DAILY){
					    	cd_soil->frootfrac[il][ip] *= bd[ip].d_vegs.c[I_root];  //root C
						} else if(md->timestep==MONTHLY){
					    	cd_soil->frootfrac[il][ip] *= bd[ip].m_vegs.c[I_root];  //root C
						} else {
					    	cd_soil->frootfrac[il][ip] *= bd[ip].y_vegs.c[I_root];  //root C
						}
					}
					totfrootc += cd_soil->frootfrac[il][ip];

				}
			}

		}

	}

	// soil fine root fraction - adjusted by both vertical distribution and root biomass of all PFTs
	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd_veg->vegcov[ip]>0.){
    		for (int il=0; il<cd_soil->numsl; il++){
    			if (cd_soil->type[il]>0 && cd_soil->frootfrac[il][ip]>0.) {   // non-moss soil layers only
    				cd_soil->frootfrac[il][ip] /= totfrootc;
    			} else {
    				cd_soil->frootfrac[il][ip] = 0.;
    			}

    		}
    	}
	}

};

double Cohort::assignSoilLayerRootFrac(const double & topz, const double & botz,
		     const double cumrootfrac[MAX_ROT_LAY], const double dzrotlay[MAX_ROT_LAY]){

	// determine soil layer's location in the root layer matrix
	int indxtop = -1;
	int indxbot = -1;
	double zrotlay[MAX_ROT_LAY];  // root layer top
	zrotlay[0] = 0.;
	for (int i=1; i<MAX_ROT_LAY; i++){
		zrotlay[i] = zrotlay[i-1]+dzrotlay[i];
		if (topz >= zrotlay[i-1] && topz<zrotlay[i]) {
			indxtop = i-1;
		}
		if ((botz >= zrotlay[i-1] && botz<zrotlay[i]) ||
			(i==MAX_ROT_LAY-1 && botz>=zrotlay[i])) {
			indxbot = i-1;
			break;
		}

	}

	//calculating fine root fraction in a layer by linearly interpolation
	double frfrac=0.;

	double sumfractop = 0.0;
	double sumfracbot = 0.0;
	if (indxtop >= 0. && indxbot>=0) {
		if (indxtop == 0) {
			sumfractop = cumrootfrac[indxtop]/dzrotlay[indxtop]*topz;
		} else {
			sumfractop = (cumrootfrac[indxtop]-cumrootfrac[indxtop-1])/dzrotlay[indxtop]
				        * (topz-zrotlay[indxtop])+cumrootfrac[indxtop-1];
		}

		if (indxbot == 0) {
			sumfracbot = cumrootfrac[indxbot]/dzrotlay[indxbot]*botz;
		} else {
			sumfracbot = (cumrootfrac[indxbot]-cumrootfrac[indxbot-1])/dzrotlay[indxbot]
				        * (botz-zrotlay[indxbot])+cumrootfrac[indxbot-1];
		}

		frfrac = sumfracbot - sumfractop;
		if (frfrac<0.0) frfrac = 0.0;
	} else {
		frfrac = 0.;
	}

	return (frfrac);

};


/////////////////////////////////////////////////////////////////////
// The following are for 'ed', 'bd' data integration from individual PFTs to all Vegetation
// Or, assign the 'atm' and 'ground' to each PFT
////////////////////////////////////////////////////////////////////////////////////////////

// assign 'atm' portion in 'edall' to all PFT's 'ed' at daily (monthly/yearly not needed, which can be done in 'ed')
void Cohort::assignAtmEd2pfts_daily(){

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0.){
    		ed[ip].d_atms = edall->d_atms;
    		ed[ip].d_atmd = edall->d_atmd;

    		ed[ip].d_a2l  = edall->d_a2l;
    	}
	}
}

// assign 'ground' portion in 'edall' to all PFT's 'ed'
void Cohort::assignGroundEd2pfts_daily(){

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0.){

    		ed[ip].d_snws = edall->d_snws;
    		ed[ip].d_sois = edall->d_sois;

    		ed[ip].d_snwd = edall->d_snwd;
    		ed[ip].d_soid = edall->d_soid;

    		ed[ip].d_soi2l  = edall->d_soi2l;
    		ed[ip].d_soi2a  = edall->d_soi2a;
    		ed[ip].d_snw2a  = edall->d_snw2a;
    		ed[ip].d_snw2soi= edall->d_snw2soi;

    	}

	}
}

// integrating (fpc weighted) 'soid.fbtran' in each 'ed' to 'edall'
void Cohort::getSoilTransfactor4all_daily(){

	for (int il=0; il<MAX_SOI_LAY; il++) {
		edall->d_soid.fbtran[il] = 0.;
		for (int ip=0; ip<NUM_PFT; ip++){
	    	if (cd.d_veg.vegcov[ip]>0.){
	    		edall->d_soid.fbtran[il] += ed[ip].d_soid.fbtran[il]  * cd.d_veg.vegcov[ip];
	    	}
		}
	}
}

// integrating (fpc weighted) 'veg' portion in 'edall' to all PFT's 'ed'
void Cohort::getEd4allveg_daily(){

	edall->d_vegs.rwater  = 0.;
	edall->d_vegs.snow    = 0.;

	edall->d_vegd.rc      = 0.;
	edall->d_vegd.cc      = 0.;
	edall->d_vegd.btran   = 0.;
	edall->d_vegd.m_ppfd  = 0.;
	edall->d_vegd.m_vpd   = 0.;

	edall->d_a2v.rnfl     = 0.;
	edall->d_a2v.rinter   = 0.;
	edall->d_a2v.snfl     = 0.;
	edall->d_a2v.sinter   = 0.;
	edall->d_a2v.swdown   = 0.;
	edall->d_a2v.swinter  = 0.;
	edall->d_a2v.pardown  = 0.;
	edall->d_a2v.parabsorb= 0.;

	edall->d_v2a.swrefl   = 0.;
	edall->d_v2a.evap     = 0.;
	edall->d_v2a.tran     = 0.;
	edall->d_v2a.evap_pet = 0.;
	edall->d_v2a.tran_pet = 0.;
	edall->d_v2a.sublim   = 0.;

	edall->d_v2g.swthfl   = 0.;
	edall->d_v2g.rdrip    = 0.;
	edall->d_v2g.rthfl    = 0.;
	edall->d_v2g.sdrip    = 0.;
	edall->d_v2g.sthfl    = 0.;

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0.){

		edall->d_vegs.rwater  += ed[ip].d_vegs.rwater * cd.d_veg.vegcov[ip];
		edall->d_vegs.snow    += ed[ip].d_vegs.snow * cd.d_veg.vegcov[ip];

		edall->d_vegd.rc      += ed[ip].d_vegd.rc * cd.d_veg.vegcov[ip];
		edall->d_vegd.cc      += ed[ip].d_vegd.cc * cd.d_veg.vegcov[ip];
		edall->d_vegd.btran   += ed[ip].d_vegd.btran * cd.d_veg.vegcov[ip];
		edall->d_vegd.m_ppfd  += ed[ip].d_vegd.m_ppfd * cd.d_veg.vegcov[ip];
		edall->d_vegd.m_vpd   += ed[ip].d_vegd.m_vpd * cd.d_veg.vegcov[ip];

		edall->d_a2v.rnfl     += ed[ip].d_a2v.rnfl * cd.d_veg.vegcov[ip];
		edall->d_a2v.rinter   += ed[ip].d_a2v.rinter * cd.d_veg.vegcov[ip];
		edall->d_a2v.snfl     += ed[ip].d_a2v.snfl * cd.d_veg.vegcov[ip];
		edall->d_a2v.sinter   += ed[ip].d_a2v.sinter * cd.d_veg.vegcov[ip];
		edall->d_a2v.swdown   += ed[ip].d_a2v.swdown * cd.d_veg.vegcov[ip];
		edall->d_a2v.swinter  += ed[ip].d_a2v.swinter * cd.d_veg.vegcov[ip];
		edall->d_a2v.pardown  += ed[ip].d_a2v.pardown * cd.d_veg.vegcov[ip];
		edall->d_a2v.parabsorb+= ed[ip].d_a2v.parabsorb * cd.d_veg.vegcov[ip];

		edall->d_v2a.swrefl   += ed[ip].d_v2a.swrefl * cd.d_veg.vegcov[ip];
		edall->d_v2a.evap     += ed[ip].d_v2a.evap * cd.d_veg.vegcov[ip];
		edall->d_v2a.tran     += ed[ip].d_v2a.tran * cd.d_veg.vegcov[ip];
		edall->d_v2a.evap_pet += ed[ip].d_v2a.evap_pet * cd.d_veg.vegcov[ip];
		edall->d_v2a.tran_pet += ed[ip].d_v2a.tran_pet * cd.d_veg.vegcov[ip];
		edall->d_v2a.sublim   += ed[ip].d_v2a.sublim * cd.d_veg.vegcov[ip];

		edall->d_v2g.swthfl   += ed[ip].d_v2g.swthfl * cd.d_veg.vegcov[ip];
		edall->d_v2g.rdrip    += ed[ip].d_v2g.rdrip * cd.d_veg.vegcov[ip];
		edall->d_v2g.rthfl    += ed[ip].d_v2g.rthfl * cd.d_veg.vegcov[ip];
		edall->d_v2g.sdrip    += ed[ip].d_v2g.sdrip * cd.d_veg.vegcov[ip];
		edall->d_v2g.sthfl    += ed[ip].d_v2g.sthfl * cd.d_veg.vegcov[ip];
    	}
	}
}

// integrating 'veg' and 'ground' portion in 'edall' as 'land (community)'
// Note: this 'l2a' is monthly/yearly integrated in 'ed->atm_endofDay/_endofMonth'
void Cohort::getEd4land_daily(){

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0.){

    		ed[ip].d_l2a.eet = ed[ip].d_v2a.evap + ed[ip].d_v2a.sublim + ed[ip].d_v2a.tran
		                  +ed[ip].d_snw2a.sublim + ed[ip].d_soi2a.evap;

    		ed[ip].d_l2a.pet = ed[ip].d_v2a.evap_pet + ed[ip].d_v2a.sublim + ed[ip].d_v2a.tran_pet
		          +ed[ip].d_snw2a.sublim + ed[ip].d_soi2a.evap_pet;
    	}
	}

	//
	edall->d_l2a.eet = edall->d_v2a.evap + edall->d_v2a.sublim + edall->d_v2a.tran
			          +edall->d_snw2a.sublim + edall->d_soi2a.evap;

	edall->d_l2a.pet = edall->d_v2a.evap_pet + edall->d_v2a.sublim + edall->d_v2a.tran_pet
			          +edall->d_snw2a.sublim + edall->d_soi2a.evap_pet;

}

// assign ground/soil in 'bdall' to each PFT's 'bd'
// so that multiple PFT share one ground/soil
void Cohort::assignSoilBd2pfts(){

	for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0. ||
    		cd.m_veg.vegcov[ip]>0. ||
    		cd.y_veg.vegcov[ip]>0.){

    		bd[ip].d_sois   = bdall->d_sois;
    		bd[ip].d_soid   = bdall->d_soid;
    		bd[ip].d_soi2l  = bdall->d_soi2l;
    		bd[ip].d_soi2a  = bdall->d_soi2a;
    		bd[ip].d_a2soi  = bdall->d_a2soi;
    		bd[ip].d_soi2soi= bdall->d_soi2soi;

    		bd[ip].m_sois   = bdall->m_sois;
    		bd[ip].m_soid   = bdall->m_soid;
    		bd[ip].m_soi2l  = bdall->m_soi2l;
    		bd[ip].m_soi2a  = bdall->m_soi2a;
    		bd[ip].m_a2soi  = bdall->m_a2soi;
    		bd[ip].m_soi2soi= bdall->m_soi2soi;

    		bd[ip].y_sois   = bdall->y_sois;
    		bd[ip].y_soid   = bdall->y_soid;
    		bd[ip].y_soi2l  = bdall->y_soi2l;
    		bd[ip].y_soi2a  = bdall->y_soi2a;
    		bd[ip].y_a2soi  = bdall->y_a2soi;
    		bd[ip].y_soi2soi= bdall->y_soi2soi;

    		for (int il=0; il<MAX_SOI_LAY; il++){
    			bd[ip].prvltrfcnque[il] = bdall->prvltrfcnque[il];
    		}

    	}
	}
}

// integrating (vegfrac weighted) 'veg' portion in 'bdall' to all PFT's 'bd'
void Cohort::getBd4allveg(){

	// monthly time-step
	if(md->timestep==MONTHLY) {
	  for (int i=0; i<NUM_PFT_PART; i++){
		bdall->m_vegs.c[i]    = 0.;
		bdall->m_vegs.strn[i] = 0.;

		bdall->m_a2v.ingpp[i] = 0.;
		bdall->m_a2v.innpp[i] = 0.;
		bdall->m_a2v.gpp[i]   = 0.;
		bdall->m_a2v.npp[i]   = 0.;
		bdall->m_v2a.rg[i]    = 0.;
 		bdall->m_v2a.rm[i]    = 0.;

 		bdall->m_v2v.nmobil[i]  = 0.;
 		bdall->m_v2v.nresorb[i] = 0.;

 		bdall->m_v2soi.ltrfalc[i] = 0.;  //excluding moss/lichen, for which 'litterfalling' really means moss/lichen death
		bdall->m_v2soi.ltrfaln[i] = 0.;

		bdall->m_soi2v.snuptake[i] = 0.;
	  }
	  bdall->m_vegs.call    = 0.;
 	  bdall->m_vegs.labn    = 0.;
	  bdall->m_vegs.strnall = 0.;
	  bdall->m_vegs.nall    = 0.;

	  bdall->m_a2v.ingppall = 0.;
	  bdall->m_a2v.innppall = 0.;
	  bdall->m_a2v.gppall   = 0.;
	  bdall->m_a2v.nppall   = 0.;
	  bdall->m_v2a.rgall    = 0.;
	  bdall->m_v2a.rmall    = 0.;

	  bdall->m_v2soi.d2wdebrisc = 0.;
	  bdall->m_v2soi.d2wdebrisn = 0.;

	  bdall->m_v2soi.ltrfalcall = 0.;  // excluding moss/lichen
	  bdall->m_v2soi.ltrfalnall = 0.;  // excluding moss/lichen
	  bdall->m_v2soi.mossdeathc = 0.;
	  bdall->m_v2soi.mossdeathn = 0.;

	  bdall->m_v2v.nmobilall  = 0.;
	  bdall->m_v2v.nresorball = 0.;

  	  bdall->m_soi2v.innuptake = 0.;
  	  for (int il=0; il<MAX_SOI_LAY; il++) {
  		bdall->m_soi2v.nextract[il] = 0.;
  	  }
 	  bdall->m_soi2v.lnuptake   = 0.;
 	  bdall->m_soi2v.snuptakeall= 0.;

	  for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.m_veg.vegcov[ip]>0.){
    		bdall->m_v2soi.d2wdebrisc += bd[ip].m_v2soi.d2wdebrisc * cd.m_veg.vegcov[ip];
    		bdall->m_v2soi.d2wdebrisn += bd[ip].m_v2soi.d2wdebrisn * cd.m_veg.vegcov[ip];

    		for (int i=0; i<NUM_PFT_PART; i++){
    			bdall->m_vegs.c[i]    += bd[ip].m_vegs.c[i] * cd.m_veg.vegcov[ip];
    			bdall->m_vegs.strn[i] += bd[ip].m_vegs.strn[i] * cd.m_veg.vegcov[ip];

    			bdall->m_a2v.ingpp[i] += bd[ip].m_a2v.ingpp[i] * cd.m_veg.vegcov[ip];
    			bdall->m_a2v.innpp[i] += bd[ip].m_a2v.innpp[i] * cd.m_veg.vegcov[ip];
    			bdall->m_a2v.gpp[i]   += bd[ip].m_a2v.gpp[i] * cd.m_veg.vegcov[ip];
    			bdall->m_a2v.npp[i]   += bd[ip].m_a2v.npp[i] * cd.m_veg.vegcov[ip];
    			bdall->m_v2a.rg[i]    += bd[ip].m_v2a.rg[i] * cd.m_veg.vegcov[ip];
    			bdall->m_v2a.rm[i]    += bd[ip].m_v2a.rm[i] * cd.m_veg.vegcov[ip];

    			bdall->m_v2v.nmobil[i]  += bd[ip].m_v2v.nmobil[i] * cd.m_veg.vegcov[ip];
    			bdall->m_v2v.nresorb[i] += bd[ip].m_v2v.nresorb[i] * cd.m_veg.vegcov[ip];

    			if (cd.m_veg.nonvascular[ip]==0) {
    				bdall->m_v2soi.ltrfalc[i] += bd[ip].m_v2soi.ltrfalc[i] * cd.m_veg.vegcov[ip];
    				bdall->m_v2soi.ltrfaln[i] += bd[ip].m_v2soi.ltrfaln[i] * cd.m_veg.vegcov[ip];
    			}

    			bdall->m_soi2v.snuptake[i] += bd[ip].m_soi2v.snuptake[i] * cd.m_veg.vegcov[ip];
    		}
    		bdall->m_vegs.labn    += bd[ip].m_vegs.labn * cd.m_veg.vegcov[ip];
    		bdall->m_vegs.call    += bd[ip].m_vegs.call * cd.m_veg.vegcov[ip];
    		bdall->m_vegs.strnall += bd[ip].m_vegs.strnall * cd.m_veg.vegcov[ip];
    		bdall->m_vegs.nall    += bd[ip].m_vegs.nall * cd.m_veg.vegcov[ip];

    		bdall->m_a2v.ingppall += bd[ip].m_a2v.ingppall * cd.m_veg.vegcov[ip];
    		bdall->m_a2v.innppall += bd[ip].m_a2v.innppall * cd.m_veg.vegcov[ip];
			bdall->m_a2v.gppall   += bd[ip].m_a2v.gppall * cd.m_veg.vegcov[ip];
			bdall->m_a2v.nppall   += bd[ip].m_a2v.nppall * cd.m_veg.vegcov[ip];
			bdall->m_v2a.rgall    += bd[ip].m_v2a.rgall * cd.m_veg.vegcov[ip];
			bdall->m_v2a.rmall    += bd[ip].m_v2a.rmall * cd.m_veg.vegcov[ip];

			if (cd.m_veg.nonvascular[ip]==0) {
				bdall->m_v2soi.ltrfalcall += bd[ip].m_v2soi.ltrfalcall * cd.m_veg.vegcov[ip];
				bdall->m_v2soi.ltrfalnall += bd[ip].m_v2soi.ltrfalnall * cd.m_veg.vegcov[ip];
			}
			if (cd.m_veg.nonvascular[ip]>0){
				bdall->m_v2soi.mossdeathc += bd[ip].m_v2soi.mossdeathc * cd.m_veg.vegcov[ip];  //NOTE: non-vascular plants' litterfalling (mortality) is for death moss layer C
				bdall->m_v2soi.mossdeathn += bd[ip].m_v2soi.mossdeathn * cd.m_veg.vegcov[ip];
			}

			bdall->m_v2v.nmobilall  += bd[ip].m_v2v.nmobilall * cd.m_veg.vegcov[ip];
			bdall->m_v2v.nresorball += bd[ip].m_v2v.nresorball * cd.m_veg.vegcov[ip];

			bdall->m_soi2v.innuptake += bd[ip].m_soi2v.innuptake * cd.m_veg.vegcov[ip];
			for (int il=0; il<cd.m_soil.numsl; il++) {
				bdall->m_soi2v.nextract[il] += bd[ip].m_soi2v.nextract[il] * cd.m_veg.vegcov[ip];
			}
			bdall->m_soi2v.lnuptake   += bd[ip].m_soi2v.lnuptake * cd.m_veg.vegcov[ip];
			bdall->m_soi2v.snuptakeall+= bd[ip].m_soi2v.snuptakeall * cd.m_veg.vegcov[ip];

    	} // end of 'vegcov[ip]>0'

	  } // end of 'for ip=1,NUM_PFT'

	  // below litter-fall vertical distribution needed to integrate from each PFT's
	  double sumrtltrfall = 0.;
	  for (int il=0; il<cd.m_soil.numsl; il++) {
		bdall->m_v2soi.rtlfalfrac[il] = 0.;
		for (int ip=0; ip<NUM_PFT; ip++) {
	    	if (cd.m_veg.vegcov[ip]>0.){
	    		bd[ip].m_v2soi.rtlfalfrac[il] = cd.m_soil.frootfrac[il][ip];
	    		bdall->m_v2soi.rtlfalfrac[il]+=bd[ip].m_v2soi.rtlfalfrac[il]*bd[ip].m_v2soi.ltrfalc[I_root];
	    	}
		}
		sumrtltrfall +=bdall->m_v2soi.rtlfalfrac[il];
	  }

	  for (int il=0; il<cd.m_soil.numsl; il++) {
		if (sumrtltrfall>0) {
			bdall->m_v2soi.rtlfalfrac[il] /=sumrtltrfall;
		} else {
			bdall->m_v2soi.rtlfalfrac[il] = 0.;
		}
	  }

	} // end of if (md->timestep == MONTHLY)

    ////////////////////////////////////////////////////////////
    // DAILY simulation if any
	if (md->timestep == DAILY) {
	  for (int i=0; i<NUM_PFT_PART; i++){
		bdall->d_vegs.c[i]    = 0.;
		bdall->d_vegs.strn[i] = 0.;

		bdall->d_a2v.ingpp[i] = 0.;
		bdall->d_a2v.innpp[i] = 0.;
		bdall->d_a2v.gpp[i]   = 0.;
		bdall->d_a2v.npp[i]   = 0.;
		bdall->d_v2a.rg[i]    = 0.;
 		bdall->d_v2a.rm[i]    = 0.;

 		bdall->d_v2v.nmobil[i]  = 0.;
 		bdall->d_v2v.nresorb[i] = 0.;

 		bdall->d_v2soi.ltrfalc[i] = 0.;  //excluding moss/lichen, for which 'litterfalling' really means moss/lichen death
		bdall->d_v2soi.ltrfaln[i] = 0.;

		bdall->d_soi2v.snuptake[i] = 0.;
	  }
	  bdall->d_vegs.call    = 0.;
 	  bdall->d_vegs.labn    = 0.;
	  bdall->d_vegs.strnall = 0.;
	  bdall->d_vegs.nall    = 0.;

	  bdall->d_a2v.ingppall = 0.;
	  bdall->d_a2v.innppall = 0.;
	  bdall->d_a2v.gppall   = 0.;
	  bdall->d_a2v.nppall   = 0.;
	  bdall->d_v2a.rgall    = 0.;
	  bdall->d_v2a.rmall    = 0.;

	  bdall->d_v2soi.d2wdebrisc = 0.;
	  bdall->d_v2soi.d2wdebrisn = 0.;

	  bdall->d_v2soi.ltrfalcall = 0.;  // excluding moss/lichen
	  bdall->d_v2soi.ltrfalnall = 0.;  // excluding moss/lichen
	  bdall->d_v2soi.mossdeathc = 0.;
	  bdall->d_v2soi.mossdeathn = 0.;

	  bdall->d_v2v.nmobilall  = 0.;
	  bdall->d_v2v.nresorball = 0.;

  	  bdall->d_soi2v.innuptake = 0.;
  	  for (int il=0; il<MAX_SOI_LAY; il++) {
  		bdall->d_soi2v.nextract[il] = 0.;
  	  }
 	  bdall->d_soi2v.lnuptake   = 0.;
 	  bdall->d_soi2v.snuptakeall= 0.;

	  for (int ip=0; ip<NUM_PFT; ip++){
    	if (cd.d_veg.vegcov[ip]>0.){
    		bdall->d_v2soi.d2wdebrisc += bd[ip].d_v2soi.d2wdebrisc * cd.d_veg.vegcov[ip];
    		bdall->d_v2soi.d2wdebrisn += bd[ip].d_v2soi.d2wdebrisn * cd.d_veg.vegcov[ip];

    		for (int i=0; i<NUM_PFT_PART; i++){
    			bdall->d_vegs.c[i]    += bd[ip].d_vegs.c[i] * cd.d_veg.vegcov[ip];
    			bdall->d_vegs.strn[i] += bd[ip].d_vegs.strn[i] * cd.d_veg.vegcov[ip];

    			bdall->d_a2v.ingpp[i] += bd[ip].d_a2v.ingpp[i] * cd.d_veg.vegcov[ip];
    			bdall->d_a2v.innpp[i] += bd[ip].d_a2v.innpp[i] * cd.d_veg.vegcov[ip];
    			bdall->d_a2v.gpp[i]   += bd[ip].d_a2v.gpp[i] * cd.d_veg.vegcov[ip];
    			bdall->d_a2v.npp[i]   += bd[ip].d_a2v.npp[i] * cd.d_veg.vegcov[ip];
    			bdall->d_v2a.rg[i]    += bd[ip].d_v2a.rg[i] * cd.d_veg.vegcov[ip];
    			bdall->d_v2a.rm[i]    += bd[ip].d_v2a.rm[i] * cd.d_veg.vegcov[ip];

    			bdall->d_v2v.nmobil[i]  += bd[ip].d_v2v.nmobil[i] * cd.d_veg.vegcov[ip];
    			bdall->d_v2v.nresorb[i] += bd[ip].d_v2v.nresorb[i] * cd.d_veg.vegcov[ip];

    			if (cd.d_veg.nonvascular[ip]==0) {
    				bdall->d_v2soi.ltrfalc[i] += bd[ip].d_v2soi.ltrfalc[i] * cd.d_veg.vegcov[ip];
    				bdall->d_v2soi.ltrfaln[i] += bd[ip].d_v2soi.ltrfaln[i] * cd.d_veg.vegcov[ip];
    			}

    			bdall->d_soi2v.snuptake[i] += bd[ip].d_soi2v.snuptake[i] * cd.d_veg.vegcov[ip];
    		}
    		bdall->d_vegs.labn    += bd[ip].d_vegs.labn * cd.d_veg.vegcov[ip];
    		bdall->d_vegs.call    += bd[ip].d_vegs.call * cd.d_veg.vegcov[ip];
    		bdall->d_vegs.strnall += bd[ip].d_vegs.strnall * cd.d_veg.vegcov[ip];
    		bdall->d_vegs.nall    += bd[ip].d_vegs.nall * cd.d_veg.vegcov[ip];

    		bdall->d_a2v.ingppall += bd[ip].d_a2v.ingppall * cd.d_veg.vegcov[ip];
    		bdall->d_a2v.innppall += bd[ip].d_a2v.innppall * cd.d_veg.vegcov[ip];
			bdall->d_a2v.gppall   += bd[ip].d_a2v.gppall * cd.d_veg.vegcov[ip];
			bdall->d_a2v.nppall   += bd[ip].d_a2v.nppall * cd.d_veg.vegcov[ip];
			bdall->d_v2a.rgall    += bd[ip].d_v2a.rgall * cd.d_veg.vegcov[ip];
			bdall->d_v2a.rmall    += bd[ip].d_v2a.rmall * cd.d_veg.vegcov[ip];

			if (cd.d_veg.nonvascular[ip]==0) {
				bdall->d_v2soi.ltrfalcall += bd[ip].d_v2soi.ltrfalcall * cd.d_veg.vegcov[ip];
				bdall->d_v2soi.ltrfalnall += bd[ip].d_v2soi.ltrfalnall * cd.d_veg.vegcov[ip];
			}
			if (cd.d_veg.nonvascular[ip]>0){
				bdall->d_v2soi.mossdeathc += bd[ip].d_v2soi.mossdeathc * cd.d_veg.vegcov[ip];  //NOTE: non-vascular plants' litterfalling (mortality) is for death moss layer C
				bdall->d_v2soi.mossdeathn += bd[ip].d_v2soi.mossdeathn * cd.d_veg.vegcov[ip];
			}

			bdall->d_v2v.nmobilall  += bd[ip].d_v2v.nmobilall * cd.d_veg.vegcov[ip];
			bdall->d_v2v.nresorball += bd[ip].d_v2v.nresorball * cd.d_veg.vegcov[ip];

			bdall->d_soi2v.innuptake += bd[ip].d_soi2v.innuptake * cd.d_veg.vegcov[ip];
			for (int il=0; il<cd.d_soil.numsl; il++) {
				bdall->d_soi2v.nextract[il] += bd[ip].d_soi2v.nextract[il] * cd.d_veg.vegcov[ip];
			}
			bdall->d_soi2v.lnuptake   += bd[ip].d_soi2v.lnuptake * cd.d_veg.vegcov[ip];
			bdall->d_soi2v.snuptakeall+= bd[ip].d_soi2v.snuptakeall * cd.d_veg.vegcov[ip];

    	} // end of 'vegcov[ip]>0'

	  } // end of for ip=1,NUM_PFT

	  // below litter-fall vertical distribution needed to integrate from each PFT's
	  double sumrtltrfall = 0.;
	  for (int il=0; il<cd.d_soil.numsl; il++) {
		bdall->d_v2soi.rtlfalfrac[il] = 0.;
		for (int ip=0; ip<NUM_PFT; ip++) {
	    	if (cd.d_veg.vegcov[ip]>0.){
	    		bd[ip].d_v2soi.rtlfalfrac[il] = cd.d_soil.frootfrac[il][ip];
	    		bdall->d_v2soi.rtlfalfrac[il]+=bd[ip].d_v2soi.rtlfalfrac[il]*bd[ip].d_v2soi.ltrfalc[I_root];
	    	}
		}
		sumrtltrfall +=bdall->d_v2soi.rtlfalfrac[il];
	  }

	  for (int il=0; il<cd.d_soil.numsl; il++) {
		if (sumrtltrfall>0) {
			bdall->d_v2soi.rtlfalfrac[il] /=sumrtltrfall;
		} else {
			bdall->d_v2soi.rtlfalfrac[il] = 0.;
		}
	  }

	} //end if (md->timestep==DAILY)

};

