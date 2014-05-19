#include "GridInputer.h"

GridInputer::GridInputer(){
	md = NULL;
};

GridInputer::~GridInputer(){

}

int GridInputer::init(){

	if(md){
	  int error = 0;
	  error = initGrid(md->grdinputdir);
	  if (error!=0) return (error);

	  error = initDrainType(md->grdinputdir);
	  if (error!=0) return (error);

	  error = initSoilTexture(md->grdinputdir);
	  if (error!=0) return (error);

	  error = initFireStatistics(md->grdinputdir);
	  if (error!=0) return (error);

    }else{
	  std::string msg ="GridInputer::init - ModelData is NULL";
	  cout<<msg+"\n";
	  return (-1);
    }
	
	return (0);
}

int GridInputer::initGrid(string& dir){
	//netcdf error
	NcError err(NcError::silent_nonfatal);

	gridfname = dir +"grid.nc";
	NcFile gridFile(gridfname.c_str(), NcFile::ReadOnly);
 	if(!gridFile.is_valid()){
 		string msg = gridfname+" is not valid";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	NcDim* grdD = gridFile.get_dim("GRIDID");
 	if(grdD && grdD->is_valid()){
 		int act_gridno = grdD->size();  //actual grid number
 		md->act_gridno=act_gridno;
 	}else{
 		string msg = "GRIDID Dimension is not valid in 'grid.nc'! ";
 		cout<<msg+"\n";
 		exit(-1);

 	}

  	return (0);

}

int GridInputer::initSoilTexture(string& dir){
	//netcdf error
	NcError err(NcError::silent_nonfatal);

	soilfname = dir +"soiltexture.nc";
	NcFile soilFile(soilfname.c_str(), NcFile::ReadOnly);
 	if(!soilFile.is_valid()){
 		string msg = soilfname+" is not valid";
 		cout<<msg+"\n";
 		exit(-1);
 	}
 	
 	NcDim* soilD = soilFile.get_dim("SOILID");
 	if(soilD && soilD->is_valid()){
 	 	int act_soilidno = soilD->size();  //actual soil dataset number
 	 	md->act_soilno=act_soilidno;
 	}else{
 		string msg ="SOILID Dimension is not valid in 'soiltexture.nc'!";
 		cout<<msg+"\n";
 		exit(-1);

 	}

 	return (0);
}

int GridInputer::initDrainType(string& dir){
	drainfname = dir +"drainage.nc";

	NcError err(NcError::silent_nonfatal);
	NcFile drainFile(drainfname.c_str(), NcFile::ReadOnly);
 	if(!drainFile.is_valid()){
 		string msg = drainfname+" is not valid";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	NcDim* drainD = drainFile.get_dim("DRAINAGEID");
 	if(drainD && drainD->is_valid()){
 	 	int act_drainno = drainD->size();  //actual drainage type datset number

 	 	md->act_drainno=act_drainno;
 	}else{
 		string msg = "DRAINAGEID Dimension is not valid in 'drainage.nc'! ";
 		cout<<msg+"\n";
 		exit(-1);

 	}

 	return (0);
}

int GridInputer::initFireStatistics(string & dir){
	//netcdf error
	NcError err(NcError::silent_nonfatal);

	gfirefname = dir+"firestatistics.nc";
	NcFile fireFile(gfirefname.c_str(), NcFile::ReadOnly);
 	if(!fireFile.is_valid()){
 		string msg = gfirefname+" is not valid";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	NcDim* gfireD = fireFile.get_dim("GFIREID");
 	if(gfireD && gfireD->is_valid()){
 	 	int act_gfireno = gfireD->size();  //actual grid fire dataset number

 	 	md->act_gfireno = act_gfireno;
 	}else{
 		string msg="GFIREID Dimension is not valid in grid 'firestatistics.nc' ! ";
 		cout<<msg+"\n";
 		exit(-1);

 	}

 	NcDim* gfsizeD = fireFile.get_dim("GFSIZENO");
	if(gfsizeD && gfsizeD->is_valid()){
 	 	int act_gfsizeno = gfsizeD->size();  //actual grid fire size class number

 	 	md->fsizeno = act_gfsizeno;
 	}else{
 		string msg="GFSIZE Dimension is not valid in grid 'firestatistics.nc' ! ";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	NcDim* gfseasonD = fireFile.get_dim("GFSEASONNO");
	if(gfseasonD && gfseasonD->is_valid()){
 	 	int act_gseasonno = gfseasonD->size();  //actual grid fire season class number

 	 	md->fseasonno = act_gseasonno;
 	}else{
 		string msg="GFSEASONNO Dimension is not valid in grid 'firestatistics.nc' ! ";
 		cout<<msg+"\n";
 		exit(-1);
 	}

 	return (0);
}

//recno - the order (from ZERO) in the .nc file, ids - the real ids in the *.nc files
int GridInputer::getGridids(int & grdid, int &grddrgid, int &grdsoilid,
		int &grdfireid,	const int & recno){
	//netcdf error
	NcError err(NcError::silent_nonfatal);
	NcFile gridFile(gridfname.c_str(), NcFile::ReadOnly);

	NcVar* grdidV = gridFile.get_var("GRIDID");
	if(grdidV){
		grdidV->set_cur(recno);
		grdidV->get(&grdid, 1);
	}else{
		string msg="Cannot get GRIDID in 'grid.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	NcVar* drgidV = gridFile.get_var("DRAINAGEID");
	if(drgidV){
		drgidV->set_cur(recno);
		drgidV->get(&grddrgid, 1);
	}else{
		string msg="Cannot get DRAINAGEID in 'grid.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	NcVar* soilidV = gridFile.get_var("SOILID");
	if(soilidV){
		soilidV->set_cur(recno);
		soilidV->get(&grdsoilid, 1);
	}else{
		string msg="Cannot get SOILID in 'grid.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	NcVar* gfireidV = gridFile.get_var("GFIREID");
	if(gfireidV){
		gfireidV->set_cur(recno);
		gfireidV->get(&grdfireid, 1);
	}else{
		string msg="Cannot get GFIREID in 'grid.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	return (0);
}

int GridInputer::getDrainId(int & drainid, const int & recno){
	//netcdf error
	NcError err(NcError::silent_nonfatal);

	NcFile drainFile(drainfname.c_str(), NcFile::ReadOnly);

	NcVar* drainidV = drainFile.get_var("DRAINAGEID");
	if(drainidV){
		drainidV->set_cur(recno);
		drainidV->get(&drainid, 1);
	}else{
		string msg="Cannot get DRAINAGEID in 'drainage.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	return (0);
}

int GridInputer::getSoilId(int & soilid, const int & recno){
	//netcdf error
	NcError err(NcError::silent_nonfatal);
	NcFile soilFile(soilfname.c_str(), NcFile::ReadOnly);

	NcVar* soilidV = soilFile.get_var("SOILID");
	if(soilidV){
		soilidV->set_cur(recno);
		soilidV->get(&soilid, 1);
	}else{
		string msg="Cannot get SOILID in 'soiltexture.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	return (0);
}

int GridInputer::getGfireId(int &gfireid, const int & recno){
	//netcdf error
	NcError err(NcError::silent_nonfatal);
	NcFile gfireFile(gfirefname.c_str(), NcFile::ReadOnly);

	NcVar* gfireidV = gfireFile.get_var("GFIREID");
	if(gfireidV){
		gfireidV->set_cur(recno);
		gfireidV->get(&gfireid, 1);
	}else{
		string msg="Cannot get GFIREID in 'firestatistics.nc'! ";
 		cout<<msg+"\n";
 		return (-1);
	}

	return (0);
}

//recid - the order (from ZERO) in the .nc file, gridid - the grid id user-defined in the dataset
void GridInputer::getLatlon(float & lat, float & lon, const int & recid ){

	NcError err(NcError::silent_nonfatal);
	NcFile gridFile(gridfname.c_str(), NcFile::ReadOnly);

	NcVar* latV = gridFile.get_var("LAT");
 	if(latV){
 	 	latV->set_cur(recid);
 		latV->get(&lat, 1);
 	}else{
 		string msg="Cannot get LAT in 'grid.nc'! ";
		cout<<msg+"\n";
		exit(-1);
 	}

 	NcVar* lonV = gridFile.get_var("LON");
 	if(lonV){
 	 	lonV->set_cur(recid);
 		lonV->get(&lon, 1);
 	}else{
 		string msg="Cannot get LON in 'grid.nc' ! ";
		cout<<msg+"\n";
		exit(-1);
 	}

} 

void GridInputer::getDrainType(int & dtype, const int & recid){
	NcError err(NcError::silent_nonfatal);

	NcFile drainageFile(drainfname.c_str(), NcFile::ReadOnly);
 	NcVar* drainV = drainageFile.get_var("DRAINAGETYPE");
 	if(drainV){
 		drainV->set_cur(recid);
 		drainV->get(&dtype, 1);
 	}else{
 		string msg="Cannot get DRAINAGETYPE in 'drainage.nc' ! ";
		cout<<msg+"\n";
		exit(-1);
 	}

};

void GridInputer::getSoilTexture(int & topsoil, int & botsoil, const int & recid ){

	NcError err(NcError::silent_nonfatal);
	NcFile soilFile(soilfname.c_str(), NcFile::ReadOnly);

	NcVar* topsoilV = soilFile.get_var("TOPSOIL");
 	if(topsoilV){
 	 	topsoilV->set_cur(recid);
 		topsoilV->get(&topsoil, 1);
 	}else{
 		string msg="Cannot get TOPSOIL in 'soiltexture.nc' ! ";
		cout<<msg+"\n";
		exit(-1);
 	}

	NcVar* botsoilV = soilFile.get_var("BOTSOIL");
 	if(botsoilV){
 	 	botsoilV->set_cur(recid);
 		botsoilV->get(&botsoil, 1);
 	}else{
 		string msg="Cannot get BOTSOIL in 'soiltexture.nc' ! ";
		cout<<msg+"\n";
		exit(-1);
  	}

} 

void GridInputer::getGfire(int &fri, double pfseason[], double pfsize[], const int & recid ){

	// the following are the currently used in model, and can be modified (actually suggested)
	int fseasonno = md->fseasonno;   //fire season class no.: 0 - pre-fireseason; 1 - early; 2 - late; 3 - post-fireseason
	int fsizeno   = md->fsizeno;   //fire-size year class no.: 0 - small; 1 - intermediate; 2 - large; 3 - very large; 4 - ultra-large

	NcError err(NcError::silent_nonfatal);
	NcFile fireFile(gfirefname.c_str(), NcFile::ReadOnly);

	NcVar* friV = fireFile.get_var("FRI");
 	if(friV){
 	 	friV->set_cur(recid);
 		friV->get(&fri, 1);
 	}else{
 		string msg="Cannot get FRI in 'firestatistics.nc'! ";
		cout<<msg+"\n";
		exit(-1);
 	}

  	NcVar* fseasonV = fireFile.get_var("PFSEASON");
 	if(fseasonV){
 		fseasonV->set_cur(recid);
 		fseasonV->get(&pfseason[0], fseasonno);
 	}else{
 		string msg="Cannot get PFSEASON 'firestatistics.nc'! ";
		cout<<msg+"\n";
		exit(-1);
 	}

	NcVar* pfsizeV = fireFile.get_var("PFSIZE");
 	if(pfsizeV){
 		pfsizeV->set_cur(recid);
 		pfsizeV->get(&pfsize[0], fsizeno);
 	}else{
 		string msg="Cannot get PFSIZE in 'firestatistics.nc'! ";
		cout<<msg+"\n";
		exit(-1);
 	}

};

void GridInputer::setModelData(ModelData* mdp){
	md = mdp;
};

