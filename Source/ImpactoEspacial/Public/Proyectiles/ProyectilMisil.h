// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Proyectiles/ProyectilBase.h"
#include "ProyectilMisil.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AProyectilMisil : public AProyectilBase
{
    GENERATED_BODY()

public:
    AProyectilMisil();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    float TiempoAcumulado;

public:
    virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion) override;
};