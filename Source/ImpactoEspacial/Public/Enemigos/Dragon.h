// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "Dragon.generated.h"

UCLASS()
class IMPACTOESPACIAL_API ADragon : public AJefeBase
{
    GENERATED_BODY()

public:
    ADragon();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Disparar() override;
};