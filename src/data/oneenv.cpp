/* 
 * define output variables for one dataset of (bio)physical variables
 * 
 */

#include "oneenv.h"

oneenv::oneenv(){
	atmstates_zeroing();
	atmfluxes_zeroing();
	vegstates_zeroing();
	vegfluxes_zeroing();
	grndstates_zeroing();
	grndfluxes_zeroing();
};

oneenv::~oneenv(){
	
};

void oneenv::clear(){

	dim_snow = snwstate_dim();

    env_atms = atmstate_env();
    env_vegs = vegstate_env();
    env_snws = snwstate_env();
    env_sois = soistate_env();

    env_atmd = atmdiag_env();
    env_vegd = vegdiag_env();
    env_snwd = snwdiag_env();
    env_soid = soidiag_env();

    env_l2a = lnd2atm_env();
    env_a2l = atm2lnd_env();
    env_a2v = atm2veg_env();
    env_v2a = veg2atm_env();
    env_v2g = veg2gnd_env();
    env_soi2l = soi2lnd_env();
    env_soi2a = soi2atm_env();
    env_snw2a = snw2atm_env();
    env_snw2soi = snw2soi_env();

    //
    monthsfrozen  = 0.;
	rtfrozendays  = 0;
	rtunfrozendays= 0;

};

void oneenv::atmstates_zeroing(){

   	env_atms.ta   = 0.;
   	env_atms.co2  = 0.;
   	env_atmd.vp   = 0.;
   	env_atmd.svp  = 0.;
   	env_atmd.vpd  = 0.;

}

void oneenv::atmfluxes_zeroing(){

   	//atm to land (including both veg/ground)
   	env_a2l.nirr = 0;
	env_a2l.par  = 0;
	env_a2l.prec = 0;
	env_a2l.rnfl = 0;
	env_a2l.snfl = 0;

	// atm to veg only
	env_a2v.pardown  = 0.; // for photosynthesis
	env_a2v.parabsorb= 0.;
	env_a2v.swdown  = 0.;  // for energy balance
	env_a2v.swinter = 0.;
	env_a2v.rnfl    = 0.;  // for water balance
	env_a2v.rinter  = 0.;
	env_a2v.snfl    = 0.;
	env_a2v.sinter  = 0.;

	//
	env_l2a.eet = 0.;
  	env_l2a.pet = 0.;

}

void oneenv::vegstates_zeroing(){

	env_vegs.rwater  = 0.;
	env_vegs.snow    = 0.;

};

void oneenv::vegfluxes_zeroing(){

	env_vegd.rc      = 0.;
	env_vegd.cc      = 0.;
	env_vegd.btran   = 0.;
	env_vegd.m_ppfd  = 0.;
	env_vegd.m_vpd   = 0.;

	env_v2a.swrefl   = 0.;
	env_v2a.evap     = 0.;
	env_v2a.tran     = 0.;
	env_v2a.evap_pet = 0.;
	env_v2a.tran_pet = 0.;
	env_v2a.sublim   = 0.;

	env_v2g.swthfl   = 0.;
	env_v2g.rdrip    = 0.;
	env_v2g.rthfl    = 0.;
	env_v2g.sdrip    = 0.;
	env_v2g.sthfl    = 0.;

};

void oneenv::grndstates_zeroing(){

	//snow
	dim_snow.numsnwl = 0.;
	dim_snow.olds = 0.;
	dim_snow.thick = 0.;
	dim_snow.dense = 0.;

	dim_snow.extramass = 0.;

	for (int i=0; i<MAX_SNW_LAY; i++){
		dim_snow.dz[i] = 0.;
		dim_snow.age[i] = 0.;
		dim_snow.rho[i] = 0.;
		dim_snow.por[i] = 0.;

		env_snws.swe[i]   = 0.;
		env_snws.snwice[i]= 0.;
		env_snws.snwliq[i]= 0.;
		env_snws.tsnw[i]  = 0.;
	}
	env_snws.swesum  = 0.;
	env_snws.tsnwave = 0.;

	env_snwd.snowfreeFst= MISSING_I;
 	env_snwd.snowfreeLst= MISSING_I;

 	//soil
   	env_soid.icesum  = 0.;
   	env_soid.liqsum  = 0.;
   	env_soid.vwcshlw = 0.;
   	env_soid.vwcdeep = 0.;
   	env_soid.vwcminea= 0.;
   	env_soid.vwcmineb= 0.;
   	env_soid.vwcminec= 0.;

   	env_soid.tsave = 0.;
   	env_soid.tshlw = 0.;
   	env_soid.tdeep = 0.;
   	env_soid.tminea= 0.;
   	env_soid.tmineb= 0.;
   	env_soid.tminec= 0.;

   	env_soid.tcshlw = 0.;
   	env_soid.tcdeep = 0.;
   	env_soid.tcminea= 0.;
   	env_soid.tcmineb= 0.;
   	env_soid.tcminec= 0.;

   	env_soid.hkshlw = 0.;
   	env_soid.hkdeep = 0.;
   	env_soid.hkminea= 0.;
   	env_soid.hkmineb= 0.;
   	env_soid.hkminec= 0.;

   	for (int il=0; il<MAX_SOI_LAY; il++){
  		env_sois.frozen[il]      = 0.;
  		env_sois.frozenfrac[il]  = 0.;

   		env_sois.ts[il]      = 0.;
   		env_sois.liq[il]     = 0.;
   		env_sois.ice[il]     = 0.;
   	}
	env_sois.watertab   = 0.;
	env_sois.draindepth = 0.;

   	for (int il=0; il<MAX_SOI_LAY; il++){
		env_soid.vwc[il] = 0.;
		env_soid.lwc[il] = 0.;
		env_soid.iwc[il] = 0.;
		env_soid.sws[il] = 0.;
		env_soid.aws[il] = 0.;

		env_soid.fbtran[il] = 0.;
	}

	env_soid.permafrost  = 1;
	env_soid.unfrzcolumn = 0;
	env_soid.alc = 0.;
	env_soid.ald = MISSING_D;

   	env_soid.frasat = 0.;

	env_soid.rtdpthawpct  = 0.;
	env_soid.rtdpts   = 0.;
 	env_soid.rtdpgdd  = 0.;
 	env_soid.rtdpgrowstart  =MISSING_I;
 	env_soid.rtdpgrowend    =MISSING_I;

	env_soid.tbotrock = 0.;

}

void oneenv::grndfluxes_zeroing(){

	//snow
	env_snw2a.swrefl = 0.;
	env_snw2a.sublim = 0.;

	env_snw2soi.melt = 0.;

 	//soil
 	env_soi2a.swrefl   = 0.;
	env_soi2a.evap     = 0.;
	env_soi2a.evap_pet = 0.;

   	env_soi2l.qover =0.;
   	env_soi2l.qinfl =0.;
   	env_soi2l.qdrain=0.;

}
