// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceImpactGameInstance.generated.h"

UCLASS()
class IMPACTOESPACIAL_API USpaceImpactGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Puntuación que persiste entre niveles
    UPROPERTY(BlueprintReadWrite)
    int32 PuntuacionFinal;

    // Nivel actual del juego
    UPROPERTY(BlueprintReadWrite)
    int32 NivelAlcanzado;

    void GuardarPuntuacion(int32 Puntos, int32 Nivel);
    void ResetearDatos();
};