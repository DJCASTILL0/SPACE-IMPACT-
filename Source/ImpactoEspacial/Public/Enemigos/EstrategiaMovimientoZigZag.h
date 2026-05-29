// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoZigZag.generated.h"

UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoZigZag : public UEstrategiaMovimiento
{
	GENERATED_BODY()

public:
	// Sobrescribimos la función matemática
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};