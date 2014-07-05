/*
 * RunCohort.cpp
 * 
 * Cohort initialization, run, and output
 * 		Note: the output modules are put here, so can be flexible for outputs
 * 
*/

#include "RunCohort.h"

RunCohort::RunCohort(){

    dstepcnt = 0;
    mstepcnt = 0;
    ystepcnt = 0;

	cohortcount = 0;   // counter for cohort have been run

 	//
	initrecno = MISSING_I;
 	clmrecno  = MISSING_I;
 	vegrecno  = MISSING_I;
 	firerecno = MISSING_I;

 	used_atmyr = MISSING_I;
    yrstart    = MISSING_I;
    yrend      = MISSING_I;

    md = NULL;

}

RunCohort::~RunCohort(){

}

void RunCohort::setModelData(ModelData * mdp){
  	md = mdp;
}

//reading cohort-level all data ids
int RunCohort::allchtids(){
	int error = 0;
	int id = MISSING_I;
	int id1 = MISSING_I;
	int id2 = MISSING_I;
	int id3 = MISSING_I;
	int id4 = MISSING_I;
	int id5 = MISSING_I;

	// from 'cohortid.nc'
	for (int i=0; i<md->act_chtno; i++) {
		error = cinputer.getChtDataids(id, id1, id2, id3, id4, id5, i);
		if (error!=0) return (error);

		chtids.push_back(id);
		chtinitids.push_back(id1);
		chtgridids.push_back(id2);
		chtclmids.push_back(id3);
		chtvegids.push_back(id4);
		chtfireids.push_back(id5);

	}

	// from 'restart.nc' or 'sitein.nc'
	if (md->initmode>1) { // 'runeq' stage doesn't require initial file
		for (int i=0; i<md->act_initchtno; i++) {
			error = cinputer.getInitchtId(id, i);
			if (error!=0) return (error);
			initids.push_back(id);
		}
	}

	// from 'climate.nc'
	for (int i=0; i<md->act_clmno; i++) {
		error = cinputer.getClmId(id, i);
		if (error!=0) return (error);
		clmids.push_back(id);
	}

	// from 'vegetation.nc'
	for (int i=0; i<md->act_vegno; i++) {
		error = cinputer.getVegId(id, i);
		if (error!=0) return (error);
		vegids.push_back(id);
	}

	// from 'fire.nc'
	for (int i=0; i<md->act_fireno; i++) {
		error = cinputer.getFireId(id, i);
		if (error!=0) return (error);
		fireids.push_back(id);
	}

	return (error);
};

// general initialization
void RunCohort::init(){

	// switches of N cycles
    md->nfeed   = true;
    md->avlnflg = true;
	md->baseline= true;

	// switches of modules
	md->envmodule = true;
    md->bgcmodule = true;
    md->dsbmodule = true;
    md->dslmodule = true;
    md->dvmmodule = true;

	// output (buffer) data connection
	 if (md->outRegn) {
		 cht.outbuffer.setRegnOutData(&regnod);
	 }

	 cht.outbuffer.setRestartOutData(&resod);   //restart output data sets connenction

	 // output operators
	 regnouter.setOutData(&regnod);
	 resouter.setRestartOutData(&resod);
}

