/*
 * Soil_Env.cpp
 *
 * Purpose: Calculating Soil thermal and hydrological processes
 *
 * History:
 *     June 28, 2011, by F.-M. Yuan:
 *          (1) Recoding based on DOS-TEM's code;
 *          (2) all unused functions are removed;
 *          (3) The soil moiture temperature calling is moved from "Ground.cpp" to here,
 *          so that "Ground.cpp" only for soil-domain operation
 *
 * Important:
 *     (1) Parameters are read from 'CohortLookup.cpp', and set to 'bgcpar' (struct:: soipar_bgc)
 *     (2) Calibrated Parameters are also read from 'CohortLookup.cpp' initially, and set to 'calpar' (strut:: soipar_cal)
 *
 *     (3) The calculation is for ONE community with multple PFT. So total water input must be from the total.
 *
 *     (4) THREE (3) data pointers must be initialized by calling corresponding 'set...' methods
 *          chtlu, ed, fd
 *
 *     (5) Daily time-step only, and all temporal integration is done in 'ed'
 *     (6) For state variables, must use monthly ones (all monthly variables must NOT be empty)
 */

#include "Soil_Env.h"

Soil_Env::Soil_Env(){
	tstepmode = DAILY;
	chtlu  = NULL;
	ground = NULL;
	gd     = NULL;
    cd_vegd = NULL;
    cd_soil = NULL;
	ed_atms = NULL;
	ed_atmd = NULL;
	ed_a2l  = NULL;
	ed_v2a  = NULL;
	ed_v2g  = NULL;
	ed_snws = NULL;
	ed_snwd = NULL;
	ed_snw2a  = NULL;
	ed_snw2soi= NULL;
	ed_sois = NULL;
	ed_soid = NULL;
	ed_soi2a= NULL;
	ed_soi2l= NULL;

};

Soil_Env::~Soil_Env(){
	
};

void Soil_Env::initializeParameter(){

    envpar.psimax  = chtlu->psimax;
    envpar.evapmin = chtlu->evapmin;

    envpar.drainmax = chtlu->drainmax;

    envpar.rtdp4gdd = chtlu->rtdp4gdd;

};

