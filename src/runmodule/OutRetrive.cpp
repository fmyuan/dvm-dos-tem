/*
 * This is for
 */

#include "OutRetrive.h"

OutRetrive::OutRetrive(){

};

OutRetrive::~OutRetrive(){

};

void OutRetrive::setDimensionData(CohortData *cdp){
	cd = cdp;
};

void OutRetrive::setProcessData(const int & ip, EnvData *edp, BgcData *bdp){
	if (ip>=0) {
		ed[ip] = edp;
		bd[ip] = bdp;
	} else {
		edall = edp;
		bdall = bdp;
	}
};

void OutRetrive::setFireData(FirData *fdp){
	fd = fdp;
};

void OutRetrive::setRestartOutData(RestartData *resodp){
	resod = resodp;
};

void OutRetrive::setRegnOutData(OutDataRegn *regnodp){
    regnod = regnodp;
};

void OutRetrive::updateRegnOutputBuffer(const int & im, const int &idoy){

	if (im==0 && idoy==1) {    //'doy' is 1-based index
		regnod->chtid = cd->chtid;
		regnod->year  = cd->year;
		regnod->yrsdist =cd->yrsdist;
	}

	bool EOY = false;  // end of year?
	if (im==11 && idoy==DINY) EOY = true;
	bool EOM = false;  // end of month?
	if (idoy==(DOYINDFST[im]+DINM[im])) EOM = true;

	regnod->month.push_back(im);
	regnod->doy.push_back(idoy);

	//
	for (int ip=0; ip<NUM_PFT; ip++) {

		if (EOY && regnod->outvarlist[I_growstart]==1) {   // yearly
			regnod->growstart[ip].push_back(ed[ip]->y_soid.rtdpgrowstart);
		} else if (EOM &&regnod->outvarlist[I_growstart]==2) {      // monthly
			regnod->growstart[ip].push_back(ed[ip]->m_soid.rtdpgrowstart);
		}else if (regnod->outvarlist[I_growstart]==3) {      // daily
			regnod->growstart[ip].push_back(ed[ip]->d_soid.rtdpgrowstart);
		}

		if (EOY && regnod->outvarlist[I_growend]==1) {
			regnod->growend[ip].push_back(ed[ip]->y_soid.rtdpgrowend);
		} else if (EOM &&regnod->outvarlist[I_growend]==2) {
			regnod->growend[ip].push_back(ed[ip]->m_soid.rtdpgrowend);
		} else if (regnod->outvarlist[I_growend]==3) {
			regnod->growend[ip].push_back(ed[ip]->d_soid.rtdpgrowend);
		}

		if (EOY && regnod->outvarlist[I_vegcov]==1) {
			regnod->vegcov[ip].push_back(cd->y_veg.vegcov[ip]);
		} else if (EOM &&regnod->outvarlist[I_vegcov]==2) {
			regnod->vegcov[ip].push_back(cd->m_veg.vegcov[ip]);
		} else if (regnod->outvarlist[I_vegcov]==3) {
			regnod->vegcov[ip].push_back(cd->d_veg.vegcov[ip]);
		}

		if (EOY && regnod->outvarlist[I_vegage]==1) {
			regnod->vegage[ip].push_back(cd->y_veg.vegage[ip]);
		} else if (EOM && regnod->outvarlist[I_vegage]==2) {
			regnod->vegage[ip].push_back(cd->m_veg.vegage[ip]);
		} else if (regnod->outvarlist[I_vegage]==3) {
			regnod->vegage[ip].push_back(cd->d_veg.vegage[ip]);
		}

		if (EOY && regnod->outvarlist[I_lai]==1) {
			regnod->lai[ip].push_back(cd->y_veg.lai[ip]);
		} else if (EOM && regnod->outvarlist[I_lai]==2) {
			regnod->lai[ip].push_back(cd->m_veg.lai[ip]);
		} else if (regnod->outvarlist[I_lai]==3) {
			regnod->lai[ip].push_back(cd->d_veg.lai[ip]);
		}

		//
		if (EOY && regnod->outvarlist[I_vegc]==1){
			regnod->vegc[ip].push_back(bd[ip]->y_vegs.call);
		} else if (EOM && regnod->outvarlist[I_vegc]==2) {
			regnod->vegc[ip].push_back(bd[ip]->m_vegs.call);
		} else if (regnod->outvarlist[I_vegc]==3) {
			regnod->vegc[ip].push_back(bd[ip]->d_vegs.call);
		}

		if (EOY && regnod->outvarlist[I_leafc]==1){
			regnod->leafc[ip].push_back(bd[ip]->y_vegs.c[I_leaf]);
		} else if (EOM && regnod->outvarlist[I_leafc]==2) {
			regnod->leafc[ip].push_back(bd[ip]->m_vegs.c[I_leaf]);
		} else if (regnod->outvarlist[I_leafc]==3) {
			regnod->leafc[ip].push_back(bd[ip]->d_vegs.c[I_leaf]);
		}

		if (EOY && regnod->outvarlist[I_stemc]==1){
			regnod->stemc[ip].push_back(bd[ip]->y_vegs.c[I_stem]);
		} else if (EOM && regnod->outvarlist[I_stemc]==2) {
			regnod->stemc[ip].push_back(bd[ip]->m_vegs.c[I_stem]);
		} else if (regnod->outvarlist[I_stemc]==3) {
			regnod->stemc[ip].push_back(bd[ip]->d_vegs.c[I_stem]);
		}

		if (EOY && regnod->outvarlist[I_rootc]==1){
			regnod->rootc[ip].push_back(bd[ip]->y_vegs.c[I_root]);
		} else if (EOM && regnod->outvarlist[I_rootc]==2) {
			regnod->rootc[ip].push_back(bd[ip]->m_vegs.c[I_root]);
		} else if (regnod->outvarlist[I_rootc]==3) {
			regnod->rootc[ip].push_back(bd[ip]->d_vegs.c[I_root]);
		}

		if (EOY && regnod->outvarlist[I_vegn]==1){
			regnod->vegn[ip].push_back(bd[ip]->y_vegs.nall);
		} else if (EOM && regnod->outvarlist[I_vegn]==2) {
			regnod->vegn[ip].push_back(bd[ip]->m_vegs.nall);
		} else if (regnod->outvarlist[I_vegn]==3) {
			regnod->vegn[ip].push_back(bd[ip]->d_vegs.nall);
		}

		if (EOY && regnod->outvarlist[I_labn]==1){
			regnod->labn[ip].push_back(bd[ip]->y_vegs.labn);
		} else if (EOM && regnod->outvarlist[I_labn]==2) {
			regnod->labn[ip].push_back(bd[ip]->m_vegs.labn);
		} else if (regnod->outvarlist[I_labn]==3) {
			regnod->labn[ip].push_back(bd[ip]->d_vegs.labn);
		}

		if (EOY && regnod->outvarlist[I_leafn]==1){
			regnod->leafn[ip].push_back(bd[ip]->y_vegs.strn[I_leaf]);
		} else if (EOM && regnod->outvarlist[I_leafn]==2) {
			regnod->leafn[ip].push_back(bd[ip]->m_vegs.strn[I_leaf]);
		} else if (regnod->outvarlist[I_leafn]==3) {
			regnod->leafn[ip].push_back(bd[ip]->d_vegs.strn[I_leaf]);
		}

		if (EOY && regnod->outvarlist[I_stemn]==1){
			regnod->stemn[ip].push_back(bd[ip]->y_vegs.strn[I_stem]);
		} else if (EOM && regnod->outvarlist[I_stemn]==2) {
			regnod->stemn[ip].push_back(bd[ip]->m_vegs.strn[I_stem]);
		} else if (regnod->outvarlist[I_stemn]==3) {
			regnod->stemn[ip].push_back(bd[ip]->d_vegs.strn[I_stem]);
		}

		if (EOY && regnod->outvarlist[I_rootn]==1){
			regnod->rootn[ip].push_back(bd[ip]->y_vegs.strn[I_root]);
		} else if (EOM && regnod->outvarlist[I_rootn]==2) {
			regnod->rootn[ip].push_back(bd[ip]->m_vegs.strn[I_root]);
		} else if (regnod->outvarlist[I_rootn]==3) {
			regnod->rootn[ip].push_back(bd[ip]->d_vegs.strn[I_root]);
		}

		if (EOY && regnod->outvarlist[I_gpp]==1){
			regnod->gpp[ip].push_back(bd[ip]->y_a2v.gppall);
		} else if (EOM && regnod->outvarlist[I_gpp]==2) {
			regnod->gpp[ip].push_back(bd[ip]->m_a2v.gppall);
		} else if (regnod->outvarlist[I_gpp]==3) {
			regnod->gpp[ip].push_back(bd[ip]->d_a2v.gppall);
		}

		if (EOY && regnod->outvarlist[I_npp]==1){
			regnod->npp[ip].push_back(bd[ip]->y_a2v.nppall);
		} else if (EOM && regnod->outvarlist[I_npp]==2) {
			regnod->npp[ip].push_back(bd[ip]->m_a2v.nppall);
		} else if (regnod->outvarlist[I_npp]==3) {
			regnod->npp[ip].push_back(bd[ip]->d_a2v.nppall);
		}

		if (EOY && regnod->outvarlist[I_ltrfalc]==1){
			regnod->ltrfalc[ip].push_back(bd[ip]->y_v2soi.ltrfalcall);
		} else if (EOM && regnod->outvarlist[I_ltrfalc]==2) {
			regnod->ltrfalc[ip].push_back(bd[ip]->m_v2soi.ltrfalcall);
		} else if (regnod->outvarlist[I_ltrfalc]==3) {
			regnod->ltrfalc[ip].push_back(bd[ip]->d_v2soi.ltrfalcall);
		}

		if (EOY && regnod->outvarlist[I_ltrfaln]==1){
			regnod->ltrfaln[ip].push_back(bd[ip]->y_v2soi.ltrfalnall);
		} else if (EOM && regnod->outvarlist[I_ltrfaln]==2) {
			regnod->ltrfaln[ip].push_back(bd[ip]->m_v2soi.ltrfalnall);
		} else if (regnod->outvarlist[I_ltrfaln]==3) {
			regnod->ltrfaln[ip].push_back(bd[ip]->d_v2soi.ltrfalnall);
		}

		if (EOY && regnod->outvarlist[I_nuptake]==1){
			regnod->nuptake[ip].push_back(bd[ip]->y_soi2v.lnuptake
					                     +bd[ip]->y_soi2v.snuptakeall);
		} else if (EOM && regnod->outvarlist[I_nuptake]==2) {
			regnod->nuptake[ip].push_back(bd[ip]->m_soi2v.lnuptake
					                     +bd[ip]->m_soi2v.snuptakeall);
		} else if (regnod->outvarlist[I_nuptake]==3) {
			regnod->nuptake[ip].push_back(bd[ip]->d_soi2v.lnuptake
					                     +bd[ip]->d_soi2v.snuptakeall);
		}
	}

	// ground-soil portion
	if (EOY && regnod->outvarlist[I_rh]==1){
		regnod->rh.push_back(bdall->y_soi2a.rhrawcsum             //note: 'bd' soil portion MUST BE exactly SAME for all PFTs
				       +bdall->y_soi2a.rhsomasum
				       +bdall->y_soi2a.rhsomprsum
				       +bdall->y_soi2a.rhsomcrsum);
	} else if (EOM && regnod->outvarlist[I_rh]==2) {
		regnod->rh.push_back(bdall->m_soi2a.rhrawcsum
			                +bdall->m_soi2a.rhsomasum
			                +bdall->m_soi2a.rhsomprsum
			                +bdall->m_soi2a.rhsomcrsum);
	} else if (regnod->outvarlist[I_rh]==3) {
		regnod->rh.push_back(bdall->d_soi2a.rhrawcsum
			                +bdall->d_soi2a.rhsomasum
			                +bdall->d_soi2a.rhsomprsum
			                +bdall->d_soi2a.rhsomcrsum);
	}

	if (EOY && regnod->outvarlist[I_permafrost]==1) {
		regnod->permafrost.push_back(edall->y_soid.permafrost);
	} else if (EOM && regnod->outvarlist[I_permafrost]==2) {
		regnod->permafrost.push_back(edall->m_soid.permafrost);
	} else if (regnod->outvarlist[I_permafrost]==3) {
		regnod->permafrost.push_back(edall->d_soid.permafrost);
	}

	if (EOY && regnod->outvarlist[I_mossdz]==1){
		regnod->mossdz.push_back(cd->y_soil.mossthick);
	} else if (EOM && regnod->outvarlist[I_mossdz]==2) {
		regnod->mossdz.push_back(cd->m_soil.mossthick);
	} else if (regnod->outvarlist[I_mossdz]==3) {
		regnod->mossdz.push_back(cd->d_soil.mossthick);
	}

	if (EOY && regnod->outvarlist[I_oshlwdz]==1){
		regnod->oshlwdz.push_back(cd->y_soil.shlwthick);
	} else if (EOM && regnod->outvarlist[I_oshlwdz]==2) {
		regnod->oshlwdz.push_back(cd->m_soil.shlwthick);
	} else if (regnod->outvarlist[I_oshlwdz]==3) {
		regnod->oshlwdz.push_back(cd->d_soil.shlwthick);
	}

	if (EOY && regnod->outvarlist[I_odeepdz]==1){
		regnod->odeepdz.push_back(cd->y_soil.deepthick);
	} else if (EOM && regnod->outvarlist[I_odeepdz]==2) {
		regnod->odeepdz.push_back(cd->m_soil.deepthick);
	} else if (regnod->outvarlist[I_odeepdz]==3) {
		regnod->odeepdz.push_back(cd->d_soil.deepthick);
	}

	///////////////////
	if (EOY && regnod->outvarlist[I_mineadz]==1){
		regnod->mineadz.push_back(cd->y_soil.mineathick);
	} else if (EOM && regnod->outvarlist[I_mineadz]==2) {
		regnod->mineadz.push_back(cd->m_soil.mineathick);
	} else if (regnod->outvarlist[I_mineadz]==3) {
		regnod->mineadz.push_back(cd->d_soil.mineathick);
	}

	if (EOY && regnod->outvarlist[I_minebdz]==1){
		regnod->minebdz.push_back(cd->y_soil.minebthick);
	} else if (EOM && regnod->outvarlist[I_minebdz]==2) {
		regnod->minebdz.push_back(cd->m_soil.minebthick);
	} else if (regnod->outvarlist[I_minebdz]==3) {
		regnod->minebdz.push_back(cd->d_soil.minebthick);
	}

	if (EOY && regnod->outvarlist[I_minecdz]==1){
		regnod->minecdz.push_back(cd->y_soil.minecthick);
	} else if (EOM && regnod->outvarlist[I_minecdz]==2) {
		regnod->minecdz.push_back(cd->m_soil.minecthick);
	} else if (regnod->outvarlist[I_minecdz]==3) {
		regnod->minecdz.push_back(cd->d_soil.minecthick);
	}

	if (EOY && regnod->outvarlist[I_oshlwc]==1){
		regnod->oshlwc.push_back(bdall->y_soid.shlwc);
	} else if (EOM && regnod->outvarlist[I_oshlwc]==2) {
		regnod->oshlwc.push_back(bdall->m_soid.shlwc);
	} else if (regnod->outvarlist[I_oshlwc]==3) {
		regnod->oshlwc.push_back(bdall->d_soid.shlwc);
	}

	if (EOY && regnod->outvarlist[I_odeepc]==1){
		regnod->odeepc.push_back(bdall->y_soid.deepc);
	} else if (EOM && regnod->outvarlist[I_odeepc]==2) {
		regnod->odeepc.push_back(bdall->m_soid.deepc);
	} else if (regnod->outvarlist[I_odeepc]==3) {
		regnod->odeepc.push_back(bdall->d_soid.deepc);
	}

	if (EOY && regnod->outvarlist[I_mineac]==1){
		regnod->mineac.push_back(bdall->y_soid.mineac);
	} else if (EOM && regnod->outvarlist[I_mineac]==2) {
		regnod->mineac.push_back(bdall->m_soid.mineac);
	} else if (regnod->outvarlist[I_mineac]==3) {
		regnod->mineac.push_back(bdall->d_soid.mineac);
	}

	if (EOY && regnod->outvarlist[I_minebc]==1){
		regnod->minebc.push_back(bdall->y_soid.minebc);
	} else if (EOM && regnod->outvarlist[I_minebc]==2) {
		regnod->minebc.push_back(bdall->m_soid.minebc);
	} else if (regnod->outvarlist[I_minebc]==3) {
		regnod->minebc.push_back(bdall->d_soid.minebc);
	}

	if (EOY && regnod->outvarlist[I_minecc]==1){
		regnod->minecc.push_back(bdall->y_soid.minecc);
	} else if (EOM && regnod->outvarlist[I_minecc]==2) {
		regnod->minecc.push_back(bdall->m_soid.minecc);
	} else if (regnod->outvarlist[I_minecc]==3) {
		regnod->minecc.push_back(bdall->d_soid.minecc);
	}

	if (EOY && regnod->outvarlist[I_orgn]==1){
		regnod->orgn.push_back(bdall->y_soid.orgnsum);
	} else if (EOM && regnod->outvarlist[I_orgn]==2) {
		regnod->orgn.push_back(bdall->m_soid.orgnsum);
	}

	if (EOY && regnod->outvarlist[I_avln]==1){
		regnod->avln.push_back(bdall->y_soid.avlnsum);
	} else if (EOM && regnod->outvarlist[I_avln]==2) {
		regnod->avln.push_back(bdall->m_soid.avlnsum);
	} else if (regnod->outvarlist[I_avln]==3) {
		regnod->avln.push_back(bdall->d_soid.avlnsum);
	}

	if (EOY && regnod->outvarlist[I_netnmin]==1){
		regnod->netnmin.push_back(bdall->y_soi2soi.netnminsum);
	} else if (EOM && regnod->outvarlist[I_netnmin]==2) {
		regnod->netnmin.push_back(bdall->m_soi2soi.netnminsum);
	} else if (regnod->outvarlist[I_netnmin]==3) {
		regnod->netnmin.push_back(bdall->d_soi2soi.netnminsum);
	}

	if (EOY && regnod->outvarlist[I_orgninput]==1){
		regnod->orgninput.push_back(bdall->y_a2soi.orgninput);
	} else if (EOM && regnod->outvarlist[I_orgninput]==2) {
		regnod->orgninput.push_back(bdall->m_a2soi.orgninput);
	} else if (regnod->outvarlist[I_orgninput]==3) {
		regnod->orgninput.push_back(bdall->d_a2soi.orgninput);
	}

	if (EOY && regnod->outvarlist[I_avlninput]==1){
		regnod->avlninput.push_back(bdall->y_a2soi.avlninput);
	} else if (EOM && regnod->outvarlist[I_avlninput]==2) {
		regnod->avlninput.push_back(bdall->m_a2soi.avlninput);
	} else if (regnod->outvarlist[I_avlninput]==3) {
		regnod->avlninput.push_back(bdall->d_a2soi.avlninput);
	}

	if (EOY && regnod->outvarlist[I_doclost]==1){
		regnod->doclost.push_back(bdall->y_soi2l.doclost);
	} else if (EOM && regnod->outvarlist[I_doclost]==2) {
		regnod->doclost.push_back(bdall->m_soi2l.doclost);
	} else if (regnod->outvarlist[I_doclost]==3) {
		regnod->doclost.push_back(bdall->d_soi2l.doclost);
	}

	if (EOY && regnod->outvarlist[I_orgnlost]==1){
		regnod->orgnlost.push_back(bdall->y_soi2l.orgnlost);
	} else if (EOM && regnod->outvarlist[I_orgnlost]==2) {
		regnod->orgnlost.push_back(bdall->m_soi2l.orgnlost);
	} else if (regnod->outvarlist[I_orgnlost]==3) {
		regnod->orgnlost.push_back(bdall->d_soi2l.orgnlost);
	}

	if (EOY && regnod->outvarlist[I_avlnlost]==1){
		regnod->avlnlost.push_back(bdall->y_soi2l.avlnlost);
	} else if (EOM && regnod->outvarlist[I_avlnlost]==2) {
		regnod->avlnlost.push_back(bdall->m_soi2l.avlnlost);
	} else if (regnod->outvarlist[I_avlnlost]==3) {
		regnod->avlnlost.push_back(bdall->d_soi2l.avlnlost);
	}

  	//
	if (EOY && regnod->outvarlist[I_eet]==1){
		regnod->eet.push_back(edall->y_l2a.eet);
	} else if (EOM && regnod->outvarlist[I_eet]==2) {
		regnod->eet.push_back(edall->m_l2a.eet);
	} else if (regnod->outvarlist[I_eet]==3) {
		regnod->eet.push_back(edall->d_l2a.eet);
	}

	if (EOY && regnod->outvarlist[I_pet]==1){
		regnod->pet.push_back(edall->y_l2a.pet);
	} else if (EOM && regnod->outvarlist[I_pet]==2) {
		regnod->pet.push_back(edall->m_l2a.pet);
	} else if (EOM && regnod->outvarlist[I_pet]==3) {
		regnod->pet.push_back(edall->d_l2a.pet);
	}

	if (EOY && regnod->outvarlist[I_qinfl]==1){
		regnod->qinfl.push_back(edall->y_soi2l.qinfl);
	} else if (EOM && regnod->outvarlist[I_qinfl]==2) {
		regnod->qinfl.push_back(edall->m_soi2l.qinfl);
	} else if (regnod->outvarlist[I_qinfl]==3) {
		regnod->qinfl.push_back(edall->d_soi2l.qinfl);
	}

	if (EOY && regnod->outvarlist[I_qdrain]==1){
		regnod->qdrain.push_back(edall->y_soi2l.qdrain);
	} else if (EOM && regnod->outvarlist[I_qdrain]==2) {
		regnod->qdrain.push_back(edall->m_soi2l.qdrain);
	} else if (regnod->outvarlist[I_qdrain]==3) {
		regnod->qdrain.push_back(edall->d_soi2l.qdrain);
	}

	if (EOY && regnod->outvarlist[I_qrunoff]==1){
		regnod->qrunoff.push_back(edall->y_soi2l.qover);
	} else if (EOM && regnod->outvarlist[I_qrunoff]==2) {
		regnod->qrunoff.push_back(edall->m_soi2l.qover);
	} else if (regnod->outvarlist[I_qrunoff]==3) {
		regnod->qrunoff.push_back(edall->d_soi2l.qover);
	}

	if (EOY && regnod->outvarlist[I_snwthick]==1){
		regnod->snwthick.push_back(cd->y_snow.thick);
	} else if (EOM && regnod->outvarlist[I_snwthick]==2) {
		regnod->snwthick.push_back(cd->m_snow.thick);
	} else if (regnod->outvarlist[I_snwthick]==3) {
		regnod->snwthick.push_back(cd->d_snow.thick);
	}

	if (EOY && regnod->outvarlist[I_swe]==1){
		regnod->swe.push_back(edall->y_snws.swesum);
	} else if (EOM && regnod->outvarlist[I_swe]==2) {
		regnod->swe.push_back(edall->m_snws.swesum);
	} else if (regnod->outvarlist[I_swe]==3) {
		regnod->swe.push_back(edall->d_snws.swesum);
	}

	if (EOY && regnod->outvarlist[I_wtd]==1){
		regnod->wtd.push_back(edall->y_sois.watertab);
	} else if (EOM && regnod->outvarlist[I_wtd]==2) {
		regnod->wtd.push_back(edall->m_sois.watertab);
	} else if (regnod->outvarlist[I_wtd]==3) {
		regnod->wtd.push_back(edall->d_sois.watertab);
	}

	if (EOY && regnod->outvarlist[I_alc]==1){
		regnod->alc.push_back(edall->y_soid.alc);
	} else if (EOM && regnod->outvarlist[I_alc]==2) {
		regnod->alc.push_back(edall->m_soid.alc);
	} else if (regnod->outvarlist[I_alc]==3) {
		regnod->alc.push_back(edall->d_soid.alc);
	}

	if (EOY && regnod->outvarlist[I_ald]==1){
		regnod->ald.push_back(edall->y_soid.ald);
	} else if (EOM && regnod->outvarlist[I_ald]==2) {
		regnod->ald.push_back(edall->m_soid.ald);
	} else if (regnod->outvarlist[I_ald]==3) {
		regnod->ald.push_back(edall->d_soid.ald);
	}

///
	if (EOY && regnod->outvarlist[I_vwcshlw]==1){
		regnod->vwcshlw.push_back(edall->y_soid.vwcshlw);
	} else if (EOM && regnod->outvarlist[I_vwcshlw]==2) {
		regnod->vwcshlw.push_back(edall->m_soid.vwcshlw);
	} else if (regnod->outvarlist[I_vwcshlw]==3) {
		regnod->vwcshlw.push_back(edall->d_soid.vwcshlw);
	}

	if (EOY && regnod->outvarlist[I_vwcdeep]==1){
		regnod->vwcdeep.push_back(edall->y_soid.vwcdeep);
	} else if (EOM && regnod->outvarlist[I_vwcdeep]==2) {
		regnod->vwcdeep.push_back(edall->m_soid.vwcdeep);
	} else if (regnod->outvarlist[I_vwcdeep]==3) {
		regnod->vwcdeep.push_back(edall->d_soid.vwcdeep);
	}

	if (EOY && regnod->outvarlist[I_vwcminea]==1){
		regnod->vwcminea.push_back(edall->y_soid.vwcminea);
	} else if (EOM && regnod->outvarlist[I_vwcminea]==2) {
		regnod->vwcminea.push_back(edall->m_soid.vwcminea);
	} else if (regnod->outvarlist[I_vwcminea]==3) {
		regnod->vwcminea.push_back(edall->d_soid.vwcminea);
	}

	if (EOY && regnod->outvarlist[I_vwcmineb]==1){
		regnod->vwcmineb.push_back(edall->y_soid.vwcmineb);
	} else if (EOM && regnod->outvarlist[I_vwcmineb]==2) {
		regnod->vwcmineb.push_back(edall->m_soid.vwcmineb);
	} else if (EOM && regnod->outvarlist[I_vwcmineb]==3) {
		regnod->vwcmineb.push_back(edall->d_soid.vwcmineb);
	}

	if (EOY && regnod->outvarlist[I_vwcminec]==1){
		regnod->vwcminec.push_back(edall->y_soid.vwcminec);
	} else if (EOM && regnod->outvarlist[I_vwcminec]==2) {
		regnod->vwcminec.push_back(edall->m_soid.vwcminec);
	} else if (regnod->outvarlist[I_vwcminec]==3) {
		regnod->vwcminec.push_back(edall->d_soid.vwcminec);
	}

	if (EOY && regnod->outvarlist[I_tshlw]==1){
		regnod->tshlw.push_back(edall->y_soid.tshlw);
	} else if (EOM && regnod->outvarlist[I_tshlw]==2) {
		regnod->tshlw.push_back(edall->m_soid.tshlw);
	} else if (EOM && regnod->outvarlist[I_tshlw]==3) {
		regnod->tshlw.push_back(edall->d_soid.tshlw);
	}

	if (EOY && regnod->outvarlist[I_tdeep]==1){
		regnod->tdeep.push_back(edall->y_soid.tdeep);
	} else if (EOM && regnod->outvarlist[I_tdeep]==2) {
		regnod->tdeep.push_back(edall->m_soid.tdeep);
	} else if (regnod->outvarlist[I_tdeep]==3) {
		regnod->tdeep.push_back(edall->d_soid.tdeep);
	}

	if (EOY && regnod->outvarlist[I_tminea]==1){
		regnod->tminea.push_back(edall->y_soid.tminea);
	} else if (EOM && regnod->outvarlist[I_tminea]==2) {
		regnod->tminea.push_back(edall->m_soid.tminea);
	} else if (regnod->outvarlist[I_tminea]==3) {
		regnod->tminea.push_back(edall->d_soid.tminea);
	}

	if (EOY && regnod->outvarlist[I_tmineb]==1){
		regnod->tmineb.push_back(edall->y_soid.tmineb);
	} else if (EOM && regnod->outvarlist[I_tmineb]==2) {
		regnod->tmineb.push_back(edall->m_soid.tmineb);
	} else if (regnod->outvarlist[I_tmineb]==3) {
		regnod->tmineb.push_back(edall->d_soid.tmineb);
	}

	if (EOY && regnod->outvarlist[I_tminec]==1){
		regnod->tminec.push_back(edall->y_soid.tminec);
	} else if (EOM && regnod->outvarlist[I_tminec]==2) {
		regnod->tminec.push_back(edall->m_soid.tminec);
	} else if (regnod->outvarlist[I_tminec]==3) {
		regnod->tminec.push_back(edall->d_soid.tminec);
	}

	if (EOY && regnod->outvarlist[I_hkshlw]==1){
		regnod->hkshlw.push_back(edall->y_soid.hkshlw);
	} else if (EOM && regnod->outvarlist[I_hkshlw]==2) {
		regnod->hkshlw.push_back(edall->m_soid.hkshlw);
	} else if (regnod->outvarlist[I_hkshlw]==3) {
		regnod->hkshlw.push_back(edall->d_soid.hkshlw);
	}

	if (EOY && regnod->outvarlist[I_hkdeep]==1){
		regnod->hkdeep.push_back(edall->y_soid.hkdeep);
	} else if (EOM && regnod->outvarlist[I_hkdeep]==2) {
		regnod->hkdeep.push_back(edall->m_soid.hkdeep);
	} else if (regnod->outvarlist[I_hkdeep]==3) {
		regnod->hkdeep.push_back(edall->d_soid.hkdeep);
	}

	if (EOY && regnod->outvarlist[I_hkminea]==1){
		regnod->hkminea.push_back(edall->y_soid.hkminea);
	} else if (EOM && regnod->outvarlist[I_hkminea]==2) {
		regnod->hkminea.push_back(edall->m_soid.hkminea);
	} else if (regnod->outvarlist[I_hkminea]==3) {
		regnod->hkminea.push_back(edall->d_soid.hkminea);
	}

	if (EOY && regnod->outvarlist[I_hkmineb]==1){
		regnod->hkmineb.push_back(edall->y_soid.hkmineb);
	} else if (EOM && regnod->outvarlist[I_hkmineb]==2) {
		regnod->hkmineb.push_back(edall->m_soid.hkmineb);
	} else if (regnod->outvarlist[I_hkmineb]==3) {
		regnod->hkmineb.push_back(edall->d_soid.hkmineb);
	}

	if (EOY && regnod->outvarlist[I_hkminec]==1){
		regnod->hkminec.push_back(edall->y_soid.hkminec);
	} else if (EOM && regnod->outvarlist[I_hkminec]==2) {
		regnod->hkminec.push_back(edall->m_soid.hkminec);
	} else if (regnod->outvarlist[I_hkminec]==3) {
		regnod->hkminec.push_back(edall->d_soid.hkminec);
	}

	if (EOY && regnod->outvarlist[I_tcshlw]==1){
		regnod->tcshlw.push_back(edall->y_soid.tcshlw);
	} else if (EOM && regnod->outvarlist[I_tcshlw]==2) {
		regnod->tcshlw.push_back(edall->m_soid.tcshlw);
	} else if (regnod->outvarlist[I_tcshlw]==3) {
		regnod->tcshlw.push_back(edall->d_soid.tcshlw);
	}

	if (EOY && regnod->outvarlist[I_tcdeep]==1){
		regnod->tcdeep.push_back(edall->y_soid.tcdeep);
	} else if (EOM && regnod->outvarlist[I_tcdeep]==2) {
		regnod->tcdeep.push_back(edall->m_soid.tcdeep);
	} else if (regnod->outvarlist[I_tcdeep]==3) {
		regnod->tcdeep.push_back(edall->d_soid.tcdeep);
	}

	if (EOY && regnod->outvarlist[I_tcminea]==1){
		regnod->tcminea.push_back(edall->y_soid.tcminea);
	} else if (EOM && regnod->outvarlist[I_tcminea]==2) {
		regnod->tcminea.push_back(edall->m_soid.tcminea);
	} else if (regnod->outvarlist[I_tcminea]==3) {
		regnod->tcminea.push_back(edall->d_soid.tcminea);
	}

	if (EOY && regnod->outvarlist[I_tcmineb]==1){
		regnod->tcmineb.push_back(edall->y_soid.tcmineb);
	} else if (EOM && regnod->outvarlist[I_tcmineb]==2) {
		regnod->tcmineb.push_back(edall->m_soid.tcmineb);
	} else if (regnod->outvarlist[I_tcmineb]==3) {
		regnod->tcmineb.push_back(edall->d_soid.tcmineb);
	}

	if (EOY && regnod->outvarlist[I_tcminec]==1){
		regnod->tcminec.push_back(edall->y_soid.tcminec);
	} else if (EOM && regnod->outvarlist[I_tcminec]==2) {
		regnod->tcminec.push_back(edall->m_soid.tcminec);
	} else if (regnod->outvarlist[I_tcminec]==3) {
		regnod->tcminec.push_back(edall->d_soid.tcminec);
	}

	if (EOY && regnod->outvarlist[I_tbotrock]==1){
		regnod->tbotrock.push_back(edall->y_soid.tbotrock);
	} else if (EOM && regnod->outvarlist[I_tbotrock]==2) {
		regnod->tbotrock.push_back(edall->m_soid.tbotrock);
	} else if (regnod->outvarlist[I_tbotrock]==3) {
		regnod->tbotrock.push_back(edall->d_soid.tbotrock);
	}

///////////////////////
	if (EOY && regnod->outvarlist[I_burnthick]==1){
		regnod->burnthick.push_back(fd->fire_soid.burnthick);
	} else if (EOM && regnod->outvarlist[I_burnthick]==2) {
		regnod->burnthick.push_back(fd->fire_soid.burnthick);
	} else if (regnod->outvarlist[I_burnthick]==3) {
		regnod->burnthick.push_back(fd->fire_soid.burnthick);
	}

	if (EOY && regnod->outvarlist[I_burnsoic]==1){
		regnod->burnsoic.push_back(fd->fire_soi2a.orgc);
	} else if (EOM && regnod->outvarlist[I_burnsoic]==2) {
		regnod->burnsoic.push_back(fd->fire_soi2a.orgc);
	} else if (regnod->outvarlist[I_burnsoic]==3) {
		regnod->burnsoic.push_back(fd->fire_soi2a.orgc);
	}

	if (EOY && regnod->outvarlist[I_burnvegc]==1){
		regnod->burnvegc.push_back(fd->fire_v2a.orgc);
	} else if (EOM && regnod->outvarlist[I_burnvegc]==2) {
		regnod->burnvegc.push_back(fd->fire_v2a.orgc);
	}

	if (EOY && regnod->outvarlist[I_burnsoin]==1){
		regnod->burnsoin.push_back(fd->fire_soi2a.orgn);
	} else if (EOM && regnod->outvarlist[I_burnsoin]==2) {
		regnod->burnsoin.push_back(fd->fire_soi2a.orgn);
	} else if (regnod->outvarlist[I_burnsoin]==3) {
		regnod->burnsoin.push_back(fd->fire_soi2a.orgn);
	}

	if (EOY && regnod->outvarlist[I_burnvegn]==1){
		regnod->burnvegn.push_back(fd->fire_v2a.orgn);
	} else if (EOM && regnod->outvarlist[I_burnvegn]==2) {
		regnod->burnvegn.push_back(fd->fire_v2a.orgn);
	}

	if (EOY && regnod->outvarlist[I_burnretainc]==1){
		regnod->burnretainc.push_back(fd->fire_v2soi.abvc
				                     +fd->fire_v2soi.blwc);   //retained abvc is burned residue, retained blwc is burn-caused root death
	} else if (EOM && regnod->outvarlist[I_burnretainc]==2) {
		regnod->burnretainc.push_back(fd->fire_v2soi.abvc
				                     +fd->fire_v2soi.blwc);
	} else if (regnod->outvarlist[I_burnretainc]==3) {
		regnod->burnretainc.push_back(fd->fire_v2soi.abvc
				                     +fd->fire_v2soi.blwc);
	}

	if (EOY &&regnod->outvarlist[I_burnretainn]==1){
		regnod->burnretainn.push_back(fd->fire_v2soi.abvn+fd->fire_v2soi.blwn);
	} else if (EOM && regnod->outvarlist[I_burnretainn]==2) {
		regnod->burnretainn.push_back(fd->fire_v2soi.abvn+fd->fire_v2soi.blwn);
	} else if (regnod->outvarlist[I_burnretainn]==3) {
		regnod->burnretainn.push_back(fd->fire_v2soi.abvn+fd->fire_v2soi.blwn);
	}

	//
	if (EOY && regnod->outvarlist[I_ch4]==1){
		regnod->ch4.push_back(bdall->y_soid.ch4avg);
	} else if (EOM && regnod->outvarlist[I_ch4]==2) {
		regnod->ch4.push_back(bdall->m_soid.ch4avg);
	} else if (regnod->outvarlist[I_ch4]==3) {
		regnod->ch4.push_back(bdall->d_soid.ch4avg);
	}

	if (EOY && regnod->outvarlist[I_ch4prod]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totProd_m);
	} else if (EOM && regnod->outvarlist[I_ch4prod]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totProd_m);
	} else if (regnod->outvarlist[I_ch4prod]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totProd_m);
	}

	if (EOY && regnod->outvarlist[I_ch4oxid]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totOxid_m);
	} else if (EOM && regnod->outvarlist[I_ch4oxid]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totOxid_m);
	} else if (regnod->outvarlist[I_ch4oxid]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totOxid_m);
	}

	if (EOY && regnod->outvarlist[I_ch4flx]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totCH4Flux_m);
	} else if (EOM && regnod->outvarlist[I_ch4flx]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totCH4Flux_m);
	} else if (regnod->outvarlist[I_ch4flx]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totCH4Flux_m);
	}

	if (EOY && regnod->outvarlist[I_ch4flx2a]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totFlux2A_m);
	} else if (EOM && regnod->outvarlist[I_ch4flx2a]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totFlux2A_m);
	} else if (regnod->outvarlist[I_ch4flx2a]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totFlux2A_m);
	}

	if (EOY && regnod->outvarlist[I_ch4flx2p]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totPlant_m);
	} else if (EOM && regnod->outvarlist[I_ch4flx2p]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totPlant_m);
	} else if (regnod->outvarlist[I_ch4flx2p]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totPlant_m);
	}

	if (EOY && regnod->outvarlist[I_ch4flx2b]==1){
		regnod->ch4.push_back(bdall->y_soi2a.totEbul_m);
	} else if (EOM && regnod->outvarlist[I_ch4flx2b]==2) {
		regnod->ch4.push_back(bdall->m_soi2a.totEbul_m);
	} else if (regnod->outvarlist[I_ch4flx2b]==3) {
		regnod->ch4.push_back(bdall->d_soi2a.totEbul_m);
	}

};

