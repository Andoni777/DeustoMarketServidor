/*
 * Gerente.h
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */

#ifndef CLASEEMPLEADO_GERENTE_H_
#define CLASEEMPLEADO_GERENTE_H_

#include "Empleado.h"

class Gerente : public Empleado {
private:
    char *departamento;
    int personas_a_cargo;

public:
    Gerente();
    Gerente(const char* dni, const char* nombre, int id_supermercado, const char* departamento, int personas_a_cargo);
    Gerente(const Gerente &g);
    virtual ~Gerente();

    virtual void mostrarInformacion();

    void setDepartamento(const char* departamento);
    const char* getDepartamento();

    void setPersonasACargo(int personas_a_cargo);
    int getPersonasACargo();

};

#endif /* CLASEEMPLEADO_GERENTE_H_ */