//read-in one-timestep data for a cohort
int RunCohort::readData(){

	//reading the climate data
	cht.cd.act_atm_drv_yr = md->act_clmyr;
	if (md->act_clmstep == MINY) { //if climate data is in monthly time-step, read it all-years
		cinputer.getClimate(cht.cd.tair, cht.cd.prec, cht.cd.nirr, cht.cd.vapo, 0, cht.cd.act_atm_drv_yr, clmrecno);
	} else if (md->act_clmstep == DINY) { //if climate data is in daily time-step, read it yearly for calculating monthly data
	    float ta[DINY];
	    float pre[DINY];
	    float nir[DINY];
	    float vap[DINY];

	    for (int iyr=0; iyr<cht.cd.act_atm_drv_yr; iyr++) {
			cinputer.getClimate(ta, pre, nir, vap, iyr, 1, clmrecno);
			for (int im=0; im<MINY; im++) {
				float mta = 0.f;
				float mpre = 0.f;
				float mnir = 0.f;
				float mvap = 0.f;
				for (int id=0; id<DINM[im]; id++) {
					mta  += ta[DOYINDFST[im]+id];
					mpre += pre[DOYINDFST[im]+id];
					mnir += nir[DOYINDFST[im]+id];
					mvap += vap[DOYINDFST[im]+id];
				}
				cht.cd.tair[iyr*MINY+im] = mta/DINM[im];
				cht.cd.prec[iyr*MINY+im] = mpre;
				cht.cd.nirr[iyr*MINY+im] = mnir/DINM[im];
				cht.cd.vapo[iyr*MINY+im] = mvap/DINM[im];
			}
	    }
		//
	}

	//reading the vegetation community type data from 'vegetation.nc'
	cht.cd.act_vegset = md->act_vegset;
	cinputer.getVegetation(cht.cd.vegyear, cht.cd.vegtype, cht.cd.vegfrac, vegrecno);

	//INDEX of veg. community codes, must be one of in those parameter files under 'config/'
	cht.cd.cmttype = cht.cd.vegtype[0];  //default, i.e., the first set of data
	for (int i=1; i<md->act_vegset; i++) {
		if (cht.cd.year>=cht.cd.vegyear[i]) {
			cht.cd.cmttype = cht.cd.vegtype[i];
		}
	}

	// read-in parameters AND initial conditions for the above 'cmttype'
	 string configdir = md->configdir; //"config/";
	 cht.chtlu.dir = configdir;
	 stringstream ss;
	 ss<<cht.cd.cmttype;
	 if (cht.cd.cmttype<10){
		 cht.chtlu.cmtcode = "CMT0"+ss.str();
	 } else {
		 cht.chtlu.cmtcode = "CMT"+ss.str();
	 }
	 cht.chtlu.init();   //put the parameter files in 'config/' with same directory of model

	//reading the fire occurence data from '.nc', if not FRI derived
  	if (!md->friderived && !md->runeq){
  		cht.cd.act_fireset = md->act_fireset;
  		cinputer.getFire(cht.cd.fireyear, cht.cd.fireseason, cht.cd.firesize, firerecno);
  		if (md->useseverity) {
  			cinputer.getFireSeverity(cht.cd.fireseverity, firerecno);
  		}
  	}

    return (0);
};

// re-initializing state variables for current cohort
int RunCohort::reinit(){

	// initializing module-calling controls
	cht.failed  = false;
	cht.errorid = 0;
	int errcode = 0;
	
    // checking
	if (initrecno < 0 && md->initmode!=1) {
		cout<<"initial condition record not exists! \n";
		return (-1);
	}
	 
	 //initial modes other than lookup (i.e., initmode = 1)
	 if (md->initmode==2) {  // not yet done!
		 //note: the cohort order in sitein.nc must be exactly same as cohort in cohortid.nc
/*		 int err=0;
		 err=sinputer->getSiteinData(cht.md->chtinputdir,&cht.sitein, cid);
		 if (err!=0) return (-1);
*/
	 } else if (md->initmode == 3) {

		 resinputer.getErrcode(errcode, initrecno);
		 if (errcode!=0) {
			 return (-1);
		 } else {

			 resinputer.getReschtId(cht.resid.chtid, initrecno);
			 resinputer.getRestartData(&cht.resid, initrecno);
		 }

	 }
	 
	 // soil texture from gridded data
	 for (int il=0; il<MAX_MIN_LAY; il++) {
		 double topthick = 0.;
		 topthick +=MINETHICK[il];
		 if (topthick <=0.30) {
			 cht.chtlu.minetexture[il] = cht.cd.gd->topsoil;
		 } else {
			 cht.chtlu.minetexture[il] = cht.cd.gd->botsoil;
		 }
	 }

	 //set initial state variables and parameters read-in from above
	 cht.initStatePar();

	 //clm/fire driving data (monthly/all years)
	 cht.prepareAllDrivingData();

     return (0);
};

