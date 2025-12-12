#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <MinimalSocket/udp/UdpSocket.h>
#include <unistd.h>

#include "structs.h"

using namespace std;

string posicion_inicial(Game_data &datos);

void sendInitialMoveMessage(const string &str, MinimalSocket::udp::Udp<true> &udp_socket,
                     MinimalSocket::Address const &recep, Game_data &datos);


//gestiona el mensaje de ball ((b) 12 13) y guarda las variables direccion y distacia 
//y tiene un flag para saber si tiene el balon o no
Game_data handle_ball(string const &message, Game_data &datos);

//gestiona el mensaje de porteria y guarda las variables direccion y distacia de ciertas partes de la porteria
//y tiene un flag para saber si veo la porteria contraria o no
Game_data handle_porteria(string const &message, Game_data &datos);

//gestiona el mensaje de jugadores del mismo equipo y guarda las variables direccion y distacia en un vector de un vector de string
//y tiene un flag para saber si veo jugadores o no
Game_data handle_jugadores_vistos(string const &message, Game_data &datos);

bool tengo_balon(Game_data const &datos);

//es true cuando hay un jugador mas cerca que yo del balon
void jugador_cerca_pase(Game_data &datos);

bool voy_balon(Game_data &datos);

//lo que hace cuando esta lejos de la porteria
string pase(Game_data const &datos);

void send_message_funtion(string const &mensaje, Game_data &datos);

string funcionEnviar(Game_data &datos);
string ataque(Game_data &datos);
string disparo(Game_data &datos);
string sendMessage(Game_data &datos);

#endif
