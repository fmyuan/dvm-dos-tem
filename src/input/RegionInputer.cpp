#include "RegionInputer.h"

RegionInputer::RegionInputer(){
	md = NULL;
	act_co2yr = MISSING_I;
};

RegionInputer::~RegionInputer(){

};

void RegionInputer::init(){

	if(md->reginputdir.length()>0){

  		act_co2yr = initCO2file(md->reginputdir);

  	}else{
  		string msg = "RegionInputer::init - no input directory";
 		cout<<msg+"\n";
 		exit(-1);
  	}

};

int RegionInputer::initCO2file(string &dir){
	//netcdf error
	NcError err(NcError::silent_nonfatal);

	co2filename = dir +"co2.nc";
	if (md->runsc) co2filename = dir +"co2_sc.nc";  //Yuan: read in CO2 ppm from projection

	NcFile co2File(co2filename.c_str(), NcFile::ReadOnly);
 	if(!co2File.is_valid()){
 		string msg = co2filename + " is not valid";
 		cout << msg + "\n";
 		exit(-1);
 	}
 	
 	NcDim* yrD = co2File.get_dim("YEAR");
 	if(yrD && yrD->is_valid()){
 	 	int yrs = yrD->size();

 	 	return (yrs);
 	}else{
 		string msg = "YEAR Dimension is not Valid in RegionInputer::initCO2file";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	return (0);
}

void RegionInputer::getCO2(RegionData *rd){
	NcError err(NcError::silent_nonfatal);

	NcFile co2File(co2filename.c_str(), NcFile::ReadOnly);
 	NcVar* co2yrV = co2File.get_var("YEAR");
 	if(co2yrV){
 		NcBool nb1 = co2yrV->get(&rd->co2year[0], rd->act_co2yr);
 		if(!nb1){
 			string msg = "problem in reading CO2 in RegionInputer::getCO2 ";
 			cout<<msg+"\n";
 			exit(-1);
 		}
 	}else{
 		string msg = "RegionInputer::getCO2 - YEAR variable is not valid ";
		cout<<msg+"\n";
		exit(-1);
 	}
 	 	
 	NcVar* co2V = co2File.get_var("CO2");
 	if(co2V){
 		NcBool nb2 = co2V->get(&rd->co2[0], rd->act_co2yr);
 		if(!nb2){
 			string msg = "problem in reading CO2 in RegionInputer::getCO2 ";
 			cout<<msg+"\n";
 			exit(-1);
 		}
 	}else{
 		string msg = "RegionInputer::getCO2 - CO2 variable is not valid ";
		cout<<msg+"\n";
		exit(-1);
 	}

	co2File.close();
}

void RegionInputer::setModelData(ModelData* mdp){
   	md = mdp;
};

