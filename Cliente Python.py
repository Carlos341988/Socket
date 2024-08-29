import socket
import os

def menu():
    print("Seleccionar una opción: ")
    print("1. Generar nombre de usuario")
    print("2. Generar contrasena")
    print("3. Salir")

def main():
    # Configurar el socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('127.0.0.1', 8080)

    try:
        # Conectar al servidor
        client_socket.connect(server_address)
        print("Conectado al Servidor")

        connected = True
        while connected:
            menu()
            opcion = input("Ingrese una opción: ").strip()
            client_socket.send(opcion.encode())

            if opcion == '1' or opcion == '2':
                longitud = input("Ingresar longitud: ").strip()
                client_socket.send(longitud.encode())
                # Recibir la respuesta del servidor, pero no mostrarla en el cliente.
                response = client_socket.recv(1024).decode()

                if response == "1":
                    # Si recibimos un "1", entonces la operación fue exitosa, 
                    # pero no mostramos nada en el cliente
                    pass
                else:
                    # Si hay un error, no mostramos nada en el cliente.
                    pass

                os.system('cls' if os.name == 'nt' else 'clear')

            elif opcion == '3':
                connected = False
                print("Saliendo...")
                break

            else:
                print("Opción no válida.")
                os.system('cls' if os.name == 'nt' else 'clear')

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Cerrar el socket
        client_socket.close()
        print("Conexión cerrada")

if __name__ == "__main__":
    main()