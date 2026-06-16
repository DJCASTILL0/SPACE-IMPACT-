// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Proyectiles/ProyectilBase.h"
#include "ProyectilLaser.generated.h"

// Láser recto: muy rápido y potente; ATRAVIESA enemigos (no se desactiva al tocar).
UCLASS()
class IMPACTOESPACIAL_API AProyectilLaser : public AProyectilBase
{
    GENERATED_BODY()

public:
    AProyectilLaser();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void AlSuperponerLaser(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion) override;
};