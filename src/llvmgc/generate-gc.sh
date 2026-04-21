#!/bin/sh

[ -z "$1" ] && echo "$0: Missing input file" && exit
[ -z "$2" ] && echo "$0: Missing output file" && exit

input=$1
output=$2
fn=$3

awk 'BEGIN {
      print("#include <string>");
      print("#include <llvmgc/libllvmgc.hh>");
      print("");
      print("namespace llvmgc");
      print("{");
      print("const char* '$fn'()");
      print("{");
      print("  return");
      printf("R\"0("); # The zero here only serves as an identifier for the raw string, it does not changes the code behavior at all.
    }
    /^#(<<|>>)/ {
      next;
    }
    {
      print($0);
    }
    END {
      print(")0\";");
      print("}");
      print("} // namespace llvmgc");
    }' "$input" > "$output".tmp

mv "$output".tmp "$output"
