#!/bin/sh

## using 'swig' to generate JNI based c++ wrapper to be calling from Java interface

export jrunnerdir=/cygwindrive/c/workspace_tem/DVMDOSTEMrunjava/src

swig -java -c++ -module temcore -o TEMwrapper.cpp -outdir $jrunnerdir/TEMJNI -package TEMJNI TEMrun.i

