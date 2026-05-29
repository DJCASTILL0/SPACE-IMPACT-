// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "Kraken.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AKraken : public AJefeBase
{
    GENERATED_BODY()

public:
    AKraken();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Disparar() override;

    UFUNCTION()
    void RecibirDanioEnOjo(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    // Ojo del Kraken (punto débil)
    UPROPERTY()
    class USphereComponent* OjoColision;
};