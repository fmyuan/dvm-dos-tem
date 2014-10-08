/*! \file 
*/

#include "BgcOutputer.h"

BgcOutputer::BgcOutputer(){
	
};

BgcOutputer::~BgcOutputer(){

};

void BgcOutputer::init(string & dirfile, const int &numpft){

	//file
	ncfname = dirfile;
    pftintegrated = true;
    if (numpft>1)pftintegrated = false;

	ncfile = new NcFile(ncfname.c_str(), NcFile::Replace);

	//dimension
	timeD    = ncfile->add_dim("tstep");
	pftD     = ncfile->add_dim("pft", numpft);
	partD    = ncfile->add_dim("vegpart", NUM_PFT_PART);
	soilayerD= ncfile->add_dim("soilayer", MAX_SOI_LAY);

    //variables
  	chtidV = ncfile->add_var("CHTID", ncInt);
  	errorV = ncfile->add_var("ERRORID", ncInt, timeD);
	yearV  = ncfile->add_var("YEAR", ncInt, timeD);
	monV   = ncfile->add_var("MONTH", ncInt, timeD);
	dayV   = ncfile->add_var("DAY", ncInt, timeD);

	// veg C/N state variables
   	callV = ncfile->add_var("VEGCSUM", ncDouble, timeD, pftD);
   	cV = ncfile->add_var("VEGCPART", ncDouble, timeD, pftD, partD);
   	nallV = ncfile->add_var("VEGNSUM", ncDouble, timeD, pftD);
   	labnV = ncfile->add_var("VEGNLAB", ncDouble, timeD);
   	strnallV = ncfile->add_var("VEGNSTRNSUM", ncDouble, timeD, pftD);
   	strnV = ncfile->add_var("VEGNSTRNPART", ncDouble, timeD, pftD, partD);
   	deadcV = ncfile->add_var("VEGCDEAD", ncDouble, timeD, pftD);
   	deadnV = ncfile->add_var("VEGNDEAD", ncDouble, timeD, pftD);
   	wdebriscV = ncfile->add_var("WDEBRISC", ncDouble, timeD);
   	wdebrisnV = ncfile->add_var("WDEBRISN", ncDouble, timeD);

 	//veg C/N fluxes
   	gppftV = ncfile->add_var("GPPFTEMP", ncDouble, timeD, pftD);
   	gppgvV = ncfile->add_var("GPPGV", ncDouble, timeD, pftD);
   	gppfnaV = ncfile->add_var("GPPFNA", ncDouble, timeD, pftD);
   	gppfcaV = ncfile->add_var("GPPFCA", ncDouble, timeD, pftD);
   	raq10V = ncfile->add_var("RAQ10", ncDouble, timeD, pftD);
   	rmkrV = ncfile->add_var("RMKR", ncDouble, timeD, pftD);

	ingppallV = ncfile->add_var("INGPPALL", ncDouble, timeD, pftD);
	ingppV = ncfile->add_var("INGPP", ncDouble, timeD, pftD, partD);
	innppallV = ncfile->add_var("INNPPALL", ncDouble, timeD, pftD);
	innppV = ncfile->add_var("INNPP", ncDouble, timeD, pftD, partD);
	gppallV = ncfile->add_var("GPPALL", ncDouble, timeD, pftD);
	gppV = ncfile->add_var("GPP", ncDouble, timeD, pftD, partD);
	nppallV = ncfile->add_var("NPPALL", ncDouble, timeD, pftD);
	nppV = ncfile->add_var("NPP", ncDouble, timeD, pftD, partD);
	rmallV = ncfile->add_var("RMALL", ncDouble, timeD, pftD);
	rmV = ncfile->add_var("RM", ncDouble, timeD, pftD, partD);
	rgallV = ncfile->add_var("RGALL", ncDouble, timeD, pftD);
	rgV = ncfile->add_var("RG", ncDouble, timeD, pftD, partD);
	ltrfalcallV = ncfile->add_var("LTRFALCALL", ncDouble, timeD, pftD);
	ltrfalcV = ncfile->add_var("LTRFALC", ncDouble, timeD, pftD, partD);

	ltrfalnallV = ncfile->add_var("LTRFALNALL", ncDouble, timeD, pftD);
	ltrfalnV = ncfile->add_var("LTRFALN", ncDouble, timeD, pftD, partD);
	innuptakeV = ncfile->add_var("INNUPTAKE", ncDouble, timeD, pftD);
	nrootextractV = ncfile->add_var("NROOTEXTRACT", ncDouble, timeD, pftD, soilayerD);
	luptakeV = ncfile->add_var("NUPTAKEL", ncDouble, timeD, pftD);
	suptakeallV = ncfile->add_var("NUPTAKESALL", ncDouble, timeD, pftD);
	suptakeV = ncfile->add_var("NUPTAKES", ncDouble, timeD, pftD, partD);
	nmobilallV = ncfile->add_var("NMOBILALL", ncDouble, timeD, pftD);
	nmobilV = ncfile->add_var("NMOBIL", ncDouble, timeD, pftD, partD);
	nresorballV = ncfile->add_var("NRESOBALL", ncDouble, timeD, pftD);
	nresorbV = ncfile->add_var("NRESORB", ncDouble, timeD, pftD, partD);

   	// soil C/N state variables
   	rawcV = ncfile->add_var("RAWC", ncDouble, timeD, soilayerD);
   	somaV = ncfile->add_var("SOMA", ncDouble, timeD, soilayerD);
   	somprV = ncfile->add_var("SOMPR", ncDouble, timeD, soilayerD);
   	somcrV = ncfile->add_var("SOMCR", ncDouble, timeD, soilayerD);
   	orgnV = ncfile->add_var("ORGN", ncDouble, timeD, soilayerD);
   	avlnV = ncfile->add_var("AVLN", ncDouble, timeD, soilayerD);

   	shlwcV = ncfile->add_var("SOMCSHLW", ncDouble, timeD);
   	deepcV = ncfile->add_var("SOMCDEEP", ncDouble, timeD);
   	mineacV = ncfile->add_var("SOMCMINEA", ncDouble, timeD);
   	minebcV = ncfile->add_var("SOMCMINEB", ncDouble, timeD);
   	mineccV = ncfile->add_var("SOMCMINEC", ncDouble, timeD);
   	rawcsumV = ncfile->add_var("RAWCSUM", ncDouble, timeD);
   	somasumV = ncfile->add_var("SOMASUM", ncDouble, timeD);
   	somprsumV = ncfile->add_var("SOMPRSUM", ncDouble, timeD);
   	somcrsumV = ncfile->add_var("SOMCRSUM", ncDouble, timeD);
   	orgnsumV = ncfile->add_var("ORGNSUM", ncDouble, timeD);
   	avlnsumV = ncfile->add_var("AVLNSUM", ncDouble, timeD);

   	// soil C/N fluxes
   	rhV  = ncfile->add_var("RH", ncDouble, timeD);
   	knmoistV = ncfile->add_var("NMITKSOIL", ncDouble, timeD, soilayerD);
   	rhmoistV = ncfile->add_var("RHMOIST", ncDouble, timeD, soilayerD);
   	rhq10V = ncfile->add_var("RHQ10", ncDouble, timeD, soilayerD);
   	soilltrfcnV = ncfile->add_var("SOILLTRFCN", ncDouble, timeD, soilayerD);

   	nepV = ncfile->add_var("NEP", ncDouble, timeD);

   	grsnminV = ncfile->add_var("GRSNMIN", ncDouble, timeD);
   	netnminV = ncfile->add_var("NETNMIN", ncDouble, timeD);

	orgcinputV = ncfile->add_var("ORGCINPUT", ncDouble, timeD);
	orgninputV = ncfile->add_var("ORGNINPUT", ncDouble, timeD);
	avlninputV = ncfile->add_var("AVLNINPUT", ncDouble, timeD);

	doclostV  = ncfile->add_var("DOCLOST", ncDouble, timeD);      //DOC lost
	avlnlostV = ncfile->add_var("AVLNLOST", ncDouble, timeD);     // N leaching
	orgnlostV = ncfile->add_var("ORGNLOST", ncDouble, timeD);     // DON loss

	burnthickV  = ncfile->add_var("BURNTHICK", ncDouble, timeD);
   	burnsoicV   = ncfile->add_var("BURNSOIC", ncDouble, timeD);
   	burnvegcV   = ncfile->add_var("BURNVEGC", ncDouble, timeD);
   	burnsoinV   = ncfile->add_var("BURNSOIN", ncDouble, timeD);
   	burnvegnV   = ncfile->add_var("BURNVEGN", ncDouble, timeD);
   	burnretaincV= ncfile->add_var("BURNRETAINC", ncDouble, timeD);
   	burnretainnV= ncfile->add_var("BURNRETAINN", ncDouble, timeD);

   	//methane
   	ch4V         = ncfile->add_var("CH4", ncDouble, timeD, soilayerD);
   	totch4fluxV  = ncfile->add_var("TOTCH4FLUX", ncDouble, timeD);;
	totch4flux2AV= ncfile->add_var("TOTCH4FLUX2A", ncDouble, timeD);;   // by diffusion
	totch4flux2PV= ncfile->add_var("TOTCH4FLUX2P", ncDouble, timeD);;   // by plant
	totch4flux2EV= ncfile->add_var("TOTCH4FLUX2E", ncDouble, timeD);;   // by ebullation

   	totch4prodV  = ncfile->add_var("TOTCH4PROD", ncDouble, timeD);;
   	ch4prodV     = ncfile->add_var("CH4PROD", ncDouble, timeD, soilayerD);
   	totch4oxidV  = ncfile->add_var("TOTCH4OXID", ncDouble, timeD);;
   	ch4oxidV     = ncfile->add_var("CH4OXID", ncDouble, timeD, soilayerD);

   	ncfile->close();
}

