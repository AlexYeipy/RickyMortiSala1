/**
 * @file posicion_abs.h
 * @brief Archivo de cabecera para el módulo de posicionamiento absoluto
 *
 * Este módulo proporciona funciones para procesar información del entorno
 * y convertir coordenadas relativas a absolutas en el campo de fútbol.
 * Utilizado en el sistema de percepción del simulador de fútbol robótico.
 */

#ifndef POSICION_ABS_H
#define POSICION_ABS_H

// Inclusión de dependencias necesarias
#include "structs.h"           // Estructuras de datos del juego (Player, Ball, Goal, etc.)
#include <memory>              // Punteros inteligentes (shared_ptr)
#include <string>              // Manipulación de cadenas
#include <vector>              // Contenedor de datos dinámico
#include <unistd.h>            // Funcionalidades del sistema POSIX
#include <cmath>               // Funciones matemáticas (sqrt, sin, cos, etc.)

using namespace std;

/**
 * @brief Inicializa el vector de flags conocidos con valores predeterminados
 *
 * Esta función prepara la estructura de datos que almacenará las referencias
 * espaciales (flags) del campo. Cada flag representa un punto de referencia
 * con coordenadas absolutas conocidas.
 *
 * @param flags Referencia al vector de shared_ptr<knownFlags> a inicializar
 */
void inicializacion_flags(vector<shared_ptr<knownFlags>> &flags);

/**
 * @brief Procesa un mensaje de percepción para crear una matriz de valores absolutos
 *
 * Analiza el mensaje recibido del servidor (generalmente mensajes "see")
 * y extrae información sobre objetos visibles, almacenándolos como flags
 * con sus respectivas coordenadas relativas.
 *
 * @param mensaje String que contiene el mensaje de percepción del servidor
 * @param flags Referencia al vector donde se almacenarán los flags procesados
 */
void crear_matriz_valores_absolutos(string const &mensaje, vector<shared_ptr<knownFlags>> &flags);

/**
 * @brief Separa una línea de datos en componentes individuales
 *
 * Procesa una cadena que contiene múltiples valores espaciales (distancia,
 * ángulo, etc.) y los separa en tokens individuales para su procesamiento.
 *
 * @param linea Cadena de entrada con múltiples valores concatenados
 * @return String procesado o token individual
 */
string vector_separar_string(string const &linea);

/**
 * @brief Imprime la matriz de flags para depuración
 *
 * Muestra en consola el contenido completo del vector de flags,
 * incluyendo coordenadas relativas y absolutas de cada objeto visible.
 *
 * @param flag Referencia al vector de flags a imprimir
 */
void imprimir_matriz(vector<shared_ptr<knownFlags>> &flag);

/**
 * @brief Convierte coordenadas relativas a coordenadas absolutas
 *
 * Utiliza información de posición y orientación del jugador para transformar
 * coordenadas relativas (distancia y ángulo desde el jugador) a coordenadas
 * absolutas en el sistema de referencia del campo.
 *
 * @param flags Vector de flags con coordenadas relativas a convertir
 * @param datos Estructura con la información del juego y posición del jugador
 */
void relative2Abssolute(vector<shared_ptr<knownFlags>> &flags, Game_data &datos);

/**
 * @brief Función de comparación para ordenar flags por distancia
 *
 * Utilizada como criterio de ordenación para algoritmos de sorting.
 * Compara dos flags basándose en su distancia al jugador.
 *
 * @param f1 Primer flag a comparar
 * @param f2 Segundo flag a comparar
 * @return true si f1 está más cerca que f2, false en caso contrario
 */
bool compareDistance(const shared_ptr<knownFlags> &f1, const shared_ptr<knownFlags> &f2);

#endif // POSICION_ABS_H
