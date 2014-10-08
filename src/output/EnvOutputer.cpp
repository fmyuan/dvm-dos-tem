/*! \file 
*/

#include "EnvOutputer.h"

EnvOutputer::EnvOutputer(){
	
};

EnvOutputer::~EnvOutputer(){

};

void EnvOutputer::init(string & dirfile, const int &numpft){

	//file
	ncfname = dirfile;

	pftintegrated = true;
	if(numpft>0) pftintegrated = false;

	ncfileenv = new NcFile(ncfname.c_str(), NcFile::Replace);

	//dimension
	timeD    = ncfileenv->add_dim("tstep");
	pftD     = ncfileenv->add_dim("pft", numpft);
	snwlayerD= ncfileenv->add_dim("snwlayer", MAX_SNW_LAY);
	soilayerD= ncfileenv->add_dim("soilayer", MAX_SOI_LAY);
	frontD  = ncfileenv->add_dim("frontnum", MAX_NUM_FNT);

    //variables
  	chtidV = ncfileenv->add_var("CHTID", ncInt);
  	errorV = ncfileenv->add_var("ERRORID", ncInt, timeD);
	yearV  = ncfileenv->add_var("YEAR", ncInt, timeD);
	monV   = ncfileenv->add_var("MONTH", ncInt, timeD);
	dayV   = ncfileenv->add_var("DAY", ncInt, timeD);

	// atm variables
   	co2V = ncfileenv->add_var("CO2", ncDouble, timeD);
   	tairV= ncfileenv->add_var("TAIR", ncDouble, timeD);
   	nirrV= ncfileenv->add_var("NIRR", ncDouble, timeD);
   	precV= ncfileenv->add_var("PREC", ncDouble, timeD);
  	vpV  = ncfileenv->add_var("VAPO", ncDouble, timeD);
   	svpV = ncfileenv->add_var("SVP", ncDouble, timeD);
   	vpdV = ncfileenv->add_var("VPD", ncDouble, timeD);
   	parV = ncfileenv->add_var("PAR", ncDouble, timeD);
   	rnflV= ncfileenv->add_var("RAINFALL", ncDouble, timeD);
   	snflV= ncfileenv->add_var("SNOWFALL", ncDouble, timeD);

   	// land-surface variables
   	pardownV= ncfileenv->add_var("PARDOWN", ncDouble, timeD);
   	parabsorbV= ncfileenv->add_var("PARABSORB", ncDouble, timeD);
   	swdownV= ncfileenv->add_var("SWDOWN", ncDouble, timeD);
   	swinterV= ncfileenv->add_var("SWINTER", ncDouble, timeD);
   	rinterV= ncfileenv->add_var("RAININTER", ncDouble, timeD);
   	sinterV= ncfileenv->add_var("SNOWINTER", ncDouble, timeD);
   	eetV= ncfileenv->add_var("EETTOTAL", ncDouble, timeD);
   	petV= ncfileenv->add_var("PETTOTAL", ncDouble, timeD);

   	// canopy-env variables
   	vegwaterV= ncfileenv->add_var("CANOPYRAIN", ncDouble, timeD, pftD);
   	vegsnowV= ncfileenv->add_var("CANOPYSNOW", ncDouble, timeD, pftD);
   	vegrcV= ncfileenv->add_var("CANOPYRC", ncDouble, timeD, pftD);
   	vegccV= ncfileenv->add_var("CANOPYCC", ncDouble, timeD, pftD);
   	vegbtranV= ncfileenv->add_var("CANOPYBTRAN", ncDouble, timeD, pftD);
   	vegm_ppfdV= ncfileenv->add_var("CANOPYM_PPFD", ncDouble, timeD, pftD);
   	vegm_vpdV= ncfileenv->add_var("CANOPYM_VPD", ncDouble, timeD, pftD);

   	vegswreflV= ncfileenv->add_var("CANOPYSWREFL", ncDouble, timeD, pftD);
   	vegswthflV= ncfileenv->add_var("CANOPYSWTHFL", ncDouble, timeD, pftD);

   	vegevapV= ncfileenv->add_var("CANOPYEVAP", ncDouble, timeD, pftD);
   	vegtranV= ncfileenv->add_var("CANOPYTRAN", ncDouble, timeD, pftD);
   	vegevap_pV= ncfileenv->add_var("CANOPYPEVAP", ncDouble, timeD, pftD);
   	vegtran_pV= ncfileenv->add_var("CANOPYPTRAN", ncDouble, timeD, pftD);
   	vegsublimV= ncfileenv->add_var("CANOPYSUBLIM", ncDouble, timeD, pftD);

   	vegrdripV= ncfileenv->add_var("CANOPYRDRIP", ncDouble, timeD, pftD);
   	vegrthflV= ncfileenv->add_var("CANOPYRTHFL", ncDouble, timeD, pftD);
   	vegsdripV= ncfileenv->add_var("CANOPYSDRIP", ncDouble, timeD, pftD);
   	vegsthflV= ncfileenv->add_var("CANOPYSTHFL", ncDouble, timeD, pftD);

	//snow
	snwthickV= ncfileenv->add_var("SNWTHICK", ncDouble, timeD);
	snwdenseV= ncfileenv->add_var("SNWDENSITY", ncDouble, timeD);
	snwextramassV= ncfileenv->add_var("SNWEXTRAMASS", ncDouble, timeD);
	snwageV= ncfileenv->add_var("SNWAGE", ncDouble, timeD);
   	sweV= ncfileenv->add_var("SNWWE", ncDouble, timeD, snwlayerD);
   	tsnwV= ncfileenv->add_var("SNWT", ncDouble, timeD, snwlayerD);
   	swesumV= ncfileenv->add_var("SNWWESUM", ncDouble, timeD);
   	tsnwaveV= ncfileenv->add_var("SNWTAVE", ncDouble, timeD);
   	snwswreflV= ncfileenv->add_var("SNWSWREFL", ncDouble, timeD);
   	snwsublimV= ncfileenv->add_var("SNWSUBLIM", ncDouble, timeD);

 	//soil
   	soilicesumV  = ncfileenv->add_var("SOILICESUM", ncDouble, timeD);
   	soilliqsumV  = ncfileenv->add_var("SOILLIQSUM", ncDouble, timeD);
   	soilvwcshlwV = ncfileenv->add_var("SOILVWCSHLW", ncDouble, timeD);
   	soilvwcdeepV = ncfileenv->add_var("SOILVWCDEEP", ncDouble, timeD);
   	soilvwcmineaV= ncfileenv->add_var("SOILVWCMINEA", ncDouble, timeD);
   	soilvwcminebV= ncfileenv->add_var("SOILVWCMINEB", ncDouble, timeD);
   	soilvwcminecV= ncfileenv->add_var("SOILVWCMINEC", ncDouble, timeD);

   	soiltaveV  = ncfileenv->add_var("SOILTAVE", ncDouble, timeD);
   	soiltshlwV = ncfileenv->add_var("SOILTSHLW", ncDouble, timeD);
   	soiltdeepV = ncfileenv->add_var("SOILTDEEP", ncDouble, timeD);
   	soiltmineaV= ncfileenv->add_var("SOILTMINEA", ncDouble, timeD);
   	soiltminebV= ncfileenv->add_var("SOILTMINEB", ncDouble, timeD);
   	soiltminecV= ncfileenv->add_var("SOILTMINEC", ncDouble, timeD);

   	soiltsV= ncfileenv->add_var("SOILTEM", ncDouble, timeD, soilayerD);
	soilliqV= ncfileenv->add_var("SOILLIQ", ncDouble, timeD, soilayerD);
	soiliceV= ncfileenv->add_var("SOILICE", ncDouble, timeD, soilayerD);
	soilvwcV= ncfileenv->add_var("SOILVWC", ncDouble, timeD, soilayerD);
	soillwcV= ncfileenv->add_var("SOILLWC", ncDouble, timeD, soilayerD);
	soiliwcV= ncfileenv->add_var("SOILIWC", ncDouble, timeD, soilayerD);
	soilfrontzV= ncfileenv->add_var("FRONTZ", ncDouble, timeD, frontD);
	soilfronttypeV= ncfileenv->add_var("FRONTTYPE", ncDouble, timeD, frontD);

	soilwatertabV= ncfileenv->add_var("WATERTABLE", ncDouble, timeD);
	permafrostV= ncfileenv->add_var("PERMAFROST", ncDouble, timeD);
	soilaldV= ncfileenv->add_var("ALD", ncDouble, timeD);
	soilalcV= ncfileenv->add_var("ALC", ncDouble, timeD);

	soilgrowstartV= ncfileenv->add_var("RZGROWSTART", ncDouble, timeD);
	soilgrowendV= ncfileenv->add_var("RZGROWEND", ncDouble, timeD);
	soiltsrtdpV= ncfileenv->add_var("RZTEM", ncDouble, timeD);
	soiltsdegdayV= ncfileenv->add_var("RZDEGDAY", ncDouble, timeD);
	soilrtthawpctV= ncfileenv->add_var("RZTHAWPCT", ncDouble, timeD);

	soilswreflV= ncfileenv->add_var("SOILSWREFL", ncDouble, timeD);
	soilevapV= ncfileenv->add_var("SOILEVAP", ncDouble, timeD);
	soilevap_pV= ncfileenv->add_var("SOILPEVAP", ncDouble, timeD);

	qoverV= ncfileenv->add_var("RUNOFF", ncDouble, timeD);
	qdrainV= ncfileenv->add_var("DRAINAGE", ncDouble, timeD);

	ncfileenv->close();

}

