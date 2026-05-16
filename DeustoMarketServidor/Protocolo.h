/*
 * Protocolo.h
 *
 *  Created on: 15 may 2026
 *      Author: andoni.g
 */

#ifndef DOMAIN_PROTOCOLO_H_
#define DOMAIN_PROTOCOLO_H_

// Códigos de operacion
typedef enum{
	// Login
	OPC_LOGIN,
	// Supermercados
	OPC_ADD_SUPER, OPC_UPDATE_SUPER, OPC_DEL_SUPER, OPC_GET_ALL_SUPER,

	// Empleados
	OPC_ADD_EMPLEADO, OPC_DEL_EMPLEADO, OPC_UPDATE_EMPLEADO, OPC_GET_ALL_EMPLEADO, OPC_GET_ALL_EMPLEADO_BY_SUPER,

	// Inventario / Productos
	OPC_ADD_PRODUCTO, OPC_DEL_PROD, OPC_UPDATE_PRICE, OPC_UPDATE_STOCK_SUPER, OPC_GET_STOCK_SUPER,

	OPC_EXIT
}OpCode;

// Estructura para enviar datos
typedef struct {
	char usuario[30];
	char contrasena[30];
} LoginData;

typedef struct {
	int id_super;
	char nombre[31];
	char direccion[41];
} SupermercadoData;

typedef struct {
	char dni_empleado [10];
	char nombre_empleado [21];
	char puesto [31];
	int id_super;
} EmpleadoData;

typedef struct {
	int id_producto;
	char nombre_producto [31];
	float precio_producto;
	int id_categoria;
} ProductoData;

typedef struct {
	int id_super;
	int id_producto;
	int stock;
} InverntarioData;
#endif
