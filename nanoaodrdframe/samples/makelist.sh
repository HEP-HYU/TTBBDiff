#!/bin/bash
voms-proxy-init --voms cms
while read -r arg1 arg2 arg3
do
    if [ ! -d "${arg1}" ]
    then
        mkdir -p ${arg1}
    fi
    /cvmfs/cms.cern.ch/common/dasgoclient -query="file dataset=${arg3} instance=prod/phys03" > ${arg1}/${arg2}.txt
done < $1
