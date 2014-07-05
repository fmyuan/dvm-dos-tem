# dvm-dos-tem Makefile 

GCC_PATH=/usr/local/gcc-4.8.1
CC=$(GCC_PATH)/bin/g++
CFLAGS=-c -Wall -O0 -g3 -fPIC -fmessage-length=0 -fpermissive
INCDIR=-I/usr/local/netcdf-4.1.3-classic/include
LIBDIR=-L/usr/local/netcdf-4.1.3-classic/lib
LIBS=-lnetcdf_c++ -lnetcdf

LIBTEM = libTEM.dylib
EXETEM = DVMDOSTEM

SRCDIR = ../src

SOURCES=$(SRCDIR)/TEM.o \
		$(SRCDIR)/assembler/RunCohort.o \
		$(SRCDIR)/assembler/RunGrid.o \
		$(SRCDIR)/assembler/Runner.o \
		$(SRCDIR)/assembler/RunRegion.o \
		$(SRCDIR)/atmosphere/Atmosphere.o \
		$(SRCDIR)/atmosphere/AtmosUtil.o \
		$(SRCDIR)/data/BgcData.o \
		$(SRCDIR)/data/CohortData.o \
		$(SRCDIR)/data/EnvData.o \
		$(SRCDIR)/data/FirData.o \
		$(SRCDIR)/data/GridData.o \
		$(SRCDIR)/data/OutDataRegn.o \
		$(SRCDIR)/data/RegionData.o \
		$(SRCDIR)/data/RestartData.o \
		$(SRCDIR)/disturb/WildFire.o \
		$(SRCDIR)/ecodomain/DoubleLinkedList.o \
		$(SRCDIR)/ecodomain/Ground.o \
		$(SRCDIR)/ecodomain/horizon/Mineral.o \
		$(SRCDIR)/ecodomain/horizon/Moss.o \
		$(SRCDIR)/ecodomain/horizon/Organic.o \
		$(SRCDIR)/ecodomain/horizon/Snow.o \
		$(SRCDIR)/ecodomain/horizon/SoilParent.o \
		$(SRCDIR)/ecodomain/layer/Layer.o \
		$(SRCDIR)/ecodomain/layer/MineralLayer.o \
		$(SRCDIR)/ecodomain/layer/MossLayer.o \
		$(SRCDIR)/ecodomain/layer/OrganicLayer.o \
		$(SRCDIR)/ecodomain/layer/ParentLayer.o \
		$(SRCDIR)/ecodomain/layer/SnowLayer.o \
		$(SRCDIR)/ecodomain/layer/SoilLayer.o \
		$(SRCDIR)/ecodomain/Vegetation.o \
		$(SRCDIR)/input/CohortInputer.o \
		$(SRCDIR)/input/GridInputer.o \
		$(SRCDIR)/input/RegionInputer.o \
		$(SRCDIR)/input/RestartInputer.o \
		$(SRCDIR)/lookup/CohortLookup.o \
		$(SRCDIR)/lookup/SoilLookup.o \
		$(SRCDIR)/output/BgcOutputer.o \
		$(SRCDIR)/output/ChtOutputer.o \
		$(SRCDIR)/output/EnvOutputer.o \
		$(SRCDIR)/output/RegnOutputer.o \
		$(SRCDIR)/output/RestartOutputer.o \
		$(SRCDIR)/runmodule/Cohort.o \
		$(SRCDIR)/runmodule/Controller.o \
		$(SRCDIR)/runmodule/Grid.o \
		$(SRCDIR)/runmodule/Integrator.o \
		$(SRCDIR)/runmodule/ModelData.o \
		$(SRCDIR)/runmodule/OutRetrive.o \
		$(SRCDIR)/runmodule/Region.o \
		$(SRCDIR)/runmodule/Timer.o \
		$(SRCDIR)/snowsoil/Richards.o \
		$(SRCDIR)/snowsoil/Snow_Env.o \
		$(SRCDIR)/snowsoil/Soil_Bgc.o \
		$(SRCDIR)/snowsoil/Soil_Env.o \
		$(SRCDIR)/snowsoil/SoilParent_Env.o \
		$(SRCDIR)/snowsoil/Stefan.o \
		$(SRCDIR)/snowsoil/TemperatureUpdator.o \
		$(SRCDIR)/util/CrankNicholson.o \
		$(SRCDIR)/util/Interpolator.o \
		$(SRCDIR)/vegetation/Vegetation_Bgc.o \
		$(SRCDIR)/vegetation/Vegetation_Env.o
OBJECTS = \
		RunCohort.o \
		RunGrid.o \
		Runner.o \
		RunRegion.o \
		Atmosphere.o \
		AtmosUtil.o \
		BgcData.o \
		CohortData.o \
		EnvData.o \
		FirData.o \
		GridData.o \
		OutDataRegn.o \
		RegionData.o \
		RestartData.o \
		WildFire.o \
		DoubleLinkedList.o \
		Ground.o \
		Mineral.o \
		Moss.o \
		Organic.o \
		Snow.o \
		SoilParent.o \
		Vegetation.o \
		CohortInputer.o \
		GridInputer.o \
		RegionInputer.o \
		RestartInputer.o \
		CohortLookup.o \
		SoilLookup.o \
		BgcOutputer.o \
		ChtOutputer.o \
		EnvOutputer.o \
		RegnOutputer.o \
		RestartOutputer.o \
		Cohort.o \
		Controller.o \
		Grid.o \
		Integrator.o \
		ModelData.o \
		OutRetrive.o \
		Region.o \
		Timer.o \
		Richards.o \
		Snow_Env.o \
		Soil_Bgc.o \
		Soil_Env.o \
		SoilParent_Env.o \
		Stefan.o \
		CrankNicholson.o \
		Interpolator.o \
		Vegetation_Bgc.o \
		Vegetation_Env.o \
		Layer.o \
		MineralLayer.o \
		MossLayer.o \
		OrganicLayer.o \
		ParentLayer.o \
		SnowLayer.o \
		SoilLayer.o \
		TemperatureUpdator.o 
TEMOBJ=	TEM.o

tem: $(SOURCES) $(TEMOBJ)
	$(CC) -o $(EXETEM) $(INCDIR) $(OBJECTS) $(TEMOBJ) $(LIBDIR) $(LIBS)

lib: $(SOURCES) 
	$(CC) -o $(LIBTEM) -shared $(INCDIR) $(OBJECTS) $(LIBDIR) $(LIBS)

.cpp.o:  
	$(CC) $(CFLAGS) $(INCDIR) $<

clean:
	rm -f $(OBJECTS) $(EXETEM) TEM.o $(LIBTEM) *~
