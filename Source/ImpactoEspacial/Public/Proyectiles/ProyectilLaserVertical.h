// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Proyectiles/ProyectilBase.h"
#include "ProyectilLaserVertical.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AProyectilLaserVertical : public AProyectilBase
{
    GENERATED_BODY()

public:
    AProyectilLaserVertical();

    UFUNCTION()
    void AlSuperponerLaserVertical(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion) override;
};