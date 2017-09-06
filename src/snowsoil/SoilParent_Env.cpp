#include "SoilParent_Env.h"

<<<<<<< HEAD
SoilParent_Env::SoilParent_Env(){
	tstepmode = DAILY;
	ground = NULL;
	ed_sois= NULL;

=======
SoilParent_Env::SoilParent_Env() {
>>>>>>> master
}

void SoilParent_Env::initializeState() {
  Layer* currl = ground->botlayer;
  int permf =1;

  while(currl!=NULL) {
    if(currl->isRock) {
      currl->liq =0.;
      currl->ice=0.;

      if(permf==0) {
        currl->tem=1;
      } else if(permf==1) {
        currl->tem=-1;
      }
    } else {
      break;
    }

    currl = currl->prevl;
  }
};

void SoilParent_Env::set_state_from_restartdata(const RestartData & rdata) {
  double TSrock[MAX_ROC_LAY];

  for (int i=0; i<MAX_ROC_LAY; i++) {
    TSrock[i] = rdata.TSrock[i];
  }

  Layer* currl = ground->lstminel;
  int rcind =-1;

  while(currl!=NULL) {
    if(currl->isRock) {
      rcind ++;
      currl->tem = TSrock[rcind];
      currl->liq =0;
      currl->ice =0;
      currl->age =0;
      currl->poro=0;
    }

    currl = currl->nextl;
  }
};

void SoilParent_Env::retrieveDailyTM(Layer* lstsoill) {
  Layer *currl = lstsoill->nextl;
  double trock = lstsoill->tem;
  int rcind = -1;

<<<<<<< HEAD
	double trock = lstsoill->tem;
	int rcind = -1;
	while (currl!=NULL) {
		if (currl->isRock) {
			rcind++;
			ed_sois->trock[rcind] = trock;
		}
		currl = currl->nextl;
	}
=======
  while (currl!=NULL) {
    if (currl->isRock) {
      rcind++;
      ed->d_sois.trock[rcind] = trock;
    }
>>>>>>> master

    currl = currl->nextl;
  }
}

void SoilParent_Env::setEnvData(EnvData *edp) {
<<<<<<< HEAD
	if(tstepmode==MONTHLY){
		ed_sois = &edp->m_sois;
	}else if(tstepmode==DAILY){
		ed_sois = &edp->d_sois;
	}
=======
  ed = edp;
>>>>>>> master
}

void SoilParent_Env::setGround(Ground *groundp) {
  ground = groundp;
}

