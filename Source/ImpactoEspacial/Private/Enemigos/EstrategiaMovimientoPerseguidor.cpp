// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/EstrategiaMovimientoPerseguidor.h"
#include "Kismet/GameplayStatics.h" // Para poder encontrar al jugador

FVector UEstrategiaMovimientoPerseguidor::CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad)
{
	FVector NuevaPosicion = PosicionActual;

	// 1. Siempre avanzamos hacia la izquierda (hacia el jugador)
	NuevaPosicion.X -= Velocidad * DeltaTime;

	// 2. Buscamos la posición Z (altura) del jugador
	UWorld* Mundo = GetWorld();
	if (Mundo)
	{
		APawn* Jugador = UGameplayStatics::GetPlayerPawn(Mundo, 0);
		if (Jugador)
		{
			float AlturaJugador = Jugador->GetActorLocation().Z;

			// FInterpTo hace una interpolación suave (se mueve gradualmente hacia la altura del jugador)
			// El "2.0f" al final es la velocidad de giro (qué tan rápido corrige su rumbo)
			NuevaPosicion.Z = FMath::FInterpTo(PosicionActual.Z, AlturaJugador, DeltaTime, 2.0f);
		}
	}

	return NuevaPosicion;
}