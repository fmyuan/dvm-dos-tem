#include "BgcData.h"

BgcData::BgcData(){
	clear();
};

BgcData::~BgcData(){
	
};

// re-initialize BgcData class explicitly
void BgcData::clear(){

	//daily
	d_vegs = vegstate_bgc();
	d_sois = soistate_bgc();
	d_vegd = vegdiag_bgc();
	d_soid = soidiag_bgc();
	d_l2a  = lnd2atm_bgc();
	d_a2v  = atm2veg_bgc();
	d_v2a  = veg2atm_bgc();
	d_v2soi = veg2soi_bgc();
	d_soi2v = soi2veg_bgc();
	d_v2v   = veg2veg_bgc();
	d_soi2l = soi2lnd_bgc();
	d_soi2a = soi2atm_bgc();
	d_a2soi = atm2soi_bgc();
	d_soi2soi = soi2soi_bgc();

	//monthly
	m_vegs = vegstate_bgc();
	m_sois = soistate_bgc();
	m_vegd = vegdiag_bgc();
	m_soid = soidiag_bgc();
	m_l2a  = lnd2atm_bgc();
	m_a2v  = atm2veg_bgc();
	m_v2a  = veg2atm_bgc();
	m_v2soi = veg2soi_bgc();
	m_soi2v = soi2veg_bgc();
	m_v2v   = veg2veg_bgc();
	m_soi2l = soi2lnd_bgc();
	m_soi2a = soi2atm_bgc();
	m_a2soi = atm2soi_bgc();
	m_soi2soi = soi2soi_bgc();

	//yearly
	y_vegs = vegstate_bgc();
	y_sois = soistate_bgc();
	y_vegd = vegdiag_bgc();
	y_soid = soidiag_bgc();
	y_l2a  = lnd2atm_bgc();
	y_a2v  = atm2veg_bgc();
	y_v2a  = veg2atm_bgc();
	y_v2soi = veg2soi_bgc();
	y_soi2v = soi2veg_bgc();
	y_v2v   = veg2veg_bgc();
	y_soi2l = soi2lnd_bgc();
	y_soi2a = soi2atm_bgc();
	y_a2soi = atm2soi_bgc();
	y_soi2soi = soi2soi_bgc();

	for (int il=0; il<MAX_SOI_LAY; il++){
		if (!prvltrfcnque[il].empty()) {
			prvltrfcnque[il].clear();
		};
	}

};

void BgcData::land_beginOfMonth(){
	m_l2a.nep = 0.;
};

void BgcData::land_beginOfYear(){
	y_l2a.nep = 0.;
};

void BgcData::land_endOfMonth(){
	m_l2a.nep = m_a2v.nppall-m_soi2a.rhtot;

	y_l2a.nep +=m_l2a.nep;
};

void BgcData::veg_beginOfYear(){
	for (int i=0; i<NUM_PFT_PART; i++){
		y_vegs.c[i]    = 0.;
		y_vegs.strn[i] = 0.;

		y_vegd.kr[i]   = 0.;

		y_a2v.ingpp[i] = 0.;
		y_a2v.innpp[i] = 0.;
		y_a2v.gpp[i]   = 0.;
		y_a2v.npp[i]   = 0.;
		y_v2a.rg[i]    = 0.;
 		y_v2a.rm[i]    = 0.;

 		y_v2v.nmobil[i]  = 0.;
 		y_v2v.nresorb[i] = 0.;

 		y_v2soi.ltrfalc[i] = 0.;
		y_v2soi.ltrfaln[i] = 0.;

		y_soi2v.snuptake[i] = 0.;
	}

	y_vegs.call    = 0.;
	y_vegs.nall    = 0.;
 	y_vegs.labn    = 0.;
	y_vegs.strnall = 0.;

 	y_vegd.fca     = 0.;
 	y_vegd.fna     = 0.;
 	y_vegd.ftemp   = 0.;
 	y_vegd.gv      = 0.;
 	y_vegd.raq10   = 0.;

	y_a2v.ingppall = 0.;
	y_a2v.innppall = 0.;
	y_a2v.gppall   = 0.;
	y_a2v.nppall   = 0.;
	y_v2a.rgall    = 0.;
	y_v2a.rmall    = 0.;

	y_v2soi.ltrfalcall = 0.;
	y_v2soi.ltrfalnall = 0.;
	y_v2soi.mossdeathc = 0.;
	y_v2soi.mossdeathn = 0.;

	y_v2v.nmobilall  = 0.;
	y_v2v.nresorball = 0.;

 	for (int il=0; il<MAX_SOI_LAY; il++){
 		y_soi2v.nextract[il]   = 0.;
 	}

 	y_soi2v.innuptake = 0.;
 	y_soi2v.lnuptake   = 0.;
 	y_soi2v.snuptakeall= 0.;

};

