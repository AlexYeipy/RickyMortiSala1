#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <thread>
#include <MinimalSocket/udp/UdpSocket.h>

using namespace std;

// Include headers
#include "funciones.h"
#include "structs.h"

/**
 * Parsea el mensaje inicial del servidor para extraer información del jugador
 * Formato del mensaje: "init <side> <unum> <playmode>"
 *
 * @param message Mensaje recibido del servidor
 * @param player Referencia al objeto Player a actualizar
 * @return Player actualizado con la información parseada
 */
Player parseInitialMessage(std::string &message, Player &player)
{
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    int i = 0;

    // Divide el mensaje por espacios y extrae la información
    while ((pos = message.find(delimiter)) != std::string::npos)
    {
        token = message.substr(0, pos);

        // Asigna cada token a la propiedad correspondiente del jugador
        if (i == 1)
        {
            player.side = token;  // Segundo token: lado del campo
        }
        else if (i == 2)
        {
            player.unum = std::stoi(token);  // Tercer token: número de jugador
        }
        else if (i == 3)
        {
            player.playmode = token;  // Cuarto token: modo de juego
        }

        // Elimina el token procesado del mensaje
        message.erase(0, pos + delimiter.length());
        i++;
    }

    cout << "Player - Lado: " << player.side
         << " | Número: " << player.unum
         << " | Modo: " << player.playmode << endl;

    return player;
}

/**
 * Separa un string en múltiples substrings usando un separador específico
 *
 * @param s String original a separar
 * @param separator Carácter o string usado como separador
 * @return Vector de strings resultantes de la separación
 */
vector<string> separate_string_separator(string &s, string separator)
{
    vector<string> v;
    int pos = 0;

    // Encuentra todas las ocurrencias del separador
    while ((pos = s.find(separator)) != string::npos)
    {
        // Añade el substring antes del separador al vector
        v.push_back(s.substr(0, pos));
        // Elimina el substring procesado
        s.erase(0, pos + separator.length());
    }

    // Añade el último substring (después del último separador)
    v.push_back(s);
    return v;
}

/**
 * Separa un string en substrings basándose en paréntesis anidados
 * Ejemplo: "((hola) (soy) (dani)) (que tal) (estas)" -> {"(hola) (soy) (dani)", "que tal", "estas"}
 *
 * @param s String original con estructura de paréntesis
 * @return Vector de strings separados por niveles de paréntesis
 */
vector<string> separate_string(string &s)
{
    vector<string> v;
    int pos = 0;
    int level = 0;  // Nivel de anidamiento de paréntesis
    string temp;    // String temporal para acumular contenido

    // Recorre cada carácter del string
    while (pos < s.size())
    {
        // Incrementa nivel al encontrar '('
        if (s[pos] == '(')
        {
            level++;
        }
        // Decrementa nivel al encontrar ')'
        else if (s[pos] == ')')
        {
            level--;
        }

        // Lógica de separación basada en niveles de paréntesis
        if (s[pos] == '(' && level == 1)
        {
            // Inicia nuevo string temporal al encontrar primer '(' del nivel 1
            temp = "";
        }
        else if (s[pos] == ')' && level == 0)
        {
            // Finaliza y guarda el string temporal al cerrar paréntesis del nivel 0
            v.push_back(temp);
        }
        else
        {
            // Añade carácter al string temporal (excepto paréntesis del nivel 0)
            temp += s[pos];
        }
        pos++;
    }

    // Verificación de integridad: todos los paréntesis deben estar balanceados
    if (level != 0)
    {
        throw std::runtime_error("Error: Paréntesis desbalanceados - no coinciden aperturas y cierres");
    }
    else
    {
        return v;
    }
}

/**
 * Envía el comando de movimiento inicial para posicionar al jugador en el campo
 * Asigna posiciones específicas basadas en una formación 4-4-2
 *
 * @param player Objeto Player con información del jugador
 * @param udp_socket Socket UDP para comunicación
 * @param recipient Dirección del servidor destino
 */
