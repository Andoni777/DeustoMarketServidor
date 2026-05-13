/*
 * Gerente.cpp
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */

#include "Gerente.h"
#include <string.h>
#include <iostream>
using namespace std;


Gerente::Gerente() : Empleado() {
	cout << "Constructor Gerente Vacio" << endl;
    this->departamento = NULL;
    this->personas_a_cargo = 0;
}

Gerente::Gerente(const char *dni, const char *nombre, int id_supermercado, const char* departamento, int personas_a_cargo) : Empleado(dni, nombre, id_supermercado) {
    cout << "Constructor Gerente" << endl;
    this->departamento = new char[strlen(departamento) + 1];
    strcpy(this->departamento, departamento);
    this->personas_a_cargo = personas_a_cargo;
}

Gerente::Gerente(const Gerente &g) : Empleado(g) {
    cout << "Constructor Copia Gerente" << endl;
    this->departamento = new char[strlen(g.departamento) + 1];
    strcpy(this->departamento, g.departamento);
    this->personas_a_cargo = g.personas_a_cargo;
}


Gerente::~Gerente() {
    cout << "Destructor Gerente" << endl;
    delete[] departamento;
}

void Gerente::mostrarInformacion() {
    cout << "DNI: " << this->dni << " | Nombre: " << this->nombre
         << " | ID Super: " << this->id_supermercado
         << " | Deartamento: " << this->departamento
         << " | Personas a cargo: " << this->personas_a_cargo << endl;
}

void Gerente::setDepartamento(const char* departamento) {
    if (this->departamento != NULL) {
        delete[] this->departamento;
    }
    this->departamento = new char[strlen(departamento) + 1];
    strcpy(this->departamento, departamento);
}

const char* Gerente::getDepartamento() {
    return this->departamento;
}

void Gerente::setPersonasACargo(int personas_a_cargo) {
    this->personas_a_cargo = personas_a_cargo;
}

int Gerente::getPersonasACargo() {
    return this->personas_a_cargo;
}