void BgcData::veg_beginOfMonth(){
	for (int i=0; i<NUM_PFT_PART; i++){
		m_vegs.c[i]    = 0.;
		m_vegs.strn[i] = 0.;

		m_vegd.kr[i]   = 0.;

		m_a2v.ingpp[i] = 0.;
		m_a2v.innpp[i] = 0.;
		m_a2v.gpp[i]   = 0.;
		m_a2v.npp[i]   = 0.;
		m_v2a.rg[i]    = 0.;
 		m_v2a.rm[i]    = 0.;

 		m_v2v.nmobil[i]  = 0.;
 		m_v2v.nresorb[i] = 0.;

 		m_v2soi.ltrfalc[i] = 0.;
		m_v2soi.ltrfaln[i] = 0.;

		m_soi2v.snuptake[i] = 0.;
	}

	m_vegs.call    = 0.;
	m_vegs.nall    = 0.;
 	m_vegs.labn    = 0.;
	m_vegs.strnall = 0.;

 	m_vegd.fca     = 0.;
 	m_vegd.fna     = 0.;
 	m_vegd.ftemp   = 0.;
 	m_vegd.gv      = 0.;
 	m_vegd.raq10   = 0.;

	m_a2v.ingppall = 0.;
	m_a2v.innppall = 0.;
	m_a2v.gppall   = 0.;
	m_a2v.nppall   = 0.;
	m_v2a.rgall    = 0.;
	m_v2a.rmall    = 0.;

	m_v2soi.ltrfalcall = 0.;
	m_v2soi.ltrfalnall = 0.;
	m_v2soi.mossdeathc = 0.;
	m_v2soi.mossdeathn = 0.;

	m_v2v.nmobilall  = 0.;
	m_v2v.nresorball = 0.;

 	for (int il=0; il<MAX_SOI_LAY; il++){
 		m_soi2v.nextract[il]   = 0.;
 	}

 	m_soi2v.innuptake = 0.;
 	m_soi2v.lnuptake   = 0.;
 	m_soi2v.snuptakeall= 0.;
};

void BgcData::veg_endOfDay(const int &dinm){

	// average yearly status variables
 	for (int i=0; i<NUM_PFT_PART; i++){
 		m_vegs.c[i] += d_vegs.c[i]/dinm;
 		m_vegs.strn[i] += d_vegs.strn[i]/dinm;
 	}
 	m_vegs.labn   += d_vegs.labn/dinm;
 	m_vegs.strnall+= d_vegs.strnall/dinm;
 	m_vegs.call   += d_vegs.call/dinm;
 	m_vegs.nall   += d_vegs.nall/dinm;
 	m_vegs.deadc  += d_vegs.deadc/dinm;
 	m_vegs.deadn  += d_vegs.deadn/dinm;

	// average yearly diagnostic variables
 	m_vegd.fca     += d_vegd.fca/MINY;
 	m_vegd.fna     += d_vegd.fna/MINY;
 	m_vegd.ftemp   += d_vegd.ftemp/MINY;
 	for (int i=0; i<NUM_PFT_PART; i++){
 		m_vegd.kr[i] += d_vegd.kr[i]/MINY;
 	}
 	m_vegd.gv      += d_vegd.gv/MINY;
 	m_vegd.raq10   += d_vegd.raq10/MINY;

	// accumulate yearly flux variables
 	for (int i=0; i<NUM_PFT_PART; i++){
 		m_a2v.ingpp[i] += d_a2v.ingpp[i];
 		m_a2v.innpp[i] += d_a2v.innpp[i];
 		m_a2v.gpp[i]   += d_a2v.gpp[i];
 		m_a2v.npp[i]   += d_a2v.npp[i];

 		m_v2a.rg[i] += d_v2a.rg[i];
 		m_v2a.rm[i] += d_v2a.rm[i];

 		m_v2v.nmobil[i] += d_v2v.nmobil[i];
 		m_v2v.nresorb[i]+= d_v2v.nresorb[i];

 		m_v2soi.ltrfalc[i] += d_v2soi.ltrfalc[i];
 		m_v2soi.ltrfaln[i] += d_v2soi.ltrfaln[i];

 		m_soi2v.snuptake[i] += d_soi2v.snuptake[i];
 	}

 	m_a2v.ingppall += d_a2v.ingppall;
	m_a2v.innppall += d_a2v.innppall;
	m_a2v.gppall   += d_a2v.gppall;
	m_a2v.nppall   += d_a2v.nppall;

	m_v2a.rgall += d_v2a.rgall;
	m_v2a.rmall += d_v2a.rmall;

	m_v2soi.ltrfalcall += d_v2soi.ltrfalcall;
	m_v2soi.ltrfalnall += d_v2soi.ltrfalnall;
	m_v2soi.mossdeathc += d_v2soi.mossdeathc;
	m_v2soi.mossdeathn += d_v2soi.mossdeathn;

	m_v2v.nmobilall  += d_v2v.nmobilall;
	m_v2v.nresorball += d_v2v.nresorball;

	for (int il=0; il<MAX_SOI_LAY; il++) {
		m_soi2v.nextract[il]  += d_soi2v.nextract[il];
	}
	m_soi2v.innuptake += d_soi2v.innuptake;
	m_soi2v.lnuptake  += d_soi2v.lnuptake;
	m_soi2v.snuptakeall+= d_soi2v.snuptakeall;

}

