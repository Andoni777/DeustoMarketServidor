/*
 * Empleado.cpp
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */

#include "Empleado.h"
#include <string.h>
#include <iostream>
using namespace std;

Empleado::Empleado() {
    cout << "Constructor Empleado vacio" << endl;
    this->dni = NULL;
    this->nombre = NULL;
    this->id_supermercado = 0;
}

Empleado::Empleado(const char *dni, const char *nombre, int id_supermercado) {
    cout << "Constructor Empleado" << endl;

    this->dni = new char[strlen(dni) + 1];
    strcpy(this->dni, dni);

    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);

    this->id_supermercado = id_supermercado;
}

Empleado::Empleado(const Empleado& e) {
    cout << "Constructor Copia Empleado" << endl;

    this->dni = new char[strlen(e.dni) + 1];
    strcpy(this->dni, e.dni);

    this->nombre = new char[strlen(e.nombre) + 1];
    strcpy(this->nombre, e.nombre);

    this->id_supermercado = e.id_supermercado;
}

Empleado::~Empleado() {
    cout << "Destructor Empleado" << endl;
    delete[] dni;
    delete[] nombre;
}

void Empleado::mostrarInformacion() {
    cout << "DNI: " << this->dni << " | Nombre: " << this->nombre
         << " | ID Super: " << this->id_supermercado << endl;
}

const char* Empleado::getDni() {
	return this->dni;
}

const char* Empleado::getNombre() {
	return this->nombre;
}

int Empleado::getIdSupermercado() {
	return this->id_supermercado;
}

void Empleado::setDni(const char* dni) {
    if (this->dni != NULL) delete[] this->dni;
    this->dni = new char[strlen(dni) + 1];
    strcpy(this->dni, dni);
}

void Empleado::setNombre(const char* nombre) {
    if (this->nombre != NULL) delete[] this->nombre;
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
}

void Empleado::setIdSupermercado(int id) {
    this->id_supermercado = id;
}
