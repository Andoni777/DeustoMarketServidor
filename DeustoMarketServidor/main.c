#include <winsock2.h>
#include <stdio.h>
#include "BDD/sqlite3.h"
#include <string.h>
#include "Protocolo.h"

#define SERVER_PORT 6000

int main(int argc, char *argv[]) {

    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    sqlite3 *db;
    int result;
    int bytes;

    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }

    result = sqlite3_open("BDD/deustomarket.db", &db);
        if (result != SQLITE_OK) {
            printf("Error: No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
            WSACleanup();
            return -1;
        }
        printf("Base de datos abierta correctamente.\n");

    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server.sin_addr.s_addr = INADDR_ANY; // Cualquier IP de este dispositivo
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Servidor DeustoMarket esperando en puerto %d...\n", SERVER_PORT);

    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

    if (comm_socket == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Conectado con cliente: %s\n", inet_ntoa(client.sin_addr));

    OpCode opcionRecibida;

    while (1) {
        bytes = recv(comm_socket, (char*)&opcionRecibida, (int)sizeof(OpCode), 0);

        if (bytes <= 0 || opcionRecibida == OPC_EXIT) {
            printf("Cerrando servidor...\n");
            break;
        }

        printf("Operacion recibida: %d\n", opcionRecibida);

        switch(opcionRecibida) {

            case OPC_ADD_SUPER: {
                SupermercadoData data;
                // Recibimos el bloque de datos del supermercado
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Recibida peticion Alta Supermercado: %s\n", data.nombre);

                // Llamamos a la funcion de la Fase 1 (adaptada)
                // insertarSupermercadoBDD(data);
                break;
            }

            case OPC_UPDATE_SUPER: {
                SupermercadoData data;
                // Recibimos el bloque de datos del supermercado
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Recibida peticion Alta Supermercado: %s\n", data.nombre);

                // Llamamos a la funcion de la Fase 1 (adaptada)
                // insertarSupermercadoBDD(data);
                break;
            }

            case OPC_DEL_SUPER: {
                SupermercadoData data;
                // Recibimos el bloque de datos del supermercado
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Recibida peticion Alta Supermercado: %s\n", data.nombre);

                // Llamamos a la funcion de la Fase 1 (adaptada)
                // insertarSupermercadoBDD(data);
                break;
            }

            case OPC_GET_ALL_SUPER: {
            	printf("Enviando lista completa de supermercados...\n");

            	    sqlite3_stmt *stmt;
            	    char sql[] = "SELECT id, nombre, direccion FROM supermercado";

            	    // 1. Preparamos la consulta
            	    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            	        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            	        break;
            	    }

            	    // 2. Iteramos por cada fila que devuelva la base de datos
            	    while (sqlite3_step(stmt) == SQLITE_ROW) {
            	        SupermercadoData dataEnvio;

            	        // Extraemos los datos de la columna y los metemos en el struct
            	        dataEnvio.id_super = sqlite3_column_int(stmt, 0);
            	        strcpy(dataEnvio.nombre, (char*)sqlite3_column_text(stmt, 1));
            	        strcpy(dataEnvio.direccion, (char*)sqlite3_column_text(stmt, 2));

            	        // Enviamos el struct al cliente
            	        send(comm_socket, (char*)&dataEnvio, sizeof(SupermercadoData), 0);
            	    }

            	    // Ponemos el ID en -1 para que el cliente sepa que es el final
            	    SupermercadoData fin;
            	    fin.id_super = -1;
            	    send(comm_socket, (char*)&fin, sizeof(SupermercadoData), 0);

            	    sqlite3_finalize(stmt); // Cerramos el statement
            	    printf("Lista enviada con éxito.\n");
            	    break;
            }

            case OPC_ADD_EMPLEADO: {
                EmpleadoData data;
                recv(comm_socket, (char*)&data, sizeof(EmpleadoData), 0);
                printf("Recibida peticion Alta Empleado: %s (DNI: %s)\n", data.nombre_empleado, data.dni_empleado);

                // insertarEmpleadoBDD(data);
                break;
            }




            default:
                printf("Codigo de operacion no reconocido.\n");
                break;
        }
    }

    closesocket(comm_socket);
    WSACleanup();
    closesocket(conn_socket);
    return 0;
}
