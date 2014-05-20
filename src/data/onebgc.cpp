#include "onebgc.h"

onebgc::onebgc(){

};

onebgc::~onebgc(){
	
};

// re-initialize onebgc class explicitly
void onebgc::clear(){

	bgc_vegs = vegstate_bgc();
	bgc_sois = soistate_bgc();
	bgc_vegd = vegdiag_bgc();
	bgc_soid = soidiag_bgc();
	bgc_l2a  = lnd2atm_bgc();
	bgc_a2v  = atm2veg_bgc();
	bgc_v2a  = veg2atm_bgc();
	bgc_v2soi = veg2soi_bgc();
	bgc_soi2v = soi2veg_bgc();
	bgc_v2v   = veg2veg_bgc();
	bgc_soi2l = soi2lnd_bgc();
	bgc_soi2a = soi2atm_bgc();
	bgc_a2soi = atm2soi_bgc();
	bgc_soi2soi = soi2soi_bgc();


	for (int il=0; il<MAX_SOI_LAY; il++){
		if (!prvltrfcnque[il].empty()) {
			prvltrfcnque[il].clear();
		};
	}

};

void onebgc::landstates_zeroing(){
//
};

void onebgc::landfluxes_zeroing(){
	bgc_l2a.nep =0.;
};

void onebgc::vegstates_zeroing(){
	for (int i=0; i<NUM_PFT_PART; i++){
		bgc_vegs.c[i]    = 0.;
		bgc_vegs.strn[i] = 0.;
	}

	bgc_vegs.call    = 0.;
	bgc_vegs.nall    = 0.;
 	bgc_vegs.labn    = 0.;
	bgc_vegs.strnall = 0.;

}

void onebgc::vegfluxes_zeroing(){
	for (int i=0; i<NUM_PFT_PART; i++){
		bgc_vegd.kr[i]   = 0.;

		bgc_a2v.ingpp[i] = 0.;
		bgc_a2v.innpp[i] = 0.;
		bgc_a2v.gpp[i]   = 0.;
		bgc_a2v.npp[i]   = 0.;
		bgc_v2a.rg[i]    = 0.;
 		bgc_v2a.rm[i]    = 0.;

 		bgc_v2v.nmobil[i]  = 0.;
 		bgc_v2v.nresorb[i] = 0.;

 		bgc_v2soi.ltrfalc[i] = 0.;
		bgc_v2soi.ltrfaln[i] = 0.;

		bgc_soi2v.snuptake[i] = 0.;
	}

 	bgc_vegd.fca     = 0.;
 	bgc_vegd.fna     = 0.;
 	bgc_vegd.ftemp   = 0.;
 	bgc_vegd.gv      = 0.;
 	bgc_vegd.raq10   = 0.;

	bgc_a2v.ingppall = 0.;
	bgc_a2v.innppall = 0.;
	bgc_a2v.gppall   = 0.;
	bgc_a2v.nppall   = 0.;
	bgc_v2a.rgall    = 0.;
	bgc_v2a.rmall    = 0.;

	bgc_v2soi.ltrfalcall = 0.;
	bgc_v2soi.ltrfalnall = 0.;
	bgc_v2soi.mossdeathc = 0.;
	bgc_v2soi.mossdeathn = 0.;

	bgc_v2v.nmobilall  = 0.;
	bgc_v2v.nresorball = 0.;

 	for (int il=0; il<MAX_SOI_LAY; il++){
 		bgc_soi2v.nextract[il]   = 0.;
 	}

 	bgc_soi2v.innuptake = 0.;
 	bgc_soi2v.lnuptake   = 0.;
 	bgc_soi2v.snuptakeall= 0.;
 
}

void onebgc::soilstates_zeroing(){

 	bgc_sois.wdebrisc= 0.;
 	bgc_sois.wdebrisn= 0.;
 	bgc_sois.dmossc  = 0.;
 	bgc_sois.dmossn  = 0.;

 	bgc_soid.shlwc   = 0.;
 	bgc_soid.deepc   = 0.;
 	bgc_soid.mineac  = 0.;  // top mineral SOMC
 	bgc_soid.minebc  = 0.;  // middle mineral SOMC
 	bgc_soid.minecc  = 0.;  // bottom mineral SOMC
 	bgc_soid.rawcsum = 0.;
 	bgc_soid.somasum = 0.;
 	bgc_soid.somprsum= 0.;
 	bgc_soid.somcrsum= 0.;
 	bgc_soid.orgnsum = 0.;
 	bgc_soid.avlnsum = 0.;

	for (int il =0; il<MAX_SOI_LAY; il++){
 		//
 		bgc_sois.rawc[il]=0.;
 		bgc_sois.soma[il]=0.;
 		bgc_sois.sompr[il]=0.;
 		bgc_sois.somcr[il]=0.;

 		bgc_sois.orgn[il] = 0.;
 		bgc_sois.avln[il] = 0.;
 		bgc_sois.ch4[il]  = 0.;

 	 	bgc_soid.tsomc[il] = 0.;

 	}

};

void onebgc::soilfluxes_zeroing(){

 	bgc_soi2a.rhtot     =0.;
 	bgc_soi2a.rhwdeb    =0.;
	bgc_soi2a.rhrawcsum =0.;
	bgc_soi2a.rhsomasum =0.;
	bgc_soi2a.rhsomprsum=0.;
	bgc_soi2a.rhsomcrsum=0.;

	bgc_soi2soi.netnminsum = 0.;
	bgc_soi2soi.nimmobsum  = 0.;

	for (int il =0; il<MAX_SOI_LAY; il++){
 		//
 		bgc_soid.ltrfcn[il] = 0.;
   		bgc_soid.knmoist[il]= 0.;
   		bgc_soid.rhmoist[il]= 0.;
   		bgc_soid.rhq10[il]  = 0.;

   		//
   		bgc_soi2a.rhrawc[il] = 0.;
   		bgc_soi2a.rhsoma[il] = 0.;
   		bgc_soi2a.rhsompr[il]= 0.;
   		bgc_soi2a.rhsomcr[il]= 0.;

   		//
   		bgc_soi2soi.netnmin[il] = 0.;
   		bgc_soi2soi.nimmob[il]  = 0.;

  		bgc_soi2a.Prod_m[il] = 0.;
  		bgc_soi2a.Oxid_m[il] = 0.;

 	}
 
	//
	bgc_a2soi.orgninput = 0.0;
	bgc_a2soi.avlninput = 0.0;
    bgc_soi2l.orgnlost  = 0.0;
    bgc_soi2l.avlnlost = 0.0;

    //
	bgc_soi2a.totProd_m    = 0.;
	bgc_soi2a.totOxid_m    = 0.;
	bgc_soi2a.totFlux2A_m  = 0.;
	bgc_soi2a.totPlant_m   = 0.;
	bgc_soi2a.totEbul_m    = 0.;
	bgc_soi2a.totCH4Flux_m = 0.;

};

