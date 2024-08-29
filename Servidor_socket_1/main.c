#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

//funciones para generar tanto nombres de usuario como contraseñas
void generarNombreUsuario(char *nombre, int longitud) {
    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int esVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            nombre[i] = vocales[rand() % strlen(vocales)];
        } else {
            nombre[i] = consonantes[rand() % strlen(consonantes)];
        }
        esVocal = !esVocal;
    }

    nombre[longitud] = '\0';
}

void generarContrasena(char *contrasena, int longitud) {
    char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
}

void recibir_datos(SOCKET sock, char *buffer) {
    int recv_size;
    memset(buffer, 0, BUFFER_SIZE);
    if ((recv_size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) == SOCKET_ERROR) {
        perror("Error al recibir datos");
        exit(EXIT_FAILURE);
    }
    buffer[recv_size] = '\0'; // Asegurarse de que el buffer esté terminado en nulo
}

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address, cliente;
    int longitud_cliente;
    char buffer[BUFFER_SIZE];
    char nombre[50];
    char pass[50];
    char longitud[50];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Error en WSAStartup");
        exit(EXIT_FAILURE);
    }

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Error al crear el socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Vincular el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en bind");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Servidor en espera de conexion......\n");

    // Escuchar conexiones entrantes
    listen(server_fd, 5);
    longitud_cliente = sizeof(cliente);
    new_socket = accept(server_fd, (struct sockaddr*)&cliente, &longitud_cliente);

    while (1) {
        printf("Cliente Activo\n");

        // Recepción del mensaje del cliente al Servidor
        printf("Esperando respuesta del cliente...\n");
        recibir_datos(new_socket, buffer);

        // Verificar si se recibió alguna opción válida
        if (strlen(buffer) == 0) {
            continue;
        }

        // Generar Usuario/Contraseña
        if (strcmp(buffer, "1") == 0) {
            printf("--Creando nombre de usuario--\n");
            recibir_datos(new_socket, longitud);

            int len = atoi(longitud);
            if (len >= 5 && len <= 15) {
                generarNombreUsuario(nombre, len);
                printf("Nombre: %s\n", nombre);
                strcpy(buffer, nombre);
                send(new_socket, buffer, (int)strlen(buffer), 0);
            } else {
                printf("Error de longitud: La longitud %d no es valida. Debe estar entre 5 y 15.\n", len);
                strcpy(buffer, "Error de longitud");
                send(new_socket, buffer, (int)strlen(buffer), 0);
            }
        } else if (strcmp(buffer, "2") == 0) {
            printf("--Creando Contraseña--\n");
            recibir_datos(new_socket, longitud);

            int len = atoi(longitud);
            if (len >= 8 && len <= 50) {
                generarContrasena(pass, len);
                printf("Contraseña: %s\n", pass);
                strcpy(buffer, pass);
                send(new_socket, buffer, (int)strlen(buffer), 0);
            } else {
                printf("Error de longitud: La longitud %d no es valida. Debe estar entre 8 y 50.\n", len);
                strcpy(buffer, "Error de longitud");
                send(new_socket, buffer, (int)strlen(buffer), 0);
            }
        } else {
            strcpy(buffer, "Opción no válida");
            send(new_socket, buffer, (int)strlen(buffer), 0);
        }
    }

    closesocket(new_socket);
    WSACleanup();
    return 0;
}
