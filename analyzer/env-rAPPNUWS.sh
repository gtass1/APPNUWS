#!/bin/bash

# base PRJ
if [ -v PRJBASE ];
then
    echo "PRJ BASE already set to "${PRJBASE}
else
    export PRJBASE="/workSpace/sw/APPNUWS"
fi
export PATH=$PRJBASE/analyzer/rAPPNUWS:$PATH

#root
if [ -v ROOTSYS ]
then
    echo "ROOT already set to "${ROOTSYS}
else
    export ROOTSYS=path_to_root
#    export ROT
    export PATH=$ROOTSYS/bin:${PATH}
    export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}
fi

# genfit2
if [ -v GENFIT2SYS ]
then
    echo "Genfit already set to "${GENFIT2SYS}
else
#    export GENFIT2SYS=path_to_genfit2
    export GENFIT2SYS=/workSpace/sw/GenFit/02-00-00/install
    export LD_LIBRARY_PATH=${GENFIT2SYS}/lib64:${LD_LIBRARY_PATH}
    export ROOT_INCLUDE_PATH=${ROOT_INCLUDE_PATH}:${GENFIT2SYS}/include
    echo "GENFIT2SYS is set to "${GENFIT2SYS}
fi

# eigen
if [ -v EIGEN3SYS ]
then
    echo "Eigen3 already set to "${EIGEN3SYS}
else
    export EIGEN3SYS=/usr
    export ROOT_INCLUDE_PATH=${ROOT_INCLUDE_PATH}:${EIGEN3SYS}/include/eigen3
    echo "EIGEN3SYS is set to "${EIGEN3SYS}
fi

# rome
if [ -v ROMESYS ]
then
    echo "ROME already set to "${ROMESYS}
else
#    export ROMESYS=path_to_rome
    export ROMESYS=/workSpace/sw/ROME/rome-v3.2.17
    export PATH=$ROMESYS/bin:${PATH}
    export LIBROME=yes
    echo "ROME is set to "${ROMESYS}
fi

#rAPPNUWS
if [ -v RAPPNSDIR ]
then
    echo "rAPPNUWS already set to "${RAPPNSDIR}
else
    export RAPPNSDIR=${PRJBASE}/analyzer/rAPPNUWS
    export LD_LIBRARY_PATH=${RAPPNSDIR}/obj:${LD_LIBRARY_PATH}
    echo "rAPPNSDIR is set to "${RAPPNSDIR}
fi

#zlib
for path in ${CMAKE_PREFIX_PATH//:/ }; do
    if  grep -q 'zlib' <<< "$path"  ; then
    export ZLIB_DIR=$path/lib
    fi
done
echo "ZLIB_DIR is set to "${ZLIB_DIR}

# base G4WORKDIR
if [ -v G4WORKDIR ];
then
	echo "G4 WORKDIR already set to "${G4WORKDIR}
else
#    export G4WORKDIR=$SIM_INSTAL_DIR
    export G4WORKDIR=${PRJBASE}/simulation/build
    export LD_LIBRARY_PATH=${G4WORKDIR}/lib:${LD_LIBRARY_PATH}
fi

# G4INCLUDE
if [ -v G4INCLUDE ];
then
	echo "G4 INCLUDE already set to "${G4INCLUDE}
else
    export G4INCLUDE=${G4INSTALL}/include
fi

# G4LIB
if [ -v G4LIB ];
then
	echo "G4 LIB already set to "${G4LIB}
else
    export G4LIB=${G4INSTALL}/lib
fi