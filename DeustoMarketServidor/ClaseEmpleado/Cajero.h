/*
 * Cajero.h
 *
 *  Created on: 13 may 2026
 *      Author: ander.iglesias.ruiz
 */

#ifndef CLASEEMPLEADO_CAJERO_H_
#define CLASEEMPLEADO_CAJERO_H_

#include "Empleado.h"

class Cajero : public Empleado{
private:
	int numCaja;
public:
	Cajero();
	Cajero(const char* dni, const char* nombre, int id_supermercado, int numCaja);
	Cajero(const Cajero &c);
	virtual ~Cajero();

	virtual void mostrarInformacion();

	void setNumCaja(int numCaja);
	int getNumcaja();
};

#endif /* CLASEEMPLEADO_CAJERO_H_ */
