#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include <pthread.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024


void menu(){
    printf("Seleccionar una opcion: \n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar contrasena\n");
    printf("3. Salir\n");

}

int main(){
    WSADATA wsaData;
    int sock;
    struct sockaddr_in server;
    char buffet[BUFFER_SIZE];
    int opcion, longitud;
    // Crear el socket
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		printf("Error al inicializar winsock\n");
		exit(-1);
	}
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        perror("[-]Socket error");
        exit(1);
    }
    printf("Socket Creado\n");
    //memset(&server, '\0', sizeof(server));
    //Configurar la direccion del servidor
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //Direccion IP del servidor
    server.sin_port = htons(PORT);
    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error al conectar al servidor: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    bool conected = true;
    while(conected){
        printf("Conectado al Servidor\n");
        menu();
        printf("ingrese una opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);
        bzero(buffet, BUFFER_SIZE);
        sprintf(buffet, "%d", opcion);
        send(sock, buffet, (int)strlen(buffet), 0);
        bzero(buffet, BUFFER_SIZE);//Vacia el Buffer, para volver a utilizarlo.
        switch(opcion){
            case 1:{//Nombre de usuario
                printf("Ingresar longitud: \n");
                gets(buffet);
                send(sock, buffet, (int)strlen(buffet), 0);//Enviar solicitud/mensaje al servidor
                system("cls");
            }break;
            case 2:{//Contrasenia
                printf("Ingresar longitud: \n");
                gets(buffet);
                send(sock, buffet, (int)strlen(buffet), 0);
                //Recibir respuesta del servidor
                bzero(buffet, BUFFER_SIZE);
                recv(sock, buffet, sizeof(buffet), 0);
                if(strcmp(buffet, "1") != 0){
                    printf("Mensaje del Servidor: %s\n", buffet);
                    system("pause");
                }
                system("cls");
            }break;
            case 3:{
                conected = false;
                exit(-1);
                break;
            }break;
            default://Error
                perror("Opcion no vailda\n");
        }

    }
    // Cerrar el socket y limpiar
    closesocket(sock);
    WSACleanup();
    printf("Fin\n");
    exit(-1);
}
