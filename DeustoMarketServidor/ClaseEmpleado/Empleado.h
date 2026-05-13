/*
 * Empleado.h
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */

#ifndef CLASEEMPLEADO_EMPLEADO_H_
#define CLASEEMPLEADO_EMPLEADO_H_

class Empleado {
private:
    char *dni;
    char *nombre;
    int id_supermercado;

public:
    Empleado();
    Empleado(const char* dni, const char* nombre, int id_supermercado);
    Empleado(const Empleado &e);
    virtual ~Empleado();


    virtual void mostrarInformacion();


    const char* getDni();
    const char* getNombre();
    int getIdSupermercado();


    void setDni(const char* dni);
    void setNombre(const char* nombre);
    void setIdSupermercado(int id);
};

#endif /* CLASEEMPLEADO_EMPLEADO_H_ */
