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
  $TC --types-compute "$TIGFILE" 2>/dev/null
  if [ $? -eq 5 ]; then
    exit 0
  else
    exit 1
  fi
elif echo "$TIGFILE" | grep -q "/llvm/"; then
  BASEDIR=$(dirname "$TIGFILE")
  BASE=$(basename "$TIGFILE" .tig)
  EXPECTED="$BASEDIR/$BASE.expected"
  TMPLL=$(mktemp /tmp/tc_llvm_XXXXXX.ll)
  TMPBIN=$(mktemp /tmp/tc_llvm_XXXXXX)
  cleanup() { rm -f "$TMPLL" "$TMPBIN"; }
  trap cleanup EXIT

  if ! $TC --llvm-runtime-display --llvm-display "$TIGFILE" > "$TMPLL" 2>/dev/null; then
    exit 1
  fi
  if ! clang -m64 "$TMPLL" -o "$TMPBIN" 2>/dev/null; then
    exit 1
  fi
  ACTUAL=$("$TMPBIN" 2>/dev/null)
  if [ -f "$EXPECTED" ]; then
    EXPECTED_CONTENT=$(cat "$EXPECTED")
    if [ "$ACTUAL" = "$EXPECTED_CONTENT" ]; then
      exit 0
    else
      exit 1
    fi
  else
    exit 0
  fi
else
  exit 99999
fi
