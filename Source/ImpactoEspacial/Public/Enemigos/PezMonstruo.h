// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "PezMonstruo.generated.h"

// ============================================================================
//  PezMonstruo  ->  JEFE del Nivel 2
//  Hereda de JefeBase. Su mec�nica especial es EMBESTIR: cada cierto tiempo
//  se lanza r�pido hacia la izquierda (hacia el jugador) durante unos segundos.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API APezMonstruo : public AJefeBase
{
    GENERATED_BODY()

public:
    APezMonstruo();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; // Mueve la embestida cuando est� activa

public:
    virtual void Disparar() override;
    void Embestir();                 // Inicia una embestida

private:
    bool bEstaEmbestiendo;           // �Est� embistiendo ahora?
    float TiempoEmbestida;           // Cu�nto lleva embistiendo
    FTimerHandle TemporizadorEmbestida; // Lanza una embestida cada X segundos
};