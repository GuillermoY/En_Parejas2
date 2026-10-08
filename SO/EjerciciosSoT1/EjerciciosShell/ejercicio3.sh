#!/bin/bash

for fichero in "$1"/*
do
	if [ -f "$fichero" ]; then
		palabras=$(wc -w < "$fichero")
		echo "$fichero $palabras"
	fi
done
