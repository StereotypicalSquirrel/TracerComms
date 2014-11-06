#!/bin/bash

#This is an example of how to send data to the data.sparkfun.com service
#This script can be called from cron

#PublicURL=http://data.sparkfun.com/streams/[publickeyhere]
PublicKey=[publickeyhere]
PrivateKey=[privatekeyhere]
#DeleteKey=[deletekeyhere]

tracerData=$(./TracerComms)

#Only continue if the tracer Data has exactly 11 commas in it
if [ $(echo $tracerData | tr -cd , | wc -c) -eq 11 ]; then
  batteryvoltage=$(echo $tracerData|cut -d, -f1)
  solarvoltage=$(echo $tracerData|cut -d, -f2)
  chargecurrent=$(echo $tracerData|cut -d, -f3)
  loadcurrent=$(echo $tracerData|cut -d, -f4)
  temperature=$(echo $tracerData|cut -d, -f5)
  loaddetected=$(echo $tracerData|cut -d, -f6)
  overloaded=$(echo $tracerData|cut -d, -f7)
  shortcircuit=$(echo $tracerData|cut -d, -f8)
  overcharged=$(echo $tracerData|cut -d, -f9)
  batterylow=$(echo $tracerData|cut -d, -f10)
  batteryfull=$(echo $tracerData|cut -d, -f11)
  charging=$(echo $tracerData|cut -d, -f12)

  wget -qO- https://data.sparkfun.com/input/$PublicKey?private_key=$PrivateKey\&batteryvoltage=$batteryvoltage\&solarvoltage=$solarvoltage\&loadcurrent=$loadcurrent\&loaddetected=$loaddetected\&overloaded=$overloaded\&shortcircuit=$shortcircuit\&overcharged=$overcharged\&batterylow=$batterylow\&batteryfull=$batteryfull\&charging=$charging\&temperature=$temperature\&chargecurrent=$chargecurrent &> /dev/null
fi
