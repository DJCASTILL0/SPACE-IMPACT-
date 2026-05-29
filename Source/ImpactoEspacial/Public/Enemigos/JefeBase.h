// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EnemigoBase.h"
#include "JefeBase.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AJefeBase : public AEnemigoBase
{
    GENERATED_BODY()

public:
    AJefeBase();

protected:
    virtual void BeginPlay() override;

    // El jefe se mueve verticalmente rebotando en los bordes
    bool bMoviendoseArriba;

public:
    virtual void Tick(float DeltaTime) override;

    // Patrón de disparo (cada jefe lo implementa distinto)
    virtual void Disparar();

protected:
    FTimerHandle TemporizadorDisparo;
    float TiempoEntreDisparos;
};