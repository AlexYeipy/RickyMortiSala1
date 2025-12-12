/**
 * @file AbsPos.cpp
 * @brief Implementación del módulo de posicionamiento absoluto
 *
 * Este archivo contiene las funciones para determinar la posición absoluta
 * del jugador en el campo utilizando triangulación con puntos de referencia
 * conocidos (flags). Implementa algoritmos de localización por trilateración.
 */

#include "AbsPos.h"
#include "structs.h"
#include <MinimalSocket/udp/UdpSocket.h>
#include "utilities.h"
#include <iostream>

/**
 * @brief Inicializa el vector de flags con las coordenadas absolutas conocidas
 *
 * Crea y configura todos los puntos de referencia estáticos del campo con
 * sus coordenadas absolutas predefinidas. El campo está representado como
 * un sistema de coordenadas de 120x80 unidades.
 *
 * @param flags Referencia al vector de shared_ptr<knownFlags> a inicializar
 *
 * @note Coordenadas del campo: X: 0-120 (ancho), Y: 0-80 (alto)
 * @note Centro del campo: (60, 40)
 */
void inicializacion_flags(vector<shared_ptr<knownFlags>> &flags)
{
    // ======================================================
    // PUNTOS CENTRALES Y ESQUINAS PRINCIPALES
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f c)", 60, 40, 999.0, 999.0));      // centro del campo
    flags.push_back(make_shared<knownFlags>("(f l t)", 8, 74, 999.0, 999.0));     // esquina superior izquierda
    flags.push_back(make_shared<knownFlags>("(f l b)", 8, 6, 999.0, 999.0));      // esquina inferior izquierda
    flags.push_back(make_shared<knownFlags>("(f r t)", 112, 74, 999.0, 999.0));   // esquina superior derecha
    flags.push_back(make_shared<knownFlags>("(f r b)", 112, 6, 999.0, 999.0));    // esquina inferior derecha

    // ======================================================
    // PUNTOS DEL ÁREA PENAL
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f p l t)", 24, 60, 999.0, 999.0));  // área superior izquierda
    flags.push_back(make_shared<knownFlags>("(f p l c)", 24, 40, 999.0, 999.0));  // área central izquierda
    flags.push_back(make_shared<knownFlags>("(f p l b)", 24, 20, 999.0, 999.0));  // área inferior izquierda
    flags.push_back(make_shared<knownFlags>("(f p r t)", 96, 60, 999.0, 999.0));  // área superior derecha
    flags.push_back(make_shared<knownFlags>("(f p r c)", 96, 40, 999.0, 999.0));  // área central derecha
    flags.push_back(make_shared<knownFlags>("(f p r b)", 96, 20, 999.0, 999.0));  // área inferior derecha

    // ======================================================
    // PALOS DE PORTERÍA
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f g l t)", 8, 47, 999.0, 999.0));   // palo superior izquierdo
    flags.push_back(make_shared<knownFlags>("(f g l b)", 8, 33, 999.0, 999.0));   // palo inferior izquierdo
    flags.push_back(make_shared<knownFlags>("(f g r t)", 112, 47, 999.0, 999.0)); // palo superior derecho
    flags.push_back(make_shared<knownFlags>("(f g r b)", 112, 33, 999.0, 999.0)); // palo inferior derecho

    // ======================================================
    // BANDA SUPERIOR (FUERA DEL CAMPO)
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f t l 10)", 50, 80, 999.0, 999.0)); // banda superior izquierda (-10)
    flags.push_back(make_shared<knownFlags>("(f t l 20)", 40, 80, 999.0, 999.0)); // banda superior izquierda (-20)
    flags.push_back(make_shared<knownFlags>("(f t l 30)", 30, 80, 999.0, 999.0)); // banda superior izquierda (-30)
    flags.push_back(make_shared<knownFlags>("(f t l 50)", 10, 80, 999.0, 999.0)); // banda superior izquierda (-50)
    flags.push_back(make_shared<knownFlags>("(f t 0)", 60, 80, 999.0, 999.0));    // centro banda superior
    flags.push_back(make_shared<knownFlags>("(f t r 10)", 70, 80, 999.0, 999.0)); // banda superior derecha (+10)
    flags.push_back(make_shared<knownFlags>("(f t r 20)", 80, 80, 999.0, 999.0)); // banda superior derecha (+20)
    flags.push_back(make_shared<knownFlags>("(f t r 30)", 90, 80, 999.0, 999.0)); // banda superior derecha (+30)
    flags.push_back(make_shared<knownFlags>("(f t r 40)", 100, 80, 999.0, 999.0)); // banda superior derecha (+40)
    flags.push_back(make_shared<knownFlags>("(f t r 50)", 110, 80, 999.0, 999.0)); // banda superior derecha (+50)

    // ======================================================
    // BANDA INFERIOR (FUERA DEL CAMPO)
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f b l 10)", 50, 0, 999.0, 999.0));   // banda inferior izquierda (-10)
    flags.push_back(make_shared<knownFlags>("(f b l 20)", 40, 0, 999.0, 999.0));   // banda inferior izquierda (-20)
    flags.push_back(make_shared<knownFlags>("(f b l 30)", 30, 0, 999.0, 999.0));   // banda inferior izquierda (-30)
    flags.push_back(make_shared<knownFlags>("(f b l 40)", 20, 0, 999.0, 999.0));   // banda inferior izquierda (-40)
    flags.push_back(make_shared<knownFlags>("(f b l 50)", 10, 0, 999.0, 999.0));   // banda inferior izquierda (-50)
    flags.push_back(make_shared<knownFlags>("(f b 0)", 60, 0, 999.0, 999.0));      // centro banda inferior
    flags.push_back(make_shared<knownFlags>("(f b r 10)", 70, 0, 999.0, 999.0));   // banda inferior derecha (+10)
    flags.push_back(make_shared<knownFlags>("(f b r 20)", 80, 0, 999.0, 999.0));   // banda inferior derecha (+20)
    flags.push_back(make_shared<knownFlags>("(f b r 30)", 90, 0, 999.0, 999.0));   // banda inferior derecha (+30)
    flags.push_back(make_shared<knownFlags>("(f b r 40)", 100, 0, 999.0, 999.0));  // banda inferior derecha (+40)
    flags.push_back(make_shared<knownFlags>("(f b r 50)", 110, 0, 999.0, 999.0));  // banda inferior derecha (+50)

    // ======================================================
    // LÍNEA DE FONDO IZQUIERDA (FUERA DEL CAMPO)
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f l t 10)", 0, 50, 999.0, 999.0));   // fondo izquierdo superior (-10)
    flags.push_back(make_shared<knownFlags>("(f l t 20)", 0, 60, 999.0, 999.0));   // fondo izquierdo superior (-20)
    flags.push_back(make_shared<knownFlags>("(f l t 30)", 0, 70, 999.0, 999.0));   // fondo izquierdo superior (-30)
    flags.push_back(make_shared<knownFlags>("(f l b 10)", 0, 30, 999.0, 999.0));   // fondo izquierdo inferior (-10)
    flags.push_back(make_shared<knownFlags>("(f l b 20)", 0, 20, 999.0, 999.0));   // fondo izquierdo inferior (-20)
    flags.push_back(make_shared<knownFlags>("(f l b 30)", 0, 10, 999.0, 999.0));   // fondo izquierdo inferior (-30)

    // ======================================================
    // LÍNEA DE FONDO DERECHA (FUERA DEL CAMPO)
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f r t 10)", 120, 50, 999.0, 999.0)); // fondo derecho superior (-10)
    flags.push_back(make_shared<knownFlags>("(f r t 20)", 120, 60, 999.0, 999.0)); // fondo derecho superior (-20)
    flags.push_back(make_shared<knownFlags>("(f r t 30)", 120, 70, 999.0, 999.0)); // fondo derecho superior (-30)
    flags.push_back(make_shared<knownFlags>("(f r b 10)", 120, 30, 999.0, 999.0)); // fondo derecho inferior (-10)
    flags.push_back(make_shared<knownFlags>("(f r b 20)", 120, 20, 999.0, 999.0)); // fondo derecho inferior (-20)
    flags.push_back(make_shared<knownFlags>("(f r b 30)", 120, 10, 999.0, 999.0)); // fondo derecho inferior (-30)

    // ======================================================
    // PUNTOS CENTRALES ADICIONALES
    // ======================================================
    flags.push_back(make_shared<knownFlags>("(f c t)", 60, 74, 999.0, 999.0));    // centro banda superior
    flags.push_back(make_shared<knownFlags>("(f c b)", 60, 6, 999.0, 999.0));     // centro banda inferior
    flags.push_back(make_shared<knownFlags>("(f c r)", 120, 40, 999.0, 999.0));   // centro línea de fondo derecha
    flags.push_back(make_shared<knownFlags>("(f c l)", 0, 40, 999.0, 999.0));     // centro línea de fondo izquierda
}