void Soil_Env::initializeState(){

	bool sitein = false;
	if (sitein) {
		//in 'chtlu', initial soil tem and vwc are in each 10 cm thickness of layers for at most 10 layers
		// here, these 10 layers of 10 cm are thickness-weightedly assigned to the soil structure already set in 'ground.cpp' (which already in cd->m_soil)
		double Zsoil[10];
		double TSsoil[10];
		double VWCsoil[10];
		for (int i=0; i<10; i++){
			Zsoil[i]=i*0.10;
			TSsoil[i]=chtlu->initts[i];
			VWCsoil[i]=chtlu->initvwc[i];
		}

		Layer* currl = ground->fstsoill;
		int ilint = 0;
		while(currl!=NULL){
			if(currl->isSoil){
				double ts  = 0.;
				double vwc = 0.;
				double dzleft = currl->dz;
				while (dzleft>0.){
					if (currl->z<=Zsoil[ilint] && currl->z<=Zsoil[ilint]+0.10) {
						double dzdone = fmin(currl->dz, (Zsoil[ilint]+0.10)-currl->z);

						ts  += TSsoil[ilint]*dzdone/currl->dz;
						vwc += VWCsoil[ilint]*dzdone/currl->dz;

						dzleft -= dzdone;
					} else {
						ilint++;
						if (ilint>=9) {
							ilint=9;     // assuming the 1.0m below is same Ts/VWCsoil as the last 10 cm' input
							ts = TSsoil[ilint];
							vwc= VWCsoil[ilint];
							dzleft = 0.;
						}
					}

				}

				currl->tem = ts;

				if (currl->tem>0.){
					currl->liq = fmax(currl->minliq, fmax(currl->maxliq, vwc*currl->dz*DENLIQ));
					currl->ice = 0.;
					currl->frozen = -1;
				} else {
					currl->ice = fmax(0., fmax(currl->maxice, vwc*currl->dz*DENICE))-currl->minliq;
					currl->liq = currl->minliq;
					currl->frozen = 1;
				}

				currl->age =0;
				currl->rho =0;    //soil layer's actual density NOT used in model

			}else if (currl->isRock) {
				currl->tem = currl->prevl->tem;
				currl->liq = currl->prevl->liq;
				currl->ice = currl->prevl->ice;
				currl->frozen = currl->prevl->frozen;
				currl->age = MISSING_I;
				currl->rho = MISSING_D;

			}else{
				break;
			}

			currl->frozenfrac = 0.;
			if(currl->frozen == 1) currl->frozenfrac = 1.0;

			currl = currl->nextl;
		}

		// end of 'sitein'

	} else {
		Layer* currl = ground->fstsoill;
		while(currl!=NULL){
			if(currl->isSoil){
				double ts  = ed_atms->ta - 1.0;  // colder soil so that water won't be losing; otherwise it's hard to get it back when spinup
				double psifc = -2.e6;  // field capacity of -2MPsi
				psifc /=currl->psisat;
			  	double vwc = pow(abs(psifc), -1.0/currl->bsw);

				currl->tem = ts;

				if (currl->tem>0.){
					currl->liq = fmax(currl->minliq, fmax(currl->maxliq, vwc*currl->dz*DENLIQ));
					currl->ice = 0.;
					currl->frozen = -1;
				} else {
					currl->ice = fmax(0., fmax(currl->maxice, vwc*currl->dz*DENICE));
					currl->liq = 0.;
					currl->frozen = 1;
				}

				currl->age =0;
				currl->rho =0;    //soil layer's actual density NOT used in model

			}else if (currl->isRock) {
				currl->tem = currl->prevl->tem;
				currl->liq = currl->prevl->liq;
				currl->ice = currl->prevl->ice;
				currl->frozen = currl->prevl->frozen;
				currl->age = MISSING_I;
				currl->rho = MISSING_D;

			}else{
				break;
			}

			currl->frozenfrac = 0.;
			if(currl->frozen == 1) currl->frozenfrac = 1.0;

			currl = currl->nextl;
		}

	}

 	//fronts initialization, if any
 	ground->frontsz.clear();
 	ground->frontstype.clear();
	int frontFT[MAX_NUM_FNT];
	double frontZ[MAX_NUM_FNT];
	std::fill_n(frontFT, MAX_NUM_FNT, -9999.0);
	std::fill_n(frontZ, MAX_NUM_FNT, -9999.0);

	Layer* currl = ground->toplayer;
	int ilint = 0;
	while(currl!=NULL){
		if(!currl->isSnow && currl->nextl!=NULL){
			if (currl->frozen != currl->nextl->frozen) {
				frontZ[ilint]=currl->z+currl->dz*0.9999;
				frontFT[ilint]=currl->frozen;
				ilint+=1;
			}
		}
		currl=currl->nextl;
	}

	for(int ifnt = 0; ifnt<MAX_NUM_FNT; ifnt++){
  	    if(frontZ[ifnt]>0.){
  	    	ground->frontsz.push_front(frontZ[ifnt]);
  	    	ground->frontstype.push_front(frontFT[ifnt]);
  	    }
  	}

	ground->setFstLstFrontLayers();
	ground->checkWaterValidity();

    // assign 'ground' data to 'ed'
	retrieveDailyTM(ed_atms->ta, ground->toplayer, ground->lstsoill);
	retrieveDailyFronts();

	//misc. items
	ed_atms->dsr     = 0;
	ed_sois->monthsfrozen   = 0;
	ed_sois->rtfrozendays   = 0;
	ed_sois->rtunfrozendays = 0;


};

