// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoPerseguidor.generated.h"

// STRATEGY concreta: PERSIGUE al jugador (ajusta su altura hacia la nave).
UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoPerseguidor : public UEstrategiaMovimiento
{
	GENERATED_BODY()

public:
	// Sobrescribimos la funci�n para seguir al jugador
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};