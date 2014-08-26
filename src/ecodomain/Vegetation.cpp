/*
 * Vegetation.cpp
 *
 * Purpose: Defining vegetation structure
 *
 * History:
 *     June 28, 2011, by F.-M. Yuan:
 *          (1) added for constructing vegetation domain (plant community)
 *
 * Important:
 *     (1) Parameters are read from 'CohortLookup.cpp', and set to 'vegdimpar' (struct:: vegpar_dim)
 *     (2) Here, all functions are on ALL pfts for ONE community. In this way, some complicated PFT interaction
 *      and dynamics/structure changing may be put here in the future.
 *
 */

#include "Vegetation.h"

Vegetation::Vegetation(){
	updateLAI5vegc = false;
	tstepmode = DAILY;
	chtlu = NULL;
	cd = NULL;
	cd_vegd = NULL;
	cd_vegs = NULL;
};

Vegetation::~Vegetation(){
	
};

// set the bgc parameters from inputs stored in 'chtlu' - reuseable
// Note: here will remove those PFT with no greater than zero 'fpc'
//       and initialize the total actual pft number

void Vegetation::initializeParameter(){
	int ipft = 0;
    for (int ip=0; ip<NUM_PFT; ip++) {
    	if (chtlu->vegcov[ipft] > 0.){    //this will remove those PFTs with 0 ground coverage. So be cautious the index consistent with 'state' variables

    		vegdimpar.sla[ipft]      = chtlu->sla[ip];
    		vegdimpar.klai[ipft]     = chtlu->klai[ip];
  
    		vegdimpar.minleaf[ipft] = chtlu->minleaf[ip];
    		vegdimpar.aleaf[ipft]   = chtlu->aleaf[ip];
    		vegdimpar.bleaf[ipft]   = chtlu->bleaf[ip];
    		vegdimpar.cleaf[ipft]   = chtlu->cleaf[ip];

    		vegdimpar.kfoliage[ipft] = chtlu->kfoliage[ip];
    		vegdimpar.cov[ipft]      = chtlu->cov[ip];
    		vegdimpar.m1[ipft] = chtlu->m1[ip];
    		vegdimpar.m2[ipft] = chtlu->m2[ip];
    		vegdimpar.m3[ipft] = chtlu->m3[ip];
    		vegdimpar.m4[ipft] = chtlu->m4[ip];

    	}

    	ipft++;
	}

};

// set the initial states from inputs
void Vegetation::initializeState(){

	//
	for (int i=0; i<NUM_PFT; i++){
		cd_vegs->vegage[i]  = 0;
	}

	// from 'lookup'
	cd->hasnonvascular = false;

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

    	}

    }

    updateFpc();

    updateFrootfrac();

};

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

    }

    updateFpc();

    updateFrootfrac();

};

// must be called after 'foliage' and 'leaf' updated
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

};

// vegetation coverage update (note - this is not same as FPC)
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

};

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
};

// functions for eet adjusted foliage growth index
// 'prvunleaf' is the unnormalized leaf from last time period
// 'prveetmx' is monthly eetmx of previous simulation period (year)

double Vegetation::getUnnormleaf(const int& ipft, double &prveetmx, const double & eet, const double & prvunleaf){
  	double normeet;
  	double unnormleaf;

  	if (prveetmx <= 0.0) {
  		prveetmx = 1.0;
  	}

  	normeet = eet/prveetmx;
  	if(normeet>1) normeet =1;

  	unnormleaf = (vegdimpar.aleaf[ipft] * normeet)
  			    +(vegdimpar.bleaf[ipft] * prvunleaf)
                +vegdimpar.cleaf[ipft];

  	if (unnormleaf < (0.1 * vegdimpar.minleaf[ipft])) {
    	unnormleaf = 0.1 * vegdimpar.minleaf[ipft];
  	}

  	return (unnormleaf);
};

//fleaf is normalized EET and previous EET determined phenology index 0~1
//i.e., f(phenology) in gpp calculation
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


// the following can be developed further for dynamic fine root distribution
// currently, it's only do some checking
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
};