/**
 * @brief Muestra en consola la información de todos los flags
 *
 * Función de depuración que imprime el estado actual de todos los flags,
 * incluyendo nombre, coordenadas absolutas y medidas relativas actuales.
 *
 * @param flag Referencia al vector de flags a imprimir
 */
void imprimir_matriz(vector<shared_ptr<knownFlags>> &flag)
{
    for (auto const &f : flag)
    {
        cout << "Nombre: " << f->name;
        cout << " Distancia: " << f->distancia;
        cout << " Direccion: " << f->direccion;
        cout << " X_absoluta: " << f->x;
        cout << " Y_absoluta: " << f->y;
        cout << endl;
    }
}

/**
 * @brief Procesa un mensaje "see" y actualiza las distancias/direcciones de flags visibles
 *
 * Analiza un mensaje de percepción del simulador, extrae información sobre
 * flags visibles y actualiza sus distancias y direcciones relativas.
 * Flags no visibles se marcan con 999.0 (valor sentinela).
 *
 * @param mensaje Cadena del mensaje "see" recibido del servidor
 * @param flags Referencia al vector de flags a actualizar
 *
 * @note Formato mensaje: "(see ((f c) 15.2 30.5) ((f l t) 20.1 45.3) ...)"
 */
void crear_matriz_valores_absolutos(string const &mensaje, vector<shared_ptr<knownFlags>> &flags)
{
    if (mensaje.find("see") == -1)
        return;
    vector<string> vector_mensaje = separate_string(mensaje);                // see ((  f l c (2132)) )
    vector<string> vector_mensaje_2 = separate_string(vector_mensaje.at(0)); // {1} see {2} (f c l)(12) (23) .... {23} ((b) 12 23.3)
    for (auto const &f : flags)
    {
        if (mensaje.find(f->name) == -1)
        {
            f->direccion = 999.0;
            f->distancia = 999.0;
        }
        else
        {
            for (auto const &v : vector_mensaje_2)
            {
                if (v.find(f->name) != -1)
                {
                    vector<string> vector_balon;
                    string nueva_linea = vector_separar_string(v);
                    vector_balon = separate_string_separator(nueva_linea, ' ');

                    // EJEMPLO ((b) 12 23.3)
                    f->distancia = stod(vector_balon.at(0)); // 12
                    f->direccion = stod(vector_balon.at(1)); // 23.3
                }
            }
            // inicializacion_flags << "Algo ha fallado" << endl;
        }
    }
    sort(flags.begin(), flags.end(), [](const shared_ptr<knownFlags> &f1, const shared_ptr<knownFlags> &f2)
         { return f1->distancia < f2->distancia; });
}