// NOTE: 'resod', restart output data, a dataset to resume a complete model run if model is paused
// This is useful and very needed for carrying out a series of model implementation, i.e., from eq->spinup->transient->scenario  runs
// OR, potentially the model can run spatially for one time-step (rather than in time series for ONE cohort)
void OutRetrive::updateRestartOutputBuffer(){

 		resod->reinitValue();
 	
 		//
 		resod->chtid = cd->chtid;

 		// atm
 		resod->dsr         = edall->d_atms.dsr;
 		resod->firea2sorgn = fd->fire_a2soi.orgn;     //this is 'fire_a2soi.orgn' to re-deposit fire-emitted N in one FRI

 		//vegegetation
 		resod->yrsdist   = cd->yrsdist;

 	    for (int ip=0; ip<NUM_PFT; ip++) {
 	    	if (cd->d_veg.vegcov[ip]>0.) {
				resod->ifwoody[ip]    = cd->d_veg.ifwoody[ip];
				resod->ifdeciwoody[ip]= cd->d_veg.ifdeciwoody[ip];
				resod->ifperenial[ip] = cd->d_veg.ifperenial[ip];
				resod->nonvascular[ip]= cd->d_veg.nonvascular[ip];

				resod->vegage[ip] = cd->d_veg.vegage[ip];
				resod->vegcov[ip] = cd->d_veg.vegcov[ip];
				resod->lai[ip]    = cd->d_veg.lai[ip];
				for (int i=0; i<MAX_ROT_LAY; i++) {
					resod->rootfrac[i][ip] = cd->d_veg.frootfrac[i][ip];
				}

				resod->vegwater[ip] = ed[ip]->d_vegs.rwater;             //canopy water - 'vegs_env'
				resod->vegsnow[ip]  = ed[ip]->d_vegs.snow;              //canopy snow  - 'vegs_env'

				for (int i=0; i<NUM_PFT_PART; i++) {
					resod->vegc[i][ip] = bd[ip]->d_vegs.c[i];   // - 'vegs_bgc'
					resod->strn[i][ip] = bd[ip]->d_vegs.strn[i];
				}
				resod->labn[ip]      = bd[ip]->d_vegs.labn;
				resod->deadc[ip]     = bd[ip]->d_vegs.deadc;
				resod->deadn[ip]     = bd[ip]->d_vegs.deadn;

				resod->eetmx[ip]        = cd->d_vegd.eetmx[ip];
				resod->topt[ip]         = cd->d_vegd.topt[ip];
				resod->unnormleafmx[ip] = cd->d_vegd.unnormleafmx[ip];
				resod->growingttime[ip] = cd->d_vegd.growingttime[ip];
				resod->foliagemx[ip]    = cd->d_vegd.foliagemx[ip];        // this is for f(foliage) in GPP to be sure f(foliage) not going down

				deque<double> tmpdeque1 = cd->toptque[ip];
				int recnum = tmpdeque1.size();
				for (int i=0; i<recnum; i++) {
					resod->toptA[i][ip] = tmpdeque1[i];
				}
				deque<double> tmpdeque2 = cd->prvunnormleafmxque[ip];
				recnum = tmpdeque2.size();
				for (int i=0; i<recnum; i++) {
					resod->unnormleafmxA[i][ip] = tmpdeque2[i];
				}
				deque<double> tmpdeque3 = cd->prvgrowingttimeque[ip];
				recnum = tmpdeque3.size();
				for (int i=0; i<recnum; i++) {
					resod->growingttimeA[i][ip]= tmpdeque3[i];
				}
				deque<double> tmpdeque4 = cd->prveetmxque[ip];
				recnum = tmpdeque4.size();
				for (int i=0; i<recnum; i++) {
					resod->eetmxA[i][ip]= tmpdeque4[i];
				}

 	    	} // end of 'if vegcov>0'

	    } // end of 'for ip loop'

 	    // snow - 'restart' from the last point, so be the daily for 'cd' and 'ed', but monthly for 'bd'
 	    resod->numsnwl = cd->d_snow.numsnwl;
 	    resod->snwextramass = cd->d_snow.extramass;
 	    for(int il =0;il<cd->d_snow.numsnwl; il++){
 	    	resod->DZsnow[il]  = cd->d_snow.dz[il];
 	    	resod->AGEsnow[il] = cd->d_snow.age[il];
 	    	resod->RHOsnow[il] = cd->d_snow.rho[il];

 	    	resod->TSsnow[il]  = edall->d_snws.tsnw[il];  // NOTE: for all PFT, ground 'ed' is same, BE sure that is done
 	    	resod->LIQsnow[il] = edall->d_snws.snwliq[il];
 	    	resod->ICEsnow[il] = edall->d_snws.snwice[il];
 		}
 		
 	    //ground-soil
 	    resod->numsl  = cd->d_soil.numsl;     //actual number of soil layers
 	    resod->monthsfrozen   = edall->d_sois.monthsfrozen;
 	    resod->rtfrozendays   = edall->d_sois.rtfrozendays;
 	    resod->rtunfrozendays = edall->d_sois.rtunfrozendays;
 	    resod->watertab   = edall->d_sois.watertab;
 		for(int il =0;il<cd->d_soil.numsl; il++){
 			resod->DZsoil[il]   = cd->d_soil.dz[il];
 			resod->AGEsoil[il]  = cd->d_soil.age[il];
 			resod->TYPEsoil[il] = cd->d_soil.type[il];
 			resod->TEXTUREsoil[il]= cd->d_soil.texture[il];

 			resod->TSsoil[il]    = edall->d_sois.ts[il];
 			resod->LIQsoil[il]   = edall->d_sois.liq[il];
 			resod->ICEsoil[il]   = edall->d_sois.ice[il];
 			resod->FROZENsoil[il]= edall->d_sois.frozen[il];
 			resod->FROZENFRACsoil[il]= edall->d_sois.frozenfrac[il];

 		}

 		for(int il =0;il<MAX_ROC_LAY; il++){
 			resod->TSrock[il] = edall->d_sois.trock[il];
 			resod->DZrock[il] = ROCKTHICK[il];
 		}

 		for(int il =0;il<MAX_NUM_FNT; il++){
 			resod->frontZ[il]  = edall->d_sois.frontsz[il];
 			resod->frontFT[il] = edall->d_sois.frontstype[il];
 		}

 		//
 		resod->wdebrisc = bdall->d_sois.wdebrisc;
 		resod->wdebrisn = bdall->d_sois.wdebrisn;
 		resod->dmossc = bdall->d_sois.dmossc;
 		resod->dmossn = bdall->d_sois.dmossn;
 		for(int il =0;il<cd->d_soil.numsl; il++){
 			resod->rawc[il]  = bdall->d_sois.rawc[il];
 			resod->soma[il]  = bdall->d_sois.soma[il];
 			resod->sompr[il] = bdall->d_sois.sompr[il];
 			resod->somcr[il] = bdall->d_sois.somcr[il];

 			resod->orgn[il] = bdall->d_sois.orgn[il];
 			resod->avln[il] = bdall->d_sois.avln[il];
 			resod->ch4[il] = bdall->d_sois.ch4[il];

 	    	deque<double> tmpdeque = bdall->prvltrfcnque[il];
 	    	int recnum = tmpdeque.size();
 	    	for (int i=0; i<recnum; i++) {
 	    		resod->prvltrfcnA[i][il]= tmpdeque[i];
 	    	}

 		}

};

