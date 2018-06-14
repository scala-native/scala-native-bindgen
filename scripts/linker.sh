#!/bin/bash

args=("$@")
libs=()

for ((i=0; i<"${#args[@]}"; ++i)); do
  case ${args[i]} in
    -lbindgentests)
      libs+="${args[i]}"
      unset args[i]
      break
      ;;
  esac
done

ld "${args[@]}" "${libs[@]}"
