#!/bin/bash

BD="agenda.txt"

#crear base datos

touch "$BD"

function listar(){
	if [ ! -s "$BD" ]; then
		echo "Agenda vacía"
		return
	fi
	
	while IFS=: read -r nombre telefono mail
	do
		echo "Nombre: $nombre"
		echo "Teléfono: $telefono"
		echo "Mail: $mail"
		echo
	done < "$BD"
}

function buscar(){
	read -p "Buscar: " patron

	while IFS=: read -r nombre telefono mail
	do
		if [[ "$nombre" == *"$patron"* ]] ||
		[[ "$telefono" == *"$patron"* ]] ||
		[[ "$mail" == *"$patron"* ]]; then

		echo "Nombre: $nombre"
		echo "Teléfono: $telefono"
		echo "Mail: $mail"
		echo
	fi
	done < "$BD"
}

function borrar(){
	read -p "Nombre: " nombre

	if grep -q "^$nombre:" "$BD"; then
		grep -v "^$nombre:" "$BD" > "$BD.tmp"
		mv "$BD.tmp" "$BD"
		echo "Registro borrado."
	else
		echo "No existe ningún registro con ese nombre."
	fi
}

function anadir() {
	read -p "Nombre: " nombre
	read -p "Teléfono: " telefono
	read -p "Mail: " mail

	echo "$nombre:$telefono:$mail" >> "$BD"

	echo "Registro añadido"
}

function menu() {
	select opt in "listar" "buscar" "borrar" "añadir" "salir"
	do
		case $opt in "listar")
		listar;;
		"buscar")
		buscar;;
		"borrar")
		borrar;;
		"añadir")
		anadir;;
		"salir")
		echo "Saliendo..."
		break;;
		*)
		echo "Error en opción";;
		esac
	done
}

menu


