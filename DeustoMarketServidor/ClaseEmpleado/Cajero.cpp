/*
 * Cajero.cpp
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */


#include "Cajero.h"
#include <iostream>
using namespace std;

Cajero::Cajero() : Empleado(){
	cout << "Constructor Cajero Vacio" << endl;
	this->numCaja = 0;
}

Cajero::Cajero(const char *dni, const char *nombre, int id_supermercado, int numCaja) : Empleado(dni, nombre, id_supermercado) {
    cout << "Constructor Cajero" << endl;
    this->numCaja = numCaja;
}

Cajero::Cajero(const Cajero &c) {
	cout << "Constructor Copia Cajero" << endl;
	this->numCaja = c.numCaja;
}

Cajero::~Cajero(){
	cout << "Destructor Cajero" << endl;
}

void Cajero::mostrarInformacion(){
	cout << "DNI: " << this->dni << " | Nombre: " << this->nombre
	     << " | ID Super: " << this->id_supermercado
	     << " | Caja: " << this->numCaja << endl;
	}

void Cajero::setNumCaja(int numCaja){
	this->numCaja = numCaja;
}

int Cajero::getNumcaja(){
	return this->numCaja;
}