void BgcData::veg_endOfMonth(){

	// average yearly status variables
 	for (int i=0; i<NUM_PFT_PART; i++){
 		y_vegs.c[i] += m_vegs.c[i]/MINY;
 		y_vegs.strn[i] += m_vegs.strn[i]/MINY;
 	}
 	y_vegs.labn   += m_vegs.labn/MINY;
 	y_vegs.strnall+= m_vegs.strnall/MINY;
 	y_vegs.call   += m_vegs.call/MINY;
 	y_vegs.nall   += m_vegs.nall/MINY;
 	y_vegs.deadc  += m_vegs.deadc/MINY;
 	y_vegs.deadn  += m_vegs.deadn/MINY;

	// average yearly diagnostic variables
 	y_vegd.fca     += m_vegd.fca/MINY;
 	y_vegd.fna     += m_vegd.fna/MINY;
 	y_vegd.ftemp   += m_vegd.ftemp/MINY;
 	for (int i=0; i<NUM_PFT_PART; i++){
 		y_vegd.kr[i] += m_vegd.kr[i]/MINY;
 	}
 	y_vegd.gv      += m_vegd.gv/MINY;
 	y_vegd.raq10   += m_vegd.raq10/MINY;

	// accumulate yearly flux variables
 	for (int i=0; i<NUM_PFT_PART; i++){
 		y_a2v.ingpp[i] += m_a2v.ingpp[i];
 		y_a2v.innpp[i] += m_a2v.innpp[i];
 		y_a2v.gpp[i]   += m_a2v.gpp[i];
 		y_a2v.npp[i]   += m_a2v.npp[i];

 		y_v2a.rg[i] += m_v2a.rg[i];
 		y_v2a.rm[i] += m_v2a.rm[i];

 		y_v2v.nmobil[i] += m_v2v.nmobil[i];
 		y_v2v.nresorb[i]+= m_v2v.nresorb[i];

 		y_v2soi.ltrfalc[i] += m_v2soi.ltrfalc[i];
 		y_v2soi.ltrfaln[i] += m_v2soi.ltrfaln[i];

 		y_soi2v.snuptake[i] += m_soi2v.snuptake[i];
 	}

 	y_a2v.ingppall += m_a2v.ingppall;
	y_a2v.innppall += m_a2v.innppall;
	y_a2v.gppall   += m_a2v.gppall;
	y_a2v.nppall   += m_a2v.nppall;

	y_v2a.rgall += m_v2a.rgall;
	y_v2a.rmall += m_v2a.rmall;

	y_v2soi.ltrfalcall += m_v2soi.ltrfalcall;
	y_v2soi.ltrfalnall += m_v2soi.ltrfalnall;
	y_v2soi.mossdeathc += m_v2soi.mossdeathc;
	y_v2soi.mossdeathn += m_v2soi.mossdeathn;

	y_v2v.nmobilall  += m_v2v.nmobilall;
	y_v2v.nresorball += m_v2v.nresorball;

	for (int il=0; il<MAX_SOI_LAY; il++) {
		y_soi2v.nextract[il]  += m_soi2v.nextract[il];
	}
	y_soi2v.innuptake += m_soi2v.innuptake;
	y_soi2v.lnuptake  += m_soi2v.lnuptake;
	y_soi2v.snuptakeall+= m_soi2v.snuptakeall;

}

