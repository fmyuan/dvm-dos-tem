#!/bin/bash
swig -java -c++ -module temcore -o TEMwrapper.cpp -outdir ../../DVMDOSTEMrunjava/src/TEMJNI -package TEMJNI TEMrun.i