void EnvOutputer::outputCohortEnvVars(const int &outtstep,
		const int &ipft, snwstate_dim *dsnow, EnvData * envod,
		const int&iy, const int&im, const int &id, const int & tstepcnt){

	if(outtstep==DAILY){
		atms = &envod->d_atms;
		atmd = &envod->d_atmd;
		a2l  = &envod->d_a2l;
		a2v  = &envod->d_a2v;
		l2a  = &envod->d_l2a;

		vegs = &envod->d_vegs;
		vegd = &envod->d_vegd;
		v2a  = &envod->d_v2a;
		v2g  = &envod->d_v2g;

		snws = &envod->d_snws;
		snw2a= &envod->d_snw2a;
		sois = &envod->d_sois;
		soid = &envod->d_soid;
		soi2a= &envod->d_soi2a;
		soi2l= &envod->d_soi2l;
	} else if(outtstep==MONTHLY){
		atms = &envod->m_atms;
		atmd = &envod->m_atmd;
		a2l  = &envod->m_a2l;
		a2v  = &envod->m_a2v;
		l2a  = &envod->m_l2a;

		vegs = &envod->m_vegs;
		vegd = &envod->m_vegd;
		v2a  = &envod->m_v2a;
		v2g  = &envod->m_v2g;

		snws = &envod->m_snws;
		snw2a= &envod->m_snw2a;
		sois = &envod->m_sois;
		soid = &envod->m_soid;
		soi2a= &envod->m_soi2a;
		soi2l= &envod->m_soi2l;
	} else if(outtstep==YEARLY){
		atms = &envod->y_atms;
		atmd = &envod->y_atmd;
		a2l  = &envod->y_a2l;
		a2v  = &envod->y_a2v;
		l2a  = &envod->y_l2a;

		vegs = &envod->y_vegs;
		vegd = &envod->y_vegd;
		v2a  = &envod->y_v2a;
		v2g  = &envod->y_v2g;

		snws = &envod->y_snws;
		snw2a= &envod->y_snw2a;
		sois = &envod->y_sois;
		soid = &envod->y_soid;
		soi2a= &envod->y_soi2a;
		soi2l= &envod->y_soi2l;
	}

	NcError err(NcError::verbose_nonfatal);

	// file and variables
	ncfileenv = new NcFile(ncfname.c_str(), NcFile::Write);
	if(ncfileenv->is_valid()) {
	    //variables
	  	chtidV = ncfileenv->get_var("CHTID");
	  	errorV = ncfileenv->get_var("ERRORID");
		yearV  = ncfileenv->get_var("YEAR");
		monV   = ncfileenv->get_var("MONTH");
		dayV   = ncfileenv->get_var("DAY");

		// atm variables
	   	co2V = ncfileenv->get_var("CO2");
	   	tairV= ncfileenv->get_var("TAIR");
	   	nirrV= ncfileenv->get_var("NIRR");
	   	precV= ncfileenv->get_var("PREC");
	  	vpV  = ncfileenv->get_var("VAPO");
	   	svpV = ncfileenv->get_var("SVP");
	   	vpdV = ncfileenv->get_var("VPD");
	   	parV = ncfileenv->get_var("PAR");
	   	rnflV= ncfileenv->get_var("RAINFALL");
	   	snflV= ncfileenv->get_var("SNOWFALL");

	   	// land-surface variables
	   	pardownV= ncfileenv->get_var("PARDOWN");
	   	parabsorbV= ncfileenv->get_var("PARABSORB");
	   	swdownV= ncfileenv->get_var("SWDOWN");
	   	swinterV= ncfileenv->get_var("SWINTER");
	   	rinterV= ncfileenv->get_var("RAININTER");
	   	sinterV= ncfileenv->get_var("SNOWINTER");
	   	eetV= ncfileenv->get_var("EETTOTAL");
	   	petV= ncfileenv->get_var("PETTOTAL");

	   	// canopy-env variables
	   	vegwaterV= ncfileenv->get_var("CANOPYRAIN");
	   	vegsnowV= ncfileenv->get_var("CANOPYSNOW");
	   	vegrcV= ncfileenv->get_var("CANOPYRC");
	   	vegccV= ncfileenv->get_var("CANOPYCC");
	   	vegbtranV= ncfileenv->get_var("CANOPYBTRAN");
	   	vegm_ppfdV= ncfileenv->get_var("CANOPYM_PPFD");
	   	vegm_vpdV= ncfileenv->get_var("CANOPYM_VPD");

	   	vegswreflV= ncfileenv->get_var("CANOPYSWREFL");
	   	vegswthflV= ncfileenv->get_var("CANOPYSWTHFL");

	   	vegevapV= ncfileenv->get_var("CANOPYEVAP");
	   	vegtranV= ncfileenv->get_var("CANOPYTRAN");
	   	vegevap_pV= ncfileenv->get_var("CANOPYPEVAP");
	   	vegtran_pV= ncfileenv->get_var("CANOPYPTRAN");
	   	vegsublimV= ncfileenv->get_var("CANOPYSUBLIM");

	   	vegrdripV= ncfileenv->get_var("CANOPYRDRIP");
	   	vegrthflV= ncfileenv->get_var("CANOPYRTHFL");
	   	vegsdripV= ncfileenv->get_var("CANOPYSDRIP");
	   	vegsthflV= ncfileenv->get_var("CANOPYSTHFL");

		//snow
		snwthickV= ncfileenv->get_var("SNWTHICK");
		snwdenseV= ncfileenv->get_var("SNWDENSITY");
		snwextramassV= ncfileenv->get_var("SNWEXTRAMASS");
		snwageV= ncfileenv->get_var("SNWAGE");

	   	sweV= ncfileenv->get_var("SNWWE");
	   	tsnwV= ncfileenv->get_var("SNWT");
	   	swesumV= ncfileenv->get_var("SNWWESUM");
	   	tsnwaveV= ncfileenv->get_var("SNWTAVE");
	   	snwswreflV= ncfileenv->get_var("SNWSWREFL");
	   	snwsublimV= ncfileenv->get_var("SNWSUBLIM");

	 	//soil
	   	soilicesumV  = ncfileenv->get_var("SOILICESUM");
	   	soilliqsumV  = ncfileenv->get_var("SOILLIQSUM");
	   	soilvwcshlwV = ncfileenv->get_var("SOILVWCSHLW");
	   	soilvwcdeepV = ncfileenv->get_var("SOILVWCDEEP");
	   	soilvwcmineaV= ncfileenv->get_var("SOILVWCMINEA");
	   	soilvwcminebV= ncfileenv->get_var("SOILVWCMINEB");
	   	soilvwcminecV= ncfileenv->get_var("SOILVWCMINEC");

	   	soiltaveV  = ncfileenv->get_var("SOILTAVE");
	   	soiltshlwV = ncfileenv->get_var("SOILTSHLW");
	   	soiltdeepV = ncfileenv->get_var("SOILTDEEP");
	   	soiltmineaV= ncfileenv->get_var("SOILTMINEA");
	   	soiltminebV= ncfileenv->get_var("SOILTMINEB");
	   	soiltminecV= ncfileenv->get_var("SOILTMINEC");

	   	soiltsV= ncfileenv->get_var("SOILTEM");
		soilliqV= ncfileenv->get_var("SOILLIQ");
		soiliceV= ncfileenv->get_var("SOILICE");
		soilvwcV= ncfileenv->get_var("SOILVWC");
		soillwcV= ncfileenv->get_var("SOILLWC");
		soiliwcV= ncfileenv->get_var("SOILIWC");
		soilfrontzV= ncfileenv->get_var("FRONTZ");
		soilfronttypeV= ncfileenv->get_var("FRONTTYPE");

		soilwatertabV= ncfileenv->get_var("WATERTABLE");
		permafrostV= ncfileenv->get_var("PERMAFROST");
		soilaldV= ncfileenv->get_var("ALD");
		soilalcV= ncfileenv->get_var("ALC");

		soilgrowstartV= ncfileenv->get_var("RZGROWSTART");
		soilgrowendV= ncfileenv->get_var("RZGROWEND");
		soiltsrtdpV= ncfileenv->get_var("RZTEM");
		soiltsdegdayV= ncfileenv->get_var("RZDEGDAY");
		soilrtthawpctV= ncfileenv->get_var("RZTHAWPCT");

		soilswreflV= ncfileenv->get_var("SOILSWREFL");
		soilevapV= ncfileenv->get_var("SOILEVAP");
		soilevap_pV= ncfileenv->get_var("SOILPEVAP");

		qoverV= ncfileenv->get_var("RUNOFF");
		qdrainV= ncfileenv->get_var("DRAINAGE");

	} else {
		cout<<"Output File: "<< ncfname <<"NOT exists! "<<"\n";
		exit(-1);
	}

	// pft-integrated output (ipft = -1)
	// OR shared variables for multiple-pfts (only write when ipft=0)
	if (ipft<=0) {

 		yearV->put_rec(&iy, tstepcnt);
 		monV->put_rec(&im, tstepcnt);
 		dayV->put_rec(&id, tstepcnt);

 		// atm variables
 		co2V->put_rec(&atms->co2, tstepcnt);
 		tairV->put_rec(&atms->ta, tstepcnt);
 		nirrV->put_rec(&a2l->nirr, tstepcnt);
 		precV->put_rec(&a2l->prec, tstepcnt);
 		vpV->put_rec(&atmd->vp, tstepcnt);
 		svpV->put_rec(&atmd->svp, tstepcnt);
 		vpdV->put_rec(&atmd->vpd, tstepcnt);
 		parV->put_rec(&a2l->par, tstepcnt);
 		rnflV->put_rec(&a2l->rnfl, tstepcnt);
 		snflV->put_rec(&a2l->snfl, tstepcnt);

 		// land-surface variables
 		pardownV->put_rec(&a2v->pardown, tstepcnt);
 		parabsorbV->put_rec(&a2v->parabsorb, tstepcnt);
 		swdownV->put_rec(&a2v->swdown, tstepcnt);
 		swinterV->put_rec(&a2v->swinter, tstepcnt);
 		rinterV->put_rec(&a2v->rinter, tstepcnt);
 		sinterV->put_rec(&a2v->sinter, tstepcnt);
 		eetV->put_rec(&l2a->eet, tstepcnt);
 		petV->put_rec(&l2a->pet, tstepcnt);

 	}

	//
	if (ipft<0) {
 		//canopy-env. variables for pft-integrated
 		vegwaterV->put_rec(&vegs->rwater, tstepcnt);
 		vegsnowV->put(&vegs->snow, tstepcnt);
 		vegrcV->put(&vegd->rc, tstepcnt);
 		vegccV->put(&vegd->cc, tstepcnt);
   		vegbtranV->put(&vegd->btran, tstepcnt);
   		vegm_ppfdV->put(&vegd->m_ppfd, tstepcnt);
   		vegm_vpdV->put(&vegd->m_vpd, tstepcnt);
   		vegswreflV->put(&v2a->swrefl, tstepcnt);
   		vegswthflV->put(&v2g->swthfl, tstepcnt);
   		vegevapV->put(&v2a->evap, tstepcnt);
   		vegtranV->put(&v2a->tran, tstepcnt);
   		vegevap_pV->put(&v2a->evap_pet, tstepcnt);
   		vegtran_pV->put(&v2a->tran_pet, tstepcnt);
   		vegsublimV->put(&v2a->sublim, tstepcnt);
   		vegrdripV->put(&v2g->rdrip, tstepcnt);
   		vegrthflV->put(&v2g->rthfl, tstepcnt);
   		vegsdripV->put(&v2g->sdrip, tstepcnt);
   		vegsthflV->put(&v2g->sthfl, tstepcnt);

 	} else if (ipft>=0) {
 		//canopy-env variables for ipft
 		vegwaterV->set_cur(tstepcnt, ipft);
 		vegwaterV->put(&vegs->rwater, 1, 1);

 		vegsnowV->set_cur(tstepcnt, ipft);
 		vegsnowV->put(&vegs->snow, 1, 1);

 		vegrcV->set_cur(tstepcnt, ipft);
 		vegrcV->put(&vegd->rc, 1, 1);

 		vegccV->set_cur(tstepcnt, ipft);
 		vegccV->put(&vegd->cc, 1, 1);

 		vegbtranV->set_cur(tstepcnt, ipft);
   		vegbtranV->put(&vegd->btran, 1, 1);

   		vegm_ppfdV->set_cur(tstepcnt, ipft);
   		vegm_ppfdV->put(&vegd->m_ppfd, 1, 1);

   		vegm_vpdV->set_cur(tstepcnt, ipft);
   		vegm_vpdV->put(&vegd->m_vpd, 1, 1);

 		vegswreflV->set_cur(tstepcnt, ipft);
   		vegswreflV->put(&v2a->swrefl, 1, 1);

   		vegswthflV->set_cur(tstepcnt, ipft);
   		vegswthflV->put(&v2g->swthfl, 1, 1);

 		vegevapV->set_cur(tstepcnt, ipft);
   		vegevapV->put(&v2a->evap, 1, 1);

   		vegtranV->set_cur(tstepcnt, ipft);
   		vegtranV->put(&v2a->tran, 1, 1);

   		vegevap_pV->set_cur(tstepcnt, ipft);
   		vegevap_pV->put(&v2a->evap_pet, 1, 1);

   		vegtran_pV->set_cur(tstepcnt, ipft);
   		vegtran_pV->put(&v2a->tran_pet, 1, 1);

   		vegsublimV->set_cur(tstepcnt, ipft);
   		vegsublimV->put(&v2a->sublim, 1, 1);

 		vegrdripV->set_cur(tstepcnt, ipft);
   		vegrdripV->put(&v2g->rdrip, 1, 1);

   		vegrthflV->set_cur(tstepcnt, ipft);
   		vegrthflV->put(&v2g->rthfl, 1, 1);

   		vegsdripV->set_cur(tstepcnt, ipft);
   		vegsdripV->put(&v2g->sdrip, 1, 1);

   		vegsthflV->set_cur(tstepcnt, ipft);
   		vegsthflV->put(&v2g->sthfl, 1, 1);

 	}

	// ground-soil variables
	if (ipft<=0) {
		//snow

		snwthickV->put_rec(&dsnow->thick, tstepcnt);
		snwdenseV->put_rec(&dsnow->dense, tstepcnt);
		snwextramassV->put_rec(&dsnow->extramass, tstepcnt);
		snwageV->put_rec(&dsnow->olds, tstepcnt);

		sweV->put_rec(&snws->swe[0], tstepcnt);
		tsnwV->put_rec(&snws->tsnw[0], tstepcnt);
		swesumV->put_rec(&snws->swesum, tstepcnt);
		tsnwaveV->put_rec(&snws->tsnwave, tstepcnt);
		snwswreflV->put_rec(&snw2a->swrefl, tstepcnt);
		snwsublimV->put_rec(&snw2a->sublim, tstepcnt);

		//soil
		soilicesumV->put_rec(&soid->icesum, tstepcnt);
		soilliqsumV->put_rec(&soid->liqsum, tstepcnt);
		soilvwcshlwV->put_rec(&soid->vwcshlw, tstepcnt);
		soilvwcdeepV->put_rec(&soid->vwcdeep, tstepcnt);
		soilvwcmineaV->put_rec(&soid->vwcminea, tstepcnt);
		soilvwcminebV->put_rec(&soid->vwcmineb, tstepcnt);
		soilvwcminecV->put_rec(&soid->vwcminec, tstepcnt);
		soiltaveV->put_rec(&soid->tsave, tstepcnt);
		soiltshlwV->put_rec(&soid->tshlw, tstepcnt);
		soiltdeepV->put_rec(&soid->tdeep, tstepcnt);
		soiltmineaV->put_rec(&soid->tminea, tstepcnt);
		soiltminebV->put_rec(&soid->tmineb, tstepcnt);
		soiltminecV->put_rec(&soid->tminec, tstepcnt);

		soiltsV->put_rec(&sois->ts[0], tstepcnt);
		soilliqV->put_rec(&sois->liq[0], tstepcnt);
		soiliceV->put_rec(&sois->ice[0], tstepcnt);
		soilvwcV->put_rec(&soid->vwc[0], tstepcnt);
		soillwcV->put_rec(&soid->lwc[0], tstepcnt);
		soiliwcV->put_rec(&soid->iwc[0], tstepcnt);
		soilfrontzV->put_rec(&sois->frontsz[0], tstepcnt);
		soilfronttypeV->put_rec(&sois->frontstype[0], tstepcnt);

		soilwatertabV->put_rec(&sois->watertab, tstepcnt);
		permafrostV->put_rec(&sois->permafrost, tstepcnt);
		soilaldV->put_rec(&sois->ald, tstepcnt);
		soilalcV->put_rec(&sois->alc, tstepcnt);

		soilgrowstartV->put_rec(&soid->rtdpgrowstart, tstepcnt);
		soilgrowendV->put_rec(&soid->rtdpgrowend, tstepcnt);
		soiltsrtdpV->put_rec(&soid->rtdpts, tstepcnt);
		soiltsdegdayV->put_rec(&soid->rtdpgdd, tstepcnt);
		soilrtthawpctV->put_rec(&soid->rtdpthawpct, tstepcnt);

		soilswreflV->put_rec(&soi2a->swrefl, tstepcnt);
		soilevapV->put_rec(&soi2a->evap, tstepcnt);
		soilevap_pV->put_rec(&soi2a->evap_pet, tstepcnt);

		qoverV->put_rec(&soi2l->qover, tstepcnt);
		qdrainV->put_rec(&soi2l->qdrain, tstepcnt);
	}

	ncfileenv->close();

}