// initializing monthly accumulating soil variables
void BgcData::soil_beginOfMonth(){

 	//initialize monthly mean/accumulative variables
 	for (int il =0; il<MAX_SOI_LAY; il++){
 	   	//mean for states
 		m_soid.tsomc[il]  = 0.;

 	    m_soid.rhmoist[il]  = 0.;
 	   	m_soid.rhq10[il]    = 0.;
 	   	m_soid.ltrfcn[il]   = 0.;

 	    m_soid.knmoist[il]  = 0.;

 	   	m_sois.rawc[il]   = 0.;
   		m_sois.soma[il]   = 0.;
   		m_sois.sompr[il]  = 0.;
   		m_sois.somcr[il]  = 0.;
   		m_sois.orgn[il]   = 0.;
   		m_sois.avln[il]   = 0.;
   		m_sois.ch4[il]   = 0.;

   		// accumulative for fluxes
   		m_soi2a.rhrawc[il] = 0.;
   		m_soi2a.rhsoma[il] = 0.;
   		m_soi2a.rhsompr[il]= 0.;
   		m_soi2a.rhsomcr[il]= 0.;

   	 	m_soi2soi.netnmin[il]= 0.;
   	 	m_soi2soi.nimmob[il] = 0.;

  		// CH4 production/oxidation
  		m_soi2a.Prod_m[il] = 0.;
  		m_soi2a.Oxid_m[il] = 0.;

 	}

 	//mean for states
   	m_sois.dmossc   = 0.;
   	m_sois.dmossn   = 0.;

   	m_sois.wdebrisc = 0.;
   	m_sois.wdebrisn = 0.;

   	m_soid.shlwc   = 0.;
   	m_soid.deepc   = 0.;
   	m_soid.mineac  = 0.;
   	m_soid.minebc  = 0.;
   	m_soid.minecc  = 0.;

 	m_soid.rawcsum   = 0.;
 	m_soid.somasum   = 0.;
 	m_soid.somprsum  = 0.;
 	m_soid.somcrsum  = 0.;

 	m_soid.avlnsum   = 0.;
 	m_soid.orgnsum   = 0.;

 	// accumulative for fluxes
 	m_soi2a.rhwdeb    = 0.;
	m_soi2a.rhmossc   = 0.;
 	m_soi2a.rhrawcsum = 0.;
 	m_soi2a.rhsomasum = 0.;
 	m_soi2a.rhsomprsum= 0.;
 	m_soi2a.rhsomcrsum= 0.;
 	m_soi2a.rhtot     = 0.;
	m_soi2soi.netnminsum= 0.;
	m_soi2soi.nimmobsum = 0.;

	m_a2soi.orgninput = 0.;
	m_soi2l.orgnlost  = 0.;
	m_a2soi.avlninput = 0.;
	m_soi2l.avlnlost  = 0.;

    // CH4 emissions
	m_soi2a.totProd_m    = 0.;
	m_soi2a.totOxid_m    = 0.;
	m_soi2a.totFlux2A_m  = 0.;
	m_soi2a.totPlant_m   = 0.;
	m_soi2a.totEbul_m    = 0.;
	m_soi2a.totCH4Flux_m = 0.;

};

