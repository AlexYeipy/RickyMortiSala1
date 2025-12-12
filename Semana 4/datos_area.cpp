/**
 * @file datos_area.cpp
 * @brief Implementación de la lógica de posicionamiento táctico por áreas del campo
 *
 * Este archivo contiene las funciones que definen las zonas de responsabilidad
 * de cada jugador según su posición táctica y generan comportamientos
 * basados en la distancia al balón cuando están en su área asignada.
 */

#include "datos_area.h"

/**
 * @brief Verifica si el jugador está dentro de su área de responsabilidad asignada
 *
 * Define zonas rectangulares específicas para cada posición táctica (portero,
 * defensas, mediocentros, delanteros) en ambos lados del campo. Estas zonas
 * representan las áreas donde cada jugador debe operar principalmente según
 * su rol en la formación.
 *
 * @param datos Estructura con información del jugador y su posición
 * @return true si el jugador está dentro de su área asignada, false en caso contrario
 *
 * @note Coordenadas del campo: X: 0-120 (ancho), Y: 0-80 (alto)
 * @note Lado izquierdo: ataca hacia derecha, Lado derecho: ataca hacia izquierda
 * @note Cada posición tiene una zona específica adaptada a su rol táctico
 */
bool comprobar_area(Game_data const &datos)
{
    // ======================================================
    // IDENTIFICACIÓN DE POSICIONES TÁCTICAS
    // ======================================================
    // PORTEROS
    bool portero = (datos.jugador.jugador_numero == "1");

    // DEFENSAS (formación 4-3-3)
    bool defensa2 = (datos.jugador.jugador_numero == "2");  // Lateral derecho
    bool defensa3 = (datos.jugador.jugador_numero == "3");  // Central derecho
    bool defensa4 = (datos.jugador.jugador_numero == "4");  // Central izquierdo
    bool defensa5 = (datos.jugador.jugador_numero == "5");  // Lateral izquierdo

    // MEDIOCAMPISTAS (centrocampistas)
    bool mc1 = (datos.jugador.jugador_numero == "6");  // Mediocentro derecho
    bool mc2 = (datos.jugador.jugador_numero == "7");  // Mediocentro central
    bool mc3 = (datos.jugador.jugador_numero == "8");  // Mediocentro izquierdo

    // DELANTEROS
    bool delantero1 = (datos.jugador.jugador_numero == "9");   // Delantero derecho
    bool delantero2 = (datos.jugador.jugador_numero == "10");  // Delantero centro
    bool delantero3 = (datos.jugador.jugador_numero == "11");  // Delantero izquierdo

    // ======================================================
    // DETECCIÓN DEL LADO DEL CAMPO
    // ======================================================
    bool lado_izquierdo = (datos.jugador.lado_campo == "l");  // Equipo juega de izquierda a derecha
    bool lado_derecho = (datos.jugador.lado_campo == "r");    // Equipo juega de derecha a izquierda

    // ======================================================
    // VERIFICACIÓN PARA LADO IZQUIERDO (ATAQUE HACIA DERECHA)
    // ======================================================
    if (lado_izquierdo)
    {
        // PORTERO (posición 1) - Área de portería
        if (portero)
        {
            // Zona: dentro del área pequeña (8-18 en X, 33-47 en Y)
            // Corresponde al área de meta donde el portero debe permanecer
            if ((datos.jugador.x_absoluta <= 18 && datos.jugador.x_absoluta >= 8) &&
                (datos.jugador.y_absoluta <= 47 && datos.jugador.y_absoluta >= 33))
            {
                return true;
            }
        }
        // DEFENSA 2 - Lateral derecho
        else if (defensa2)
        {
            // Zona: banda derecha defensiva (8-40 en X, 40-74 en Y)
            // Cubre la banda derecha en zona defensiva
            if ((datos.jugador.x_absoluta <= 40 && datos.jugador.x_absoluta >= 8) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 40))
            {
                return true;
            }
        }
        // DEFENSA 3 - Central derecho
        else if (defensa3)
        {
            // Zona: centro-derecha defensiva (8-35 en X, 30-60 en Y)
            // Área central-derecha del campo defensivo
            if ((datos.jugador.x_absoluta <= 35 && datos.jugador.x_absoluta >= 8) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 30))
            {
                return true;
            }
        }
        // DEFENSA 4 - Central izquierdo
        else if (defensa4)
        {
            // Zona: centro-izquierda defensiva (8-35 en X, 20-50 en Y)
            // Área central-izquierda del campo defensivo
            if ((datos.jugador.x_absoluta <= 35 && datos.jugador.x_absoluta >= 8) &&
                (datos.jugador.y_absoluta <= 50 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // DEFENSA 5 - Lateral izquierdo
        else if (defensa5)
        {
            // Zona: banda izquierda defensiva (8-40 en X, 8-40 en Y)
            // Cubre la banda izquierda en zona defensiva
            if ((datos.jugador.x_absoluta <= 40 && datos.jugador.x_absoluta >= 8) &&
                (datos.jugador.y_absoluta <= 40 && datos.jugador.y_absoluta >= 8))
            {
                return true;
            }
        }
        // MEDIOCENTRO 1 - Derecho
        else if (mc1)
        {
            // Zona: centro-derecha del mediocampo (30-80 en X, 40-60 en Y)
            // Área de creación por la derecha en mediocampo
            if ((datos.jugador.x_absoluta <= 80 && datos.jugador.x_absoluta >= 30) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 40))
            {
                return true;
            }
        }
        // MEDIOCENTRO 2 - Central
        else if (mc2)
        {
            // Zona: centro del mediocampo (30-80 en X, 20-40 en Y)
            // Área central del mediocampo, pivote defensivo/ofensivo
            if ((datos.jugador.x_absoluta <= 80 && datos.jugador.x_absoluta >= 30) &&
                (datos.jugador.y_absoluta <= 40 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // MEDIOCENTRO 3 - Izquierdo
        else if (mc3)
        {
            // Zona: centro-izquierda del mediocampo (30-80 en X, 20-60 en Y)
            // Área de creación por la izquierda (zona más amplia)
            if ((datos.jugador.x_absoluta <= 80 && datos.jugador.x_absoluta >= 30) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // DELANTERO 1 - Derecho
        else if (delantero1)
        {
            // Zona: ataque por derecha (45-112 en X, 8-74 en Y)
            // Tercera parte derecha del campo ofensivo
            if ((datos.jugador.x_absoluta <= 112 && datos.jugador.x_absoluta >= 45) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 8))
            {
                return true;
            }
        }
        // DELANTERO 2 - Centro
        else if (delantero2)
        {
            // Zona: ataque central (45-112 en X, 6-74 en Y)
            // Área central ofensiva (ligeramente más amplia en Y)
            if ((datos.jugador.x_absoluta <= 112 && datos.jugador.x_absoluta >= 45) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 6))
            {
                return true;
            }
        }
        // DELANTERO 3 - Izquierdo
        else if (delantero3)
        {
            // Zona: ataque por izquierda (45-112 en X, 6-74 en Y)
            // Tercera parte izquierda del campo ofensivo
            if ((datos.jugador.x_absoluta <= 112 && datos.jugador.x_absoluta >= 45) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 6))
            {
                return true;
            }
        }
    }

    // ======================================================
    // VERIFICACIÓN PARA LADO DERECHO (ATAQUE HACIA IZQUIERDA)
    // ======================================================
    // Nota: Los números de jugador se mantienen, pero las zonas se reflejan
    //       Defensa2 en lado derecho corresponde al lateral izquierdo, etc.
    if (lado_derecho)
    {
        // PORTERO (posición 1) - Área de portería (lado derecho)
        if (portero)
        {
            // Zona: área de meta derecha (94-104 en X, 33-47 en Y)
            // Portería del lado derecho del campo
            if ((datos.jugador.x_absoluta >= 94 && datos.jugador.x_absoluta <= 104) &&
                (datos.jugador.y_absoluta <= 47 && datos.jugador.y_absoluta >= 33))
            {
                return true;
            }
        }
        // DEFENSA 5 - Lateral izquierdo (en lado derecho, defiende banda izquierda)
        else if (defensa5)
        {
            // Zona: banda izquierda defensiva (75-112 en X, 40-74 en Y)
            if ((datos.jugador.x_absoluta >= 75 && datos.jugador.x_absoluta <= 112) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 40))
            {
                return true;
            }
        }
        // DEFENSA 4 - Central izquierdo
        else if (defensa4)
        {
            // Zona: centro-izquierda defensiva (80-112 en X, 30-60 en Y)
            if ((datos.jugador.x_absoluta >= 80 && datos.jugador.x_absoluta <= 112) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 30))
            {
                return true;
            }
        }
        // DEFENSA 3 - Central derecho
        else if (defensa3)
        {
            // Zona: centro-derecha defensiva (80-112 en X, 20-50 en Y)
            if ((datos.jugador.x_absoluta >= 80 && datos.jugador.x_absoluta <= 112) &&
                (datos.jugador.y_absoluta <= 50 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // DEFENSA 2 - Lateral derecho
        else if (defensa2)
        {
            // Zona: banda derecha defensiva (75-112 en X, 8-40 en Y)
            if ((datos.jugador.x_absoluta >= 75 && datos.jugador.x_absoluta <= 112) &&
                (datos.jugador.y_absoluta <= 40 && datos.jugador.y_absoluta >= 8))
            {
                return true;
            }
        }
        // MEDIOCENTRO 2 - Central
        else if (mc2)
        {
            // Zona: centro del mediocampo (40-90 en X, 40-60 en Y)
            if ((datos.jugador.x_absoluta >= 40 && datos.jugador.x_absoluta <= 90) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 40))
            {
                return true;
            }
        }
        // MEDIOCENTRO 1 - Derecho
        else if (mc1)
        {
            // Zona: centro-derecha del mediocampo (40-90 en X, 20-40 en Y)
            if ((datos.jugador.x_absoluta >= 40 && datos.jugador.x_absoluta <= 90) &&
                (datos.jugador.y_absoluta <= 40 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // MEDIOCENTRO 3 - Izquierdo
        else if (mc3)
        {
            // Zona: centro-izquierda del mediocampo (40-90 en X, 20-60 en Y)
            if ((datos.jugador.x_absoluta >= 40 && datos.jugador.x_absoluta <= 90) &&
                (datos.jugador.y_absoluta <= 60 && datos.jugador.y_absoluta >= 20))
            {
                return true;
            }
        }
        // DELANTERO 2 - Centro
        else if (delantero2)
        {
            // Zona: ataque central (8-75 en X, 8-74 en Y)
            if ((datos.jugador.x_absoluta >= 8 && datos.jugador.x_absoluta <= 75) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 8))
            {
                return true;
            }
        }
        // DELANTERO 1 - Derecho
        else if (delantero1)
        {
            // Zona: ataque por derecha (8-75 en X, 6-74 en Y)
            if ((datos.jugador.x_absoluta >= 8 && datos.jugador.x_absoluta <= 75) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 6))
            {
                return true;
            }
        }
        // DELANTERO 3 - Izquierdo
        else if (delantero3)
        {
            // Zona: ataque por izquierda (8-75 en X, 6-74 en Y)
            if ((datos.jugador.x_absoluta >= 8 && datos.jugador.x_absoluta <= 75) &&
                (datos.jugador.y_absoluta <= 74 && datos.jugador.y_absoluta >= 6))
            {
                return true;
            }
        }
    }

    // Si no se cumple ninguna condición, el jugador está fuera de su área
    return false;
}

/**
 * @brief Genera comandos de movimiento basados en la distancia al balón y posición táctica
 *
 * Asigna diferentes comportamientos según la distancia al balón cuando el jugador
 * está dentro de su área asignada. Los valores de retorno representan:
 * - "10": Mantener posición / movimiento conservador
 * - "0": Esperar / no moverse activamente
 * - "80": Ir activamente hacia el balón
 * - "-1": El balón está muy cerca (posiblemente para chutar/pasar)
 * - "-2": Fuera del área asignada
 * - "-3": Error / valor por defecto
 *
 * @param datos Estructura con información del juego y posición del jugador
 * @return String con el código de acción a realizar
 *
 * @note Los umbrales de distancia varían según la posición:
 *       - Porteros y defensas: 40m, 20m, 1m
 *       - Mediocentros: 30m, 10m, 1m
 *       - Delanteros: 25m, 15m, 1m
 */
string player_moves(Game_data const &datos)
{
    // Solo generamos comportamientos si el jugador está en su área asignada
    if (comprobar_area(datos))
    {
        // ======================================================
        // REIDENTIFICACIÓN DE POSICIONES (para claridad en esta función)
        // ======================================================
        // PORTEROS
        bool portero = (datos.jugador.jugador_numero == "1");

        // DEFENSAS
        bool defensa2 = (datos.jugador.jugador_numero == "2");
        bool defensa3 = (datos.jugador.jugador_numero == "3");
        bool defensa4 = (datos.jugador.jugador_numero == "4");
        bool defensa5 = (datos.jugador.jugador_numero == "5");

        // MEDIOCAMPISTAS
        bool mc1 = (datos.jugador.jugador_numero == "6");
        bool mc2 = (datos.jugador.jugador_numero == "7");
        bool mc3 = (datos.jugador.jugador_numero == "8");

        // DELANTEROS
        bool delantero1 = (datos.jugador.jugador_numero == "9");
        bool delantero2 = (datos.jugador.jugador_numero == "10");
        bool delantero3 = (datos.jugador.jugador_numero == "11");

        // ======================================================
        // LÓGICA DE COMPORTAMIENTO POR POSICIÓN
        // ======================================================
        // NOTA: Hay un error lógico - la condición if(comprobar_area(datos))
        // se verifica dos veces (línea 218 y 225), lo que es redundante

        if (comprobar_area(datos))  // Esta verificación es redundante
        {
            // ==================================================
            // COMPORTAMIENTO DEFENSIVO (Porteros y Defensas)
            // ==================================================
            // Porteros y defensas tienen umbrales más conservadores (40m, 20m)
            // para mantener estructura defensiva

            if (portero)
            {
                // Balón muy lejos (>40m): movimiento conservador
                if (datos.ball.balon_distancia >= "40")
                    return "10";
                // Balón a distancia media (20-40m): esperar
                else if (datos.ball.balon_distancia < "40" && datos.ball.balon_distancia >= "20")
                    return "0";
                // Balón cerca (1-20m): ir activamente hacia él
                else if (datos.ball.balon_distancia < "20" && datos.ball.balon_distancia >= "1")
                    return "80";
                else // Balón muy cerca (<1m): posible acción de captura/pase
                    return "-1";
            }
            else if (defensa2)
            {
                // Misma lógica que portero para mantener línea defensiva
                if (datos.ball.balon_distancia >= "40")
                    return "10";
                else if (datos.ball.balon_distancia < "40" && datos.ball.balon_distancia >= "20")
                    return "0";
                else if (datos.ball.balon_distancia < "20" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
            else if (defensa3)
            {
                if (datos.ball.balon_distancia >= "40")
                    return "10";
                else if (datos.ball.balon_distancia < "40" && datos.ball.balon_distancia >= "20")
                    return "0";
                else if (datos.ball.balon_distancia < "20" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
            else if (defensa4)
            {
                if (datos.ball.balon_distancia >= "40")
                    return "10";
                else if (datos.ball.balon_distancia < "40" && datos.ball.balon_distancia >= "20")
                    return "0";
                else if (datos.ball.balon_distancia < "20" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
            else if (defensa5)
            {
                if (datos.ball.balon_distancia >= "40")
                    return "10";
                else if (datos.ball.balon_distancia < "40" && datos.ball.balon_distancia >= "20")
                    return "0";
                else if (datos.ball.balon_distancia < "20" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
            // ==================================================
            // COMPORTAMIENTO DE MEDIOCAMPISTAS
            // ==================================================
            // Mediocentros tienen umbrales intermedios (30m, 10m)
            // más agresivos que defensas pero más conservadores que delanteros

            else if (mc1)
            {
                // Balón lejos (>30m): movimiento moderado
                if (datos.ball.balon_distancia >= "30")
                    return "10";
                // Balón a distancia media (10-30m): mantener posición
                else if (datos.ball.balon_distancia < "30" && datos.ball.balon_distancia >= "10")
                    return "0";
                // Balón cerca (1-10m): ir activamente
                else if (datos.ball.balon_distancia < "10" && datos.ball.balon_distancia >= "1")
                    return "80";
                else // Balón muy cerca (<1m): acción inmediata
                    return "-1";
            }
            else if (mc2)
            {
                if (datos.ball.balon_distancia >= "30")
                    return "10";
                else if (datos.ball.balon_distancia < "30" && datos.ball.balon_distancia >= "10")
                    return "0";
                else if (datos.ball.balon_distancia < "10" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
            else if (mc3)
            {
                if (datos.ball.balon_distancia >= "30")
                    return "10";
                else if (datos.ball.balon_distancia < "30" && datos.ball.balon_distancia >= "10")
                    return "0";
                else if (datos.ball.balon_distancia < "10" && datos.ball.balon_distancia >= "1")
                    return "80";
                else
                    return "-1";
            }
        }
        // ==================================================
        // COMPORTAMIENTO DE DELANTEROS
        // ==================================================
        // Delanteros tienen umbrales más agresivos (25m, 15m)
        // para presionar alto y buscar oportunidades de gol

        else if (delantero1)
        {
            // Balón lejos (>25m): movimiento ofensivo
            if (datos.ball.balon_distancia >= "25")
                return "10";
            // Balón a distancia media (15-25m): prepararse
            else if (datos.ball.balon_distancia < "25" && datos.ball.balon_distancia >= "15")
                return "0";
            // Balón cerca (1-15m): atacar activamente
            else if (datos.ball.balon_distancia < "15" && datos.ball.balon_distancia >= "1")
                return "80";
            else // Balón muy cerca (<1m): disparar/pasar
                return "-1";
        }
        else if (delantero2)
        {
            if (datos.ball.balon_distancia >= "25")
                return "10";
            else if (datos.ball.balon_distancia < "25" && datos.ball.balon_distancia >= "15")
                return "0";
            else if (datos.ball.balon_distancia < "15" && datos.ball.balon_distancia >= "1")
                return "80";
            else
                return "-1";
        }
        else if (delantero3)
        {
            if (datos.ball.balon_distancia >= "25")
                return "10";
            else if (datos.ball.balon_distancia < "25" && datos.ball.balon_distancia >= "15")
                return "0";
            else if (datos.ball.balon_distancia < "15" && datos.ball.balon_distancia >= "1")
                return "80";
            else
                return "-1";
        }
        else
        {
            // Caso por defecto para jugadores no identificados
            return "0";
        }
    }
    else
    {
        // Jugador fuera de su área asignada
        return "-2";
    }

    // Retorno por defecto para evitar warning del compilador
    // Llega aquí solo si hay un error lógico en las condiciones anteriores
    return "-3";
}