void Soil_Env::initializeState5restart(RestartData* resin){

	double TSsoil[MAX_SOI_LAY];
	double LIQsoil[MAX_SOI_LAY];
	double ICEsoil[MAX_SOI_LAY];
	int FROZENsoil[MAX_SOI_LAY];
	double FROZENFRACsoil[MAX_SOI_LAY];

	for (int i=0; i<MAX_SOI_LAY; i++){
		TSsoil[i]=resin->TSsoil[i];
		LIQsoil[i]=resin->LIQsoil[i];
		ICEsoil[i]=resin->ICEsoil[i];
		FROZENsoil[i]=resin->FROZENsoil[i];
		FROZENFRACsoil[i]=resin->FROZENFRACsoil[i];
	}

	Layer* currl = ground->fstsoill;
	int slind =-1;
	while(currl!=NULL){
		if(currl->isSoil){
		  slind ++;
		  currl->tem = TSsoil[slind];
		  currl->liq = LIQsoil[slind];
		  currl->ice = ICEsoil[slind];
		  currl->frozenfrac = FROZENFRACsoil[slind];
		  currl->frozen = FROZENsoil[slind];

		}else{
		  break;
		}

		currl = currl->nextl;
	}

  	// fronts
  	ground->frontsz.clear();
  	ground->frontstype.clear();
	int frontFT[MAX_NUM_FNT];
	double frontZ[MAX_NUM_FNT];
	for (int i=0; i<MAX_NUM_FNT; i++){
		frontZ[i]=resin->frontZ[i];
		frontFT[i]=resin->frontFT[i];
	}
   	for(int ifnt = 0; ifnt<MAX_NUM_FNT; ifnt++){
   	    if(frontZ[ifnt]>0.){
   	    	ground->frontsz.push_front(frontZ[ifnt]);
   	    	ground->frontstype.push_front(frontFT[ifnt]);
   	    }
   	}
	ground->setFstLstFrontLayers();
	ground->checkWaterValidity();

    // assign 'ground' data to 'ed'
	retrieveDailyTM(ed_atms->ta, ground->toplayer, ground->lstsoill);
	retrieveDailyFronts();

	//
	ed_atms->dsr     = resin->dsr;
	ed_sois->monthsfrozen   = resin->monthsfrozen;
	ed_sois->rtfrozendays   = resin->rtfrozendays;
	ed_sois->rtunfrozendays = resin->rtunfrozendays;
	ed_soid->rtdpgdd = resin->growingttime[0];

};

// Ground (snow-soil) thermal process
void Soil_Env::updateDailyGroundT(const double & tdrv, const double & dayl){

	double tsurface;
	tsurface = tdrv *ed_soid->nfactor;

	if(ground->toplayer->isSoil){
		updateDailySurfFlux(ground->toplayer, dayl);
		ed_snw2a->swrefl = 0.;
		ed_snw2a->sublim = 0.;
	}

    // solution for snow-soil column thermal process
	int nstep = 1;
    if (ground->toplayer->isSoil){
    	//when there is an abrupt change of surface status, reduce timestep
   		if((ground->fstsoill->frozen==1 and tsurface>0.0)
   			  || (ground->fstsoill->frozen==-1 and tsurface<0.0)
   			  || ground->fstsoill->frozen==0) {

   			nstep = 24;

   		}
     }

     double timestep=86400.0/nstep;
     bool meltsnow =false;
     if (ed_snw2soi->melt>0.) meltsnow = true;  //So, 'snow melting in snow water process' must be done prior to this

     stefan.initpce();
     for (int i=0; i<nstep ;i++){
    	// 1) find the thawing/freezing fronts
    	int tstate = ground->ststate;
    	if(ground->fstfntl==NULL && ground->lstfntl==NULL ){   // no front
       		if((tstate == 1 && tsurface>0) ||     // frozen soil and above-zero air
       		   (tstate == -1 && tsurface<0) ||    // unfrozen soil and below-zero air
       		    tstate == 0){                     // partially frozen soil column

       			stefan.updateFronts(tsurface, timestep);
       		}
    	}else{
    		stefan.updateFronts(tsurface, timestep);
    	}

    	// 2) ground (snow/soil) temperature solution
    	ground->setFstLstFrontLayers();        // this must be called before the following
    	tempupdator.updateTemps(tsurface, ground->toplayer, ground->botlayer, ground->fstsoill,
    			ground->fstfntl, ground->lstfntl, timestep, meltsnow);

       	// checking
       	ground->checkWaterValidity();

       	ground->updateWholeFrozenStatus();        // for the whole column

     }

	// 3) at end of each day, 'ed' should be updated for thermal properties
     // (better to do here, because some variables are needed for soil moisture module)
	updateDailySoilThermal4Growth(ground->fstsoill, tsurface);  // this is needed for growing
	updateLayerStateAfterThermal(ground->fstsoill, ground->lstsoill, ground->botlayer);  //this shall be done before the following
	retrieveDailyFronts();  // update 'ed' with new soil thawing/freezing fronts, and daily 'ald', 'cld'

};

