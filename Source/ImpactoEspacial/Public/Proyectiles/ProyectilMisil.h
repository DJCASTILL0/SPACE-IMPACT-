// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Proyectiles/ProyectilBase.h"
#include "ProyectilMisil.generated.h"

// Misil: se mueve ondulando arriba y abajo (movimiento senoidal en su Tick).
UCLASS()
class IMPACTOESPACIAL_API AProyectilMisil : public AProyectilBase
{
    GENERATED_BODY()

public:
    AProyectilMisil();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; // Aplica la oscilación cada frame

private:
    float TiempoAcumulado;   // Tiempo desde que se disparó (para la onda)

public:
    virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion) override;
};