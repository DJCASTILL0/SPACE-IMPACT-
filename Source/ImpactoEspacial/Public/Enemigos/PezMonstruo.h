// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "PezMonstruo.generated.h"

UCLASS()
class IMPACTOESPACIAL_API APezMonstruo : public AJefeBase
{
    GENERATED_BODY()

public:
    APezMonstruo();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    virtual void Disparar() override;
    void Embestir();

private:
    bool bEstaEmbestiendo;
    float TiempoEmbestida;
    FTimerHandle TemporizadorEmbestida;
};