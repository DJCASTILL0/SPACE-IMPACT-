// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/JefeBase.h"
#include "TimerManager.h"

AJefeBase::AJefeBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Vida = 1500.f;
    Velocidad = 300.f;
    bMoviendoseArriba = true;
    TiempoEntreDisparos = 1.0f;

    // Más grande que los enemigos normales
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AJefeBase::BeginPlay()
{
    Super::BeginPlay();

    // Iniciar el patrón de disparo en bucle
    GetWorldTimerManager().SetTimer(TemporizadorDisparo, this, &AJefeBase::Disparar, TiempoEntreDisparos, true);
}

void AJefeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Movimiento vertical con rebote
    FVector PosicionActual = GetActorLocation();
    float LimiteSuperior = 800.f;
    float LimiteInferior = -800.f;

    if (bMoviendoseArriba)
    {
        PosicionActual.Z += Velocidad * DeltaTime;
        if (PosicionActual.Z >= LimiteSuperior)
        {
            PosicionActual.Z = LimiteSuperior;
            bMoviendoseArriba = false;
        }
    }
    else
    {
        PosicionActual.Z -= Velocidad * DeltaTime;
        if (PosicionActual.Z <= LimiteInferior)
        {
            PosicionActual.Z = LimiteInferior;
            bMoviendoseArriba = true;
        }
    }

    SetActorLocation(PosicionActual);
}

void AJefeBase::Disparar()
{
    // Vacío, cada jefe lo implementa
}