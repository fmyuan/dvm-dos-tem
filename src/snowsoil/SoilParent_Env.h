#ifndef SOILPARENT_ENV_H_
#define SOILPARENT_ENV_H_

#include "../data/EnvData.h"
#include "../data/RestartData.h"

#include "../ecodomain/Ground.h"

<<<<<<< HEAD
class SoilParent_Env{
	public:

		int tstepmode;

		SoilParent_Env();
=======
class SoilParent_Env {
public:
  SoilParent_Env();
>>>>>>> master

  void initializeState();
  void set_state_from_restartdata(const RestartData & rdata);

  void retrieveDailyTM(Layer* lstsoill);

<<<<<<< HEAD
	private:
		Ground *ground;
		soistate_env *ed_sois;
=======
  void  setEnvData(EnvData* ed);
  void  setGround(Ground* ground);

private:
  Ground *ground;
  EnvData *ed;
>>>>>>> master

};

#endif /*SOILPARENT_ENV_H_*/
