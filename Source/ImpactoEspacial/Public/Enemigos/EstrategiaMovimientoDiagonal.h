// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoDiagonal.generated.h"

UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoDiagonal : public UEstrategiaMovimiento
{
    GENERATED_BODY()

public:
    virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};