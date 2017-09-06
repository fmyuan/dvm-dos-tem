/*
Notes from C. Tucker, Feb 2015

baseline on does not result in N loss.... This might be a useful way to deal
with nonvascular N input, alternately it might be indicate a problem in the
nitrogen cycle.

A fix to this problem is proposed, ie, offfpen ncycle..., so right now, the
minimum change will be best....

TEM6 does this, but dealing with nfixation, emissions, etc, is upcoming

Continue to deal with this as a closed N cycle, give nonvascular plants priority
in N uptake over soil.
*/


/*
 * Vegetation_Bgc.cpp
 *
 * Purpose: Calculating (IN)GPP, (IN)NPP, RM, RG, N UPTAKE
 *            and resulted C & N changes
 *
 * History:
 *   June 28, 2011, by F.-M. Yuan:
 *     (1) Recoding based on DOS-TEM's code;
 *     (2) Multiple vegetation C&N pools added for DVM
 *
 * Important:
 *   (1) Parameters are read from 'CohortLookup.cpp', and set to
 *       'bgcpar' (struct:: vegpar_bgc)
 *
 *   (2) Calibrated Parameters are also read from 'CohortLookup.cpp'
 *       initially, and set to 'calpar' (strut:: vegpar_cal)
 *
 *   (3) The calculation is for ONE PFT only, so when calling it must
 *       be set the important index:
 *          pfttype
 *
 *   (4) FIVE (5) data pointers must be initialized by calling
 *       corresponding 'set...' methods
 *          chtlu, ed, bd, sb, fd
 *
 *   (5) all calculations are NOT 'fpc' adjusted - so, the
 *       unit is PER projected ground
 *
 */

#include "../TEMLogger.h"
#include "../TEMUtilityFunctions.h"

#include "Vegetation_Bgc.h"

<<<<<<< HEAD
Vegetation_Bgc::Vegetation_Bgc(){
	tstepmode = MONTHLY;
	ipft      = 0;
	nfeed     = false;
	avlnflg   = false;
	baseline  = false;

	bgcpar.dc2n = 0.000519;
	//increase of c:n = 0.176 if the nitrogen of veg is decreased by 15% per doubling of co2
	//proportional change in c:n per change co2
	//0.15 increase in C:N with double co2
	
	chtlu = NULL;
	cd    = NULL;
	rd    = NULL;
	cd_vegs = NULL;
	cd_vegd = NULL;
	cd_sois = NULL;

	bd_vegs = NULL;
	bd_vegd = NULL;
	bd_a2v = NULL;
	bd_v2a = NULL;
	bd_v2v = NULL;
	bd_sois = NULL;
	bd_soid = NULL;
	bd_v2soi = NULL;
	bd_soi2v = NULL;

	edyly_atms = NULL;
	edyly_l2a  = NULL;
	ed_atms = NULL;
	ed_a2l  = NULL;
	ed_l2a  = NULL;
	ed_sois = NULL;
	ed_soid = NULL;

	d2wdebrisc = 0.0;
	d2wdebrisn = 0.0;
	dleafc     = 0.0;
	fltrfall   = 0.0;
	totrzavln  = 0.0;

=======
extern src::severity_logger< severity_level > glg;

Vegetation_Bgc::Vegetation_Bgc() {
  bgcpar.dc2n = 0.000519;
  //increase of c:n = 0.176 if the nitrogen of veg is
  //  decreased by 15% per doubling of co2
  //proportional change in c:n per change co2
  //0.15 increase in C:N with double co2
>>>>>>> master
};

Vegetation_Bgc::~Vegetation_Bgc() {
};

//Yuan: the parameterization and initialization is done for one PFT
//set the bgc parameters from inputs
void Vegetation_Bgc::initializeParameter() {
  BOOST_LOG_SEV(glg, info) << "Initializing parameters in Vegetation_Bgc from chtlu (CohortLookup) values.";
  bgcpar.kc      = chtlu->kc[ipft];
  bgcpar.ki      = chtlu->ki[ipft];
  bgcpar.tmin    = chtlu->tmin[ipft];
  bgcpar.toptmin = chtlu->toptmin[ipft];
  bgcpar.toptmax = chtlu->toptmax[ipft];
  bgcpar.tmax    = chtlu->tmax[ipft];
  bgcpar.raq10a0 = chtlu->raq10a0[ipft];
  bgcpar.raq10a1 = chtlu->raq10a1[ipft];
  bgcpar.raq10a2 = chtlu->raq10a2[ipft];
  bgcpar.raq10a3 = chtlu->raq10a3[ipft];
  bgcpar.knuptake = chtlu->knuptake[ipft];

  for (int i=0; i<NUM_PFT_PART; i++) {
    bgcpar.cpart[i]   = chtlu->cpart[i][ipft];
  }

  bgcpar.c2na = chtlu->c2na[ipft];

  for (int i=0; i<NUM_PFT_PART; i++) {
    bgcpar.c2nb[i]   = chtlu->c2nb[i][ipft];
    bgcpar.c2neven[i] = chtlu->initc2neven[i][ipft];
    bgcpar.c2nmin[i] = chtlu->c2nmin[i][ipft];
  }

  bgcpar.labncon = chtlu->labncon[ipft];
  // Calibrated parameters for vegetation BGC
  calpar.cmax = chtlu->cmax[ipft];
  calpar.nmax = chtlu->nmax[ipft];

  for (int i=0; i<NUM_PFT_PART; i++) {
    calpar.cfall[i] = chtlu->cfall[i][ipft];
    calpar.nfall[i] = chtlu->nfall[i][ipft];
  }

  calpar.kra = chtlu->kra[ipft];

  for (int i=0; i<NUM_PFT_PART; i++) {
    calpar.krb[i] = chtlu->krb[i][ipft];
  }

  calpar.frg = chtlu->frg[ipft];

}

// set the initial states from inputs
<<<<<<< HEAD
void Vegetation_Bgc::initializeState(){
	
	double totvegn = 0.;
	for (int i=0; i<NUM_PFT_PART; i++){
		bd_vegs->c[i]    = chtlu->initvegc[i][ipft];
		bd_vegs->strn[i] = chtlu->initvegn[i][ipft]*0.95;

		totvegn += chtlu->initvegn[i][ipft];
	}
	bd_vegs->labn  = totvegn*0.05;

	bd_vegs->deadc = chtlu->initdeadc[ipft];
	bd_vegs->deadn = chtlu->initdeadn[ipft];

	//
	bd_vegs->call    = 0.;
	bd_vegs->strnall = 0.;
	for (int i=0; i<NUM_PFT_PART; i++) {
		bd_vegs->call += bd_vegs->c[i];
		if(nfeed) {
			bd_vegs->strnall += bd_vegs->strn[i];
		}
	}
	if (nfeed) bd_vegs->nall = bd_vegs->strnall + bd_vegs->labn;
=======
void Vegetation_Bgc::initializeState() {
  double totvegn = 0.;

  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_vegs.c[i]    = chtlu->initvegc[i][ipft];
    // Save five percent of structural nitrogen for labile. See below.
    bd->m_vegs.strn[i] = chtlu->initvegn[i][ipft]*0.95;
    totvegn += chtlu->initvegn[i][ipft];
  }

  bd->m_vegs.labn  = totvegn*0.05;// Use the reserved five percent of str N
  bd->m_vegs.deadc = chtlu->initdeadc[ipft];
  bd->m_vegs.deadn = chtlu->initdeadn[ipft];
  //
  bd->m_vegs.call    = 0.;
  bd->m_vegs.strnall = 0.;

  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_vegs.call += bd->m_vegs.c[i];

    if(nfeed) {
      bd->m_vegs.strnall += bd->m_vegs.strn[i];
    }
  }
>>>>>>> master

  if (nfeed) {
    bd->m_vegs.nall = bd->m_vegs.strnall + bd->m_vegs.labn;
  }
};

//set the initial states from restart inputs
<<<<<<< HEAD
void Vegetation_Bgc::initializeState5restart(RestartData *resin){

	for (int i=0; i<NUM_PFT_PART; i++){
	  bd_vegs->c[i]    = resin->vegc[i][ipft];
	  bd_vegs->strn[i] = resin->strn[i][ipft];
	}
	bd_vegs->labn = resin->labn[ipft];

	bd_vegs->deadc = resin->deadc[ipft];
	bd_vegs->deadn = resin->deadn[ipft];
 	
	//
	bd_vegs->call    = 0.;
	bd_vegs->strnall = 0.;
	for (int i=0; i<NUM_PFT_PART; i++) {
		bd_vegs->call += bd_vegs->c[i];
		if(nfeed) {
			bd_vegs->strnall += bd_vegs->strn[i];
		}
	}
	if (nfeed) bd_vegs->nall = bd_vegs->strnall + bd_vegs->labn;
=======
void Vegetation_Bgc::set_state_from_restartdata(const RestartData & rdata) {
  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_vegs.c[i]    = rdata.vegc[i][ipft];
    bd->m_vegs.strn[i] = rdata.strn[i][ipft];
  }

  bd->m_vegs.labn = rdata.labn[ipft];
  bd->m_vegs.deadc = rdata.deadc[ipft];
  bd->m_vegs.deadn = rdata.deadn[ipft];
  //
  bd->m_vegs.call    = 0.;
  bd->m_vegs.strnall = 0.;

  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_vegs.call += bd->m_vegs.c[i];

    if(nfeed) {
      bd->m_vegs.strnall += bd->m_vegs.strn[i];
    }
  }