void sendInitialMoveMessage(Player &player,
                            MinimalSocket::udp::Udp<true> &udp_socket,
                            MinimalSocket::Address const &recipient)
{
    struct Posicion { int x; int y; };

    // Formación 4-4-2 (La misma que tenías)
    vector<Posicion> posiciones = {
        {-50, 0},     // 1: Portero
        {-35, -20},   // 2: Defensa izquierdo
        {-35, 0},     // 3: Defensa central izq
        {-35, 20},    // 4: Defensa central der
        {-35, -20},   // 5: Defensa derecho
        {-15, -20},   // 6: Medio derecho
        {-5, -30},    // 7: Delantero derecho
        {-15, 20},    // 8: Medio izquierdo
        {-5, 0},      // 9: Delantero centro
        {-10, 0},     // 10: Medio centro ofensivo
        {-5, 30}      // 11: Delantero izquierdo
    };

    // Aseguramos que el índice sea válido
    if (player.unum > 0 && player.unum <= 11) {
        Posicion myPos = posiciones[player.unum - 1];

        // Guardamos la posición en el jugador
        player.home_x = myPos.x;
        player.home_y = myPos.y;
        // --------------------------------------------------

        auto moveCommand = "(move " + to_string(myPos.x) + " " + to_string(myPos.y) + ")";
        udp_socket.sendTo(moveCommand, recipient);
        cout << "Jugador " << player.unum << " inicializado en: " << moveCommand << endl;
    }
}

