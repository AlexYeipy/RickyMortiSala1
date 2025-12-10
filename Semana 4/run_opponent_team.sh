#!/bin/bash

# Solicita al usuario que ingrese el nombre del equipo contrario
echo "Ingrese el nombre del equipo contrario: "
read TEAM_NAME

# Ruta del ejecutable
PLAYER_EXECUTABLE="./player"

# Puerto base para el equipo contrario
BASE_PORT=8111

# Verifica que el ejecutable exista antes de continuar
if [ ! -f "$PLAYER_EXECUTABLE" ]; then
    echo "ERROR: Compila primero con: cmake .. && make"
    exit 1
fi

echo "Iniciando equipo contrario: $TEAM_NAME"

# Lanzar a los 11 jugadores
for i in {1..11}; do
    
    # Cada jugador de este equipo usa puertos BASE+11+NUM
    PORT=$((BASE_PORT + 11 + i))

    echo "Jugador $i -> puerto $PORT"

    # Jugador 1 → portero
    if [ "$i" -eq 1 ]; then
        ./player "$TEAM_NAME" "$PORT" "goalie" &
    else
        ./player "$TEAM_NAME" "$PORT" &
    fi

    sleep 0.3
done

# Fin del script (los jugadores siguen en background)