void Soil_Env::updateDailySurfFlux(Layer* toplayer, const double & dayl){

	// soil surface radiation budget
	double albvis = dynamic_cast<SoilLayer*>(toplayer)->getAlbedoVis();
	double albnir = dynamic_cast<SoilLayer*>(toplayer)->getAlbedoNir();

	double insw =  ed_v2g->swthfl * cd_vegd->fpcsum
			      + ed_a2l->nirr * (1.- cd_vegd->fpcsum);

	ed_soi2a->swrefl = insw *0.5 * albvis + insw *0.5*albnir;

	// soil evaporation
	double rad = insw-ed_soi2a->swrefl;
	double availliq =0;

	double totthick =0.10; // assuming occurred only from top 10 cm soil
	double dzsum =0;
	Layer* currl = toplayer;
	while(currl!=NULL){
		if (currl->isSoil) {
			if(dzsum <totthick){
				if(dzsum + currl->dz <totthick){
					dzsum +=currl->dz;
					availliq += fmax(0., currl->liq-0.01*currl->maxliq);

				}else{
					availliq += fmax(0., currl->liq-0.01*currl->maxliq) * (totthick-dzsum)/totthick;
					dzsum = totthick;

				}

			}else {
				break;
			}
		}
		currl= currl->nextl;
	}
	if(availliq<0.) availliq = 0.;    //unit: kg/m2 or mm H2O

	double evap =0.;
	if(availliq>0 && toplayer->frozen==-1 && toplayer->isSoil){
		evap = getEvaporation(dayl, rad);
		ed_soi2a->evap = fmin(availliq,evap);
	}else{
	    ed_soi2a->evap = 0.;
	}

};

void Soil_Env::updateDailySoilThermal4Growth(Layer* fstsoill, const double &tsurface){

	Layer* currl = fstsoill;

	double toprtdep = 0.; //top root zone depth
	double toptsrtdep  = 0.; // top root zone soil temperature
	double unfrzrtdep = 0.;

	while (currl!=NULL){
		if(currl->isSoil){
				if(toprtdep<envpar.rtdp4gdd) {
					  double restrtdz = fmax(0., envpar.rtdp4gdd-toprtdep);

					  toprtdep += fmin(currl->dz, restrtdz);

			  		  toptsrtdep += currl->tem *fmin(currl->dz, restrtdz);

		  			  // unfrozen thickness of root zone
		  			  if(currl->frozen==-1){//unfrozen
		  				  unfrzrtdep+=currl->dz;

		  			  }else if(currl->frozen==0){//with front
		  				  if(currl->prevl==NULL) {
		  					  if (tsurface>0.) unfrzrtdep +=(currl->frozenfrac*currl->dz);
		  				  } else if (currl->prevl->frozen==-1) {
		  					  unfrzrtdep +=(currl->frozenfrac*currl->dz);
		  				  }
		  			  }

				}


		}else{
			break;
		}

		currl =currl->nextl;
	}

	if (toprtdep > 0.) {
		ed_soid->rtdpts = toptsrtdep/toprtdep;
	} else {
		ed_soid->rtdpts = MISSING_D;
	}

	if (ed_soid->rtdpts!=MISSING_D) {
		if(ed_soid->rtdpts>=0.10 ){
			ed_soid->rtdpthawpct =1.;
		}else{
			ed_soid->rtdpthawpct =0.;
		}

	} else {
		ed_soid->rtdpthawpct = MISSING_D;
	}


};

//
void Soil_Env::updateLayerStateAfterThermal(Layer* fstsoill, Layer *lstsoill, Layer* botlayer){

	Layer * currl=fstsoill;

	double unfrzcolumn=0.;
	while(currl!=NULL){
	  if(currl->isSoil){
		  unfrzcolumn+= (1. - currl->frozenfrac)*currl->dz;
	  } else {
		  break;
	  }

	  currl = currl->nextl;
	}

	ed_soid->unfrzcolumn = unfrzcolumn;
	ed_soid->tbotrock = botlayer->tem;

	if(lstsoill->frozen==-1){        //Yuan: -1 should be unfrozen
		ed_sois->permafrost =0;
	}else{
		ed_sois->permafrost =1;
	}

}