// Encontrar informacion en la funcion see_message
void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field)
{
    vector<string> ball_coords;
    bool found_ball = false;
    player.flags_seen = 0;

    player.see_ball = false;
    player.seeing_zone = false;

        if (own_goal.side == "l")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }
    if (own_goal.side == "r")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }

    for (size_t i = 0; i < see_message.size(); i++)
    {
        // Search for the ball
        if (see_message[i].find("(b)") != string::npos)
        {
            player.see_ball = true;
            ball_coords = separate_string_separator(see_message[i], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];
            cout << "Ball coordinates: " << ball.x << " " << ball.y << endl;
            // Calculate the distance to the ball
            double distance = sqrt(pow(stod(ball.x), 2) + pow(stod(ball.y), 2));
            ball.distance = distance;
            // Calculate the angle to the ball
            double angle = atan2(stod(ball.y), stod(ball.x));
            ball.angle = angle * 180 / M_PI;
            found_ball = true;
        }

        // Search for the right goal
        if (see_message[i].find("(g r)") != string::npos)
        {
            cout << "The player sees the right goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "r")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the left goal
        if (see_message[i].find("(g l)") != string::npos)
        {
            cout << "The player sees the left goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "l")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the flags
        // Search for the center flag
        if (see_message[i].find("(f c)") != string::npos)
        {
            player.flags_seen++;
            vector<string> center_coords = separate_string_separator(see_message[i], " ");
            field.flag_center = {stof(center_coords[2]), stof(center_coords[3])};
            field.flag_center_distance = sqrt(pow(stof(center_coords[2]),2) + pow(stof(center_coords[3]), 2));
            cout << "Center flag coordinates: " << field.flag_center[0] << " " << field.flag_center[1] << endl;
        }
        if (player.zone_name == "(f c)")
        {
            player.seeing_zone = true;
        }

        // Search for the center top flag
        if (see_message[i].find("(f c t)") != string::npos)
        {
            vector<string> center_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_top = {stof(center_top_coords[3]), stof(center_top_coords[4])};
            field.flag_center_top_distance = sqrt(pow(stof(center_top_coords[3]),2) + pow(stof(center_top_coords[4]), 2));
            player.flags_seen++;
            if (player.zone_name == "(f c t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the center bottom flag
        if (see_message[i].find("(f c b)") != string::npos)
        {
            vector<string> center_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_bottom = {stof(center_bottom_coords[3]), stof(center_bottom_coords[4])};
            field.flag_center_bottom_distance = sqrt(pow(stof(center_bottom_coords[3]),2) + pow(stof(center_bottom_coords[4]), 2));
            player.flags_seen++;
            if (player.zone_name == "(f c b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left top flag
        if (see_message[i].find("(f l t)") != string::npos)
        {
            vector<string> left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_top = {stof(left_top_coords[3]), stof(left_top_coords[4])};
            field.flag_left_top_distance = sqrt(pow(stof(left_top_coords[3]),2) + pow(stof(left_top_coords[4]), 2));
            field.flag_left_top_angle = atan2(stof(left_top_coords[4]), stof(left_top_coords[3])) * 180 / M_PI;
            player.flags_seen++;
            if (player.zone_name == "(f l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left bottom flag
        if (see_message[i].find("(f l b)") != string::npos)
        {
            vector<string> left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_bottom = {stof(left_bottom_coords[3]), stof(left_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right top flag
        if (see_message[i].find("(f r t)") != string::npos)
        {
            vector<string> right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_top = {stof(right_top_coords[3]), stof(right_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right bottom flag
        if (see_message[i].find("(f r b)") != string::npos)
        {
            vector<string> right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_bottom = {stof(right_bottom_coords[3]), stof(right_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f r b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left top flag
        if (see_message[i].find("(f p l t)") != string::npos)
        {
            vector<string> penalty_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_top = {stof(penalty_left_top_coords[4]), stof(penalty_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left center flag
        if (see_message[i].find("(f p l c)") != string::npos)
        {
            vector<string> penalty_left_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_center = {stof(penalty_left_center_coords[4]), stof(penalty_left_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left bottom flag
        if (see_message[i].find("(f p l b)") != string::npos)
        {
            vector<string> penalty_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_bottom = {stof(penalty_left_bottom_coords[4]), stof(penalty_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right top flag
        if (see_message[i].find("(f p r t)") != string::npos)
        {
            vector<string> penalty_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_top = {stof(penalty_right_top_coords[4]), stof(penalty_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right center flag
        if (see_message[i].find("(f p r c)") != string::npos)
        {
            vector<string> penalty_right_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_center = {stof(penalty_right_center_coords[4]), stof(penalty_right_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right bottom flag
        if (see_message[i].find("(f p r b)") != string::npos)
        {
            vector<string> penalty_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_bottom = {stof(penalty_right_bottom_coords[4]), stof(penalty_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal left top flag
        if (see_message[i].find("(f g l t)") != string::npos)
        {
            vector<string> goal_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_top = {stof(goal_left_top_coords[4]), stof(goal_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal left bottom flag
        if (see_message[i].find("(f g l b)") != string::npos)
        {
            vector<string> goal_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_bottom = {stof(goal_left_bottom_coords[4]), stof(goal_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right top flag
        if (see_message[i].find("(f g r t)") != string::npos)
        {
            vector<string> goal_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_top = {stof(goal_right_top_coords[4]), stof(goal_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right bottom flag
        if (see_message[i].find("(f g r b)") != string::npos)
        {
            vector<string> goal_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_bottom = {stof(goal_right_bottom_coords[4]), stof(goal_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g r b)")
            {
                player.seeing_zone = true;
            }
        }
    }
}

/**
 * Verifica si un jugador está en su posición asignada en el campo
 * Versión simplificada para evitar condiciones demasiado restrictivas
 *
 * @param field Información del campo y distancias a banderas
 * @param player Jugador a verificar
 * @param own_goal Portería propia del jugador
 * @param opponent_goal Portería contraria
 * @return true si el jugador está en su posición, false en caso contrario
 */
bool estasentusitio(const Field &field, const Player &player, const Goal &own_goal, const Goal &opponent_goal)
{
    // Si el campo no tiene datos válidos, considerar que no está en posición
    if (field.flag_center_distance <= 0.1) {
        return false;
    }

    // Lado derecho (equipo que juega de derecha a izquierda, portería en -52.5)
    if (player.side == "r")
    {
        switch (player.unum)
        {
        case 1: // Portero (lado derecho)
            // Portero debe estar cerca de su portería derecha
            // Condiciones más realistas y menos restrictivas
            if (field.flag_goal_right_top_distance < 15 ||
                field.flag_goal_right_bottom_distance < 15 ||
                field.flag_penalty_right_center_distance < 20)
            {
                return true;
            }
            // Alternativa: si está cerca del centro del área
            if (field.flag_center_distance > 40 && field.flag_center_distance < 55)
            {
                return true;
            }
            break;

        case 2: // Defensa derecho (equipo derecho)
            // Zona defensiva derecha
            if (field.flag_right_top_distance < 40 &&
                field.flag_center_distance > 25)
            {
                return true;
            }
            break;

        case 3: // Defensa central derecho
            // Centro de la defensa, lado derecho
            if (field.flag_center_distance > 20 &&
                field.flag_center_distance < 40 &&
                abs(field.flag_center_top_angle) < 45)
            {
                return true;
            }
            break;

        case 4: // Defensa central izquierdo
            // Centro de la defensa, lado izquierdo
            if (field.flag_center_distance > 20 &&
                field.flag_center_distance < 40 &&
                abs(field.flag_center_top_angle) < 45)
            {
                return true;
            }
            break;

        case 5: // Defensa izquierdo
            // Zona defensiva izquierda
            if (field.flag_left_top_distance < 40 &&
                field.flag_center_distance > 25)
            {
                return true;
            }
            break;

        case 6: // Mediocampista derecho
            // Mediocampista zona derecha
            if (field.flag_right_top_distance > 20 &&
                field.flag_right_top_distance < 50 &&
                field.flag_center_distance > 30)
            {
                return true;
            }
            break;

        case 7: // Delantero derecho
            // Delantero ala derecha
            if (field.flag_right_top_distance > 30 &&
                field.flag_right_top_distance < 60 &&
                field.flag_center_distance > 35)
            {
                return true;
            }
            break;

        case 8: // Mediocampista izquierdo
            // Mediocampista zona izquierda
            if (field.flag_left_top_distance > 20 &&
                field.flag_left_top_distance < 50 &&
                field.flag_center_distance > 30)
            {
                return true;
            }
            break;

        case 9: // Delantero centro
            // Delantero central
            if (field.flag_center_distance > 35 &&
                field.flag_center_distance < 55 &&
                abs(field.flag_center_top_angle) < 30)
            {
                return true;
            }
            break;

        case 10: // Mediocampista central ofensivo
            // Mediocampista ofensivo
            if (field.flag_center_distance > 25 &&
                field.flag_center_distance < 45 &&
                abs(field.flag_center_top_angle) < 35)
            {
                return true;
            }
            break;

        case 11: // Delantero izquierdo
            // Delantero ala izquierda
            if (field.flag_left_top_distance > 30 &&
                field.flag_left_top_distance < 60 &&
                field.flag_center_distance > 35)
            {
                return true;
            }
            break;

        default:
            // Para cualquier otro caso (no debería ocurrir)
            if (field.flag_center_distance > 15 && field.flag_center_distance < 60)
            {
                return true;
            }
            break;
        }
    }
    // Lado izquierdo (equipo que juega de izquierda a derecha, portería en 52.5)
    else if (player.side == "l")
    {
        switch (player.unum)
        {
        case 1: // Portero (lado izquierdo)
            // Portero debe estar cerca de su portería izquierda
            if (field.flag_goal_left_top_distance < 15 ||
                field.flag_goal_left_bottom_distance < 15 ||
                field.flag_penalty_left_center_distance < 20)
            {
                return true;
            }
            // Alternativa: si está cerca del centro del área
            if (field.flag_center_distance > 40 && field.flag_center_distance < 55)
            {
                return true;
            }
            break;

        case 2: // Defensa izquierdo (equipo izquierdo)
            // Zona defensiva izquierda
            if (field.flag_left_top_distance < 40 &&
                field.flag_center_distance > 25)
            {
                return true;
            }
            break;

        case 3: // Defensa central izquierdo
            // Centro de la defensa, lado izquierdo
            if (field.flag_center_distance > 20 &&
                field.flag_center_distance < 40 &&
                abs(field.flag_center_top_angle) < 45)
            {
                return true;
            }
            break;

        case 4: // Defensa central derecho
            // Centro de la defensa, lado derecho
            if (field.flag_center_distance > 20 &&
                field.flag_center_distance < 40 &&
                abs(field.flag_center_top_angle) < 45)
            {
                return true;
            }
            break;

        case 5: // Defensa derecho
            // Zona defensiva derecha
            if (field.flag_right_top_distance < 40 &&
                field.flag_center_distance > 25)
            {
                return true;
            }
            break;

        case 6: // Mediocampista izquierdo
            // Mediocampista zona izquierda
            if (field.flag_left_top_distance > 20 &&
                field.flag_left_top_distance < 50 &&
                field.flag_center_distance > 30)
            {
                return true;
            }
            break;

        case 7: // Delantero izquierdo
            // Delantero ala izquierda
            if (field.flag_left_top_distance > 30 &&
                field.flag_left_top_distance < 60 &&
                field.flag_center_distance > 35)
            {
                return true;
            }
            break;

        case 8: // Mediocampista derecho
            // Mediocampista zona derecha
            if (field.flag_right_top_distance > 20 &&
                field.flag_right_top_distance < 50 &&
                field.flag_center_distance > 30)
            {
                return true;
            }
            break;

        case 9: // Delantero centro
            // Delantero central
            if (field.flag_center_distance > 35 &&
                field.flag_center_distance < 55 &&
                abs(field.flag_center_top_angle) < 30)
            {
                return true;
            }
            break;

        case 10: // Mediocampista central ofensivo
            // Mediocampista ofensivo
            if (field.flag_center_distance > 25 &&
                field.flag_center_distance < 45 &&
                abs(field.flag_center_top_angle) < 35)
            {
                return true;
            }
            break;

        case 11: // Delantero derecho
            // Delantero ala derecha
            if (field.flag_right_top_distance > 30 &&
                field.flag_right_top_distance < 60 &&
                field.flag_center_distance > 35)
            {
                return true;
            }
            break;

        default:
            // Para cualquier otro caso
            if (field.flag_center_distance > 15 && field.flag_center_distance < 60)
            {
                return true;
            }
            break;
        }
    }
    // Si no cumple ninguna condición, no está en posición
    return false;
}

void logica_portero(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp, Ball &ball, Field &field)
{
    if (player.see_ball)
    {
        if (ball.distance < 1.0)
        {
            if (ball.angle < 10 && ball.angle > -10)
            {
                std::string catch_command = "(catch " + to_string(ball.angle) + ")";
                udp_socket.sendTo(catch_command, server_udp);
            }
        }
        else
        {
            // Mantener la misma coordenada en y que la pelota
            if (stod(ball.y) > 0 && (field.flag_goal_left_top[1] < 0 || field.flag_goal_left_bottom[1] < 0))
            {
                std::string dash_command = "(dash 100 90)";
                udp_socket.sendTo(dash_command, server_udp);
            }
            else if (stod(ball.y) < 0 && (field.flag_goal_right_top[1] > 0 || field.flag_goal_right_bottom[1] > 0))
            {
                std::string dash_command = "(dash 100 -90)";
                udp_socket.sendTo(dash_command, server_udp);
            }
        }
    }
}

