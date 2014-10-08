/*! \file 
*/

#include "ChtOutputer.h"

ChtOutputer::ChtOutputer(){
	
};

ChtOutputer::~ChtOutputer(){

};

void ChtOutputer::init(string & dirfile){

	//file
	ncfname = dirfile;

	ncfile = new NcFile(ncfname.c_str(), NcFile::Replace);

	//dimension
	timeD     = ncfile->add_dim("tstep");
	pftD      = ncfile->add_dim("pft", NUM_PFT);
	rotlayerD = ncfile->add_dim("rootlayer", MAX_ROT_LAY);
	snwlayerD = ncfile->add_dim("snwlayer", MAX_SNW_LAY);
	soilayerD = ncfile->add_dim("soilayer", MAX_SOI_LAY);

    //variables
  	ncfile->add_var("CHTID", ncInt);
	yearV  = ncfile->add_var("YEAR", ncInt, timeD);
	monV   = ncfile->add_var("MONTH", ncInt, timeD);
	dayV   = ncfile->add_var("DAY", ncInt, timeD);

	// veg dimension variables
   	cmttypeV  = ncfile->add_var("CMTTYPE", ncInt, timeD);

   	vegageV      = ncfile->add_var("VEGAGE", ncInt, timeD, pftD);
   	ifwoodyV     = ncfile->add_var("IFWOODY", ncInt, timeD, pftD);
   	ifdeciwoodyV = ncfile->add_var("IFDECIWOODY", ncInt, timeD, pftD);
   	ifperenialV  = ncfile->add_var("IFPERENIAL", ncInt, timeD, pftD);
   	nonvascularV = ncfile->add_var("IFNONVASCULAR", ncInt, timeD, pftD);

   	vegcovV  = ncfile->add_var("VEGCOV", ncDouble, timeD, pftD);
   	laiV     = ncfile->add_var("LAI", ncDouble, timeD, pftD);
   	fpcV     = ncfile->add_var("FPC", ncDouble, timeD, pftD);
   	rtfracV  = ncfile->add_var("ROOTFRAC", ncDouble, timeD, rotlayerD, pftD);
   	fleafV   = ncfile->add_var("FLEAF", ncDouble, timeD, pftD);
   	ffoliageV= ncfile->add_var("FFOLIAGE", ncDouble, timeD, pftD);
	aerenchymaV  = ncfile->add_var("FAERENCHYMA", ncDouble, timeD, pftD);

   	// snow dimension variables
	snwlynumV = ncfile->add_var("SNWLAYERNO", ncDouble, timeD, snwlayerD);
	snwlydzV  = ncfile->add_var("SNWLAYERDZ", ncDouble, timeD, snwlayerD);
	snwlyrhoV = ncfile->add_var("SNWLAYERRHO", ncDouble, timeD, snwlayerD);
	snwlyporV = ncfile->add_var("SNWLAYERPOR", ncDouble, timeD, snwlayerD);

	snwageV = ncfile->add_var("SNWAGE", ncDouble, timeD);
   	snwthickV = ncfile->add_var("SNWTHICK", ncDouble, timeD);
   	snwdenseV = ncfile->add_var("SNWDENSE", ncDouble, timeD);
   	snwextramassV  = ncfile->add_var("SNWEXTRAMASS", ncDouble, timeD);   // 'snowextramass' refers to the snow mass which not enough for a a now layer (partially covered)

 	//soil dimension variables
   	soillayernumV = ncfile->add_var("SOILLAYERNO", ncInt, timeD);
   	soilmossnumV  = ncfile->add_var("MOSSLAYERNO", ncInt, timeD);
   	soilshlwnumV  = ncfile->add_var("SHLWOLAYERNO", ncInt, timeD);
   	soildeepnumV  = ncfile->add_var("DEEPOLAYERNO", ncInt, timeD);
   	soilminenumV  = ncfile->add_var("MINELAYERNO", ncInt, timeD);

   	soilthickV = ncfile->add_var("SOILTHICK", ncDouble, timeD);
   	mossthickV = ncfile->add_var("MOSSTHICK", ncDouble, timeD);
   	shlwthickV = ncfile->add_var("SHLWOTHICK", ncDouble, timeD);
   	deepthickV = ncfile->add_var("DEEPOTHICK", ncDouble, timeD);
   	mineathickV = ncfile->add_var("MINEATHICK", ncDouble, timeD);
   	minebthickV = ncfile->add_var("MINEBTHICK", ncDouble, timeD);
   	minecthickV = ncfile->add_var("MINECTHICK", ncDouble, timeD);

   	soilzV      = ncfile->add_var("SOILZ", ncDouble, timeD, soilayerD);
   	soildzV     = ncfile->add_var("SOILDZ", ncDouble, timeD, soilayerD);
   	soiltypeV   = ncfile->add_var("SOILTYPE", ncInt, timeD, soilayerD);
   	soilporoV   = ncfile->add_var("SOILPORO", ncDouble, timeD, soilayerD);
   	soiltextureV= ncfile->add_var("SOILTEXTURE", ncInt, timeD, soilayerD);
   	soilrtfracV = ncfile->add_var("SOILRTFRAC", ncDouble, timeD, soilayerD, pftD);

   	ncfile->close();

}