void Soil_Env::retrieveDailyFronts(){
   for (int il=0; il<MAX_NUM_FNT; il++){
	   ed_sois->frontsz[il]   = MISSING_D;
	   ed_sois->frontstype[il]= MISSING_I;
   }

   int frntnum = ground->frontsz.size();
   for(int il=0; il<frntnum; il++){
	   ed_sois->frontsz[il]   = ground->frontsz[il];
	   ed_sois->frontstype[il]= ground->frontstype[il];
   }

   // determine the depth of daily active layer depth (seasonal or permafrost)
   ed_sois->ald = MISSING_D;
    for (int il =0; il<MAX_NUM_FNT; il++){
	   if (il==0 && ed_soid->unfrzcolumn<=0.) {
		   ed_sois->ald = 0.;
		   break;
	   } else if (il==0 && ed_soid->unfrzcolumn>=cd_soil->totthick) {
		   ed_sois->ald = cd_soil->totthick;
		   break;
	   } else if(ed_sois->frontsz[il]>0. && ed_sois->frontstype[il]==-1){
		   if(ed_sois->ald < ed_sois->frontsz[il]){    // assuming the deepest thawing front
			   ed_sois->ald = ed_sois->frontsz[il];
		   }
	  }

	}

   // determine the top depth of daily active layer (seasonal)
    ed_sois->alc = 0.;
	for (int il =0; il<MAX_NUM_FNT; il++){
		  if (il==0 && ed_soid->unfrzcolumn==0.) {
			  ed_sois->alc = 0.;
			  break;
		  } else if (il==0 && ed_soid->unfrzcolumn>=cd_soil->totthick) {
			  ed_sois->alc = cd_soil->totthick;
			  break;
		  } else if(ed_sois->frontsz[il]>0. && ed_sois->frontstype[il]==1){
		  	 if(ed_sois->alc < ed_sois->frontsz[il]){    // assuming the deepest freezing front
		  	 	ed_sois->alc = ed_sois->frontsz[il];
		  	 }
		  }
	}

};

// soil moisture calculation
void Soil_Env::updateDailySM(){

	// define the soil water module's domain
	Layer * fstsoill = ground->fstsoill;
	Layer * lstsoill = ground->lstsoill;
	Layer * drainl    = ground->drainl;
	double draindepth = ground->draindepth;

	// First, data connection
	double trans[MAX_SOI_LAY], melt, evap, rnth;

	for (int i=0; i<MAX_SOI_LAY; i++) {
		trans[i] = ed_v2a->tran*ed_soid->fbtran[i];  //mm/day: summed for all Vegetations
	}
    evap  = ed_soi2a->evap; //mm/day: summed for soil evaporation
    rnth  = (ed_v2g->rthfl +ed_v2g->rdrip)   // note: rthfl and rdrip are already fpc adjusted
    		+(1.- cd_vegd->fpcsum)*ed_a2l->rnfl; //mm/day

    melt  = ed_snw2soi->melt; //mm/day

	// 1) calculate the surface runoff and infiltration
    ed_soi2l->qover  = 0.;
    ed_soi2l->qdrain = 0.;

    ed_sois->watertab = getWaterTable(lstsoill);
    if(rnth+melt>0){
       	ed_soi2l->qover  = getRunoff(fstsoill, drainl, rnth, melt); //mm/day
    }else{
       	ed_soi2l->qover  = 0.;
    }

    double infil = rnth+melt-ed_soi2l->qover;
    ed_soi2l->qinfl = infil;

    // 2) Then soil water dynamics at daily time step

    double sinday= 86400.;

	for (int i=0; i<MAX_SOI_LAY; i++) {
		trans[i] /=sinday; // mm/day to mm/s
	}
	infil /=sinday; // mm/day to mm/s
	evap /=sinday;  // mm/day to mm/s

 	// water drainage condition
	double baseflow = 1.;    //fraction of bottom drainage (free) into water system: 0 - 1 upon drainage condition
    if(gd->drgtype==1){  //0: well-drained; 1: poorly-drained
 		baseflow = 0.;
  	}

   	richards.update(fstsoill, drainl, draindepth, baseflow, trans, evap, infil, sinday);
   	ed_soi2l->qdrain  += richards.qdrain;

   	//
   	ground->checkWaterValidity();
};


