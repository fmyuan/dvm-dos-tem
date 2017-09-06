/*! \file
 *
 */
#include "ParentLayer.h"

#include "../../TEMLogger.h"
extern src::severity_logger< severity_level > glg;

ParentLayer::ParentLayer(const double & thick) {
  BOOST_LOG_SEV(glg, debug) << "==> Creating a ParentLayer object...";
  tkey=I_ROCK;
  isRock = true;
  solind = MISSING_I;
  dz = thick;
  updateProperty();
};

ParentLayer::~ParentLayer() {
  BOOST_LOG_SEV(glg, debug) << "--> Deleting a ParentLayer object...";
};

void ParentLayer::updateProperty() {
  poro =  0;
  tcsolid =2;//
  tcsatunf= tcsolid;
  tcsatfrz= tcsolid;
  vhcsolid = 2700000;
};

// get frozen layer specific heat capcity
<<<<<<< HEAD
double ParentLayer::getFrzVolHeatCapa(){
	 double vhc = vhcsolid ;
	 return (vhc);
};

double ParentLayer::getUnfVolHeatCapa(){
	   double vhc= vhcsolid ;
	 	return (vhc);
};
	   
double ParentLayer::getMixVolHeatCapa(){
	   double vhc= vhcsolid ;
	 	return (vhc);
=======
double ParentLayer::getFrzVolHeatCapa() {
  double vhc = vhcsolid ;
  return vhc;
};

double ParentLayer::getUnfVolHeatCapa() {
  double vhc= vhcsolid ;
  return vhc;
};

double ParentLayer::getMixVolHeatCapa() {
  double vhc= vhcsolid ;
  return vhc;
>>>>>>> master
};

// get frozen layer thermal conductivity
<<<<<<< HEAD
double ParentLayer::getFrzThermCond(){
	  	double tc=tcsolid;
	  	
	  	return (tc);
=======
double ParentLayer::getFrzThermCond() {
  double tc=tcsolid;
  return tc;
>>>>>>> master
};

// get unfrozen layer thermal conductivity
<<<<<<< HEAD
double ParentLayer::getUnfThermCond(){
  	double tc=tcsolid;
	  	
  	return (tc);
=======
double ParentLayer::getUnfThermCond() {
  double tc=tcsolid;
  return tc;
>>>>>>> master
};

// get albedo of visible radition
<<<<<<< HEAD
double ParentLayer::getAlbedoVis(){//should not used
    double vis=0.2;
    return (vis);
=======
double ParentLayer::getAlbedoVis() { //should not used
  double vis=0.2;
  return vis;
>>>>>>> master
};

// get albedo of nir radition
<<<<<<< HEAD
double ParentLayer::getAlbedoNir(){//should not used
    double nir=0.2;
       	
    return (nir);
=======
double ParentLayer::getAlbedoNir() { //should not used
  double nir=0.2;
  return nir;
>>>>>>> master
};

