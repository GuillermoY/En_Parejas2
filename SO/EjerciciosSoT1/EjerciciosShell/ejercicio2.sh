#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Uso: $0 <ruta>"
	exit 1
fi

if [ -f "$1" ]; then
	lineas=$(wc -l < "$1")
	echo "El fichero $1 tiene $lineas líneas"
else
	echo "El fichero $1 no existe o no es regular"
fi