// run one cohort for a period of time
void RunCohort::run_OneCohort(){

	    //
	    cht.timer->reset();

	    //
		if(cht.md->runeq){
		    used_atmyr = fmin(MAX_ATM_NOM_YR, cht.cd.act_atm_drv_yr);

			// a quick pre-run to get reasonably-well 'env' conditions, which may be good for 'eq' run
			runEnvmodule();

			//
			cht.timer->reset();
			md->envmodule = true;
		    md->bgcmodule = true;
		    md->dsbmodule = true;
		    md->dslmodule = true;
		    md->dvmmodule = true;

			md->friderived = true;
			cht.timer->stageyrind = 0;

			cht.cd.yrsdist = 0;

		    yrstart = 0;

		    if (cht.gd->fri>0) {
		    	int nfri = fmax(MIN_EQ_YR/cht.gd->fri, 20);
		    	nfri = fmin(nfri, MAX_EQ_YR/cht.gd->fri); //20 FRI and within range of min. and max. MAX_EQ_YR
		    	yrend= nfri*cht.gd->fri-1;   // ending just prior to the fire occurrency year
		    } else {
		    	yrend = MAX_EQ_YR;
		    }

		    run_timeseries();

		}

		if(cht.md->runsp){
			cht.timer->stageyrind = 0;
			cht.timer->eqend = true;

		    used_atmyr = fmin(MAX_ATM_NOM_YR, cht.cd.act_atm_drv_yr);

		    yrstart = cht.timer->spbegyr;
		    yrend   = cht.timer->spendyr;

		    md->friderived= false;

		    run_timeseries();

		}
		
		if(cht.md->runtr){	
			cht.timer->stageyrind = 0;
			cht.timer->eqend = true;
			cht.timer->spend = true;

		    used_atmyr = cht.cd.act_atm_drv_yr;

		    yrstart = cht.timer->trbegyr;
		    yrend   = cht.timer->trendyr;

		    md->friderived= false;

		    run_timeseries();

		}

		if(cht.md->runsc){
			cht.timer->stageyrind = 0;
			cht.timer->eqend = true;
			cht.timer->spend = true;
			cht.timer->trend = true;

		    used_atmyr = cht.cd.act_atm_drv_yr;

		    yrstart = cht.timer->scbegyr;
		    yrend   = cht.timer->scendyr;

		    md->friderived= false;

		    run_timeseries();

		}

		//'restart.nc' always output at the end of run-time
		resouter.outputVariables(cohortcount);
	
}; 

void RunCohort::runEnvmodule(){
	//run model with "ENV module" only

	 md->envmodule = true;
     md->bgcmodule = false;
     md->dsbmodule = false;
     md->dslmodule = false;
     md->dvmmodule = false;

     cht.cd.yrsdist = 1000;

     yrstart = 0;
     yrend   = 100;

     run_timeseries();

};

