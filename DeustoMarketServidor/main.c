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

    // Apertura de Base de Datos
    result = sqlite3_open("BDD/deustomarket.db", &db);
    if (result != SQLITE_OK) {
        printf("Error: No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        WSACleanup();
        return -1;
    }
    printf("Base de datos abierta correctamente.\n");

    // Creación del Socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        sqlite3_close(db);
        WSACleanup();
        return -1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // Bind y Listen
    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        sqlite3_close(db);
        WSACleanup();
        return -1;
    }

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(conn_socket);
        sqlite3_close(db);
        WSACleanup();
        return -1;
    }

    printf("Servidor DeustoMarket esperando en puerto %d...\n", SERVER_PORT);

    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

    if (comm_socket == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(conn_socket);
        sqlite3_close(db);
        WSACleanup();
        return -1;
    }

    printf("Conectado con cliente: %s\n", inet_ntoa(client.sin_addr));

    OpCode opcionRecibida;

    // 4. Bucle principal de operaciones
    while (1) {
        bytes = recv(comm_socket, (char*)&opcionRecibida, (int)sizeof(OpCode), 0);

        if (bytes <= 0 || opcionRecibida == OPC_EXIT) {
            printf("Cerrando servidor...\n");
            break;
        }

        printf("Operacion recibida: %d\n", opcionRecibida);

        switch(opcionRecibida) {

        case OPC_LOGIN: {
				LoginData datosCliente;
				recv(comm_socket, (char*)&datosCliente, sizeof(LoginData), 0);
				printf("Intento de login recibido para el usuario: %s\n", datosCliente.usuario);

				FILE *fichero = fopen("Configuracion/credenciales.txt", "r");
				int loginValido = 0; // 0 = Falso, 1 = Verdadero

				if (fichero == NULL) {
					printf("[ERROR] No se pudo abrir config.txt en el servidor.\n");
				} else {
					char linea[100];
					char userConfig[30] = "";
					char passConfig[30] = "";

					while (fgets(linea, sizeof(linea), fichero)) {
						if (strncmp(linea, "usuario=", 8) == 0) {
							sscanf(linea, "usuario=%s", userConfig);
						} else if (strncmp(linea, "contrasena=", 11) == 0) {
							sscanf(linea, "contrasena=%s", passConfig);
						}
					}
					fclose(fichero);

					if (strcmp(datosCliente.usuario, userConfig) == 0 &&
						strcmp(datosCliente.contrasena, passConfig) == 0) {
						loginValido = 1; // Credenciales correctas
					}
				}

				// Enviamos la respuesta de vuelta al cliente
				send(comm_socket, (char*)&loginValido, sizeof(int), 0);

				if (loginValido == 1) {
					printf("Login EXITOSO para %s\n", datosCliente.usuario);
				} else {
					printf("Login FALLIDO para %s\n", datosCliente.usuario);
				}
				break;
			}

            case OPC_ADD_SUPER: {
                SupermercadoData data;
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Insertando Supermercado: %s\n", data.nombre);

                sqlite3_stmt *stmt;
                char sql[] = "INSERT INTO supermercado (id, nombre, direccion) VALUES (?, ?, ?)";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_int(stmt, 1, data.id_super);
                    sqlite3_bind_text(stmt, 2, data.nombre, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 3, data.direccion, -1, SQLITE_STATIC);

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        printf("Alta realizada correctamente.\n");
                    } else {
                        printf("Error al insertar: %s\n", sqlite3_errmsg(db));
                    }
                    sqlite3_finalize(stmt);
                }
                break;
            }

            case OPC_UPDATE_SUPER: {
                SupermercadoData data;
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Modificando Supermercado ID %d...\n", data.id_super);

                sqlite3_stmt *stmt;
                char sql[] = "UPDATE supermercado SET nombre = ?, direccion = ? WHERE id = ?";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, data.nombre, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 2, data.direccion, -1, SQLITE_STATIC);
                    sqlite3_bind_int(stmt, 3, data.id_super);

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        printf("Modificacion realizada.\n");
                    }
                    sqlite3_finalize(stmt);
                }
                break;
            }

            case OPC_DEL_SUPER: {
                SupermercadoData data;
                recv(comm_socket, (char*)&data, sizeof(SupermercadoData), 0);
                printf("Eliminando Supermercado ID: %d\n", data.id_super);

                sqlite3_stmt *stmt;
                char sql[] = "DELETE FROM supermercado WHERE id = ?";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_int(stmt, 1, data.id_super);

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        printf("Eliminacion realizada.\n");
                    }
                    sqlite3_finalize(stmt);
                }
                break;
            }

            case OPC_GET_ALL_SUPER: {
                printf("Enviando lista completa de supermercados...\n");
                sqlite3_stmt *stmt;
                char sql[] = "SELECT id, nombre, direccion FROM supermercado";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        SupermercadoData dataEnvio;
                        dataEnvio.id_super = sqlite3_column_int(stmt, 0);
                        strcpy(dataEnvio.nombre, (char*)sqlite3_column_text(stmt, 1));
                        strcpy(dataEnvio.direccion, (char*)sqlite3_column_text(stmt, 2));

                        send(comm_socket, (char*)&dataEnvio, sizeof(SupermercadoData), 0);
                    }
                    sqlite3_finalize(stmt);
                }

                // Enviar centinela de fin
                SupermercadoData fin;
                fin.id_super = -1;
                send(comm_socket, (char*)&fin, sizeof(SupermercadoData), 0);
                printf("Lista enviada con exito.\n");
                break;
            }


            default:
                printf("Codigo de operacion no reconocido: %d\n", opcionRecibida);
                break;
        }
    }

    // Cierre y Limpieza
    closesocket(comm_socket);
    closesocket(conn_socket);
    sqlite3_close(db);
    WSACleanup();

    return 0;
}
