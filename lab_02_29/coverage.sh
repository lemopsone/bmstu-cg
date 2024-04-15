#!/bin/bash

unit_test_output=$(./build/"$1")
llvm-profdata merge -sparse default.profraw -o default.profdata
llvm_export_output=$(llvm-cov export ./build/"$1" -instr-profile=default.profdata -ignore-filename-regex=".*/(ui_|moc_).*")
llvm_report_output=$(llvm-cov report ./build/"$1" -instr-profile=default.profdata -ignore-filename-regex=".*/(ui_|moc_).*")
percent=$(echo "$llvm_export_output" | jq '.data[].totals.lines.percent')


printf "Coverage: %.2f%%\n\n" "$percent"
echo "LLVM-COV REPORT:"
echo "$llvm_report_output"

if [ "$1" == "unit" ]; then
  echo "UNIT TESTS OUTPUT:"
  echo "$unit_test_output"
fi
