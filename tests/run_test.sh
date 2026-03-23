#!/bin/sh

TIGFILE=$1

if echo "$TIGFILE" | grep -q "/good/"; then
  $TC --bindings-compute "$TIGFILE" 2>/dev/null
  if [ $? -eq 0 ]; then
    exit 0
  else
    exit 1
  fi
elif echo "$TIGFILE" | grep -q "/minimal/"; then
  $TC --parse "$TIGFILE" 2>/dev/null
  if [ $? -eq 0 ]; then
    exit 0
  else
    exit 1
  fi
elif echo "$TIGFILE" | grep -q "/syntax/"; then
  $TC --parse "$TIGFILE" 2>/dev/null
  RES=$?
  if [ $RES -eq 3 ]||[ $RES -eq 2 ]; then
    exit 0
  else
    exit 1
  fi
elif echo "$TIGFILE" | grep -q "/bind/"; then
  $TC --bindings-compute "$TIGFILE" 2>/dev/null
  if [ $? -eq 4 ]; then
    exit 0
  else
    exit 1
  fi
elif echo "$TIGFILE" | grep -q "/type/"; then
  $TC --type-check "$TIGFILE" 2>/dev/null
  if [ $? -eq 5 ]; then
    exit 0
  else
    exit 1
  fi
else
  exit 99
fi