double Soil_Env::getPenMonET(const double & ta, const double& vpd, const double &irad,
				const double &rv, const double & rh){
		double et; // out , mmH2O/m2s= kgH2o/m2s
		double CP =1004.64 ; // specific heat capacity of dry air [J/kgK)
		double tk = ta+273.15;
		double pa = 101300;// pressure , Pa
		double rho = 1.292- (0.00428 * ta); // air density	kg/m3
		double EPS=0.6219; // ratio of mole weights
		double SBC= 5.67e-8; //Stefan-boltzmann constant W/m2K4
		/*resistance to raiative heat transfer through air*/
		double rr = rho * CP /(4.0 * SBC * tk* tk*tk);		
		/* resistance to convective heat tranfer: rh*/
		/*resistance to latent heat transfer rv*/
		/*combined resistance to convectie and radiative heat transfer,
		 * parallel resistances:rhr= (rh*rr)/(rh+rr)*/
		 double rhr = (rh*rr)/(rh+rr);
		/*latent heat of vaporization as a function of ta*/
		double lhvap = 2.5023e6 -2430.54 *ta;
		double dt =0.2;
		double t1 = ta+dt;
		double t2 =ta-dt;
		/*saturated vapor pressure at t1, t2*/
		double pvs1 = 610.7 *exp(17.38 *t1/(239.+t1));
		double pvs2 = 610.7 *exp(17.38 *t2/(239.+t2));
		/*slope of pvs vs. T curve at T*/
		double slope = (pvs1-pvs2)/(t1-t2);	
		/*evapotranspiration*/
		et = (slope*irad+ rho*CP *vpd/rhr)/((pa * CP *rv)/(lhvap*EPS *rhr)+slope);
		return (et/lhvap);
};

double Soil_Env::getEvaporation(const double & dayl, const double &rad){

	//dayl , dayl length  , hour
	//rad, radiation pass through vegetation, MJ/m2day
	double evap=0.;
	double tair = ed_atms->ta;
	double vpdpa = ed_atmd->vpd;
	double daylsec = dayl*3600;
	if (daylsec < 0.1) { // 0.1 sec for mathmatical purpose, otherwise 'daytimerad' below will be 'inf'
		return (0.);
	}
	double daytimerad = rad*86400/daylsec; //w/m2
	/* correct conductances for temperature and pressure based on Jones (1992)
	with standard conditions assumed to be 20 deg C, 101300 Pa */
	double rcorr = 1.0/(pow((tair+273.15)/293.15, 1.75) );
	
	double rbl = 107 * rcorr;
	
	double pmet = getPenMonET( tair, vpdpa, daytimerad,rbl, rbl);
	double dsr = ed_atms->dsr;
	if (dsr<=1.0) dsr=1.0;
	double ratiomin =envpar.evapmin;
 
	evap = pmet *  daylsec;
	
	if(ed_v2g->rdrip + ed_v2g->rthfl+ ed_snw2soi->melt >= evap){
        evap *=0.6;
	}else{
		/* calculate the realized proportion of potential evaporation
		as a function of the days since rain */
		double ratio =0.3/pow(dsr, 2.0) ;
		
		if(ratio<ratiomin) ratio  = ratiomin;
		
	    evap *=ratio;	
	}

	return (evap);
};

// modified to set that from the bottom soil layer
// this will eliminate the 'fake watertable' due to temporary water saturation of upper
double Soil_Env::getWaterTable(Layer* lstsoill){
	Layer* currl = lstsoill;
	double wtd=0;
	double s, si, dz, por;
	double thetai, thetal;

	bool bottomsat  = true;   // initialize the bottom layer as saturated
	bool bottomclog = true;   // initialize the bottom layer as water-clogged
	double sums=0.;
	double ztot=0.;
	while (currl!=NULL){
		if(!currl->isRock){
			dz = currl->dz;
			ztot +=dz;
			por = currl->poro;
			thetai = currl->getVolIce();
			thetai = fmin(por, thetai);
			thetal = currl->getVolLiq();
			thetal = fmin(por-thetai, thetal);

			s = (thetal+thetai)/fmax(por,0.01);
			si= thetai/fmax(por,0.01);
			if (bottomsat || bottomclog) {    //if bottom-layer saturated

				// if too-much ice or too-cold, the layer is assumed to be water-logged
				// the criteria for those two are arbitrary now, can be modified
				if (si>0.75 || currl->tem<-2.0) {
					bottomclog = true;
					bottomsat = true;   // resetting the layer to raise water-table
				} else {
					bottomclog = false;
				}

				if (s>0.999 || bottomclog) {   //
					sums = ztot;
				} else {
					bottomsat = false;
					sums+=(fmax(0., s-0.6))/(1.0-0.6)*dz;
					//if over 0.6 saturation, let the lower portion be part of below water table
					// this is arbitrary, but useful if the deeper layer is thick (1 or 2 m)
				}

			}

		} else {
			if (currl->isSnow) break;
		}

		currl=currl->prevl;

	}

	wtd = ztot - sums;  //the water table is measured from ground surface
	return (wtd);
};