void BgcData::soil_beginOfYear(){

 	y_sois.wdebrisc= 0.;
 	y_sois.wdebrisn= 0.;
 	y_sois.dmossc  = 0.;
 	y_sois.dmossn  = 0.;

 	y_soid.shlwc   = 0.;
 	y_soid.deepc   = 0.;
 	y_soid.mineac  = 0.;  // top mineral SOMC
 	y_soid.minebc  = 0.;  // middle mineral SOMC
 	y_soid.minecc  = 0.;  // middle mineral SOMC
 	y_soid.rawcsum = 0.;
 	y_soid.somasum = 0.;
 	y_soid.somprsum= 0.;
 	y_soid.somcrsum= 0.;
 	y_soid.orgnsum = 0.;
 	y_soid.avlnsum = 0.;

 	y_soi2a.rhtot     =0.;
 	y_soi2a.rhwdeb    =0.;
	y_soi2a.rhrawcsum =0.;
	y_soi2a.rhsomasum =0.;
	y_soi2a.rhsomprsum=0.;
	y_soi2a.rhsomcrsum=0.;

	y_soi2soi.netnminsum = 0.;
	y_soi2soi.nimmobsum  = 0.;

	for (int il =0; il<MAX_SOI_LAY; il++){
 		//
 		y_sois.rawc[il]=0.;
 		y_sois.soma[il]=0.;
 		y_sois.sompr[il]=0.;
 		y_sois.somcr[il]=0.;

 		y_sois.orgn[il]=0.;
 		y_sois.avln[il]  = 0.;

 	 	y_soid.tsomc[il] = 0.;

 		y_soid.ltrfcn[il] = 0.;
   		y_soid.knmoist[il]= 0.;
   		y_soid.rhmoist[il]= 0.;
   		y_soid.rhq10[il]  = 0.;

   		//
   		y_soi2a.rhrawc[il] = 0.;
   		y_soi2a.rhsoma[il] = 0.;
   		y_soi2a.rhsompr[il]= 0.;
   		y_soi2a.rhsomcr[il]= 0.;

   		//
   		y_soi2soi.netnmin[il] = 0.;
   		y_soi2soi.nimmob[il]  = 0.;

  		y_soi2a.Prod_m[il] = 0.;
  		y_soi2a.Oxid_m[il] = 0.;

 	}
 
	//
	y_a2soi.orgninput = 0.0;
	y_a2soi.avlninput = 0.0;
    y_soi2l.orgnlost  = 0.0;
    y_soi2l.avlnlost = 0.0;

    //
	y_soi2a.totProd_m    = 0.;
	y_soi2a.totOxid_m    = 0.;
	y_soi2a.totFlux2A_m  = 0.;
	y_soi2a.totPlant_m   = 0.;
	y_soi2a.totEbul_m    = 0.;
	y_soi2a.totCH4Flux_m = 0.;


};