/**
 * @brief Calcula la posición absoluta del jugador mediante trilateración
 *
 * Utiliza tres flags visibles para triangular la posición del jugador en
 * coordenadas absolutas del campo. Implementa algoritmo de trilateración
 * geométrica con manejo de casos especiales (colinealidad, errores).
 *
 * @param flags Vector de flags visibles ordenados por distancia
 * @param datos Estructura Game_data donde se almacenará la posición calculada
 *
 * @algorithm Basado en trilateración con 3 puntos y solución de triángulos
 * @note Usa los 3 flags más cercanos para mayor precisión
 */
void relative2Abssolute(vector<shared_ptr<knownFlags>> &flags, Game_data &datos)
{
    // ======================================================
    // EXTRACCIÓN DE DATOS DE LOS 3 FLAGS MÁS CERCANOS
    // ======================================================
    float theta1 = -flags.at(0)->direccion;  // Ángulo al flag 1 (convertido a sistema matemático)
    float d1 = flags.at(0)->distancia;       // Distancia al flag 1
    float x1 = flags.at(0)->x;               // Coordenada X absoluta del flag 1
    float y1 = flags.at(0)->y;               // Coordenada Y absoluta del flag 1

    float theta2 = -flags.at(1)->direccion;  // Ángulo al flag 2
    float d2 = flags.at(1)->distancia;       // Distancia al flag 2
    float x2 = flags.at(1)->x;               // Coordenada X absoluta del flag 2
    float y2 = flags.at(1)->y;               // Coordenada Y absoluta del flag 2

    float theta3 = -flags.at(2)->direccion;  // Ángulo al flag 3
    float d3 = flags.at(2)->distancia;       // Distancia al flag 3
    float x3 = flags.at(2)->x;               // Coordenada X absoluta del flag 3
    float y3 = flags.at(2)->y;               // Coordenada Y absoluta del flag 3

    // ======================================================
    // DETECCIÓN Y MANEJO DE COLINEALIDAD
    // ======================================================
    // Si flag1 y flag2 están casi alineados (ángulos similares),
    // usamos flag3 en lugar de flag2 para evitar indeterminación geométrica
    float errorTheta = abs((theta1 - theta2) / theta1);
    if (errorTheta < 0.01)  // Umbral de 1% de diferencia angular
    {
        // Intercambiamos flag2 con flag3
        float auxTheta = theta2;
        float auxD = d2;
        float auxX = x2;
        float auxY = y2;

        theta2 = theta3;
        d2 = d3;
        x2 = x3;
        y2 = y3;

        theta3 = auxTheta;
        d3 = auxD;
        x3 = auxX;
        y3 = auxY;

        // cout << "f1 and f2 collinear, changing to f3" << endl;
    }

    // ======================================================
    // CÁLCULOS TRIGONOMÉTRICOS PRELIMINARES
    // ======================================================
    // Conversión de grados a radianes para funciones trigonométricas
    float cosTheta1 = cos((M_PI / 180) * theta1);
    float sinTheta1 = sin((M_PI / 180) * theta1);
    float cosTheta2 = cos((M_PI / 180) * theta2);
    float sinTheta2 = sin((M_PI / 180) * theta2);

    // ======================================================
    // CÁLCULO DE DISTANCIA ENTRE FLAGS (LADO R DEL TRIÁNGULO)
    // ======================================================
    // Ley de cosenos: R² = d1² + d2² - 2*d1*d2*cos(θ1-θ2)
    float R;
    R = sqrt(d1 * d1 + d2 * d2 - 2 * d1 * d2 * cos((M_PI / 180) * (theta1 - theta2)));
    // cout << "R: " << R << endl;

    // ======================================================
    // CÁLCULO DEL ÁNGULO PSI (FASE)
    // ======================================================
    // Componentes para calcular atan2(b/a)
    float psi; // Ángulo de fase en radianes
    float b = d2 * sinTheta2 - d1 * sinTheta1;
    float a = d1 * cosTheta1 - d2 * cosTheta2;

    // Manejo de valores cercanos a cero para estabilidad numérica
    if (abs(a) < 0.01) { a = 0; }
    if (abs(b) < 0.01) { b = 0; }
    // cout << "a: " << a << "\t" << "b: " << b << endl;

    psi = atan2(b, a);  // atan2 maneja correctamente el cuadrante
    // cout << "psiº: " << (180/M_PI)*psi << endl;

    // ======================================================
    // CÁLCULO DEL ÁNGULO BETA
    // ======================================================
    // cos(β) = (x1 - x2) / R (distancia entre flags en X dividido por R)
    float beta; // Ángulo en radianes
    float cosBeta = (x1 - x2) / R;
    // cout << "cosBeta: " << cosBeta << endl;

    // Validación del coseno (debe estar entre -1 y 1)
    if (abs(cosBeta) <= 1)
    {
        beta = acos(cosBeta);
    }
    else
    {
        beta = 0; // Valor por defecto en caso de error numérico
        // cout << "error, cosBeta > 1" << endl;
    }
    // cout << "beta: " << beta << endl;

    // ======================================================
    // DOS SOLUCIONES POSIBLES (AMBIGÜEDAD DE TRILATERACIÓN)
    // ======================================================
    // La trilateración con 2 puntos da 2 soluciones simétricas
    float alpha1 = psi + beta;  // Solución 1
    float alpha2 = psi - beta;  // Solución 2
    // cout << "alpha1º: " << (180/M_PI)*alpha1 << "\t" << "alpha2º: " << (180/M_PI)*alpha2 << endl;

    // ======================================================
    // CÁLCULO DE LAS DOS POSICIONES CANDIDATAS
    // ======================================================
    // Fórmula de rotación: posición = flag - d * [cos(θ)cos(α)-sin(θ)sin(α), cos(θ)sin(α)+sin(θ)cos(α)]
    float xp1 = x1 - d1 * (cosTheta1 * cos(alpha1) - sinTheta1 * sin(alpha1));
    float yp1 = y1 - d1 * (cosTheta1 * sin(alpha1) + sinTheta1 * cos(alpha1));
    // cout << "x1: " << xp1 << "\t" << "y1: " << yp1 << endl;

    float xp2 = x1 - d1 * (cosTheta1 * cos(alpha2) - sinTheta1 * sin(alpha2));
    float yp2 = y1 - d1 * (cosTheta1 * sin(alpha2) + sinTheta1 * cos(alpha2));
    // cout << "x2: " << xp2 << "\t" << "y2: " << yp2 << endl;

    // ======================================================
    // DESAMBIGUACIÓN USANDO EL TERCER FLAG
    // ======================================================
    // Calculamos distancia de cada solución candidata al tercer flag
    float distPlayer1Flag3 = sqrt((xp1 - x3) * (xp1 - x3) + (yp1 - y3) * (yp1 - y3));
    float distPlayer2Flag3 = sqrt((xp2 - x3) * (xp2 - x3) + (yp2 - y3) * (yp2 - y3));

    float x_player;
    float y_player;

    // Calculamos error relativo para cada solución
    float error1 = abs((d3 - distPlayer1Flag3) / d3);
    float error2 = abs((d3 - distPlayer2Flag3) / d3);

    // ======================================================
    // SELECCIÓN DE LA MEJOR SOLUCIÓN
    // ======================================================
    // Caso 1: Ambas soluciones tienen alto error -> tomamos promedio
    if (error1 >= 0.5 && error2 >= 0.5)
    {
        x_player = (xp1 + xp2) / 2;
        y_player = (yp1 + yp2) / 2;
        // cout << "Position not accurate, approximation given" << endl;
    }
    // Caso 2: Solución 1 tiene menor error
    else if (error1 < error2)
    {
        x_player = xp1;
        y_player = yp1;
    }
    // Caso 3: Solución 2 tiene menor error
    else
    {
        x_player = xp2;
        y_player = yp2;
    }

    // ======================================================
    // ALMACENAMIENTO DEL RESULTADO
    // ======================================================
    // cout << "xPlayer: " << x_player << "\t" << "yPlayer: " << y_player << endl;
    datos.jugador.x_absoluta = x_player;
    datos.jugador.y_absoluta = y_player;
}

/**
 * @brief Función de comparación para ordenar flags por distancia
 *
 * Predicado utilizado por algoritmos de ordenación para organizar
 * flags de más cercano a más lejano.
 *
 * @param f1 Primer flag a comparar
 * @param f2 Segundo flag a comparar
 * @return true si f1 está más cerca que f2, false en caso contrario
 */
bool compareDistance(const shared_ptr<knownFlags> &f1, const shared_ptr<knownFlags> &f2)
{
    return (f1->distancia < f2->distancia);
}
