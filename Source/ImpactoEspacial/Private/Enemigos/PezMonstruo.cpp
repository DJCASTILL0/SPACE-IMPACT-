// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/PezMonstruo.h"
#include "Proyectiles/ProyectilBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

APezMonstruo::APezMonstruo()
{
    Vida = 1500.f;
    Velocidad = 600.f; // 1.5x
    TiempoEntreDisparos = 0.57f; // 1.75x ratio
    bEstaEmbestiendo = false;
    TiempoEmbestida = 0.0f;

    SetActorScale3D(FVector(4.0f, 4.0f, 4.0f));
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
    FVector Ubicacion = GetActorLocation();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = this;

    // Disparo en abanico de 3
    FVector Salida1 = Ubicacion + FVector(-150.f, 0.f, 0.f);
    AProyectilBase* Proy1 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida1, FRotator::ZeroRotator, Params);
    if (Proy1) { Proy1->EstablecerTiempoVida(5.0f); Proy1->ActivarProyectil(Salida1, FRotator(180.f, 0.f, 0.f)); }

    FVector Salida2 = Ubicacion + FVector(-150.f, 0.f, 100.f);
    AProyectilBase* Proy2 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida2, FRotator::ZeroRotator, Params);
    if (Proy2) { Proy2->EstablecerTiempoVida(5.0f); Proy2->ActivarProyectil(Salida2, FRotator(160.f, 0.f, 0.f)); }

    FVector Salida3 = Ubicacion + FVector(-150.f, 0.f, -100.f);
    AProyectilBase* Proy3 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida3, FRotator::ZeroRotator, Params);
    if (Proy3) { Proy3->EstablecerTiempoVida(5.0f); Proy3->ActivarProyectil(Salida3, FRotator(200.f, 0.f, 0.f)); }
}

void APezMonstruo::Embestir()
{
    bEstaEmbestiendo = true;
    TiempoEmbestida = 0.0f;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("¡PEZ MONSTRUO EMBISTE!"));
}