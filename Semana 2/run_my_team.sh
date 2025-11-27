#!/bin/bash

echo "Ingrese el nombre del equipo: "
read TEAM_NAME

PLAYER_EXECUTABLE="./player"
BASE_PORT=8111

# Verificar ejecutable
if [ ! -f "$PLAYER_EXECUTABLE" ]; then
    echo "ERROR: Compila primero con: cmake .. && make"
    exit 1
fi

echo "Iniciando equipo: $TEAM_NAME"

for i in {1..11}; do
    PORT=$((BASE_PORT + i))
    echo "Jugador $i -> puerto $PORT"
    ./player "$TEAM_NAME" "$PORT" &
    sleep 0.3
done
