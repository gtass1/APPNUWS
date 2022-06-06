#!/bin/bash

G4VER=$( echo $GEANT4_VERSION | sed -e 's/v\(.*\)-.*/\1/' )

#echo $G4VER

if [ -v G4ABLADATA ];
then
	echo "G4ABLADATA BASE already set to "${G4ABLADATA}
else
	export G4ABLADATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4ABLA*)
fi

if [ -v G4ENSDFSTATEDATA ];
then
	echo "G4ENSDFSTATEDATA BASE already set to "${G4ENSDFSTATEDATA}
else
	export G4ENSDFSTATEDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4ENSDFSTATE*)
fi

if [ -v G4INCLDATA ];
then
	echo "G4INCLDATA BASE already set to "${G4INCLDATA}
else
	export G4INCLDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4INCL*)
fi

if [ -v G4LEDATA ];
then
	echo "G4LEDATA BASE already set to "${G4LEDATA}
else
	export G4LEDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4EML*)
fi

if [ -v G4LEVELGAMMADATA ];
then
	echo "G4LEVELGAMMADATA BASE already set to "${G4LEVELGAMMADATA}
else
	export G4LEVELGAMMADATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/PhotonEvaporation*)
fi

if [ -v G4NEUTRONHPDATA ];
then
	echo "G4NEUTRONHPDATA BASE already set to "${G4NEUTRONHPDATA}
else
	export G4NEUTRONHPDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4NDL*)
fi

if [ -v G4PARTICLEXSDATA ];
then
	echo "G4PARTICLEXSDATA BASE already set to "${G4PARTICLEXSDATA}
else
	export G4PARTICLEXSDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4PARTICLEXS*)
fi

if [ -v G4PIIDATA ];
then
	echo "G4PIIDATA BASE already set to "${G4PIIDATA}
else
	export G4PIIDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4PII*)
fi

if [ -v G4RADIOACTIVEDATA ];
then
	echo "G4RADIOACTIVEDATA BASE already set to "${G4RADIOACTIVEDATA}
else
	export G4RADIOACTIVEDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/RadioactiveDecay*)
fi

if [ -v G4REALSURFACEDATA ];
then
	echo "G4REALSURFACEDATA BASE already set to "${G4REALSURFACEDATA}
else
	export G4REALSURFACEDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/RealSurface*)
fi

if [ -v G4SAIDXSDATA ];
then
	echo "G4SAIDXSDATA BASE already set to "${G4SAIDXSDATA}
else
	export G4SAIDXSDATA=$(echo ${G4INSTALL}/share/Geant4-${G4VER}/data/G4SAIDDATA*)
fi