// run one cohort in time series
void RunCohort::run_timeseries(){

	for (int icalyr=yrstart; icalyr<=yrend; icalyr++){

		 int yrindex = cht.timer->getCurrentYearIndex();   //starting from 0
		 cht.cd.year = cht.timer->getCalendarYear();

		 if (md->act_clmstep == DINY) { //if climate data is in daily time-step, read it yearly
			int clmyrcount = yrindex%used_atmyr;  // this will recycle climate data series
			cinputer.getClimate(cht.cd.d_tair, cht.cd.d_prec, cht.cd.d_nirr, cht.cd.d_vapo, clmyrcount, 1, clmrecno);
		 }
		 cht.prepareDayDrivingData(yrindex, used_atmyr);

		 int outputyrind = cht.timer->getOutputYearIndex();
		 for (int im=0; im<12; im++){
		   int currmind=  im;
		   cht.cd.month = im+1;
		   int dinmcurr = cht.timer->getDaysInMonth(im);;

		   for (int id=0; id<dinmcurr; id++) {
			   cht.updateOneTimestep(yrindex, currmind, id);

			   // site-daily output
			   if (outputyrind >=0 && md->outSiteDay){
				   envdlyouter.outputCohortEnvVars_dly(-1, &cht.cd.d_snow, cht.edall,
						   icalyr, im, id, dstepcnt);     // this will output non-veg (multiple PFT) related variables
				   for (int ip=0; ip<NUM_PFT; ip++) {
					   if (cht.cd.d_veg.vegcov[ip]>0.){
	    			    	envdlyouter.outputCohortEnvVars_dly(ip,&cht.cd.d_snow, cht.edall,
	    			    			icalyr, im, id, dstepcnt);
	    			   }
				   }

				   bgcdlyouter.outputCohortBgcVars_dly(-1, &cht.cd, cht.bdall,
						   icalyr, im, id, dstepcnt);     // this will output non-veg (multiple PFT) related variables

				   dstepcnt++;
	    	   }
		   } // end of for loop of daily
		   cht.timer->advanceOneMonth();

		   // site-monthly output
		   if (outputyrind >=0 && md->outSiteMonth){
	    		   dimmlyouter.outputCohortDimVars_mly(&cht.cd, mstepcnt);
	    		   envmlyouter.outputCohortEnvVars_mly(-1, &cht.cd.m_snow, cht.edall,
	    				                               icalyr, im, mstepcnt);
		    	   bgcmlyouter.outputCohortBgcVars_mly(-1, cht.bdall, cht.fd,
		    			                               icalyr, im, mstepcnt);

		    	   for (int ip=0; ip<NUM_PFT; ip++) {
	    		    	if (cht.cd.m_veg.vegcov[ip]>0.){
	    		    		envmlyouter.outputCohortEnvVars_mly(ip, &cht.cd.m_snow, &cht.ed[ip],
	    		    				icalyr, im, mstepcnt);
	    		    		bgcmlyouter.outputCohortBgcVars_mly(ip, &cht.bd[ip], cht.fd,
	    		    				icalyr, im, mstepcnt);
	    		    	}
	    		   }
	    		   mstepcnt++;
		   }

		   // site-yearly output
		   if (outputyrind >= 0 && md->outSiteYear && im==11){
	    		   dimylyouter.outputCohortDimVars_yly(&cht.cd, ystepcnt);
	    		   envylyouter.outputCohortEnvVars_yly(-1, &cht.cd.y_snow, cht.edall, icalyr, ystepcnt);
		    	   bgcylyouter.outputCohortBgcVars_yly(-1, cht.bdall, cht.fd, icalyr, ystepcnt);

		    	   for (int ip=0; ip<NUM_PFT; ip++) {
	    		    	if (cht.cd.y_veg.vegcov[ip]>0.){
	    		    		envylyouter.outputCohortEnvVars_yly(ip, &cht.cd.y_snow, &cht.ed[ip],
	    		    				icalyr, ystepcnt);
	    		    		bgcylyouter.outputCohortBgcVars_yly(ip, &cht.bd[ip], cht.fd,
	    		    				icalyr, ystepcnt);
	    		    	}
	    		   }
	    		   ystepcnt++;
	       }

		 } // end of for loop of monthly

		 // output of summarized variables as requested at end of a year
		 if (md->outRegn && outputyrind >=0){
			regnouter.outputCohortVars(outputyrind, cohortcount, 0);  // "0" implies good data
		 }

		 if(cht.md->consoledebug){
			cout <<"TEM " << cht.md->runstages <<" run: year "
					   <<icalyr<<" @cohort "<<cohortcount+1<<"\n";

		 }

		 // if EQ run, option for simulation break (temporarily off)
		 if (cht.md->runeq) {
  	   		//cht.equiled = cht.testEquilibrium();
  	   		//if(cht.equiled )break;
		 }

	} //end of for loop of yearly

};

// run one cohort at one time-step (monthly)
void RunCohort::run_OneTimestep(){

	 // timing
	 int yrindex = cht.timer->getCurrentYearIndex();     // starting from 0
	 cht.cd.year = cht.timer->getCalendarYear();

     int mnindex = cht.timer->getCurrentMonthIndex();    // 0 - 11
	 cht.cd.month = mnindex + 1;

     int dinmcurr = cht.timer->getDaysInMonth(mnindex);  // 28/30/31

     int outputyrind = cht.timer->getOutputYearIndex();  // starting from 0 (i.e., md->outstartyr)

     // driving data re-set when timer is ticking
     cht.prepareAllDrivingData();
	 cht.prepareDayDrivingData(yrindex, used_atmyr);

     // calling the core model modules
	 cht.updateOneTimestep(yrindex, mnindex, dinmcurr);

	 //'restart.nc' always output at the end of time-step (monthly)
	 resouter.outputVariables(cohortcount);

   	 // output module calling at end of year
	 if (md->outRegn && (outputyrind>=0 && cht.cd.month==11)){
		 regnouter.outputCohortVars(outputyrind, cohortcount, 0);  // "0" implies good data
	 }

};


////////////////////////////////////////////////////////////////////////
