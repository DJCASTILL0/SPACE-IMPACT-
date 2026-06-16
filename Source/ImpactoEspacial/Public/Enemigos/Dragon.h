// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "Dragon.generated.h"

// ============================================================================
//  Dragon  ->  JEFE del Nivel 1
//  Hereda de JefeBase. Solo define su vida/malla y su forma de disparar:
//  3 balas en abanico (recto, arriba y abajo).
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API ADragon : public AJefeBase
{
    GENERATED_BODY()

public:
    ADragon();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Disparar() override; // Su ataque propio (sobrescribe el de JefeBase)
};