// Copyright Epic Games, Inc. All Rights Reserved.

// Incluimos el archivo de cabecera de esta misma clase
#include "Enemigos/EstrategiaMovimientoDiagonal.h"

// Definición del método CalcularNuevaPosicion de la estrategia Diagonal
// Este método se llama cada frame desde el Tick del enemigo
FVector UEstrategiaMovimientoDiagonal::CalcularNuevaPosicion(
    FVector PosicionActual,   
    float DeltaTime,          
    float Velocidad)          
{
   
    //          AvanceX = PosicionActual.X - (400 * 0.016)
    //          AvanceX = PosicionActual.X - 6.4  (se mueve 6.4 unidades a la izquierda)
    float AvanceX = PosicionActual.X - (Velocidad * DeltaTime);


    //          AvanceZ = PosicionActual.Z - (400 * 0.5 * 0.016)
    //          AvanceZ = PosicionActual.Z - 3.2  (se mueve 3.2 unidades hacia abajo)
    float AvanceZ = PosicionActual.Z - (Velocidad * 0.5f * DeltaTime);


    // - Y: Se mantiene igual (no se mueve en profundidad)
    // - Z: Avanza hacia abajo (más lento que en X)
    return FVector(AvanceX, PosicionActual.Y, AvanceZ);

    
}