//
double Soil_Env::getRunoff(Layer* toplayer, Layer* drainl, const double & rnth,const double & melt){
	double runoff = 0.; // overland runoff

	double s, dz, por;

	double	thetai;
	double	thetal;
	double frasat = 0.;

	double sums=0.;
	double ztot=0.;
	int numl =0;

	Layer* currl = toplayer;
	if (drainl==NULL || !toplayer->isSoil) {
		runoff = rnth+melt;
	} else {
		while (currl!=NULL){

			por = currl->poro;
			dz = currl->dz;
			thetai = currl->getVolIce();
			thetal = currl->getVolLiq();
				
			s = (thetai + thetal)/por;
			s = fmin((double)s , 1.0);

			sums+=s * dz;
			ztot +=dz;

			frasat +=s;

			currl=currl->nextl;

			numl++;
			if(numl>=drainl->solind) break;
		}

		double avgs = sums/ztot;
		frasat /=numl;
		runoff = (frasat  + (1.-frasat)*pow((double)avgs, 4.) )* (rnth +melt); //So, unit same as "rainfall/snowmelt)"

	}

	return (runoff);
};
 
/*! calculates the factor which provides controls from soil on transpiration
 *  Oleson, T. R., 2004
 *  */
void Soil_Env::getSoilTransFactor(double btran[MAX_SOI_LAY], Layer* fstsoill, const double rootfr[MAX_SOI_LAY]){

	double psimax, psi, psisat;
	double rresis;
	psimax = envpar.psimax;
	
	Layer * currl = fstsoill;
	int sind = -1;
	double sumbtran = 0.;
	while(currl!=NULL){
		if(currl->isSoil){
			sind++;
			if(currl->tem>=0.0){
				psisat = currl->psisat;
				psi = dynamic_cast<SoilLayer*>(currl)->getMatricPotential();

				psi = fmax(psimax, psi);
				psi = fmin(psisat, psi);
				rresis = (1.- psi/psimax)/(1.- psisat/psimax);

				btran[sind] = rootfr[sind]* rresis;
				sumbtran   += rootfr[sind]* rresis;
			}

		}

		currl=currl->nextl;
	}

	if (sumbtran>1.) {
		for (int il=0; il<cd_soil->numsl; il++) {
			btran[sind] /=sumbtran;
		}

	}

}