void BgcData::soil_endOfDay(const int & dinm){

	// status variable (diagnostics)
 	d_soid.shlwc   = 0.;
 	d_soid.deepc   = 0.;
 	d_soid.mineac  = 0.;
 	d_soid.minebc  = 0.;
 	d_soid.minecc  = 0.;

 	d_soid.orgnsum = 0.;
 	d_soid.avlnsum = 0.;

 	d_soid.rawcsum = 0.;
 	d_soid.somasum = 0.;
 	d_soid.somprsum= 0.;
 	d_soid.somcrsum= 0.;

 	int mlind = 0;
 	for (int il =0; il<MAX_SOI_LAY; il++){
 		d_soid.tsomc[il]= d_sois.rawc[il]+d_sois.soma[il]+d_sois.sompr[il]+d_sois.somcr[il];
 		if(cd->d_soil.type[il]==1){
   	 		d_soid.shlwc += d_soid.tsomc[il];
   		} else if(cd->d_soil.type[il]==2){
   	 		d_soid.deepc += d_soid.tsomc[il];
   		} else if(cd->d_soil.type[il]==3){
   			if (mlind>=0 && mlind<=MINEZONE[0])
   				d_soid.mineac += d_soid.tsomc[il];

   			if (mlind>MINEZONE[0] && mlind<=MINEZONE[1])
   				d_soid.minebc += d_soid.tsomc[il];

   			if (mlind>MINEZONE[1] && mlind<=MINEZONE[2])
   				d_soid.minecc += d_soid.tsomc[il];

   			mlind ++;
   		}

   		d_soid.rawcsum += d_sois.rawc[il];
   		d_soid.somasum += d_sois.soma[il];
   		d_soid.somprsum+= d_sois.sompr[il];
   		d_soid.somcrsum+= d_sois.somcr[il];

   		d_soid.orgnsum += d_sois.orgn[il];
   		d_soid.avlnsum += d_sois.avln[il];

 	}

 	//monthly mean variables
 	for (int il =0; il<MAX_SOI_LAY; il++){
 	   	m_soid.tsomc[il]  += d_soid.tsomc[il]/dinm;

 	    m_soid.rhmoist[il]+= d_soid.rhmoist[il]/dinm;
 	   	m_soid.rhq10[il]  += d_soid.rhq10[il]/dinm;
 	   	m_soid.ltrfcn[il] += d_soid.ltrfcn[il]/dinm;

 	    m_soid.knmoist[il]+= d_soid.knmoist[il]/dinm;

 	   	m_sois.rawc[il] += d_sois.rawc[il]/dinm;
   		m_sois.soma[il] += d_sois.soma[il]/dinm;
   		m_sois.sompr[il]+= d_sois.sompr[il]/dinm;
   		m_sois.somcr[il]+= d_sois.somcr[il]/dinm;
   		m_sois.orgn[il] += d_sois.orgn[il]/dinm;
   		m_sois.avln[il] += d_sois.avln[il]/dinm;
   		m_sois.ch4[il] += d_sois.ch4[il]/dinm;

 	}
   	m_sois.dmossc += d_sois.dmossc/dinm;
   	m_sois.dmossn += d_sois.dmossn/dinm;

   	m_sois.wdebrisc += d_sois.wdebrisc/dinm;
   	m_sois.wdebrisn += d_sois.wdebrisn/dinm;

   	m_soid.shlwc += d_soid.shlwc/dinm;
   	m_soid.deepc += d_soid.deepc/dinm;
   	m_soid.mineac += d_soid.mineac/dinm;
   	m_soid.minebc += d_soid.minebc/dinm;
   	m_soid.minecc += d_soid.minecc/dinm;

 	m_soid.rawcsum += d_soid.rawcsum/dinm;
 	m_soid.somasum += d_soid.somasum/dinm;
 	m_soid.somprsum+= d_soid.somprsum/dinm;
 	m_soid.somcrsum+= d_soid.somcrsum/dinm;

 	m_soid.avlnsum += d_soid.avlnsum/dinm;
 	m_soid.orgnsum += d_soid.orgnsum/dinm;

   	// fluxes
   	m_soi2a.rhrawcsum = 0.;
 	m_soi2a.rhsomasum = 0.;
 	m_soi2a.rhsomprsum= 0.;
 	m_soi2a.rhsomcrsum= 0.;
 	m_soi2soi.netnminsum= 0.;
 	m_soi2soi.nimmobsum = 0.;

 	for (int il =0; il<MAX_SOI_LAY; il++){

   		m_soi2a.rhrawcsum += d_soi2a.rhrawc[il];
   		m_soi2a.rhsomasum += d_soi2a.rhsoma[il];
   		m_soi2a.rhsomprsum+= d_soi2a.rhsompr[il];
   		m_soi2a.rhsomcrsum+= d_soi2a.rhsomcr[il];

   	 	m_soi2soi.netnminsum+= d_soi2soi.netnmin[il];
   	 	m_soi2soi.nimmobsum += d_soi2soi.nimmob[il];

 	}
 	m_soi2a.rhtot = m_soi2a.rhrawcsum + m_soi2a.rhsomasum
 			    +m_soi2a.rhsomprsum + m_soi2a.rhsomcrsum
 			    +m_soi2a.rhwdeb + m_soi2a.rhmossc;

 	//cumulative monthly
 	m_soi2a.rhwdeb    += d_soi2a.rhwdeb;
	m_soi2a.rhmossc   += d_soi2a.rhmossc;
 	m_soi2a.rhrawcsum += d_soi2a.rhrawcsum;
 	m_soi2a.rhsomasum += d_soi2a.rhsomasum;
 	m_soi2a.rhsomprsum+= d_soi2a.rhsomprsum;
 	m_soi2a.rhsomcrsum+= d_soi2a.rhsomcrsum;
 	m_soi2a.rhtot     += d_soi2a.rhtot;
 	for (int il =0; il<MAX_SOI_LAY; il++){
   		m_soi2a.rhrawc[il] += d_soi2a.rhrawc[il];
   		m_soi2a.rhsoma[il] += d_soi2a.rhsoma[il];
   		m_soi2a.rhsompr[il]+= d_soi2a.rhsompr[il];
   		m_soi2a.rhsomcr[il]+= d_soi2a.rhsomcr[il];

   	 	m_soi2soi.netnmin[il]+= d_soi2soi.netnmin[il];
   	 	m_soi2soi.nimmob[il] += d_soi2soi.nimmob[il];

  		m_soi2a.Prod_m[il] += d_soi2a.Prod_m[il];
  		m_soi2a.Oxid_m[il] += d_soi2a.Oxid_m[il];

 	}
	m_soi2soi.netnminsum+= d_soi2soi.netnminsum;
	m_soi2soi.nimmobsum += d_soi2soi.nimmobsum;

    // connection to open-N cycle
	m_a2soi.orgninput += d_a2soi.orgninput;
	m_soi2l.orgnlost += d_soi2l.orgnlost;
	m_a2soi.avlninput += d_a2soi.avlninput;
	m_soi2l.avlnlost += d_soi2l.avlnlost;

	m_soi2a.totProd_m    += d_soi2a.totProd_m;
	m_soi2a.totOxid_m    += d_soi2a.totOxid_m;
	m_soi2a.totFlux2A_m  += d_soi2a.totFlux2A_m;
	m_soi2a.totPlant_m   += d_soi2a.totPlant_m;
	m_soi2a.totEbul_m    += d_soi2a.totEbul_m;
	m_soi2a.totCH4Flux_m += d_soi2a.totCH4Flux_m;

};