void BgcOutputer::outputCohortBgcVars(const int &outtstep, const int &ipft,
		BgcData *bd, FirData *fd,
		const int &calyr, const int &calmon, const int &calday, const int & tstepcnt){

	vegstate_dim *cdveg;
	vegstate_bgc *vegs;
	vegdiag_bgc *vegd;
	atm2veg_bgc *a2v;
	veg2atm_bgc *v2a;
	veg2veg_bgc *v2v;
	veg2soi_bgc *v2soi;
	soistate_bgc *sois;
	soidiag_bgc *soid;
	atm2soi_bgc *a2soi;
	soi2atm_bgc *soi2a;
	soi2lnd_bgc *soi2l;
	soi2veg_bgc *soi2v;
	lnd2atm_bgc *l2a;

	if(outtstep == DAILY) {
		cdveg = &bd->cd->d_veg;
		vegs = &bd->d_vegs;
		vegd = &bd->d_vegd;
		a2v = &bd->d_a2v;
		v2a = &bd->d_v2a;
		v2v = &bd->d_v2v;
		v2soi = &bd->d_v2soi;
		sois = &bd->d_sois;
		soid = &bd->d_soid;
		a2soi = &bd->d_a2soi;
		soi2a = &bd->d_soi2a;
		soi2l = &bd->d_soi2l;
		soi2v = &bd->d_soi2v;
		l2a = &bd->d_l2a;
	} else if(outtstep == MONTHLY) {
		cdveg = &bd->cd->m_veg;
		vegs = &bd->m_vegs;
		vegd = &bd->m_vegd;
		a2v = &bd->m_a2v;
		v2a = &bd->m_v2a;
		v2v = &bd->m_v2v;
		v2soi = &bd->m_v2soi;
		sois = &bd->m_sois;
		soid = &bd->m_soid;
		a2soi = &bd->m_a2soi;
		soi2a = &bd->m_soi2a;
		soi2l = &bd->m_soi2l;
		soi2v = &bd->m_soi2v;
		l2a = &bd->m_l2a;
	} else if(outtstep == YEARLY) {
		cdveg = &bd->cd->y_veg;
		vegs = &bd->y_vegs;
		vegd = &bd->y_vegd;
		a2v = &bd->y_a2v;
		v2a = &bd->y_v2a;
		v2v = &bd->y_v2v;
		v2soi = &bd->y_v2soi;
		sois = &bd->y_sois;
		soid = &bd->y_soid;
		a2soi = &bd->y_a2soi;
		soi2a = &bd->y_soi2a;
		soi2l = &bd->y_soi2l;
		soi2v = &bd->y_soi2v;
		l2a = &bd->y_l2a;
	}

	NcError err(NcError::verbose_nonfatal);

	ncfile = new NcFile(ncfname.c_str(), NcFile::Write);

    //variables
  	chtidV = ncfile->get_var("CHTID");
  	errorV = ncfile->get_var("ERRORID");
	yearV  = ncfile->get_var("YEAR");
	monV   = ncfile->get_var("MONTH");
	dayV   = ncfile->get_var("DAY");

	// veg C/N state variables
   	callV = ncfile->get_var("VEGCSUM");
   	cV = ncfile->get_var("VEGCPART");
   	nallV = ncfile->get_var("VEGNSUM");
   	labnV = ncfile->get_var("VEGNLAB");
   	strnallV = ncfile->get_var("VEGNSTRNSUM");
   	strnV = ncfile->get_var("VEGNSTRNPART");
   	deadcV = ncfile->get_var("VEGCDEAD");
   	deadnV = ncfile->get_var("VEGNDEAD");
   	wdebriscV = ncfile->get_var("WDEBRISC");
   	wdebrisnV = ncfile->get_var("WDEBRISN");

 	//veg C/N fluxes
   	gppftV = ncfile->get_var("GPPFTEMP");
   	gppgvV = ncfile->get_var("GPPGV");
   	gppfnaV = ncfile->get_var("GPPFNA");
   	gppfcaV = ncfile->get_var("GPPFCA");
   	raq10V = ncfile->get_var("RAQ10");
   	rmkrV = ncfile->get_var("RMKR");

	ingppallV = ncfile->get_var("INGPPALL");
	ingppV = ncfile->get_var("INGPP");
	innppallV = ncfile->get_var("INNPPALL");
	innppV = ncfile->get_var("INNPP");
	gppallV = ncfile->get_var("GPPALL");
	gppV = ncfile->get_var("GPP");
	nppallV = ncfile->get_var("NPPALL");
	nppV = ncfile->get_var("NPP");
	rmallV = ncfile->get_var("RMALL");
	rmV = ncfile->get_var("RM");
	rgallV = ncfile->get_var("RGALL");
	rgV = ncfile->get_var("RG");
	ltrfalcallV = ncfile->get_var("LTRFALCALL");
	ltrfalcV = ncfile->get_var("LTRFALC");

	ltrfalnallV = ncfile->get_var("LTRFALNALL");
	ltrfalnV = ncfile->get_var("LTRFALN");
	innuptakeV = ncfile->get_var("INNUPTAKE");
	nrootextractV = ncfile->get_var("NROOTEXTRACT");
	luptakeV = ncfile->get_var("NUPTAKEL");
	suptakeallV = ncfile->get_var("NUPTAKESALL");
	suptakeV = ncfile->get_var("NUPTAKES");
	nmobilallV = ncfile->get_var("NMOBILALL");
	nmobilV = ncfile->get_var("NMOBIL");
	nresorballV = ncfile->get_var("NRESOBALL");
	nresorbV = ncfile->get_var("NRESORB");

   	// soil C/N state variables
   	rawcV = ncfile->get_var("RAWC");
   	somaV = ncfile->get_var("SOMA");
   	somprV = ncfile->get_var("SOMPR");
   	somcrV = ncfile->get_var("SOMCR");
   	orgnV = ncfile->get_var("ORGN");
   	avlnV = ncfile->get_var("AVLN");

   	shlwcV = ncfile->get_var("SOMCSHLW");
   	deepcV = ncfile->get_var("SOMCDEEP");
   	mineacV = ncfile->get_var("SOMCMINEA");
   	minebcV = ncfile->get_var("SOMCMINEB");
   	mineccV = ncfile->get_var("SOMCMINEC");
   	rawcsumV = ncfile->get_var("RAWCSUM");
   	somasumV = ncfile->get_var("SOMASUM");
   	somprsumV = ncfile->get_var("SOMPRSUM");
   	somcrsumV = ncfile->get_var("SOMCRSUM");
   	orgnsumV = ncfile->get_var("ORGNSUM");
   	avlnsumV = ncfile->get_var("AVLNSUM");

   	// soil C/N fluxes
   	rhV  = ncfile->get_var("RH");
   	knmoistV = ncfile->get_var("NMITKSOIL");
   	rhmoistV = ncfile->get_var("RHMOIST");
   	rhq10V = ncfile->get_var("RHQ10");
   	soilltrfcnV = ncfile->get_var("SOILLTRFCN");

   	nepV = ncfile->get_var("NEP");

   	grsnminV = ncfile->get_var("GRSNMIN");
   	netnminV = ncfile->get_var("NETNMIN");

	orgcinputV = ncfile->get_var("ORGCINPUT");
	orgninputV = ncfile->get_var("ORGNINPUT");
	avlninputV = ncfile->get_var("AVLNINPUT");

	doclostV  = ncfile->get_var("DOCLOST");
	avlnlostV = ncfile->get_var("AVLNLOST");
	orgnlostV = ncfile->get_var("ORGNLOST");

	if (outtstep == MONTHLY) {
		burnthickV  = ncfile->get_var("BURNTHICK");
		burnsoicV   = ncfile->get_var("BURNSOIC");
		burnvegcV   = ncfile->get_var("BURNVEGC");
		burnsoinV   = ncfile->get_var("BURNSOIN");
		burnvegnV   = ncfile->get_var("BURNVEGN");
		burnretaincV= ncfile->get_var("BURNRETAINC");
		burnretainnV= ncfile->get_var("BURNRETAINN");
	}

   	//methane
   	ch4V         = ncfile->get_var("CH4");
   	totch4fluxV  = ncfile->get_var("TOTCH4FLUX");
	totch4flux2AV= ncfile->get_var("TOTCH4FLUX2A");
	totch4flux2PV= ncfile->get_var("TOTCH4FLUX2P");
	totch4flux2EV= ncfile->get_var("TOTCH4FLUX2E");

   	totch4prodV  = ncfile->get_var("TOTCH4PROD");
   	ch4prodV     = ncfile->get_var("CH4PROD");
   	totch4oxidV  = ncfile->get_var("TOTCH4OXID");
   	ch4oxidV     = ncfile->get_var("CH4OXID");

 	if (ipft <= 0) {  // for 'all PFTs', the index is -1
		if (tstepcnt==0) chtidV->put(&bd->cd->chtid);
		yearV->put_rec(&calyr, tstepcnt);
		monV->put_rec(&calmon, tstepcnt);
		dayV->put_rec(&calday, tstepcnt);

		// soil C/N state variables for all PFTs
   		wdebriscV->put_rec(&sois->wdebrisc, tstepcnt);
   		wdebrisnV->put_rec(&sois->wdebrisn, tstepcnt);
   		rawcV->put_rec(&sois->rawc[0], tstepcnt);
   		somaV->put_rec(&sois->soma[0], tstepcnt);
   		somprV->put_rec(&sois->sompr[0], tstepcnt);
   		somcrV->put_rec(&sois->somcr[0], tstepcnt);
   		orgnV->put_rec(&sois->orgn[0], tstepcnt);
   		avlnV->put_rec(&sois->avln[0], tstepcnt);
   		ch4V->put_rec(&sois->ch4[0], tstepcnt);

   		shlwcV->put_rec(&soid->shlwc, tstepcnt);
   		deepcV->put_rec(&soid->deepc, tstepcnt);
   		mineacV->put_rec(&soid->mineac, tstepcnt);
   		minebcV->put_rec(&soid->minebc, tstepcnt);
   		mineccV->put_rec(&soid->minecc, tstepcnt);
   		rawcsumV->put_rec(&soid->rawcsum, tstepcnt);
   		somasumV->put_rec(&soid->somasum, tstepcnt);
   		somprsumV->put_rec(&soid->somprsum, tstepcnt);
   		somcrsumV->put_rec(&soid->somcrsum, tstepcnt);
   		orgnsumV->put_rec(&soid->orgnsum, tstepcnt);
   		avlnsumV->put_rec(&soid->avlnsum, tstepcnt);

   		//soil C/N fluxes

   	 	rhV->put_rec(&soi2a->rhtot, tstepcnt);
		knmoistV->put_rec(&soid->knmoist[0], tstepcnt);
   		rhmoistV->put_rec(&soid->rhmoist[0], tstepcnt);

   		rhq10V->put_rec(&soid->rhq10[0], tstepcnt);
   		soilltrfcnV->put_rec(&soid->ltrfcn[0], tstepcnt);

   		nepV->put_rec(&l2a->nep, tstepcnt);

   		orgcinputV->put_rec(&a2soi->orgcinput, tstepcnt);
   		orgninputV->put_rec(&a2soi->orgninput, tstepcnt);
   		avlninputV->put_rec(&a2soi->avlninput, tstepcnt);

   		doclostV->put_rec(&soi2l->doclost, tstepcnt);      //DOC lost
   		avlnlostV->put_rec(&soi2l->avlnlost, tstepcnt);     // N leaching
   		orgnlostV->put_rec(&soi2l->orgnlost, tstepcnt);     // DON loss

   		//
   		if(outtstep == MONTHLY){
   			burnthickV->put_rec(&fd->fire_soid.burnthick, tstepcnt);
   			burnsoicV->put_rec(&fd->fire_soi2a.orgc, tstepcnt);
   			burnvegcV->put_rec(&fd->fire_v2a.orgc, tstepcnt);
   			burnsoinV->put_rec(&fd->fire_soi2a.orgn, tstepcnt);
   			burnvegnV->put_rec(&fd->fire_v2a.orgn, tstepcnt);
   			double ftmp1 = fd->fire_v2soi.abvc+fd->fire_v2soi.blwc;
   			burnretaincV->put_rec(&ftmp1, tstepcnt);
   			double ftmp2 = fd->fire_v2soi.abvn+fd->fire_v2soi.blwn;
   			burnretainnV->put_rec(&ftmp2, tstepcnt);
   		}

   		//methane
   		ch4V->put_rec(&sois->ch4[0], tstepcnt);
   		totch4fluxV->put_rec(&soi2a->totCH4Flux_m, tstepcnt);
   		totch4flux2AV->put_rec(&soi2a->totFlux2A_m, tstepcnt);
   		totch4flux2PV->put_rec(&soi2a->totPlant_m, tstepcnt);
   		totch4flux2EV->put_rec(&soi2a->totEbul_m, tstepcnt);

   		totch4prodV->put_rec(&soi2a->totProd_m, tstepcnt);
  		ch4prodV->put_rec(&soi2a->Prod_m[0], tstepcnt); //[MAX_SOI_LAY];
   		totch4oxidV->put_rec(&soi2a->totOxid_m, tstepcnt);
  		ch4oxidV->put_rec(&soi2a->Oxid_m[0], tstepcnt); //[MAX_SOI_LAY];

	}

 	if (ipft<0) {
 		// veg C/N state variables - output for PFT-integrated (ipft = -1)
 		callV->put_rec(&vegs->call, tstepcnt);

 		cV->put_rec(&vegs->c[0], tstepcnt);

 		nallV->put_rec(&vegs->nall, tstepcnt);

 		labnV->put_rec(&vegs->labn, tstepcnt);

 		strnallV->put_rec(&vegs->strnall, tstepcnt);

 		strnV->put_rec(&vegs->strn[0], tstepcnt);

 		deadcV->put_rec(&vegs->deadc, tstepcnt);

 		deadnV->put_rec(&vegs->deadn, tstepcnt);

 		//veg C/N fluxes for each PFT
 		gppftV->put_rec(&vegd->ftemp, tstepcnt);

 		gppgvV->put_rec(&vegd->gv, tstepcnt);

 		gppfnaV->put_rec(&vegd->fna, tstepcnt);

 		gppfcaV->put_rec(&vegd->fca, tstepcnt);

 		raq10V->put_rec(&vegd->raq10, tstepcnt);

 		rmkrV->put_rec(&vegd->kr[0], tstepcnt);

 		// C/N fluxes
 		ingppallV->put_rec(&a2v->ingppall, tstepcnt);

 		ingppV->put_rec(&a2v->ingpp[0], tstepcnt);

 		innppallV->put_rec(&a2v->innppall, tstepcnt);

 		innppV->put_rec(&a2v->innpp[0], tstepcnt);

 		gppallV->put_rec(&a2v->gppall, tstepcnt);

 		gppV->put_rec(&a2v->gpp[0], tstepcnt);

 		nppallV->put_rec(&a2v->nppall, tstepcnt);

 		nppV->put_rec(&a2v->npp[0], tstepcnt);

 		rmallV->put_rec(&v2a->rmall, tstepcnt);

 		rmV->put_rec(&v2a->rm[0], tstepcnt);

 		rgallV->put_rec(&v2a->rgall, tstepcnt);

 		rgV->put_rec(&v2a->rg[0], tstepcnt);

 		if (cdveg->nonvascular[ipft]>0) {
 			ltrfalcallV->put_rec(&v2soi->mossdeathc, tstepcnt);
 		} else {
 			ltrfalcallV->put_rec(&v2soi->ltrfalcall, tstepcnt);
 		}

 		ltrfalcV->put_rec(&v2soi->ltrfalc[0], tstepcnt);

 		//
 		if (cdveg->nonvascular[ipft]>0) {
 			ltrfalnallV->put_rec(&v2soi->mossdeathn, tstepcnt);
 		} else {
 			ltrfalnallV->put_rec(&v2soi->ltrfalnall, tstepcnt);
 		}

 		ltrfalnV->put_rec(&v2soi->ltrfaln[0], tstepcnt);

 		innuptakeV->put_rec(&soi2v->innuptake, tstepcnt);

 		nrootextractV->put_rec(&soi2v->nextract[0], tstepcnt);

 		luptakeV->put_rec(&soi2v->lnuptake, tstepcnt);

 		suptakeallV->put_rec(&soi2v->snuptakeall, tstepcnt);

 		suptakeV->put_rec(&soi2v->snuptake[0], tstepcnt);

 		nmobilallV->put_rec(&v2v->nmobilall, tstepcnt);

 		nmobilV->put_rec(&v2v->nmobil[0], tstepcnt);

 		nresorballV->put_rec(&v2v->nresorball, tstepcnt);

 		nresorbV->put_rec(&v2v->nresorb[0], tstepcnt);

 	} else if (ipft>=0) {
 		// veg C/N state variables - output for each PFT
 		callV->set_cur(tstepcnt, ipft);
 		callV->put(&vegs->call, 1, 1);

 		cV->set_cur(tstepcnt, ipft, 0);
 		cV->put(&vegs->c[0], 1, 1, NUM_PFT_PART);

 		nallV->set_cur(tstepcnt, ipft);
 		nallV->put(&vegs->nall, 1, 1);

 		labnV->set_cur(tstepcnt, ipft);
 		labnV->put(&vegs->labn, 1, 1);

 		strnallV->set_cur(tstepcnt, ipft);
 		strnallV->put(&vegs->strnall, 1, 1);

 		strnV->set_cur(tstepcnt, ipft, 0);
 		strnV->put(&vegs->strn[0], 1, 1, NUM_PFT_PART);

 		deadcV->set_cur(tstepcnt, ipft);
 		deadcV->put(&vegs->deadc, 1, 1);

 		deadnV->set_cur(tstepcnt, ipft);
 		deadnV->put(&vegs->deadn, 1, 1);

 		//veg C/N fluxes for each PFT
 		gppftV->set_cur(tstepcnt, ipft);
 		gppftV->put(&vegd->ftemp, 1, 1);

 		gppgvV->set_cur(tstepcnt, ipft);
 		gppgvV->put(&vegd->gv, 1, 1);

 		gppfnaV->set_cur(tstepcnt, ipft);
 		gppfnaV->put(&vegd->fna, 1, 1);

 		gppfcaV->set_cur(tstepcnt, ipft);
 		gppfcaV->put(&vegd->fca, 1, 1);

 		raq10V->set_cur(tstepcnt, ipft);
 		raq10V->put(&vegd->raq10, 1, 1);

 		rmkrV->set_cur(tstepcnt, ipft, 0);
 		rmkrV->put(&vegd->kr[0], 1, 1, NUM_PFT_PART);

 		// C/N fluxes
 		ingppallV->set_cur(tstepcnt, ipft);
 		ingppallV->put(&a2v->ingppall, 1, 1);

 		ingppV->set_cur(tstepcnt, ipft, 0);
 		ingppV->put(&a2v->ingpp[0], 1, 1, NUM_PFT_PART);

 		innppallV->set_cur(tstepcnt, ipft);
 		innppallV->put(&a2v->innppall, 1, 1);

 		innppV->set_cur(tstepcnt, ipft, 0);
 		innppV->put(&a2v->innpp[0], 1, 1, NUM_PFT_PART);

 		gppallV->set_cur(tstepcnt, ipft);
 		gppallV->put(&a2v->gppall, 1, 1);

 		gppV->set_cur(tstepcnt, ipft, 0);
 		gppV->put(&a2v->gpp[0], 1, 1, NUM_PFT_PART);

 		nppallV->set_cur(tstepcnt, ipft);
 		nppallV->put(&a2v->nppall, 1, 1);

 		nppV->set_cur(tstepcnt, ipft, 0);
 		nppV->put(&a2v->npp[0], 1, 1, NUM_PFT_PART);

 		rmallV->set_cur(tstepcnt, ipft);
 		rmallV->put(&v2a->rmall,  1, 1);

 		rmV->set_cur(tstepcnt, ipft, 0);
 		rmV->put(&v2a->rm[0], 1, 1, NUM_PFT_PART);

 		rgallV->set_cur(tstepcnt, ipft);
 		rgallV->put(&v2a->rgall,  1, 1);

 		rgV->set_cur(tstepcnt, ipft, 0);
 		rgV->put(&v2a->rg[0], 1, 1, NUM_PFT_PART);

 		ltrfalcallV->set_cur(tstepcnt, ipft);
 		if (cdveg->nonvascular[ipft]>0) {
 			ltrfalcallV->put(&v2soi->mossdeathc,  1, 1);
 		} else {
 			ltrfalcallV->put(&v2soi->ltrfalcall,  1, 1);
 		}

 		ltrfalcV->set_cur(tstepcnt, ipft, 0);
 		ltrfalcV->put(&v2soi->ltrfalc[0], 1, 1, NUM_PFT_PART);

 		//
 		ltrfalnallV->set_cur(tstepcnt, ipft);
 		if (cdveg->nonvascular[ipft]>0) {
 			ltrfalnallV->put(&v2soi->mossdeathn,  1, 1);
 		} else {
 			ltrfalnallV->put(&v2soi->ltrfalnall,  1, 1);
 		}

 		ltrfalnV->set_cur(tstepcnt, ipft, 0);
 		ltrfalnV->put(&v2soi->ltrfaln[0], 1, 1, NUM_PFT_PART);

 		innuptakeV->set_cur(tstepcnt, ipft);
 		innuptakeV->put(&soi2v->innuptake,  1, 1);

 		nrootextractV->set_cur(tstepcnt, ipft, 0);
 		nrootextractV->put(&soi2v->nextract[0], 1, 1, MAX_SOI_LAY);

 		luptakeV->set_cur(tstepcnt, ipft);
 		luptakeV->put(&soi2v->lnuptake,  1, 1);

 		suptakeallV->set_cur(tstepcnt, ipft);
 		suptakeallV->put(&soi2v->snuptakeall,  1, 1);

 		suptakeV->set_cur(tstepcnt, ipft);
 		suptakeV->put(&soi2v->snuptake[0], 1, 1, NUM_PFT_PART);

 		nmobilallV->set_cur(tstepcnt, ipft);
 		nmobilallV->put(&v2v->nmobilall, 1, 1);

 		nmobilV->set_cur(tstepcnt, ipft);
 		nmobilV->put(&v2v->nmobil[0], 1, 1, NUM_PFT_PART);

 		nresorballV->set_cur(tstepcnt, ipft);
 		nresorballV->put(&v2v->nresorball, 1, 1);

 		nresorbV->set_cur(tstepcnt, ipft);
 		nresorbV->put(&v2v->nresorb[0], 1, 1, NUM_PFT_PART);
 	}

 	ncfile->close();
}

