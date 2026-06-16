// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceImpactGameInstance.generated.h"

// ============================================================================
//  SpaceImpactGameInstance
//  El GameInstance "vive" durante toda la partida, aunque cambies de nivel.
//  Por eso aqu� guardamos lo que debe SOBREVIVIR entre mapas: la puntuaci�n
//  final, el nivel alcanzado y la referencia al GameManager.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API USpaceImpactGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Puntuaci�n que persiste entre niveles
    UPROPERTY(BlueprintReadWrite)
    int32 PuntuacionFinal;

    // Nivel actual del juego
    UPROPERTY(BlueprintReadWrite)
    int32 NivelAlcanzado;

    void GuardarPuntuacion(int32 Puntos, int32 Nivel);
    void ResetearDatos();

    // Referencia "fuerte" al GameManager activo. Al vivir aqu� (el GameInstance
    // persiste entre niveles y est� protegido del GC), el GameManager no se borra
    // a mitad de partida ni queda como puntero fantasma al cambiar de nivel.
    UPROPERTY()
    class UGameManager* GameManagerActivo;
};