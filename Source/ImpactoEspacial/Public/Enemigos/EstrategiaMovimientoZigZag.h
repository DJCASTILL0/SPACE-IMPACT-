// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoZigZag.generated.h"

// STRATEGY concreta: avanza a la izquierda mientras sube y baja (onda senoidal).
UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoZigZag : public UEstrategiaMovimiento
{
	GENERATED_BODY()

public:
	// Sobrescribimos la funci�n matem�tica
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};