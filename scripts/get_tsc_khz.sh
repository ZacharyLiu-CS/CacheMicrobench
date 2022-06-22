#!/bin/bash
tsc_khz_freq=$(sudo gdb /dev/null /proc/kcore -ex 'x/uw 0x'$(grep '\<tsc_khz\>' /proc/kallsyms | cut -d' ' -f1) -batch 2>/dev/null | tail -n 1 | cut -f2)
echo TSC KHZ freq : ${tsc_khz_freq}
sed -i "s/#define TSC_KHz.*/#define TSC_KHz ${tsc_khz_freq}/g" $1
