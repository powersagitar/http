#!/bin/zsh

which doxygen >/dev/null

if [[ $? -ne 0 ]]; then
  echo 'Doxygen must be installed to generate documentation.'
  exit -1
fi

doxyfile="Doxyfile"

if [[ ! -f "$doxyfile" ]]; then
  echo 'Please run the script in the directory of Doxyfile'
  exit -1
fi

doxygen "$doxyfile"
