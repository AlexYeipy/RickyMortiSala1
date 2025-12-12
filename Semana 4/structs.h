/**
 * @file structs.h
 * @brief Archivo de cabecera que define las estructuras de datos del sistema
 *
 * Contiene todas las estructuras de datos utilizadas para representar el estado
 * del juego en el simulador de fútbol robótico. Estas estructuras almacenan
 * información sobre jugadores, balón, porterías, compañeros y el estado general
 * del juego.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

// Inclusión de dependencias
#include <string>    // Para cadenas de texto
#include <vector>    // Para contenedores dinámicos

using namespace std;

/**
 * @brief Estructura que representa puntos de referencia conocidos (flags) en el campo
 *
 * Los flags son elementos estáticos del campo (banderines, líneas, porterías)
 * con posiciones absolutas conocidas, utilizados para localización y navegación.
 */
struct knownFlags
{
    string name;        ///< Nombre identificativo del flag (ej: "f c t")
    int x;              ///< Coordenada X absoluta en el campo
    int y;              ///< Coordenada Y absoluta en el campo
    float distancia;    ///< Distancia relativa desde el jugador al flag (metros)
    float direccion;    ///< Ángulo relativo desde el jugador al flag (grados)
};

/**
 * @brief Estructura que representa al jugador controlado por este agente
 *
 * Contiene toda la información de estado y configuración del jugador,
 * incluyendo posición, pertenencia al equipo y estado de posesión del balón.
 */
struct Player
{
    string jugador_numero;   ///< Número identificativo del jugador (1-11)
    string lado_campo;       ///< Lado del campo: "l" (izquierda) o "r" (derecha)
    string nombre_equipo;    ///< Nombre del equipo al que pertenece
    float x_absoluta;        ///< Coordenada X absoluta en el campo (si se conoce)
    float y_absoluta;        ///< Coordenada Y absoluta en el campo (si se conoce)
    bool tengo_balon;        ///< Indica si el jugador tiene posesión del balón
    bool equipo_tiene_balon; ///< Indica si algún compañero tiene posesión del balón
    bool saque_puerta;       ///< Indica si el jugador debe realizar saque de puerta

    /**
     * @brief Constructor por defecto que inicializa todos los miembros con valores seguros
     *
     * Valores iniciales:
     * - Coordenadas: 999.0 (valor sentinela indicando "desconocido")
     * - Banderas booleanas: false (estado inicial seguro)
     * - Cadenas: vacías
     */
    Player()
        : jugador_numero(""), lado_campo(""), nombre_equipo(""),
        x_absoluta(999.0f), y_absoluta(999.0f), tengo_balon(false),
        equipo_tiene_balon(false), saque_puerta(false) {}
};

/**
 * @brief Estructura que almacena información sobre jugadores visibles en el campo
 *
 * Clasifica y organiza la información de compañeros y rivales detectados
 * mediante mensajes de percepción ("see").
 */
struct Jugadores_Vistos
{
    vector<vector<string>> jugadores;           ///< Matriz de compañeros detectados [distancia, ángulo, número]
    bool veo_equipo;                            ///< Indica si se detectaron compañeros
    vector<vector<string>> jugadores_contrario; ///< Matriz de rivales detectados [distancia, ángulo, número]
    bool veo_equipo_contrario;                  ///< Indica si se detectaron rivales

    /**
     * @brief Constructor por defecto
     */
    Jugadores_Vistos()
        : jugadores(), veo_equipo(false),
        jugadores_contrario(), veo_equipo_contrario(false) {}
};

/**
 * @brief Estructura que representa el estado del balón respecto al jugador
 *
 * Almacena información de percepción sobre el balón: distancia, dirección
 * relativa y estado de visibilidad.
 */
struct Ball
{
    string balon_distancia;  ///< Distancia al balón en metros (como string)
    string balon_direccion;  ///< Ángulo relativo al balón en grados (como string)
    bool veo_balon;          ///< Indica si el balón está actualmente visible

    /**
     * @brief Constructor por defecto con valores iniciales seguros
     *
     * Inicializa con distancia y dirección a 999.0 (indicando "no visible")
     * y veo_balon en false.
     */
    Ball()
        : balon_distancia("999.0"), balon_direccion("999.0"), veo_balon(false) {}
};

/**
 * @brief Estructura que representa información sobre las porterías
 *
 * Almacena datos detallados sobre diferentes partes de las porterías
 * (palo superior, palo inferior, centro) tanto propia como contraria.
 */
struct Goal
{
    string palo_abajo_distancia;     ///< Distancia al palo inferior de la portería
    string palo_abajo_direccion;     ///< Ángulo al palo inferior de la portería
    string palo_arriba_distancia;    ///< Distancia al palo superior de la portería
    string palo_arriba_direccion;    ///< Ángulo al palo superior de la portería
    string centro_distancia;         ///< Distancia al centro de la portería
    string centro_direccion;         ///< Ángulo al centro de la portería
    bool veo_porteria_contraria;     ///< Indica si la portería contraria es visible
    bool veo_porteria_propia;        ///< Indica si la portería propia es visible

    /**
     * @brief Constructor por defecto con valores iniciales seguros
     */
    Goal()
        : palo_abajo_distancia("999.0"), palo_abajo_direccion("999.0"),
        palo_arriba_distancia("999.0"), palo_arriba_direccion("999.0"),
        centro_distancia("999.0"), centro_direccion("999.0"),
        veo_porteria_contraria(false), veo_porteria_propia(false) {}
};

/**
 * @brief Estructura para identificar compañeros bien posicionados para recibir pase
 *
 * Utilizada en lógica de juego colectivo para determinar el mejor receptor
 * de pase en situaciones ofensivas.
 */
struct jugadorCercaPase
{
    bool hayJugador;        ///< Indica si existe un compañero bien posicionado
    string direccion;        ///< Ángulo hacia el compañero (grados como string)
    string distancia;        ///< Distancia al compañero (metros como string)
    string numero_jugador;   ///< Número identificativo del compañero

    /**
     * @brief Constructor por defecto
     */
    jugadorCercaPase()
        : hayJugador(false), direccion("999.0"),
        distancia("999.0"), numero_jugador("") {}
};

/**
 * @brief Estructura principal que agrega todo el estado del juego
 *
 * Contiene todas las subestructuras y representa el estado completo
 * conocido del partido desde la perspectiva del agente.
 */
struct Game_data
{
    string nombre_equipo;            ///< Nombre del equipo controlado
    Player jugador;                  ///< Información específica del jugador
    Ball ball;                       ///< Estado del balón
    Goal porteria;                   ///< Información de porterías
    Jugadores_Vistos jugadores_vistos; ///< Jugadores visibles
    jugadorCercaPase jugadorCerca;   ///< Posible receptor de pase
    string evento;                   ///< Evento actual del juego (kick_off, goal, etc.)
    string evento_anterior;          ///< Evento previo (para detección de cambios)
    string lado_campo;               ///< Lado del campo ("l" o "r")
    bool flag_kick_off;              ///< Indica si es el inicio del partido o tras gol

    /**
     * @brief Constructor por defecto que inicializa todos los componentes
     */
    Game_data()
        : nombre_equipo(""), jugador(), ball(), porteria(),
        jugadores_vistos(), jugadorCerca(),
        evento("999.0"), evento_anterior("999.0"),
        lado_campo("l"), flag_kick_off(false) {}
};

#endif // STRUCTS_H
