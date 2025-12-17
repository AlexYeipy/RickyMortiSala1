# Simulador de Fútbol Robótico - RCSS

## Descripción
Proyecto de simulación de fútbol robótico para la RoboCup Soccer Simulator (RCSS). Implementa un equipo completo de 11 jugadores con lógica de comportamiento individual y colectivo.

## Requisitos Previos

### 1. Instalar dependencias en Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install cmake g++ libminimalsocket-dev
```

### 2. Instalar el servidor RCSS (simulador):
```bash
sudo apt-get install rcssserver rcssmonitor rcsslogplayer
```

## Estructura del Proyecto

```
├── run_my_team.sh          # Script para iniciar el equipo completo
├── main.cpp                # Punto de entrada principal
├── CMakeLists.txt          # Configuración de compilación
├── funciones.cpp/.h        # Lógica principal del jugador
├── structs.h              # Estructuras de datos
├── AbsPos.cpp/.h          # Posicionamiento absoluto
├── datos_area.cpp/.h      # Gestión de áreas del campo
├── utilities.cpp/.h       # Funciones utilitarias
└── Funciones.h           # Cabecera principal
```

## Instrucciones de Ejecución

### 1. Compilar el proyecto:
```bash
mkdir build
cd build
cmake ..
make
```

### 2. Iniciar el servidor RCSS (en otra terminal):
```bash
rcssserver
```

### 3. Usar el monitor para visualizar:
```bash
rcssmonitor
```

### 4. Iniciar el equipo completo (11 jugadores):
```bash
chmod +x run_my_team.sh
./run_my_team.sh
```
Cuando se solicite, ingresar el nombre del equipo.

## Uso Individual (para pruebas)

### Ejecutar un jugador específico:
```bash
# Jugador normal
./player <nombre_equipo> <puerto>

# Portero
./player <nombre_equipo> <puerto> goalie
```

Ejemplos:
```bash
./player MiEquipo 9000
./player MiEquipo 9001 goalie
```

## Script de Inicio Automático (run_my_team.sh)

El script `run_my_team.sh` automatiza:
- Inicio de los 11 jugadores (puertos 9000-9011)
- Formación táctica 4-4-2

## Características Implementadas

### Lógica por Posición:
- **Portero**: Atrapes, saques de puerta, salidas
- **Defensas**: Marcaje, recuperación, pases seguros
- **Mediocentros**: Distribución, transición ofensiva-defensiva
- **Delanteros**: Finalización, desmarques, presión

### Funcionalidades:
- Posicionamiento absoluto usando flags del campo
- Comportamiento diferenciado por áreas del campo
- Detección de compañeros y rivales
- Toma de decisiones táctica (pase, disparo, regate)
- Manejo de saques especiales (córner, falta, penalti)

## Solución de Problemas

### Error "No se puede abrir el socket":
- Asegurarse que los puertos 9000-9011 estén libres
- Verificar que el servidor RCSS esté ejecutándose

### Jugadores no aparecen en el campo:
- Verificar conexión al puerto 6000 del servidor
- Comprobar que todos los jugadores se hayan iniciado

### Errores de compilación:
- Verificar instalación de MinimalSocket
- Asegurar que CMake encuentra todas las dependencias

## Formación del Equipo (4-4-2)

1. Portero (-50, 0)
2. Lateral derecho (-35, 20)
3. Lateral izquierdo (-35, -20)
4. Defensa central derecho (-35, 5)
5. Defensa central izquierdo (-35, -5)
6. Mediocentro derecho (-15, -10)
7. Extremo derecho (-15, 25)
8. Mediocentro izquierdo (-15, 10)
9. Delantero centro uno (-2, 10)
10. Delantero centro dos (-2, -10)
11. Extremo izquierdo (-15, -25)

## Notas Técnicas

- **Protocolo**: S-Expressions (formato LISP-like)
- **Comunicación**: UDP/IPv6 con IPv4 compatible
- **Ciclo de juego**: Percepción → Decisión → Acción

## Créditos
Desarrollado para el curso de formación en C++ para Oesía por parte de Ibertech
