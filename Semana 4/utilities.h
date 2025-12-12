/**
 * @file utils.h
 * @brief Archivo de cabecera para funciones utilitarias de procesamiento de cadenas
 *
 * Este módulo proporciona funciones auxiliares para manipulación y procesamiento
 * de strings, especialmente útiles para parsear mensajes del protocolo del simulador.
 * Incluye funciones para dividir cadenas, manejar paréntesis y procesar tokens.
 */

#ifndef UTILS_H
#define UTILS_H

// Inclusión de dependencias
#include <iostream>
#include <string>    // Para manipulación de cadenas
#include <vector>    // Para contenedores dinámicos de strings

using namespace std;

/**
 * @brief Divide una cadena en subcadenas usando un separador específico
 *
 * Similar a la función split() de otros lenguajes, divide un string en tokens
 * basándose en un carácter delimitador. Útil para procesar mensajes con
 * formato delimitado por espacios, comas u otros caracteres.
 *
 * @param str Cadena de entrada a dividir
 * @param separador Carácter delimitador para la división (ej: ' ', ',', ';')
 * @return Vector de strings conteniendo los tokens resultantes
 *
 * @example
 * split("hola mundo ejemplo", ' ') -> {"hola", "mundo", "ejemplo"}
 * split("10,20,30", ',') -> {"10", "20", "30"}
 */
vector<string> separate_string_separator(string const &str, char separador);

/**
 * @brief Procesa una cadena con estructura de paréntesis anidados
 *
 * Extrae bloques de texto delimitados por paréntesis, respetando la anidación.
 * Esencial para procesar mensajes S-expressions del simulador RCSS,
 * que utilizan paréntesis para estructurar información jerárquica.
 *
 * @param str Cadena con paréntesis anidados (ej: "(see (b) (g l))")
 * @return Vector de strings con los bloques extraídos, sin los paréntesis externos
 *
 * @example
 * GestionParentesis("(see (b) (g l))") -> {"see", "(b)", "(g l)"}
 * GestionParentesis("((a b) (c d) (e f))") -> {"a b", "c d", "e f"}
 */
vector<string> separate_string(string const &str);

/**
 * @brief Procesa una línea de datos para extraer valores individuales
 *
 * Función especializada para procesar líneas del simulador que contienen
 * múltiples valores numéricos y simbólicos. Maneja formatos mixtos
 * que incluyen paréntesis, números y palabras clave.
 *
 * @param linea Cadena con múltiples valores (ej: "(b) 12.5 45.3")
 * @return String procesado o primer token extraído
 *
 * @example
 * vector_separar_string("(b) 12.5 45.3") -> "b"
 * vector_separar_string("(g l) 30.2 15.8") -> "g l"
 */
string vector_separar_string(string const &linea);

#endif // UTILS_H
