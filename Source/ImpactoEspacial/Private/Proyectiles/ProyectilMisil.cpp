// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyectiles/ProyectilMisil.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProyectilMisil::AProyectilMisil()
{
    PrimaryActorTick.bCanEverTick = true;

    ComponenteMovimiento->InitialSpeed = 1200.f;
    ComponenteMovimiento->MaxSpeed = 1200.f;
    CantidadDano = 150.f;
    TiempoVida = 3.0f;

    MallaProyectil->SetWorldScale3D(FVector(0.5f, 0.2f, 0.2f));
}

void AProyectilMisil::BeginPlay()
{
    Super::BeginPlay();
    TiempoAcumulado = 0.0f;
}

void AProyectilMisil::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bEstaActivo) return;

    TiempoAcumulado += DeltaTime;

    // Movimiento oscilatorio en Z (senoidal)
    FVector PosicionActual = GetActorLocation();
    float Frecuencia = 8.0f;
    float Amplitud = 1000.0f;
    float OscilacionZ = FMath::Sin(TiempoAcumulado * Frecuencia) * Amplitud * DeltaTime;

    SetActorLocation(FVector(PosicionActual.X, PosicionActual.Y, PosicionActual.Z + OscilacionZ));
}

void AProyectilMisil::ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion)
{
    bEstaActivo = true;
    TiempoAcumulado = 0.0f;
    SetActorLocation(NuevaUbicacion);
    SetActorRotation(NuevaRotacion);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    ComponenteMovimiento->Activate();
    ComponenteMovimiento->Velocity = NuevaRotacion.Vector() * ComponenteMovimiento->InitialSpeed;

    GetWorldTimerManager().SetTimer(TemporizadorDesactivacion, this, &AProyectilMisil::DesactivarProyectil, TiempoVida, false);
}