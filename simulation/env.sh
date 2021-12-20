#!/bin/bash

#  $Id: envGMC.sh 268 2013-10-31 17:21:49Z tassielli $
#  $Author:  $
#  $Revision:  $

# base PRJ
if [ -v PRJBASE ];
then
	echo "PRJ BASE already set to "${PRJBASE}
else
	export PRJBASE="/mnt/c/workSpace/sw/APPNUWS"
fi

#gcc xml

#geant4
export G4INSTBASE=/mnt/c/workSpace/sw/Geant4/10.07.p01/lx/install
#cd $G4INSTBASE/share/Geant4-10.1.3/geant4make
cd $G4INSTBASE/share/Geant4-10.7.1/geant4make
source geant4make.sh
cd -
#export CLHEP_VERSION=2.4.0.4

#set the workdir for g4GMC
#export homedir=`pwd`
export G4WORKDIR=${PRJBASE}/simulation/build
export LD_LIBRARY_PATH=${G4WORKDIR}/lib:${LD_LIBRARY_PATH}

export HEPMC_DIR=/mnt/c/workSpace/sw/HepMC/2.06.11/install-gcc8
#export HEPMC_DIR=/mnt/c/workSpace/sw/HepMC/3.2.2/hepmc3-install
export PYTHIA6=/mnt/c/workSpace/sw/Pythia/6.4.28/install/lib
export PYTHIA6_VERSION=6.4.28
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HEPMC_DIR/lib:$PYTHIA6

#rome
#if [ -v ROMESYS ]
#then
#	echo "ROME already set to "${ROMESYS}
#else
# export ROMESYS=/mnt/c/workSpace/sw/ROME/rome-v3.2.15.1
# export PATH=$ROMESYS/bin:${PATH}
# export LIBROME=yes
#fi

#root
if [ -v ROOTSYS ]
then
        echo "ROOT already set to "${ROOTSYS}
else
        source /mnt/c/workSpace/sw/ROOT/v6.22.06/lx/install/bin/thisroot.sh
##export ROOTSYS=/pro/root_v5_34_30
#export PATH=$ROOTSYS/bin:${PATH}
#export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}
fi

#midas
#export MIDASSYS=/mnt/sndhd/sw/meg/midas
#export MIDAS_DIR=/mnt/sndhd/sw/meg/midas
#export MIDAS_EXPTAB=$MIDAS_DIR/exptab
#export MIDAS_EXPT_NAME=FIRB_DAQ
#export PATH=${MIDASSYS}/linux/bin:$PATH
#export LD_LIBRARY_PATH=${MIDASSYS}/linux/lib:${LD_LIBRARY_PATH}

#GMC
if [ -v GMCDIR ]
then
	echo "GMCDIR already set to "${GMCDIR}
else
	export GMCDIR=${PRJBASE}/analyzer/GMC
	export LD_LIBRARY_PATH=${GMCDIR}/obj:${LD_LIBRARY_PATH}
fi
