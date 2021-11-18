#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "json-c/json.h"
#include "constants.h"
#include "Partida/flujo.h"

struct partida partida1 = {0}, partida2 = {0};

void setInicio(int flag){
    int ghosts, frutas;
    printf("%s", "Ingrese la cantidad de ghosts: ");
    scanf("%d", &ghosts);
    printf("%s", "Ingrese la cantidad de frutas: ");
    scanf("%d", &frutas);
    if (flag){
        partida2.ghosts = ghosts;
        partida2.frutas = frutas;
        partida2.jugador.puntaje = 0;
        partida2.jugador.vidas = 3;
        printf("Se agrega partida 2 \n");
    }
    else{
        partida1.ghosts = ghosts;
        partida1.frutas = frutas;
        partida1.jugador.puntaje = 0;
        partida1.jugador.vidas = 3;
        printf("Se agrega partida 1 \n");
    }

}
void startServer(){
    int opt = TRUEFLAG;
    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025]; //Buffer de datos

    //set de descriptores de archivos
    fd_set readfds;

    //Mensaje de incio
    char *message = "Login";

    //inicializando array de clientes
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //crear master socket para el manejo de mutiples conexiones
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("fallo del socket master");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //Crear socket de la familia AF_INET para TCP
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind del socket al localhost en el puerto 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("fallo el enlace");
        exit(EXIT_FAILURE);
    }
    printf("\nConexion en el puerto %d \n", PORT);

    //el master socket maneja 3 conexiones en cola
    if (listen(master_socket, 3) < 0)
    {
        perror("escuchando");
        exit(EXIT_FAILURE);
    }

    //aceptar conexion
    addrlen = sizeof(address);
    puts("esperando conexiones...");

    while(TRUEFLAG)
    {
        //resetear el socket set
        FD_ZERO(&readfds);

        //se agrega master socket al set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //agrega socket de cliente al set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //descriptor del socket
            sd = client_socket[i];

            //agregar si es valido
            if(sd > 0)
                FD_SET( sd , &readfds);

            //cambiar el màximo valor del descriptor para funcion select
            if(sd > max_sd)
                max_sd = sd;
        }

        //activity se encarga de escuchar donde hay actividad en
        // los descriptores de archivos, el parametro timeout es NULL
        //de manera que escucha indefinidamente
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //saber si el master recibiò un nuevo cliente,
        //entonces se acepta la conexiòn
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Nueva conexion, socket fd es %d , ip es : %s , puerto : %d \n" ,
                   new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));

            //enviar mensaje de recepcion de conexion "Login"
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            //agregar socket al array
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Conexion numero: %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //chequear el mensaje y si fue para desconexion del socket
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
                    printf("Desconexion de , ip %s , puerto %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Cerrar el socket y su campo en el array
                    close( sd );
                    client_socket[i] = 0;
                }

                    //Procesamiento de mensajes
                else
                {

                    buffer[valread] = '\0';
                    printf("Cliente: %s \n", buffer);

                    if (strcmp(buffer, "Login")==0){
                        setInicio(i);
                        send(sd , "Puntaje" , strlen("Puntaje") , 0 );
                    }
                    else{
                        send(sd , buffer , strlen(buffer) , 0 );
                        memset(&buffer[0], 0, sizeof(buffer));
                    }

                }
            }
        }
    }

}

int main(int argc , char *argv[])
{
    /*Creating a json object*/
    json_object * jobj = json_object_new_object();
    /*Creating a json integer*/
    json_object *jint = json_object_new_int(10);
    /*Form the json object*/
    json_object_object_add(jobj,"Puntaje", jint);
    const char *puntaje = json_object_to_json_string(jobj);
    /*Now printing the json object*/

    startServer();
	return 0;
}