>>>>>>> master

  if (nfeed) {
    bd->m_vegs.nall = bd->m_vegs.strnall + bd->m_vegs.labn;
  }
};

<<<<<<< HEAD
// the calculation is done for PFTs one by one
void Vegetation_Bgc::prepareIntegration(){

	//canopy conductance for GPP
	double pet = ed_l2a->pet;    //
   	if (pet<=0.0) pet = 0.0001;
    double eet = ed_l2a->eet;    //
	bd_vegd->gv = getGV(eet,  pet);

	// previous 'topt' is the average of previous 10 years
	// which is used as Topt in GPP tfactor AND updated every year in 'phenology'
	double prvtopt = 0.;
	deque<double> toptdeque = cd->toptque[ipft];
	int recnum=toptdeque.size();
	if (recnum>0) {
		for(int i=0; i<recnum; i++){
			prvtopt += toptdeque[i]/recnum;
		}
	} else {  //no previous year 'topt'
		prvtopt = (bgcpar.tmin+bgcpar.tmax)/2.;
	}

	// temperature factor for GPP
	bd_vegd->ftemp = getTempFactor4GPP(ed_atms->ta, prvtopt);

	// temperature factor for plant respiration
	bd_vegd->raq10 = getRaq10(ed_atms->ta);

	// leaf C requirement for growth, used to dynamically determine C allocation
	dleafc = cd_vegd->maxleafc[ipft]*cd_vegd->fleaf[ipft];  //yearly max. leaf C, adjusted by seasonal foliage growth index 'fleaf'
	dleafc -= bd_vegs->c[I_leaf];   // C requirement of foliage growth at current timestep
	dleafc = fmax(0., dleafc);

	// litter-falling seasonal adjustment
	double prvttime = 0.;   //previous 10 year mean of growing season degree-day, using for normalizing current growing period needed for litterfalling
	deque<double> ttimedeque = cd->prvgrowingttimeque[ipft];
	recnum=ttimedeque.size();
	if (recnum>0) {
		for(int i=0; i<recnum; i++){
			prvttime += ttimedeque[i]/recnum;
		}
	} else {  //no previous year 'ttime'
		prvttime = (bgcpar.tmin+bgcpar.tmax)/2.;
	}

	for (int i=0; i<NUM_PFT_PART; i++){
		// assuming 'calpar.cfall' is the max. monthly fraction, we need to modify them for specific PFT types
		double ncfall = 1.; // n:c ratio of litter-falling: 0 - 1.0, which determines N resorption
		if (calpar.cfall[i]>0.) ncfall = fmin(1.0, calpar.nfall[i]/calpar.cfall[i]);
		if (cd_vegs->ifdeciwoody[ipft]) {
			calpar.cfall[I_leaf] = 1.0;     // for deciduous woody species, leaf max. falling fraction is 1.0; stem/root will take the calibrated.
		}
		if (!cd_vegs->ifperenial[ipft]) {
			calpar.cfall[i] = 1.0;     // for annual species, leaf/stem/root max. falling/dying fraction is 1.0, no matter what calibrated
		}
		calpar.nfall[i] = ncfall*calpar.cfall[i]; // this will limit nfall not greater than cfall.
 	}
	// assuming 'calpar.cfall' is the max. monthly fraction, and allowing the following seasonal variation
	fltrfall = 1.;              // non-growing season, max. litterfall assumed
	if (cd_vegd->growingttime[ipft]>0. && prvttime>0.) {
		fltrfall = fmin(1., cd_vegd->growingttime[ipft]/prvttime);
	}

	// dead standing C falling
    if(cd->yrsdist<9.){
 		if(bd_vegs->deadc>0){
 			d2wdebrisc = bd_vegs->deadc/9./12.;
 			if (nfeed==1) d2wdebrisn = bd_vegs->deadn/9./12.;
 		}
 	 }else{
 		bd_vegs->deadc =0.;
 		if (nfeed==1) bd_vegs->deadn =0.;
 	 }

	//assign states to temporary state variable
    tmp_vegs.deadc            = bd_vegs->deadc;
    if (nfeed) tmp_vegs.deadn = bd_vegs->deadn;
	tmp_vegs.call    = 0.;
	if (nfeed) tmp_vegs.strnall = 0.;
	for (int i=0; i<NUM_PFT_PART; i++){
		tmp_vegs.c[i]    = bd_vegs->c[i];
		tmp_vegs.call   += bd_vegs->c[i];

		if (nfeed) {
			tmp_vegs.strn[i] = bd_vegs->strn[i];
			tmp_vegs.strnall+= bd_vegs->strn[i];
		}
	}
	if (nfeed) {
		tmp_vegs.labn = bd_vegs->labn;
		tmp_vegs.nall = bd_vegs->labn+bd_vegs->strnall;
	}
=======
//Yuan: the calculation is done for PFTs one by one
void Vegetation_Bgc::prepareIntegration(const bool &nfeedback) {
  //option of N module
  this->set_nfeed(nfeedback);
  //canopy conductance for GPP
  double pet = ed->m_l2a.pet;    //

  if (pet<=0.0) {
    pet = 0.0001;
  }

  double eet = ed->m_l2a.eet;    //
  bd->m_vegd.gv = getGV(eet,  pet);
  // previous 'topt' is the average of previous 10 years
  // which is used as Topt in GPP tfactor AND updated every year in 'phenology'
  double prvtopt = 0.;
  deque<double> toptdeque = cd->toptque[ipft];
  int recnum=toptdeque.size();

  if (recnum>0) {
    for(int i=0; i<recnum; i++) {
      prvtopt += toptdeque[i]/recnum;
    }
  } else {  //no previous year 'topt'
    prvtopt = (bgcpar.tmin+bgcpar.tmax)/2.;
  }

  // temperature factor for GPP
  bd->m_vegd.ftemp = getTempFactor4GPP(ed->m_atms.ta, prvtopt);

  // temperature factor for plant respiration
  bd->m_vegd.raq10 = getRaq10(ed->m_atms.ta);

  // dleafc is for "delta" or "difference"
  // this is the difference between the yearly maximum leaf biomass
  // (potentially affected by drought) and the (current) monthly value of
  // leaf biomass.

  // leaf C requirement for growth, used to dynamically determine C allocation
  // yearly max. leaf C, adjusted by seasonal foliage growth index 'fleaf'
  dleafc = cd->m_vegd.maxleafc[ipft] * cd->m_vegd.fleaf[ipft];

  // C requirement of foliage growth at current timestep
  dleafc -= bd->m_vegs.c[I_leaf];
  dleafc = fmax(0.0, dleafc);
  // litter-falling seasonal adjustment

  //previous 10 year mean of growing season degree-day, using for
  //  normalizing current growing period needed for litterfalling
  double prvttime = 0.0;

  deque<double> ttimedeque = cd->prvgrowingttimeque[ipft];
  recnum=ttimedeque.size();

  if (recnum>0) {
    for(int i=0; i<recnum; i++) {
      prvttime += ttimedeque[i]/recnum;
    }
  } else {  //no previous year 'ttime'
    prvttime = (bgcpar.tmin+bgcpar.tmax)/2.;
  }

  /*TUCKER FEB 2015: some of the logic behind the functional type and temporal 
    adjustment of litterfall appears to be faulty. For now, simplifying model
    so that litterfall == cfall * vegc, for annual plants, cfall for all 
    components = 0.083 (= 1 per year), for deciduous plants, cfall for leaves
    = 0.083. These values are set in cmt_calparbgc.txt. No values should exceed 0.083.
  for (int i=0; i<NUM_PFT_PART; i++) {
    //assuming 'calpar.cfall' is the max. monthly fraction,
    //  we need to modify them for specific PFT types
    if (cd->m_veg.ifdeciwoody[ipft]) {
      //for deciduous woody species, leaf max. falling fraction is 1.0;
      //  stem/root will take the calibrated.
      calpar.cfall[I_leaf] = 1.0;
      calpar.nfall[I_leaf] = 1.0;
    }

    if (!cd->m_veg.ifperenial[ipft]) {
      //for annual species, leaf/stem/root max. falling/dying fraction is 1.0,
      //  no matter what calibrated
      calpar.cfall[i] = 1.0;
      calpar.nfall[i] = 1.0;
    }
  }


  //assuming 'calpar.cfall' is the max. monthly fraction,
  //  and allowing the following seasonal variation
  fltrfall = 1.; // non-growing season, max. litterfall assumed

  if (cd->m_vegd.growingttime[ipft]>0. && prvttime>0.) {
    fltrfall = fmin(1., cd->m_vegd.growingttime[ipft]/prvttime);
  }
  */

  // dead standing C falling
  //if(cd->yrsdist < 10.0) {//cd->mthsdist<108
  if(cd->mthsdist < 108) {
    if(bd->m_vegs.deadc0 > 0) {
      bd->m_v2soi.d2wdebrisc = bd->m_vegs.deadc0/9.0/12.0;

      if(bd->m_v2soi.d2wdebrisc > bd->m_vegs.deadc){
        bd->m_v2soi.d2wdebrisc = bd->m_vegs.deadc; 
        bd->m_vegs.deadc = 0;
      }
      else{
        bd->m_vegs.deadc -= bd->m_v2soi.d2wdebrisc;
      }

      if (nfeed==1) {
        bd->m_v2soi.d2wdebrisn = bd->m_vegs.deadn0/9.0/12.0;

        if(bd->m_v2soi.d2wdebrisn > bd->m_vegs.deadn){
          bd->m_v2soi.d2wdebrisn = bd->m_vegs.deadn;
          bd->m_vegs.deadn = 0;
        }
        else{
          bd->m_vegs.deadn -= bd->m_v2soi.d2wdebrisn;
        }

      }
    }
  } else {
    bd->m_vegs.deadc = 0.0;

    if (nfeed==1) {
      bd->m_vegs.deadn = 0.0;
    }
  }

  //assign states to temporary state variable
  tmp_vegs.deadc            = bd->m_vegs.deadc;

  if (nfeed) {
    tmp_vegs.deadn = bd->m_vegs.deadn;
  }
>>>>>>> master

  tmp_vegs.call    = 0.;

  if (nfeed) {
    tmp_vegs.strnall = 0.;
  }

  for (int i=0; i<NUM_PFT_PART; i++) {
    tmp_vegs.c[i]    = bd->m_vegs.c[i];
    tmp_vegs.call   += bd->m_vegs.c[i];

    if (nfeed) {
      tmp_vegs.strn[i] = bd->m_vegs.strn[i];
      tmp_vegs.strnall+= bd->m_vegs.strn[i];
    }
  }

  if (nfeed) {
    tmp_vegs.labn = bd->m_vegs.labn;
    tmp_vegs.nall = bd->m_vegs.labn+bd->m_vegs.strnall;
  }
};

// C and N fluxes without N limitation
<<<<<<< HEAD
void Vegetation_Bgc::delta(){

	/// some environment variables
	double co2 = ed_atms->co2;
	double par = ed_a2l->par;

	//leaf phenology
	// 1) current EET and previous max. EET controlled
  	double fleaf = cd_vegd->fleaf[ipft];
  
  	// 2) plant size (biomass C) or age controlled
 	double ffoliage = cd_vegd->ffoliage[ipft];

  	//GPP without N limitation
  	double ingppall = getGPP(co2, par, fleaf, ffoliage,
  			        bd_vegd->ftemp, bd_vegd->gv);

  	// maintainence respiration first estimation
  	double rm = 0.;
	for (int i=0; i<NUM_PFT_PART; i++){
		if (tmp_vegs.c[i]>0.) {
			bd_vegd->kr[i] = getKr(tmp_vegs.c[i], i);
			del_v2a.rm[i]    = getRm(tmp_vegs.c[i], bd_vegd->raq10, bd_vegd->kr[i]);
		} else {
			del_v2a.rm[i]    = 0.;
		}
		rm += del_v2a.rm[i];
	}
	double rmadj = 1.0;  //used below for summarizing ingpp[]
	if (rm>ingppall && rm>0.0) rmadj=ingppall/rm;

	// total available C for allocation
	double innppall = fmax(0., ingppall-rm)/(1.0+calpar.frg);  // if C assimilation available, first goes to maintaince respiration

	// NPP allocation to leaf estimated first
  	double nppl = dleafc;   // this is the potential estimated above in 'prepareIntegration()'
  	del_a2v.innpp[I_leaf] = fmin(nppl, innppall);    // leaf has the second priority for C assimilation
  	del_v2a.rg[I_leaf] = calpar.frg * del_a2v.innpp[I_leaf];
  	double npprgl = del_a2v.innpp[I_leaf]+del_v2a.rg[I_leaf];

  	// the rest goes to stem/root, assuming equal priority
  	double innpprest = fmax(0., ingppall-rm-npprgl)/(1.0+calpar.frg);
  	double cpartrest = 0.;
	for (int i=I_leaf+1; i<NUM_PFT_PART; i++){
		cpartrest +=bgcpar.cpart[i];
	}

	for (int i=I_leaf+1; i<NUM_PFT_PART; i++){
		del_a2v.innpp[i] = 0.;
		del_v2a.rg[i]    = 0.;
		if (cpartrest>0. && innpprest>0.) {
			del_a2v.innpp[i] = innpprest *bgcpar.cpart[i]/cpartrest;
			del_v2a.rg[i]    = calpar.frg * del_a2v.innpp[i];
		}
	}

	// summary of INGPP
	for (int i=0; i<NUM_PFT_PART; i++){
		del_a2v.ingpp[i] = del_a2v.innpp[i]+del_v2a.rm[i]*rmadj+del_v2a.rg[i];
		//may be over the 'ingppall' due to 'rm', which are biomass-C based, if not adjusted by 'rmadj'
	}

  	// litter-falling
	for (int i=0; i<NUM_PFT_PART; i++){
		if (calpar.cfall[i]>0.) {
			del_v2soi.ltrfalc[i] = fmax(0., fltrfall*calpar.cfall[i] * tmp_vegs.c[i]);

		} else {
			del_v2soi.ltrfalc[i] = 0.;
		}
 	}

 
};

// C and N fluxes regulated by N uptakes
void Vegetation_Bgc::deltanfeed(){

	if(nfeed){

		// max. N uptake determined by plant f(foliage), air temperature, and soil conditions
		if (cd_vegs->nonvascular[ipft]==0) {
			del_soi2v.innuptake = getNuptake(cd_vegd->ffoliage[ipft], bd_vegd->raq10,
	  			                             bgcpar.knuptake, calpar.nmax);
		} else {
			del_soi2v.innuptake = calpar.nmax * cd_vegd->ffoliage[ipft];    //need more mechanism algorithm for non-vascular plants:
			                                                       // they absorb N mainly from wet-deposition, and could from substrate (soil) through co-existed plants, or from biofixation
		}
	  	if (del_soi2v.innuptake < 0.0) del_soi2v.innuptake = 0.0;
	  	double avln = totrzavln;   // NOTE: 'totrzavln' already updated in calling 'getNuptake()' above
	  	if (del_soi2v.innuptake > 0.95*avln) del_soi2v.innuptake = 0.95*avln;

		// N litterfall and accompanying re-absorption
		for (int i=0; i<NUM_PFT_PART; i++){
			if (calpar.nfall[i]>0.) {
				// assuming 'calpar.nfall' is the max. monthly fraction, and allowing the following seasonal variation
				del_v2soi.ltrfaln[i] = fmax(0., fltrfall*calpar.nfall[i] * tmp_vegs.strn[i]);

			} else {
				del_v2soi.ltrfaln[i] = 0.;
			}

		}

		double nresorball = 0.;
		for (int i=0; i<NUM_PFT_PART; i++){
			double c2n = 0.;
			if (tmp_vegs.strn[i]>0.) c2n=tmp_vegs.c[i]/tmp_vegs.strn[i];
			if (c2n>0.) {
				if(del_v2soi.ltrfaln[i] <= del_v2soi.ltrfalc[i]/c2n){
					del_v2v.nresorb[i] = del_v2soi.ltrfalc[i]/c2n
						             - del_v2soi.ltrfaln[i];
				}else{
					del_v2soi.ltrfaln[i] = del_v2soi.ltrfalc[i]/c2n;
					del_v2v.nresorb[i]   = 0.;
				}

				nresorball +=del_v2v.nresorb[i];

			} else {
				del_v2soi.ltrfaln[i] = 0.;
				del_v2v.nresorb[i]   = 0.;

				nresorball +=del_v2v.nresorb[i];

			}
		}

		//N requirements for new growth if fully no N limitation
		double nrequireall = 0.0;
		double nrequire[NUM_PFT_PART];
		for (int i=0; i<NUM_PFT_PART; i++){
			if (del_a2v.innpp[i]>0.){
				nrequire[i] = del_a2v.innpp[i]/bgcpar.c2neven[i];
				nrequireall+= nrequire[i];
			} else {
				nrequire[i] = 0.;
			}
		}

		// all N supply
		double tempnuptake = del_soi2v.innuptake;
		double templabn = nresorball+tmp_vegs.labn;   //resorbed N is portion of available 'labile N'
		double nsupply = tempnuptake+templabn;

		//C fluxes regulated by N uptake and labile N and N requirements
		double reduction = 1.0;
		if (nrequireall>0.) reduction = fmax(0., nsupply/nrequireall);

		if (reduction < 1.0) {

			for (int i=0; i<NUM_PFT_PART; i++){
				// npp/gpp reduction due to N limitation
    			if (bgcpar.cpart[i]>0.) {
    				del_a2v.npp[i] = del_a2v.innpp[i]*reduction;
    				if (del_a2v.npp[i] > 0.0) {
    					del_v2a.rg[i] = calpar.frg * del_a2v.npp[i];
    				} else {
    					del_v2a.rg[i] = 0.0;
    				}

    				del_a2v.gpp[i] = del_a2v.npp[i] + del_v2a.rg[i] + del_v2a.rm[i];
    				if (del_a2v.gpp[i] < 0.0) {
    					del_a2v.gpp[i] = 0.0;
    				}

    			} else {
    				del_a2v.npp[i] = 0.;
    				del_v2a.rg[i]  = 0.;
    				del_a2v.gpp[i] = 0.;
    			}

    			// N allocation
    			if (bgcpar.cpart[i]>0.) {

    				//empties the labile N pools to each nmobil pools, which later on added to each N pool
    				del_v2v.nmobil[i]  = fmin(1., fmax(0., nrequire[i]/fmax(1.e-8, nrequireall)))
						            *templabn;

    				//allocates uptaken N to each structural N pool
    				del_soi2v.snuptake[i] = fmin(1., fmax(0., nrequire[i]/fmax(1.e-8, nrequireall)))
				                       *tempnuptake;
    			} else {
    				del_v2v.nmobil[i] = 0.;
    				del_soi2v.snuptake[i] = 0.;

    			}
			}

		// N requirement less than max.Nuptake+labN (total N potential supply), which may need down-regulated in turn
		} else {
			// no N limitation on GPP/NPP
			double nppall = 0;
			for (int i=0; i<NUM_PFT_PART; i++){
				del_a2v.gpp[i] = del_a2v.ingpp[i];
				del_a2v.npp[i] = del_a2v.innpp[i];

				nppall += del_a2v.npp[i];
			}

			//N uptake down-regulation
			double inprodcn = nppall/fmax(1.e-8, nsupply);
			double cneven = nppall/fmax(1.e-8, nrequireall);
			double nmin = 0.;
			for (int i=0; i<NUM_PFT_PART; i++){
				if (del_a2v.npp[i]>1.e-8) nmin += del_a2v.npp[i]/bgcpar.c2nmin[i];
			}
			double cnmin = nppall/fmax(1.e-8, nmin);

		  	//double nadj = inprodcn*(inprodcn-2.0*cneven);  // this eq. is from E.E. 2008 EA paper (something wrong within the code)
		  	double nadj = 1.0;
		  	double a1= fmax(1.e-8, fabs((inprodcn-cnmin)*(inprodcn-2.0*cneven+cnmin)));
		  	nadj = a1/(a1+pow(fabs(inprodcn-cneven), 2.0));   // this eq. is from DOS-TEM

		  	tempnuptake *=nadj;
		  	if (tempnuptake< 0.0 ) {tempnuptake = 0.0; }

		  	// if N require even less than labile N + resorbed N
		  	if (templabn >= nrequireall) {
		  		for (int i=0; i<NUM_PFT_PART; i++){
        			if (bgcpar.cpart[i]>0.) {
        				del_v2v.nmobil[i] = nrequire[i];
        				del_soi2v.snuptake[i] = 0.;
        			} else {
        				del_v2v.nmobil[i] = 0.;
        				del_soi2v.snuptake[i] = 0.;
        			}
		  		}

		  	// N requirement greater than labile N + resorbed N, but less than total potential supply - implying must uptake from soil
		  	} else {

        		for (int i=0; i<NUM_PFT_PART; i++){
        			if (bgcpar.cpart[i]>0.) {
        				del_v2v.nmobil[i]     = templabn*nrequire[i]/fmax(1.e-8, nrequireall); //empty all available labile N first
        				del_soi2v.snuptake[i] = (del_a2v.npp[i]/bgcpar.c2neven[i]) - del_v2v.nmobil[i];
				
        				if ( del_soi2v.snuptake[i] < 0.0 ) del_soi2v.snuptake[i] = 0.0; // it's possible, because npp may be negative
        			} else {
        				del_v2v.nmobil[i] = 0.;
        				del_soi2v.snuptake[i] = 0.;
        			}

		  		}

      	    }

		} // end of N requirement < N supply

		//N uptake for labile N, if possible, after N budget estimation

		nsupply = tempnuptake+templabn;   // note: 'tempnuptake' may be changed
		double snuptakeall = 0.;
		double nmobilall = 0.;
		double nfall = 0.;
		for (int i=0; i<NUM_PFT_PART; i++) {
			snuptakeall+=del_soi2v.snuptake[i];
			nmobilall  +=del_v2v.nmobil[i];
			nfall      +=del_v2soi.ltrfaln[i];
		}

		double nsurplus = nsupply - snuptakeall - nmobilall;    //vegetation N labile N after changes ('nmobile' already includes contribution from 'resorbed N')
		double nstruc = tmp_vegs.strnall + snuptakeall + nmobilall - nfall - nresorball;  //vegetation structural N after changes

		if (nsurplus <= bgcpar.labncon*nstruc) { //
			del_soi2v.lnuptake = tempnuptake - snuptakeall;

		} else {  //reducing actual 'lnuptake'
        	del_soi2v.lnuptake = bgcpar.labncon * nstruc
        			            -(tmp_vegs.labn + nresorball - nmobilall);
		}
		if (del_soi2v.lnuptake < 1.0e-10 ) del_soi2v.lnuptake = 0.0;

        // end of nfeed
	} else {

		for (int i=0; i<NUM_PFT_PART; i++){
			del_a2v.gpp[i] = del_a2v.ingpp[i];
			del_a2v.npp[i] = del_a2v.innpp[i];
		}

	}
};
=======
void Vegetation_Bgc::delta() {

  // Finding cpart_all might be totally unnecessary - the sum of cpart across
  // compartments should always be 1.0?
  // This is checked at startup in CohortLookup::assignBgc4Vegetation()
  // And a cursory test run with the following assert statement seems to
  // indicate that when the parameters are properly defined (and pass the
  // checks in CohortLookup) that cpart_all is indeed always 1.0:
  // assert ((temutil::AlmostEqualRelative, 1.0) && "cpart_all should always be 1.0");
  double cpart_all = 0.0;
  for (int i=I_leaf; i<NUM_PFT_PART; i++) {
    cpart_all += bgcpar.cpart[i];
  }

  /// some environment variables
  double co2 = ed->m_atms.co2;
  double par = ed->m_a2l.par;
  //leaf phenology
  // 1) current EET and previous max. EET controlled
  double fleaf = cd->m_vegd.fleaf[ipft];
  // 2) plant size (biomass C) or age controlled
  double ffoliage = cd->m_vegd.ffoliage[ipft];
  //GPP without N limitation
  double gpp_all = getGPP(co2, par, fleaf, ffoliage,
                           bd->m_vegd.ftemp, bd->m_vegd.gv);


  // First compute maintenance respiration based on GPP, respiration
  // rate (kr) and temperature. This is the amount of respiration needed to
  // keep living tissue alive, so it is basically based on the amount of
  // living biomass of the PFT.

  double rm_wholePFT = 0.0;
  for (int i=0; i<NUM_PFT_PART; i++) {
    if (tmp_vegs.c[i] > 0.0) {
      bd->m_vegd.kr[i] = getKr(tmp_vegs.c[i], i);
      del_v2a.rm[i] = getRm(tmp_vegs.c[i], bd->m_vegd.raq10, bd->m_vegd.kr[i]);
    } else {
      del_v2a.rm[i] = 0.0;
    }
    rm_wholePFT += del_v2a.rm[i];
  }

  // C available from GPP for allocation to new tissue after
  // accounting for maintenance respiration.
  double C_avail = gpp_all - rm_wholePFT;

  // For instances when Rm is larger than GPP, i.e. winter months
  if (rm_wholePFT > gpp_all) {

    // No growth is possible, growth respiration should therefore be zero.
    for (int i=I_leaf; i<NUM_PFT_PART; i++) { 
      del_v2a.rg[i] = 0.0;
    }

    // NPP is negative in order to represent the effect of
    // negative vegetation C balance on vegetation C pools and overall
    // ecosystem C balance. The negative NPP should be allocated based on
    //   (1) the allocation coefficients among compartments,
    //   (2) the existing C pool of each compartment.
    // We decided to first affect the stem and root compartment because they
    // are considered as "storage compartment", and then leaves (for evergreens)
    for (int i=I_leaf; i<NUM_PFT_PART; i++) {
      if (tmp_vegs.c[i] > 0.0) {
        del_a2v.innpp[i] = (gpp_all * bgcpar.cpart[i] / cpart_all) - del_v2a.rm[i];
        if (del_a2v.innpp[i] > 0.0) {
          del_a2v.innpp[i] = ((gpp_all * bgcpar.cpart[i] / cpart_all) - del_v2a.rm[i]) / (1.0+calpar.frg);
          del_v2a.rg[i] = del_a2v.innpp[i] * calpar.frg;
        }
      } else {
        del_a2v.innpp[i] = 0.0;
      }
    }

  } else {
    // When C from GPP is available after maintenance respiration, it is
    // allocated to the leaf compartment first, then stems and roots.

    // First - allocation for leaf growth (NPP and associated growth respiration)
    // dleafc is the default C allocation (from parameter files) to leaves
    // affected by drought.
    del_a2v.innpp[I_leaf] = fmin(dleafc, C_avail / (1.0 + calpar.frg)); 
    del_v2a.rg[I_leaf] = calpar.frg * del_a2v.innpp[I_leaf];

    // update temporary variable after down-regulation
    double npp_and_rg_leaves = 0.0;
    npp_and_rg_leaves = del_a2v.innpp[I_leaf] + del_v2a.rg[I_leaf];

    // Remaining C after maintenance respiration and leaf growth. Stems and
    // roots are the lowest priority so they take remaining C after leaf growth
    // and maintenance resp. have been accounted for.
    C_avail = gpp_all - rm_wholePFT - npp_and_rg_leaves;

    // Allocate (distribute) the remaining available C amongst roots and stems
    // based on the cpart distrubution specified in the parameter files.
    double cpart_stems_and_roots = 0.0;

    for (int i=I_leaf+1; i<NUM_PFT_PART; i++) { // <-- cpart of everything but leaves!!!
      cpart_stems_and_roots += bgcpar.cpart[i];
    }

    if (cpart_stems_and_roots > 0.0) {
      for (int i=I_leaf+1; i<NUM_PFT_PART; i++) {
        del_a2v.innpp[i] = 0.0;
        del_v2a.rg[i]    = 0.0;
        if (cpart_stems_and_roots > 0.0 && C_avail > 0.0){
          del_a2v.innpp[i] = (C_avail / (1.0 + calpar.frg))* (bgcpar.cpart[i] / cpart_stems_and_roots); 
          del_v2a.rg[i]    = calpar.frg * del_a2v.innpp[i];
        }
      }
    } else {
      // This PFT is a moss or lichen and has no stems and roots. Allocate the
      // remaining availabe GPP to leaf growth and associated growth respiration.
      del_a2v.innpp[I_leaf] += C_avail;
      del_v2a.rg[I_leaf] = del_a2v.innpp[I_leaf] * calpar.frg;
    }
  }

  // Compute NPP all
  double innpp_all = 0.0;
  for (int i=0; i<NUM_PFT_PART; i++){
    innpp_all += del_a2v.innpp[i];
  }

  // Partition 'ingpp' based on initial NPP, Rg, and Rm
  for (int i=0; i<NUM_PFT_PART; i++) {
    del_a2v.ingpp[i] = del_a2v.innpp[i] + del_v2a.rm[i] + del_v2a.rg[i];
  }

  // Handle litterfall
  for (int i=0; i<NUM_PFT_PART; i++) {
    if (calpar.cfall[i] > 0.0) {
      del_v2soi.ltrfalc[i] = fmax(0.0, calpar.cfall[i] * tmp_vegs.c[i]);
    } else {
      del_v2soi.ltrfalc[i] = 0.0;
    }
  }
}

// C and N fluxes regulated by N uptakes
void Vegetation_Bgc::deltanfeed() {
  if(nfeed) {

    // Figure out the initial N uptake.
    if (cd->m_veg.nonvascular[ipft] == 0) {
      // For vascular plants, N uptake is determined by foliage,
      // air temp, and soil conditions.
      del_soi2v.innuptake = getNuptake(
          cd->m_vegd.ffoliage[ipft],
          bd->m_vegd.raq10,
          bgcpar.knuptake,
          calpar.nmax
      );
    } else {
      // For non-vascular plants, we are not sure of the best algorithm yet.
      // They absorb N mainly from wet-deposition, from substrate (soil,
      // rock, etc), from neighboring plants, or from biofixation.
>>>>>>> master

      // For now simply scale with a calibrated parameter and foliage...
      del_soi2v.innuptake = calpar.nmax * cd->m_vegd.ffoliage[ipft];
    }

    // Force innuptake to be non-negative.
    if (del_soi2v.innuptake < 0.0) {
      del_soi2v.innuptake = 0.0;
    }

    // Adjust innuptake based on the soil available N.
    if (cd->m_veg.nonvascular[ipft] == 0) {

      // For vascular plants, find all the available N in soil layers
      // that have fine roots and then give some percentage to innuptake.
      double avln = 0.0;
      for(int il=0; il<cd->m_soil.numsl; il++) {
        if (cd->m_soil.frootfrac[il][ipft] > 0.0) {
          avln += bd->m_sois.avln[il];
        }
      }
      if (del_soi2v.innuptake > 0.95 * avln) {
        del_soi2v.innuptake = 0.95 * avln;
      }

    } else {
      // For non-vascular plants innuptake is not influenced by soil
      // available Nitrogen! Nothing to do...
    }


    // N litterfall and accompanying resorbtion
    for (int i=0; i<NUM_PFT_PART; i++) {
      if (calpar.nfall[i] > 0.0) {
        //assuming 'calpar.nfall' is the max. monthly fraction,
        //  and allowing the following seasonal variation
        //del_v2soi.ltrfaln[i] = fmax(0., fltrfall*calpar.nfall[i]
        //                                * tmp_vegs.strn[i]);
        del_v2soi.ltrfaln[i] = fmax(0.0, calpar.nfall[i]*tmp_vegs.strn[i]);
      } else {
        del_v2soi.ltrfaln[i] = 0.0;
      }
    }

    double nresorball = 0.0;

    for (int i=0; i<NUM_PFT_PART; i++) {
      double c2n = 0.0;

      if (tmp_vegs.strn[i] > 0.0) {
        c2n = tmp_vegs.c[i] / tmp_vegs.strn[i];
      }

      if (c2n > 0.0) {
        if(del_v2soi.ltrfaln[i] <= del_v2soi.ltrfalc[i]/c2n) {
          del_v2v.nresorb[i] = del_v2soi.ltrfalc[i]/c2n
                               - del_v2soi.ltrfaln[i];
        } else {
          del_v2soi.ltrfaln[i] = del_v2soi.ltrfalc[i]/c2n;
          del_v2v.nresorb[i]   = 0.;
        }

        nresorball +=del_v2v.nresorb[i];
      } else {
        del_v2soi.ltrfaln[i] = 0.;
        del_v2v.nresorb[i]   = 0.;
        nresorball +=del_v2v.nresorb[i];
      }
    }

    //N requirements if fully no N limitation
    double nrequireall = 0.0;
    double nrequire[NUM_PFT_PART];

    for (int i=0; i<NUM_PFT_PART; i++) {
      if (del_a2v.innpp[i]>0.) {
        nrequire[i] = del_a2v.innpp[i]/bgcpar.c2neven[i];
        nrequireall+= nrequire[i];
      } else {
        nrequire[i] = 0.;
      }
    }

    // all N supply
    double tempnuptake = del_soi2v.innuptake;
    double templabn = nresorball + tmp_vegs.labn; //resorbed N is portion of
                                                  //  available 'labile N'
    double nsupply = tempnuptake + templabn;
    //C fluxes regulated by N uptake and labile N and N requirements
    double reduction = 1.0;

    if (nrequireall > 0.0) {
      reduction = fmax(0.0, nsupply/nrequireall);
    }

    if (reduction < 1.0) {
      for (int i=0; i<NUM_PFT_PART; i++) {
        // npp/gpp reduction due to N limitation
        if (bgcpar.cpart[i] > 0.0) {
          del_a2v.npp[i] = del_a2v.innpp[i] * reduction;

          if (del_a2v.npp[i] > 0.0) {
            del_v2a.rg[i] = calpar.frg * del_a2v.npp[i];
          } else {
            del_v2a.rg[i] = 0.0;
          }

          del_a2v.gpp[i] = del_a2v.npp[i] + del_v2a.rg[i] + del_v2a.rm[i];

          if (del_a2v.gpp[i] < 0.0) {
            del_a2v.gpp[i] = 0.0;
          }
        } else {
          del_a2v.npp[i] = 0.0;
          del_v2a.rg[i]  = 0.0;
          del_a2v.gpp[i] = 0.0;
        }

        // N allocation
        if (bgcpar.cpart[i]>0.) {
          //empties the labile N pools to each nmobil pools,
          //  which later on added to each N pool
          del_v2v.nmobil[i]  = fmin(1., fmax(0., nrequire[i]/fmax(1.e-8, nrequireall)))
                               *templabn;
          //allocates uptaken N to each structural N pool
          del_soi2v.snuptake[i] = fmin(1., fmax(0., nrequire[i]/fmax(1.e-8, nrequireall)))
                                  *tempnuptake;
        } else {
          del_v2v.nmobil[i] = 0.;
          del_soi2v.snuptake[i] = 0.;
        }
      }

      //N requirement less than max.Nuptake+labN (total N potential supply),
      //  which may need down-regulated in turn
    } else {
      // no N limitation on GPP/NPP
      double nppall = 0;

      for (int i=0; i<NUM_PFT_PART; i++) {
        del_a2v.gpp[i] = del_a2v.ingpp[i];
        del_a2v.npp[i] = del_a2v.innpp[i];
        nppall += del_a2v.npp[i];
      }

      double inprodcn = nppall / nsupply;
      double tempnuptakeFactor = 0;
      tempnuptakeFactor = inprodcn * (inprodcn - 2*nppall/fmax(1.e-8, nrequireall));  //Tucker: corrected from E.E. 2009 paper and tem-dvm code
      tempnuptake *= tempnuptakeFactor / (tempnuptakeFactor - pow(nppall/nrequireall, 2.0));
      if (tempnuptake < 0.0 ) {
        tempnuptake = 0.0;
      }

      // if N require even less than labile N + resorbed N
      if (templabn >= nrequireall) {
        for (int i=0; i<NUM_PFT_PART; i++) {
          if (bgcpar.cpart[i]>0.) {
            del_v2v.nmobil[i] = nrequire[i];
            del_soi2v.snuptake[i] = 0.;
          } else {
            del_v2v.nmobil[i] = 0.;
            del_soi2v.snuptake[i] = 0.;
          }
        }

        //N requirement greater than labile N + resorbed N, but less than
        //  total potential supply - implying must uptake from soil
      } else {
        for (int i=0; i<NUM_PFT_PART; i++) {
          if (bgcpar.cpart[i]>0.) {
            //empty all available labile N first
            del_v2v.nmobil[i] = templabn*nrequire[i]/fmax(1.e-8, nrequireall);
            del_soi2v.snuptake[i] = (del_a2v.npp[i]/bgcpar.c2neven[i])
                                    - del_v2v.nmobil[i];

            if ( del_soi2v.snuptake[i] < 0.0 ) {
              del_soi2v.snuptake[i] = 0.0;//it's possible, because npp may be negative
            }
          } else {
            del_v2v.nmobil[i] = 0.;
            del_soi2v.snuptake[i] = 0.;
          }
        }
      }
    } // end of N requirement < N supply

    //N uptake for labile N, if possible, after N budget estimation
    nsupply = tempnuptake+templabn; // note: 'tempnuptake' may be changed
    double snuptakeall = 0.;
    double nmobilall = 0.;
    double nfall = 0.;

    for (int i=0; i<NUM_PFT_PART; i++) {
      snuptakeall += del_soi2v.snuptake[i];
      nmobilall   += del_v2v.nmobil[i];
      nfall       += del_v2soi.ltrfaln[i];
    }

    //vegetation N labile N after changes ('nmobile' already
    //  includes contribution from 'resorbed N')
    double nsurplus = nsupply - snuptakeall - nmobilall;
    //vegetation structural N after changes
    double nstruc = tmp_vegs.strnall + snuptakeall + nmobilall
                    - nfall - nresorball;

    if (nsurplus <= bgcpar.labncon*nstruc) { //
      del_soi2v.lnuptake = tempnuptake - snuptakeall;
    } else {  //reducing actual 'lnuptake'
      del_soi2v.lnuptake = bgcpar.labncon * nstruc
                           - (tmp_vegs.labn + nresorball - nmobilall);
    }

    if (del_soi2v.lnuptake < 1.0e-10 ) {
      del_soi2v.lnuptake = 0.0;
    }

    // end of nfeed
  } else {
    for (int i=0; i<NUM_PFT_PART; i++) {
      del_a2v.gpp[i] = del_a2v.ingpp[i];
      del_a2v.npp[i] = del_a2v.innpp[i];
    }
  }
}

// summarize C and N state variable changes
void Vegetation_Bgc::deltastate() {
  del_vegs.deadc = d2wdebrisc;

  for (int i=0; i<NUM_PFT_PART; i++) {
    del_vegs.c[i] = del_a2v.npp[i] - del_v2soi.ltrfalc[i];
  }

  if(nfeed) {
    del_vegs.deadn = d2wdebrisn;
    del_vegs.labn = del_soi2v.lnuptake;

    for (int i=0; i<NUM_PFT_PART; i++) {
      del_vegs.strn[i] = del_soi2v.snuptake[i] + del_v2v.nmobil[i]
                         - del_v2soi.ltrfaln[i] - del_v2v.nresorb[i];
      //'nresorb' - N transfer from dead/falling tissues to
      //  'labn', 'nmobil' in reverse direction
      del_vegs.labn += del_v2v.nresorb[i] - del_v2v.nmobil[i];
    }
  }
};

// summarize some variables not done in 'integrator'
<<<<<<< HEAD
void Vegetation_Bgc::afterIntegration(){
	// states
	bd_vegs->call    = 0.;
	bd_vegs->strnall = 0.;
	for (int i=0; i<NUM_PFT_PART; i++) {
		bd_vegs->call += bd_vegs->c[i];
		if(nfeed) {
			bd_vegs->strnall += bd_vegs->strn[i];
		}
	}
	if (nfeed) bd_vegs->nall = bd_vegs->strnall + bd_vegs->labn;

	// fluxes
	bd_a2v->ingppall = 0.;
	bd_a2v->innppall = 0.;
	bd_a2v->gppall = 0.;
	bd_a2v->nppall = 0.;
	bd_v2a->rgall  = 0.;
	bd_v2a->rmall  = 0.;

	bd_v2soi->ltrfalcall = 0.;
	bd_v2soi->ltrfalnall = 0.;
	bd_v2soi->mossdeathc = 0.;
	bd_v2soi->mossdeathn = 0.;

	bd_v2v->nmobilall  = 0.;
	bd_v2v->nresorball = 0.;

 	bd_soi2v->snuptakeall= 0.;

	for (int i=0; i<NUM_PFT_PART; i++) {
		bd_a2v->ingppall += bd_a2v->ingpp[i];
		bd_a2v->innppall += bd_a2v->innpp[i];
		bd_a2v->gppall   += bd_a2v->gpp[i];
		bd_a2v->nppall   += bd_a2v->npp[i];
		bd_v2a->rgall    += bd_v2a->rg[i];
		bd_v2a->rmall    += bd_v2a->rm[i];

		if (cd_vegs->nonvascular[ipft]==0) {
			bd_v2soi->ltrfalcall += bd_v2soi->ltrfalc[i];
		} else if (cd_vegs->nonvascular[ipft]>0) {
			bd_v2soi->mossdeathc += bd_v2soi->ltrfalc[i];
		}

		if(nfeed) {
			if (cd_vegs->nonvascular[ipft]==0) {
				bd_v2soi->ltrfalnall += bd_v2soi->ltrfaln[i];
			} else if (cd_vegs->nonvascular[ipft]>0) {
				bd_v2soi->mossdeathn += bd_v2soi->ltrfaln[i];
			}

			bd_v2v->nmobilall  += bd_v2v->nmobil[i];
			bd_v2v->nresorball += bd_v2v->nresorb[i];

		 	bd_soi2v->snuptakeall+= bd_soi2v->snuptake[i];
		}
	}

	if (nfeed) {
   		// total actual N uptake for root N extraction from different soil layers
		double tempnuptake = bd_soi2v->snuptakeall+bd_soi2v->lnuptake;
	 	for (int il=0; il<cd_sois->numsl; il++) {
	 		if (fracnuptake[il]>0.) {
	 			bd_soi2v->nextract[il] = tempnuptake * fracnuptake[il];   //
	 		} else {
	 			bd_soi2v->nextract[il] = 0.;
	 		}
	 	}

	}
=======
void Vegetation_Bgc::afterIntegration() {
  // states
  bd->m_vegs.call    = 0.;
  bd->m_vegs.strnall = 0.;

  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_vegs.call += bd->m_vegs.c[i];

    if(nfeed) {
      bd->m_vegs.strnall += bd->m_vegs.strn[i];
    }
  }

  if (nfeed) {
    bd->m_vegs.nall = bd->m_vegs.strnall + bd->m_vegs.labn;
  }

  // fluxes
  bd->m_a2v.ingppall = 0.;
  bd->m_a2v.innppall = 0.;
  bd->m_a2v.gppall = 0.;
  bd->m_a2v.nppall = 0.;
  bd->m_v2a.rgall  = 0.;
  bd->m_v2a.rmall  = 0.;
  bd->m_v2soi.ltrfalcall = 0.;
  bd->m_v2soi.ltrfalnall = 0.;
  bd->m_v2soi.mossdeathc = 0.;
  bd->m_v2soi.mossdeathn = 0.;
  bd->m_v2v.nmobilall  = 0.;
  bd->m_v2v.nresorball = 0.;
  bd->m_soi2v.snuptakeall= 0.;

  for (int i=0; i<NUM_PFT_PART; i++) {
    bd->m_a2v.ingppall += bd->m_a2v.ingpp[i];
    bd->m_a2v.innppall += bd->m_a2v.innpp[i];
    bd->m_a2v.gppall   += bd->m_a2v.gpp[i];
    bd->m_a2v.nppall   += bd->m_a2v.npp[i];
    bd->m_v2a.rgall    += bd->m_v2a.rg[i];
    bd->m_v2a.rmall    += bd->m_v2a.rm[i];

    if (cd->m_veg.nonvascular[ipft]==0) {
      bd->m_v2soi.ltrfalcall += bd->m_v2soi.ltrfalc[i];
    } else if (cd->m_veg.nonvascular[ipft]>0) {
      bd->m_v2soi.mossdeathc += bd->m_v2soi.ltrfalc[i];
    }
>>>>>>> master

    if(nfeed) {
      if (cd->m_veg.nonvascular[ipft]==0) {
        bd->m_v2soi.ltrfalnall += bd->m_v2soi.ltrfaln[i];
      } else if (cd->m_veg.nonvascular[ipft]>0) {
        bd->m_v2soi.mossdeathn += bd->m_v2soi.ltrfaln[i];
      }

      bd->m_v2v.nmobilall  += bd->m_v2v.nmobil[i];
      bd->m_v2v.nresorball += bd->m_v2v.nresorb[i];
      bd->m_soi2v.snuptakeall+= bd->m_soi2v.snuptake[i];
    }
  }

  if (nfeed) {
    // total actual N uptake for root N extraction from different soil layers
    double tempnuptake = bd->m_soi2v.snuptakeall+bd->m_soi2v.lnuptake;

    for (int il=0; il<cd->m_soil.numsl; il++) {
      if (fracnuptake[il]>0.) {
        bd->m_soi2v.nextract[il] = tempnuptake * fracnuptake[il];   //
      } else {
        bd->m_soi2v.nextract[il] = 0.;
      }
    }
  }
};

/////////////////////////////////////////
/// private functions
/////////////////////////////////////////

//Note that - fpc not adjusted here
<<<<<<< HEAD
double  Vegetation_Bgc::getGPP(const double &co2, const double & par,
                   const double &leaf, const double & foliage,
                   const double &ftemp, const double & gv) {

  	double ci  = co2 * gv;
  	double thawpcnt = ed_soid->rtdpthawpct;
  	double fpar = par/(bgcpar.ki +par);   // par : photosynthetically active radiation in J/(m2s)
  	double gpp = calpar.cmax * foliage * ci / (bgcpar.kc + ci);
  	gpp *= leaf * fpar;
  	gpp *= ftemp;
  	gpp *= thawpcnt;
  	if(gpp<0)gpp=0.;
  	return (gpp);
=======
double  Vegetation_Bgc::getGPP(const double &co2, const double &par,
                               const double &leaf, const double &foliage,
                               const double &ftemp, const double &gv) {
  // par:             photosynthetically active radiation in J/m2/s
  // co2:             should be ppm
  // leaf:
  // foliage:
  // ftemp:
  // gv:

  // ci:
  // thawpct:         percentage of month frozen or thawed
  // fpar:            could be J/m2/s
  // gpp:             should be gC/m2/m
  // calpar.cmax:     g/m2/month

  double ci  = co2 * gv;
  double thawpcnt = ed->m_soid.rtdpthawpct;
  double fpar = par / (bgcpar.ki + par);

  double gpp = calpar.cmax * foliage * ci / (bgcpar.kc + ci);
  gpp *= fpar;
  gpp *= ftemp;
  gpp *= thawpcnt;

  if(gpp < 0.0) {
    gpp = 0.0;
  }

  //double gpp = calpar.cmax * ci / (bgcpar.kc + ci);
  //gpp *= leaf * fpar;
  //gpp *= leaf;

  return gpp;
}
>>>>>>> master

double Vegetation_Bgc::getRm(const double & vegc, const double & raq10,
                             const double &kr) {
  double respq10 = raq10;
  double rm = 0.;
  rm = kr * vegc;
  rm *= respq10;

  if (rm < 0.0) {
    rm = 0.0;
  }

  if (rm > 0.10 * vegc) {
    rm = 0.10 * vegc;  // maintenance resp. cannot be over 10% of veg C
  }

  return rm;
}

double Vegetation_Bgc::getGV(const double & eet,const double & pet ) {
  double gv;

<<<<<<< HEAD
 	return (rm);
=======
  if ( eet/pet <= 0.1 ) {
    gv = (-10.0 * pow( (eet/pet),2.0 ))
         + (2.9 * (eet/pet));
>>>>>>> master

    if ( gv < 0.0 ) {
      gv = 0.0;
    }
  } else {
    gv = 0.1 + (0.9 * eet / pet);
  }

  if(gv>1) {
    gv =1;
  }
  //gv=1;
  return gv;
};

double Vegetation_Bgc::getTempFactor4GPP(const double & tair,
                                         const double &tgppopt) {
  double ftemp = 0.;
  double tmin = bgcpar.tmin;
  double toptmax = bgcpar.toptmax;
  double tmax = bgcpar.tmax;

  if ( tair <=tmin|| tair >=tmax ) {
    ftemp = 0.0;
  } else {
    if ( tair >= tgppopt && tair <= toptmax ) {
      ftemp = 1.0;
    } else {
      if (tair >tmin && tair< tgppopt ) {
        ftemp = (tair-tmin)*(tair-tmax)
                /((tair-tmin)*(tair-tmax)
                  - pow( (tair-tgppopt),2.0 ));
      } else {
        ftemp = (tair-tmin)*(tair-tmax)
                /((tair-tmin)*(tair-tmax)
                  - pow( (tair-toptmax),2.0 ));
      }
    }
<<<<<<< HEAD
  
  	if(gv>1) gv =1;

  	return (gv);
};

double Vegetation_Bgc::getTempFactor4GPP(const double & tair, const double &tgppopt){
  	double ftemp = 0.;
  	double tmin = bgcpar.tmin;
  	double toptmax = bgcpar.toptmax;
  	double tmax = bgcpar.tmax;

  	if ( tair <=tmin|| tair >=tmax ){
    	ftemp = 0.0;
  	} else {
    	if ( tair >= tgppopt && tair <= toptmax ){
      		ftemp = 1.0;
    	} else {
      		if (tair >tmin && tair< tgppopt ) {
				ftemp = (tair-tmin)*(tair-tmax)
               			/((tair-tmin)*(tair-tmax)
               				- pow( (tair-tgppopt),2.0 ));
      		} else {
				ftemp = (tair-tmin)*(tair-tmax)
               			/((tair-tmin)*(tair-tmax)
              				- pow( (tair-toptmax),2.0 ));
      		}
    	}
  	}

  	return (ftemp);
};

// the maintainence respiration constant
double Vegetation_Bgc::getKr(const double & vegc, const int & ipart){
	// kr is for calculating maintainance respiration
  	double kr  = 0.;
  	double kra = calpar.kra;
  	double krb = calpar.krb[ipart];
  
  	kr = exp((kra*vegc)+krb);	
  	return (kr);
};

// the maintainence respiration temperature factor
double Vegetation_Bgc::getRaq10(const double & tair){
  	double raq10;
  	double raq10a0 = bgcpar.raq10a0;
  	double raq10a1 = bgcpar.raq10a1;
  	double raq10a2 = bgcpar.raq10a2;
  	double raq10a3 = bgcpar.raq10a3;
  
  	double q10 = raq10a0+ (raq10a1*tair) + (raq10a2*pow( tair,2.0 ))
                + (raq10a3*pow(tair,3.0 ));
  	q10 = fmax(0., q10);
  	raq10 = pow( q10,tair/10.0 );
  	return (raq10);
=======
  }

  return ftemp;
};

// the maintainence respiration constant
double Vegetation_Bgc::getKr(const double & vegc, const int & ipart) {
  // kr is for calculating maintainance respiration
  double kr  = 0.;
  double kra = calpar.kra;
  double krb = calpar.krb[ipart];
  kr = exp((kra*vegc)+krb);
  return kr;
};

// the maintainence respiration temperature factor
double Vegetation_Bgc::getRaq10(const double & tair) {
  double raq10;
  double raq10a0 = bgcpar.raq10a0;
  double raq10a1 = bgcpar.raq10a1;
  double raq10a2 = bgcpar.raq10a2;
  double raq10a3 = bgcpar.raq10a3;
  double q10 = raq10a0+ (raq10a1*tair) + (raq10a2*pow( tair,2.0 ))
               + (raq10a3*pow(tair,3.0 ));
  raq10 = pow( q10,tair/10.0 );
  // if(isnan(raq10)){
  //  cout <<"raq10 is nan\n";
  //  exit(-1);
  // }
  return raq10;
>>>>>>> master
};

/** Plant new production C/N ratios (bgcpar.cneven) adjusting at end of a year.
 Determine vegetation C/N parameter as a function of vegetation type, annual 
 PET, annual EET, CO2 concentration.
*/
void Vegetation_Bgc::adapt_c2n_ratio_with_co2(
    const double & yreet, const double & yrpet, const double & initco2,
    const double & currentco2 ) {

  for (int i=0; i<NUM_PFT_PART; i++) {
    if (yrpet > 0.0) {
      bgcpar.c2neven[i] = bgcpar.c2nb[i] + bgcpar.c2na*(yreet/yrpet);
    } else {
      bgcpar.c2neven[i] = bgcpar.c2nb[i];
    }

    if (bgcpar.c2neven[i] < bgcpar.c2nmin[i]) {
      bgcpar.c2neven[i] = bgcpar.c2nmin[i];
    }

<<<<<<< HEAD
      updateCNeven(edyly_l2a->eet, edyly_l2a->pet, rd->initco2, edyly_atms->co2);
=======
    double adjc2n = 1.0 + (bgcpar.dc2n * (currentco2 - initco2));
    bgcpar.c2neven[i] *= adjc2n;
  }
>>>>>>> master
}

//
void Vegetation_Bgc::updateCNeven(const double & yreet, const double & yrpet,
                                  const double & initco2,
                                  const double & currentco2 ) {
  for (int i=0; i<NUM_PFT_PART; i++) {
    if (yrpet > 0.0) {
      bgcpar.c2neven[i] = bgcpar.c2nb[i] + bgcpar.c2na*(yreet/yrpet);
    } else {
      bgcpar.c2neven[i] = bgcpar.c2nb[i];
    }

    if (bgcpar.c2neven[i] < bgcpar.c2nmin[i]) {
      bgcpar.c2neven[i] = bgcpar.c2nmin[i];
    }

    double adjc2n = 1.0 + (bgcpar.dc2n * (currentco2 - initco2));
    bgcpar.c2neven[i] *= adjc2n;
  }
};

// modified to reflect N uptake upon layered soil N, water content, and root distribution
double Vegetation_Bgc::getNuptake(const double & foliage, const double & raq10,
<<<<<<< HEAD
                            const double & kn1, const double &nmax){

	double nuptake = 0.;

	double totrz = 0.;           // total root zone thickness
	double meanrzksoil = 0.;     // N uptake factor related to root zone liq water
	double totrzliq    = 0.;     // root zone liq water content for N
	double totfrootfrac= 0.;     // pft's fraction of roots of all PFTs

	totrzavln   = 0.;     // root zone avaliable N concent for N uptake
	for(int il =0; il<cd_sois->numsl; il++){
		if (cd_sois->frootfrac[il][ipft]> 0.) {
			totrz +=cd_sois->dz[il];
			meanrzksoil += bd_soid->knmoist[il]*cd_sois->dz[il];  //NOTE: 'bd->m_soid.knmoist' is updated in Soil_bgc.cpp

			totrzliq += ed_sois->liq[il];
			totrzavln+= bd_sois->avln[il];

			totfrootfrac +=cd_sois->frootfrac[il][ipft];
		}
=======
                                  const double & kn1, const double &nmax) {
  double nuptake = 0.;
  double totrz = 0.;           // total root zone thickness
  double meanrzksoil = 0.;     // N uptake factor related to root zone liq water
  double totrzliq    = 0.;     // root zone liq water content for N
  double totrzavln   = 0.;     // root zone avaliable N concent for N uptake
  double totfrootfrac= 0.;     // pft's fraction of roots of all PFTs

  for(int il =0; il < cd->m_soil.numsl; il++) {
    if (cd->m_soil.frootfrac[il][ipft]> 0.) {
      totrz += cd->m_soil.dz[il];
      meanrzksoil += bd->m_soid.knmoist[il]*cd->m_soil.dz[il];  //NOTE: 'bd->m_soid.knmoist' is updated in Soil_bgc.cpp
      totrzliq += ed->m_sois.liq[il];
      totrzavln += bd->m_sois.avln[il];
      totfrootfrac += cd->m_soil.frootfrac[il][ipft];
>>>>>>> master
    }
  }

  if (totrz > 0.0) {
    meanrzksoil /= totrz;
  }

  // nuptake rate
  if(totrzliq > 0.0 && totrzavln > 0.0 && meanrzksoil > 0.0) {
    // soil condition factor
    nuptake  = meanrzksoil*(totrzavln/totrzliq);
    nuptake /= (kn1 +nuptake);
    // root factor: root fraction of current PFT over all PFTs
    nuptake *= totfrootfrac;
    // plant phenological factor
    nuptake *= foliage;
    // air temperature factor
    nuptake *= raq10;
  } else {
    nuptake = 0.0;
  }

  nuptake *= nmax;
  //need to distribute N uptake for each soil layer based on
  //  root, water, and avail-N distribution
  double sumrootavln = 0.0;

  for(int il =0; il<cd->m_soil.numsl; il++) {
    fracnuptake[il] = 0.0;

    if (cd->m_soil.frootfrac[il][ipft] > 0.0 &&
        bd->m_sois.avln[il] > 0.0 && ed->m_sois.liq[il] > 0.0) {

      fracnuptake[il] = cd->m_soil.frootfrac[il][ipft] *
                        bd->m_sois.avln[il] * ed->m_sois.liq[il];

    }

    sumrootavln +=fracnuptake[il];
  }

<<<<<<< HEAD
    // need to distribute N uptake for each soil layer based on root, water, and avail-N distribution
    double sumrootavln = 0.;
	for(int il =0; il<cd_sois->numsl; il++){
		fracnuptake[il] = 0.;
		if (cd_sois->frootfrac[il][ipft]>0.
				&& bd_sois->avln[il]>0.
				&& ed_sois->liq[il]>0.) {
			fracnuptake[il] = cd_sois->frootfrac[il][ipft]
			                  *bd_sois->avln[il]*ed_sois->liq[il];
		}
		sumrootavln +=fracnuptake[il];
	}
	if (sumrootavln>0.) {
		for(int il =0; il<cd_sois->numsl; il++){
			fracnuptake[il] /= sumrootavln;
		}
	}

  	return (nuptake);
=======
  if (sumrootavln > 0.0) {
    for(int il=0; il<cd->m_soil.numsl; il++) {
      fracnuptake[il] /= sumrootavln;
    }
  }

  return nuptake;
}
>>>>>>> master

void Vegetation_Bgc::setCohortLookup(CohortLookup* chtlup) {
  chtlu = chtlup;
}

void Vegetation_Bgc::setCohortData(CohortData* cdp) {
  cd = cdp;
}

<<<<<<< HEAD
void Vegetation_Bgc::setCohortData(CohortData* cdp){
  	 cd = cdp;
  	 rd = cdp->rd;
  	 if(tstepmode == MONTHLY){
  		 cd_vegs = &cdp->m_veg;
  		 cd_vegd = &cdp->m_vegd;
  		 cd_sois = &cdp->m_soil;
  	 }else if(tstepmode == DAILY){
  		 cd_vegs = &cdp->d_veg;
  		 cd_vegd = &cdp->d_vegd;
  		 cd_sois = &cdp->d_soil;
  	 }
};

void Vegetation_Bgc::setEnvData(EnvData* edp){
	edyly_atms = &edp->y_atms;
	edyly_l2a  = &edp->y_l2a;
	if(tstepmode == MONTHLY) {
		ed_atms = &edp->m_atms;
		ed_a2l  = &edp->m_a2l;
		ed_l2a  = &edp->m_l2a;
		ed_sois = &edp->m_sois;
		ed_soid = &edp->m_soid;
	}else if(tstepmode == DAILY) {
		ed_atms = &edp->d_atms;
		ed_a2l  = &edp->d_a2l;
		ed_l2a  = &edp->d_l2a;
		ed_sois = &edp->d_sois;
		ed_soid = &edp->d_soid;
	}
};

void Vegetation_Bgc::setBgcData(BgcData* bdp){
	if(tstepmode == MONTHLY) {
		bd_vegs = &bdp->m_vegs;
		bd_vegd = &bdp->m_vegd;
		bd_a2v = &bdp->m_a2v;
		bd_v2a = &bdp->m_v2a;
		bd_v2v = &bdp->m_v2v;
		bd_sois = &bdp->m_sois;
		bd_soid = &bdp->m_soid;
		bd_v2soi = &bdp->m_v2soi;
		bd_soi2v = &bdp->m_soi2v;
	}else if(tstepmode == DAILY) {
		bd_vegs = &bdp->d_vegs;
		bd_vegd = &bdp->d_vegd;
		bd_a2v = &bdp->d_a2v;
		bd_v2a = &bdp->d_v2a;
		bd_v2v = &bdp->d_v2v;
		bd_sois = &bdp->d_sois;
		bd_soid = &bdp->d_soid;
		bd_v2soi = &bdp->d_v2soi;
		bd_soi2v = &bdp->d_soi2v;
	}
};
=======
void Vegetation_Bgc::setEnvData(EnvData* edp) {
  ed = edp;
}

void Vegetation_Bgc::setBgcData(BgcData* bdp) {
  bd =bdp;
}

bool Vegetation_Bgc::get_nfeed() {
  return this->nfeed;
}
void Vegetation_Bgc::set_nfeed(const std::string &value) {
  BOOST_LOG_SEV(glg, info) << "Setting Vegetation_Bgc.nfeed to " << value;
  this->nfeed = temutil::onoffstr2bool(value);
}
void Vegetation_Bgc::set_nfeed(const bool value) {
  BOOST_LOG_SEV(glg, info) << "Setting Vegetation_Bgc.nfeed to " << value;
  this->nfeed = value;
}

>>>>>>> master
