#!/bin/sh
cc='g++ -std=c++17'
#cc='clang'
#cc='clang -c -DSTC_HEADER'
#cc='cl -nologo'
#cc='cl -nologo -TP'
#cc='cl -nologo -std:c11'
run=0
if [ "$1" == '-h' -o "$1" == '--help' ]; then
  echo usage: runall.sh [-run] [compiler + options]
  exit
fi
if [ "$1" == '-run' ]; then
  run=1
  shift
fi
if [ ! -z "$1" ] ; then
    cc=$@
fi
if [ $run = 0 ] ; then
    for i in *.cpp ; do
        echo $cc -I../include $i
        $cc -I../include $i
    done
else
    for i in *.c ; do
        echo $cc -I../include $i
        $cc -I../include $i
        if [ -f $(basename -s .c $i).exe ]; then ./$(basename -s .c $i).exe; fi
        if [ -f ./a.exe ]; then ./a.exe; fi
        if [ -f ./a.out ]; then ./a.out; fi
    done
fi

rm -f a.out *.o *.obj *.exe
