/**
 * @file area.h
 * @brief Archivo de cabecera para el módulo de gestión de áreas del campo
 *
 * Este módulo proporciona funciones para analizar la posición de los jugadores
 * en relación con diferentes zonas del campo y determinar comportamientos
 * estratégicos basados en la ubicación actual.
 */

#ifndef AREA_H
#define AREA_H

// Inclusión de dependencias
#include "structs.h"    // Estructuras de datos del juego (Game_data, Player, etc.)

/**
 * @brief Verifica si el jugador se encuentra dentro de una área específica del campo
 *
 * Analiza las coordenadas actuales del jugador para determinar si se encuentra
 * dentro de una zona estratégica predefinida (ej: área defensiva, área ofensiva,
 * banda, centro del campo, etc.). Esta determinación es crucial para activar
 * comportamientos específicos según la ubicación.
 *
 * @param datos Referencia constante a la estructura Game_data que contiene
 *              toda la información del estado actual del juego, incluyendo
 *              posición del jugador, balón y otros elementos relevantes
 * @return true si el jugador está dentro del área definida, false en caso contrario
 */
bool comprobar_area(Game_data const &datos);

/**
 * @brief Determina el movimiento adecuado para el jugador basado en su área actual
 *
 * Genera comandos de movimiento estratégicos según la posición del jugador
 * en el campo. Los movimientos pueden incluir:
 * - Avanzar hacia el balón
 * - Retornar a posición base
 * - Cubrir zonas defensivas
 * - Realizar desmarques ofensivos
 * - Mantener posición
 *
 * @param datos Referencia constante a la estructura Game_data que contiene
 *              el estado completo del juego y la posición del jugador
 * @return String que representa el comando de movimiento a ejecutar
 *         (ej: "(turn 45)", "(dash 80 0)", "(move -30 15)", etc.)
 */
string player_moves(Game_data const &datos);

#endif // AREA_H
