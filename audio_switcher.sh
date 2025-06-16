#!/bin/bash
#PRVI_IZVOR="$(pactl get-default-source)"
#TEMP="$(pactl list short sinks | grep "audio")"
BROJ_LINIJA="$(pactl list short sinks | awk '{for (i=1; i<=NF; i++) print $i}' | wc -l)" 
LISTA=($(pactl list short sinks | awk '{for (i=1; i<=NF; i++) print $i}'))
for ((j=0; j<=BROJ_LINIJA; j++)); do
if (($j % 7==1)) && [[ ${LISTA[6]} == "SUSPENDED" ]]; then
        $(pactl set-default-sink ${LISTA[1]})
        break
    elif (($j % 7==1)) && [[ ${LISTA[6]} != "SUSPENDED" ]]; then
        $(pactl set-default-sink ${LISTA[8]})

        break
        
    fi
done