void BgcData::soil_endOfMonth(){
 
	// status variable (diagnostics)
 	m_soid.shlwc   = 0.;
 	m_soid.deepc   = 0.;
 	m_soid.mineac  = 0.;
 	m_soid.minebc  = 0.;
 	m_soid.minecc  = 0.;

 	m_soid.orgnsum = 0.;
 	m_soid.avlnsum = 0.;

 	m_soid.rawcsum = 0.;
 	m_soid.somasum = 0.;
 	m_soid.somprsum= 0.;
 	m_soid.somcrsum= 0.;

 	int mlind = 0;
 	for (int il =0; il<MAX_SOI_LAY; il++){
 		m_soid.tsomc[il]= m_sois.rawc[il]+m_sois.soma[il]+m_sois.sompr[il]+m_sois.somcr[il];
 		if(cd->m_soil.type[il]==1){
   	 		m_soid.shlwc += m_soid.tsomc[il];
   		} else if(cd->m_soil.type[il]==2){
   	 		m_soid.deepc += m_soid.tsomc[il];
   		} else if(cd->m_soil.type[il]==3){
   			if (mlind>=0 && mlind<=MINEZONE[0])
   				m_soid.mineac += m_soid.tsomc[il];

   			if (mlind>MINEZONE[0] && mlind<=MINEZONE[1])
   				m_soid.minebc += m_soid.tsomc[il];

   			if (mlind>MINEZONE[1] && mlind<=MINEZONE[2])
   				m_soid.minecc += m_soid.tsomc[il];

   			mlind ++;
   		}

   		m_soid.rawcsum += m_sois.rawc[il];
   		m_soid.somasum += m_sois.soma[il];
   		m_soid.somprsum+= m_sois.sompr[il];
   		m_soid.somcrsum+= m_sois.somcr[il];

   		m_soid.orgnsum += m_sois.orgn[il];
   		m_soid.avlnsum += m_sois.avln[il];
 	}

 	// previous monthly accumulating variables
 	for (int il =0; il<MAX_SOI_LAY; il++){
 		double mltrfcn = m_soid.ltrfcn[il];
 		prvltrfcnque[il].push_front(mltrfcn);
 		if (prvltrfcnque[il].size()>12) {
 			prvltrfcnque[il].pop_back();
 		}

 	}

 	//yearly mean variables
 	for (int il =0; il<MAX_SOI_LAY; il++){
 	   	y_soid.tsomc[il]  += m_soid.tsomc[il]/MINY;

 	    y_soid.rhmoist[il]+= m_soid.rhmoist[il]/MINY;
 	   	y_soid.rhq10[il]  += m_soid.rhq10[il]/MINY;
 	   	y_soid.ltrfcn[il] += m_soid.ltrfcn[il]/MINY;

 	    y_soid.knmoist[il]+= m_soid.knmoist[il]/MINY;

 	   	y_sois.rawc[il] += m_sois.rawc[il]/MINY;
   		y_sois.soma[il] += m_sois.soma[il]/MINY;
   		y_sois.sompr[il]+= m_sois.sompr[il]/MINY;
   		y_sois.somcr[il]+= m_sois.somcr[il]/MINY;
   		y_sois.orgn[il] += m_sois.orgn[il]/MINY;
   		y_sois.avln[il] += m_sois.avln[il]/MINY;
 	}
   	y_sois.dmossc += m_sois.dmossc/MINY;
   	y_sois.dmossn += m_sois.dmossn/MINY;

   	y_sois.wdebrisc += m_sois.wdebrisc/MINY;
   	y_sois.wdebrisn += m_sois.wdebrisn/MINY;

   	y_soid.shlwc += m_soid.shlwc/MINY;
   	y_soid.deepc += m_soid.deepc/MINY;
   	y_soid.mineac += m_soid.mineac/MINY;
   	y_soid.minebc += m_soid.minebc/MINY;
   	y_soid.minecc += m_soid.minecc/MINY;
   
 	y_soid.rawcsum += m_soid.rawcsum/MINY;
 	y_soid.somasum += m_soid.somasum/MINY;
 	y_soid.somprsum+= m_soid.somprsum/MINY;
 	y_soid.somcrsum+= m_soid.somcrsum/MINY;

 	y_soid.avlnsum += m_soid.avlnsum/MINY;
 	y_soid.orgnsum += m_soid.orgnsum/MINY;

   	// summarized variables
   	m_soi2a.rhrawcsum = 0.;
 	m_soi2a.rhsomasum = 0.;
 	m_soi2a.rhsomprsum= 0.;
 	m_soi2a.rhsomcrsum= 0.;
 	m_soi2soi.netnminsum= 0.;
 	m_soi2soi.nimmobsum = 0.;

 	for (int il =0; il<MAX_SOI_LAY; il++){

   		m_soi2a.rhrawcsum += m_soi2a.rhrawc[il];
   		m_soi2a.rhsomasum += m_soi2a.rhsoma[il];
   		m_soi2a.rhsomprsum+= m_soi2a.rhsompr[il];
   		m_soi2a.rhsomcrsum+= m_soi2a.rhsomcr[il];

   	 	m_soi2soi.netnminsum+= m_soi2soi.netnmin[il];
   	 	m_soi2soi.nimmobsum += m_soi2soi.nimmob[il];

 	}
 	m_soi2a.rhtot = m_soi2a.rhrawcsum + m_soi2a.rhsomasum
 			    +m_soi2a.rhsomprsum + m_soi2a.rhsomcrsum
 			    +m_soi2a.rhwdeb + m_soi2a.rhmossc;

 	//cumulative annually
 	y_soi2a.rhwdeb    += m_soi2a.rhwdeb;
	y_soi2a.rhmossc   += m_soi2a.rhmossc;
 	y_soi2a.rhrawcsum += m_soi2a.rhrawcsum;
 	y_soi2a.rhsomasum += m_soi2a.rhsomasum;
 	y_soi2a.rhsomprsum+= m_soi2a.rhsomprsum;
 	y_soi2a.rhsomcrsum+= m_soi2a.rhsomcrsum;
 	y_soi2a.rhtot     += m_soi2a.rhtot;
 	for (int il =0; il<MAX_SOI_LAY; il++){
   		y_soi2a.rhrawc[il] += m_soi2a.rhrawc[il];
   		y_soi2a.rhsoma[il] += m_soi2a.rhsoma[il];
   		y_soi2a.rhsompr[il]+= m_soi2a.rhsompr[il];
   		y_soi2a.rhsomcr[il]+= m_soi2a.rhsomcr[il];

   	 	y_soi2soi.netnmin[il]+= m_soi2soi.netnmin[il];
   	 	y_soi2soi.nimmob[il] += m_soi2soi.nimmob[il];

  		y_soi2a.Prod_m[il] += m_soi2a.Prod_m[il];
  		y_soi2a.Oxid_m[il] += m_soi2a.Oxid_m[il];

 	}
	y_soi2soi.netnminsum+= m_soi2soi.netnminsum;
	y_soi2soi.nimmobsum += m_soi2soi.nimmobsum;

    // connection to open-N cycle
	y_a2soi.orgninput += m_a2soi.orgninput;
	y_soi2l.orgnlost += m_soi2l.orgnlost;
	y_a2soi.avlninput += m_a2soi.avlninput;
	y_soi2l.avlnlost += m_soi2l.avlnlost;

	// CH4
	y_soi2a.totProd_m    += m_soi2a.totProd_m;
	y_soi2a.totOxid_m    += m_soi2a.totOxid_m;
	y_soi2a.totFlux2A_m  += m_soi2a.totFlux2A_m;
	y_soi2a.totPlant_m   += m_soi2a.totPlant_m;
	y_soi2a.totEbul_m    += m_soi2a.totEbul_m;
	y_soi2a.totCH4Flux_m += m_soi2a.totCH4Flux_m;

};