// refresh snow-soil 'ed' from double-linked layer matrix after Thermal/Hydrological processes are done
void Soil_Env::retrieveDailyTM(const double & tsurface, Layer* toplayer, Layer *lstsoill){
	
	//first empty the 'ed' arrays: the reason is that NOT ALL will be refreshed below (e.g., layer is melted or burned)
	for (int i=0; i<MAX_SNW_LAY ; i++){
	    ed_snws->tsnw[i]  = MISSING_D;
	    ed_snws->snwice[i]= MISSING_D;
	    ed_snws->snwliq[i]= MISSING_D;
	    ed_snwd->tcond[i] = MISSING_D;
	}
	for(int il =0; il<MAX_SOI_LAY; il++){
		ed_sois->frozen[il]     = MISSING_I;
		ed_sois->frozenfrac[il] = MISSING_D;
		ed_sois->ts[il]  = MISSING_D;
		ed_sois->liq[il] = MISSING_D;
		ed_sois->ice[il] = MISSING_D;

		ed_soid->vwc[il] = MISSING_D;
		ed_soid->lwc[il] = MISSING_D;
		ed_soid->iwc[il] = MISSING_D;
		ed_soid->sws[il] = MISSING_D;
		ed_soid->aws[il] = MISSING_D;

		ed_soid->tcond[il] = MISSING_D;
		ed_soid->hcond[il] = MISSING_D;
	}
	for (int i=0; i<MAX_ROC_LAY ; i++){
	    ed_sois->trock[i]  = MISSING_D;
	}

	//
	Layer * curr2=toplayer;
	
	int soilind = 0;
	double soldep   = 0.;
	double soltave  = 0.;
	double solicesum= 0.;
	double solliqsum= 0.;

	int snwind = 0;
	double snwdep  = 0.;
	double snwtave = 0.;

	int rockind = 0;

	while(curr2!=NULL){
	  if(curr2->isSoil){
	  	
		  ed_sois->frozen[soilind] = curr2->frozen;
		  ed_sois->frozenfrac[soilind] = curr2->frozenfrac;
		  ed_sois->ts[soilind]  = curr2->tem;
		  ed_sois->liq[soilind] = curr2->liq;
		  ed_sois->ice[soilind] = curr2->ice;

		  ed_soid->vwc[soilind]= curr2->getVolWater();
	  	  ed_soid->iwc[soilind]= curr2->getVolIce();
	  	  ed_soid->lwc[soilind]= curr2->getVolLiq();
		  ed_soid->minliq[soilind] = curr2->minliq;
	  	  	
	  	  ed_soid->sws[soilind]= curr2->getVolLiq()/curr2->poro;
	  	  ed_soid->aws[soilind]= curr2->getVolLiq()/
	  			  fmax(curr2->minliq,curr2->poro-curr2->getVolIce());

	  	  ed_soid->tcond[soilind] = curr2->tcond;
	  	  ed_soid->hcond[soilind] = curr2->hcond;

	  	  // some cumulative variables for whole soil column
	  	  soldep   += curr2->dz;
	  	  soltave  += curr2->tem*curr2->dz;
	  	  solicesum+= curr2->ice;
	  	  solliqsum+= curr2->liq;

	  	  soilind++;
	  	
	  } else if (curr2->isSnow) {

		  ed_snwd->tcond[snwind] = curr2->tcond;
		  ed_snws->tsnw[snwind]  = curr2->tem;
		  ed_snws->snwliq[snwind]= curr2->liq;
		  ed_snws->snwice[snwind]= curr2->ice;

		  snwdep  += curr2->dz;
		  snwtave += curr2->tem*curr2->dz;

		  snwind++;

	  } else if (curr2->isRock) {

		  ed_sois->trock[rockind] = curr2->tem;
		  ed_soid->tbotrock = curr2->tem;
		  rockind++;
	  }

	  curr2 = curr2->nextl;
	  //if (curr2->indl>) break;

	}	
	
	if (soldep > 0.) ed_soid->tsave = soltave/soldep;
	ed_soid->liqsum = solliqsum;
	ed_soid->icesum = solicesum;

	if (snwdep > 0.) ed_snws->tsnwave = snwtave/snwdep;

	ed_sois->draindepth = ground->draindepth;

	// the following may not be needed (already called in updateDailyGroundT, but just in case
	updateDailySoilThermal4Growth(ground->fstsoill, tsurface);  // this is needed for growing
	updateLayerStateAfterThermal(ground->fstsoill, ground->lstsoill, ground->botlayer);  //this shall be done before the following
	retrieveDailyFronts();
}

void Soil_Env::setGround(Ground* grndp){
	ground = grndp;
	stefan.setGround(grndp);
	tempupdator.setGround(grndp);
};

void Soil_Env::setCohortLookup(CohortLookup * chtlup){
    chtlu =chtlup;
};

void Soil_Env::setCohortData(CohortData* cdp){
	gd = cdp->gd;
	if(tstepmode==MONTHLY) {
		cd_vegd = &cdp->m_vegd;
		cd_soil = &cdp->m_soil;
	}else if(tstepmode==DAILY) {
		cd_vegd = &cdp->d_vegd;
		cd_soil = &cdp->d_soil;
	}
};

void Soil_Env::setEnvData(EnvData* edp){
	// always at daily time-step
	ed_atms = &edp->d_atms;
	ed_atmd = &edp->d_atmd;
	ed_a2l  = &edp->d_a2l;
	ed_v2a  = &edp->d_v2a;
	ed_v2g  = &edp->d_v2g;
	ed_snws = &edp->d_snws;
	ed_snwd = &edp->d_snwd;
	ed_snw2a  = &edp->d_snw2a;
	ed_snw2soi= &edp->d_snw2soi;
	ed_sois = &edp->d_sois;
	ed_soid = &edp->d_soid;
	ed_soi2a = &edp->d_soi2a;
	ed_soi2l = &edp->d_soi2l;
};

