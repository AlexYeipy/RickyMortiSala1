/**
 * @file main.cpp
 * @brief Programa principal del agente de fútbol robótico
 *
 * Punto de entrada del programa. Maneja la conexión con el servidor RCSS,
 * inicialización del jugador y bucle principal de percepción-decisión-acción.
 */

#include <iostream>
#include "Funciones.h"
#include "AbsPos.h"
#include "structs.h"


int main(int argc, char *argv[])
{
    Game_data datos;

    vector<shared_ptr<knownFlags>> flags;
    inicializacion_flags(flags);

    // Verificación de argumentos de línea de comandos
    if (argc != 3 && argc != 4)
    {
        cout << "Uso: " << argv[0] << " <nombre-equipo> <puerto> [goalie]" << endl;
        return 1;
    }

    bool soy_portero = (argc == 4 && string(argv[3]) == "goalie");
    datos.nombre_equipo = argv[1];

    //numero del puerto stoi pasa de letra a numero
    MinimalSocket::Port this_socket_port = stoi(argv[2]);

    cout << "Creando socket UDP..." << endl;

    // Creación del socket UDP IPv6 para comunicación con el servidor
    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);
    cout << "Socket creado correctamente" << endl;

    // Apertura del socket
    bool success = udp_socket.open();
    if (!success)
    {
        cout << "Error al abrir el socket" << endl;
        return 1;
    }

    // Configuración de dirección del servidor (localhost puerto 6000)
    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};

    string init_msg;
    if (soy_portero)
        init_msg = "(init " + datos.nombre_equipo + " (version 19) (goalie))";
    else
        init_msg = "(init " + datos.nombre_equipo + " (version 19))";

    udp_socket.sendTo(init_msg, other_recipient_udp);
    cout << "Mensaje de inicialización enviado: " << init_msg << endl;

    // Espera y recepción de la respuesta del servidor
    std::size_t message_max_size = 1000000;
    cout << "Esperando mensaje del servidor..." << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // Actualización de la dirección del servidor con el puerto que respondió
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};
    sendInitialMoveMessage(received_message_content, udp_socket, server_udp, datos);

    while(true){
        string received_message_content = received_message-> received_message;
        do{
            received_message = udp_socket.receive(message_max_size);
            received_message_content = received_message->received_message;
            try
            {
                send_message_funtion(received_message_content, datos);
                if (datos.jugador.jugador_numero != "1" || stof(datos.ball.balon_distancia) < 5)
                {
                    crear_matriz_valores_absolutos(received_message_content, flags); // ABSOLUTOS
                    relative2Abssolute(flags, datos);                                // ABSOLUTOS
                }
            }
            catch (const std::exception &e)
            {
                cout << e.what() << endl;
            }
        } while (received_message_content.find("(see") == -1);
        string envio = sendMessage(datos);
        //cout << envio << endl;
        if (envio != ""){
            udp_socket.sendTo(envio,server_udp);
        }
    }
}

/**
 * @brief Ciclo de percepción-decisión-acción del agente:
 *
 * 1. PERCEPCIÓN: Recibe y procesa mensajes del servidor
 *    - Mensajes "hear": eventos del juego
 *    - Mensajes "see": percepción visual
 *    - Calcula posición absoluta con flags
 *
 * 2. DECISIÓN: Genera comando basado en estado
 *    - Usa lógica de sendMessage() en Funciones.cpp
 *    - Considera posición, balón, compañeros, rivales
 *    - Implementa estrategias tácticas por posición
 *
 * 3. ACCIÓN: Envía comando al servidor
 *    - Comandos: move, turn, dash, kick, catch
 *    - Se ejecutan en el siguiente ciclo del simulador
 *
 * @note El ciclo se sincroniza con los mensajes "see" del servidor
 * @note Cada mensaje "see" corresponde a un ciclo de simulación
 * @note La posición absoluta se calcula solo cuando es necesario
 */

/**
 * @param argc Número de argumentos (debe ser 3)
 * @param argv Argumentos: [0]nombre_programa [1]nombre_equipo [2]puerto
 * @return 0 si éxito, 1 si error de configuración
 *
 * @example Ejecución: ./player MiEquipo 5000
 * @example Ejecución: ./player Barcelona 6001
 */

/**
 * @file main.cpp - Arquitectura del agente:
 *
 * CONFIGURACIÓN → PERCEPCIÓN → DECISIÓN → ACCIÓN
 *      ↓              ↓           ↓         ↓
 *   Socket UDP   Procesamiento  Lógica    Envío
 *   Inicializa   mensajes       táctica   comando
 *   Conexión     Posición       SendMessage
 *
 * Flujo principal:
 * 1. Inicializa estructuras y socket
 * 2. Conecta con servidor RCSS
 * 3. Recibe configuración inicial
 * 4. Bucle infinito:
 *    a. Espera mensaje "see"
 *    b. Procesa percepción
 *    c. Calcula posición (si aplica)
 *    d. Decide acción
 *    e. Envía comando
 */
