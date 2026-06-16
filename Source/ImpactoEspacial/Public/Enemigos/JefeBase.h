// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EnemigoBase.h"
#include "JefeBase.generated.h"

// ============================================================================
//  JefeBase
//  Clase padre de los jefes (Dragon, PezMonstruo, Kraken). Hereda de EnemigoBase
//  y a�ade el comportamiento com�n de un jefe: se mueve arriba/abajo rebotando y
//  dispara en bucle por temporizador. El M�TODO Disparar() es virtual: cada jefe
//  lo implementa a su manera (esto es polimorfismo / Template Method).
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AJefeBase : public AEnemigoBase
{
    GENERATED_BODY()

public:
    AJefeBase();

protected:
    virtual void BeginPlay() override; // Arranca el temporizador de disparo

    // El jefe se mueve verticalmente rebotando en los bordes
    bool bMoviendoseArriba;            // Direcci�n actual del rebote (true = sube)

public:
    virtual void Tick(float DeltaTime) override; // Movimiento vertical con rebote

    // Patr�n de disparo (cada jefe lo implementa distinto)
    virtual void Disparar();

protected:
    FTimerHandle TemporizadorDisparo;  // Temporizador que llama a Disparar() en bucle
    float TiempoEntreDisparos;         // Cada cu�ntos segundos dispara
};