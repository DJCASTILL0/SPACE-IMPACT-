// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/PezMonstruo.h"
#include "Proyectiles/ProyectilBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

APezMonstruo::APezMonstruo()
{
    Vida = 1500.f;
    Velocidad = 400.f;
    TiempoEntreDisparos = 0.57f;
    bEstaEmbestiendo = false;
    TiempoEmbestida = 0.0f;
    SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));

    // Cargar mesh del Pez Monstruo
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshPez(TEXT("/Game/Jugador/regularshiw/Destroyer.Destroyer"));
    if (MeshPez.Succeeded())
    {
        MallaEnemigo->SetStaticMesh(MeshPez.Object);
    }
}

void APezMonstruo::BeginPlay()
{
    Super::BeginPlay();

    // Embestir cada 5 segundos
    GetWorldTimerManager().SetTimer(TemporizadorEmbestida, this, &APezMonstruo::Embestir, 5.0f, true, 2.0f);
}

void APezMonstruo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bEstaEmbestiendo)
    {
        TiempoEmbestida += DeltaTime;

        // Avanzar rápido hacia la izquierda
        FVector PosicionActual = GetActorLocation();
        PosicionActual.X -= 1200.f * DeltaTime;
        SetActorLocation(PosicionActual);

        // Terminar embestida después de 1.5 segundos
        if (TiempoEmbestida >= 1.5f)
        {
            bEstaEmbestiendo = false;
            TiempoEmbestida = 0.0f;
        }
    }
}

void APezMonstruo::Disparar()
{
   
}

void APezMonstruo::Embestir()
{
    bEstaEmbestiendo = true;
    TiempoEmbestida = 0.0f;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("¡PEZ MONSTRUO EMBISTE!"));
}