void ChtOutputer::outputCohortDimVars(const int & tstepcnt){
 	NcError err(NcError::verbose_nonfatal);

   	ncfile = new NcFile(ncfname.c_str(),NcFile::Write);

   	if (ncfile->data_mode()) {
   		if (tstepcnt==0) {
   			chtidV = ncfile->get_var("CHTID");
   			chtidV->put(&dimod->chtid);
   		}

   		yearV = ncfile->get_var("YEAR");
   		yearV->put_rec(&dimod->year, tstepcnt);

   		monV = ncfile->get_var("MONTH");
   		monV->put_rec(&dimod->month, tstepcnt);

   		dayV = ncfile->get_var("DAY");
   		dayV->put_rec(&dimod->day, tstepcnt);

   		cmttypeV = ncfile->get_var("CMTTYPE");
   		cmttypeV->put_rec(&dimod->cmttype, tstepcnt);

   		vegageV = ncfile->get_var("VEGAGE");
   		vegageV->put_rec(&vegs->vegage[0], tstepcnt);

		ifwoodyV = ncfile->get_var("IFWOODY");
		ifwoodyV->put_rec(&vegs->ifwoody[0], tstepcnt);
		ifdeciwoodyV = ncfile->get_var("IFDECIWOODY");
		ifdeciwoodyV->put_rec(&vegs->ifdeciwoody[0], tstepcnt);
		ifperenialV = ncfile->get_var("IFPERENIAL");
		ifperenialV->put_rec(&vegs->ifperenial[0], tstepcnt);
		nonvascularV = ncfile->get_var("IFNONVASCULAR");
		nonvascularV->put_rec(&vegs->nonvascular[0], tstepcnt);

		aerenchymaV = ncfile->get_var("FAERENCHYMA");
		aerenchymaV->put_rec(&vegs->aerenchyma[0], tstepcnt);

		vegcovV = ncfile->get_var("VEGCOV");
		vegcovV->put_rec(&vegs->vegcov[0], tstepcnt);
		laiV = ncfile->get_var("LAI");
		laiV->put_rec(&vegs->lai[0], tstepcnt);
		fpcV = ncfile->get_var("FPC");
		fpcV->put_rec(&vegs->fpc[0], tstepcnt);
		rtfracV = ncfile->get_var("ROOTFRAC");
		rtfracV->put_rec(&vegs->frootfrac[0][0], tstepcnt);
		fleafV = ncfile->get_var("FLEAF");
		fleafV->put_rec(&vegd->fleaf[0], tstepcnt);
		ffoliageV = ncfile->get_var("FFOLIAGE");
		ffoliageV->put_rec(&vegd->ffoliage[0], tstepcnt);

   	//
		snwlynumV = ncfile->get_var("SNWLAYERNO");
		snwlynumV->put_rec(&snws->numsnwl, tstepcnt);

		snwlydzV = ncfile->get_var("SNWLAYERDZ");
		snwlydzV->put_rec(&snws->dz[0], tstepcnt);

		snwlyrhoV = ncfile->get_var("SNWLAYERRHO");
		snwlyrhoV->put_rec(&snws->rho[0], tstepcnt);

		snwlyporV = ncfile->get_var("SNWLAYERPOR");
		snwlyporV->put_rec(&snws->por[0], tstepcnt);

		snwthickV = ncfile->get_var("SNWTHICK");
		snwthickV->put_rec(&snws->thick, tstepcnt);

		snwdenseV = ncfile->get_var("SNWDENSE");
		snwdenseV->put_rec(&snws->dense, tstepcnt);

		snwextramassV = ncfile->get_var("SNWEXTRAMASS");
		snwextramassV->put_rec(&snws->extramass, tstepcnt);

		snwageV = ncfile->get_var("SNWAGE");
		snwageV->put_rec(&snws->olds, tstepcnt);

   	//
		soillayernumV = ncfile->get_var("SOILLAYERNO");
		soillayernumV->put_rec(&sois->numsl, tstepcnt);

		soilmossnumV = ncfile->get_var("MOSSLAYERNO");
		soilmossnumV->put_rec(&sois->mossnum, tstepcnt);

		soilshlwnumV = ncfile->get_var("SHLWOLAYERNO");
		soilshlwnumV->put_rec(&sois->shlwnum, tstepcnt);

		soildeepnumV = ncfile->get_var("DEEPOLAYERNO");
		soildeepnumV->put_rec(&sois->deepnum, tstepcnt);

		soilminenumV = ncfile->get_var("MINELAYERNO");
		soilminenumV->put_rec(&sois->minenum, tstepcnt);

		soilthickV = ncfile->get_var("SOILTHICK");
		soilthickV->put_rec(&sois->totthick, tstepcnt);

		mossthickV = ncfile->get_var("MOSSTHICK");
		mossthickV->put_rec(&sois->mossthick, tstepcnt);

		shlwthickV = ncfile->get_var("SHLWOTHICK");
		shlwthickV->put_rec(&sois->shlwthick, tstepcnt);

		deepthickV = ncfile->get_var("DEEPOTHICK");
		deepthickV->put_rec(&sois->deepthick, tstepcnt);

		mineathickV = ncfile->get_var("MINEATHICK");
		mineathickV->put_rec(&sois->mineathick, tstepcnt);

		minebthickV = ncfile->get_var("MINEBTHICK");
		minebthickV->put_rec(&sois->minebthick, tstepcnt);

   		minecthickV = ncfile->get_var("MINECTHICK");
		minecthickV->put_rec(&sois->minecthick, tstepcnt);

		soilzV = ncfile->get_var("SOILZ");
		soilzV->put_rec(&sois->z[0], tstepcnt);

		soildzV = ncfile->get_var("SOILDZ");
		soildzV->put_rec(&sois->dz[0], tstepcnt);

		soiltypeV = ncfile->get_var("SOILTYPE");
		soiltypeV->put_rec(&sois->type[0], tstepcnt);

   		soilporoV = ncfile->get_var("SOILPORO");
		soilporoV->put_rec(&sois->por[0], tstepcnt);

		soiltextureV = ncfile->get_var("SOILTEXTURE");
		soiltextureV->put_rec(&sois->texture[0], tstepcnt);

   		soilrtfracV = ncfile->get_var("SOILRTFRAC");
   		soilrtfracV->put_rec(&sois->frootfrac[0][0], tstepcnt);

   	}

   	ncfile->close();

}

void ChtOutputer::setOutData(const int &outtstep, CohortData *cdp){

	dimod = cdp;
 	if (outtstep==DAILY){
 		vegs = &dimod->d_veg;
 		vegd = &dimod->d_vegd;
 		snws = &dimod->d_snow;
 		sois = &dimod->d_soil;
 	} else if (outtstep==MONTHLY){
 		vegs = &dimod->m_veg;
 		vegd = &dimod->m_vegd;
 		snws = &dimod->m_snow;
 		sois = &dimod->m_soil;
 	} else {
 		vegs = &dimod->y_veg;
 		vegd = &dimod->y_vegd;
 		snws = &dimod->y_snow;
 		sois = &dimod->y_soil;
 	}
}
