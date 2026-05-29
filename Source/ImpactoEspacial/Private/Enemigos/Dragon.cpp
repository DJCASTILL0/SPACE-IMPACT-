// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/Dragon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Proyectiles/ProyectilBase.h"
#include "Kismet/GameplayStatics.h"

ADragon::ADragon()
{
    Vida = 1500.f;
    Velocidad = 400.f; // 1x
    TiempoEntreDisparos = 2.57f; // 1.75x ratio de disparo
}

void ADragon::BeginPlay()
{
    Super::BeginPlay();
}

void ADragon::Disparar()
{
    FVector Ubicacion = GetActorLocation();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = this;

    // Disparo recto (izquierda)
    FVector Salida1 = Ubicacion + FVector(-150.f, 0.f, 0.f);
    AProyectilBase* Proy1 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida1, FRotator::ZeroRotator, Params);
    if (Proy1)
    {
        Proy1->EstablecerTiempoVida(5.0f);
        Proy1->ActivarProyectil(Salida1, FRotator(180.f, 0.f, 0.f));
    }

    // Disparo arriba-izquierda (spawneo más arriba)
    FVector Salida2 = Ubicacion + FVector(-150.f, 0.f, 100.f);
    AProyectilBase* Proy2 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida2, FRotator::ZeroRotator, Params);
    if (Proy2)
    {
        Proy2->EstablecerTiempoVida(5.0f);
        Proy2->ActivarProyectil(Salida2, FRotator(160.f, 0.f, 0.f));
    }

    // Disparo abajo-izquierda (spawneo más abajo)
    FVector Salida3 = Ubicacion + FVector(-150.f, 0.f, -100.f);
    AProyectilBase* Proy3 = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida3, FRotator::ZeroRotator, Params);
    if (Proy3)
    {
        Proy3->EstablecerTiempoVida(5.0f);
        Proy3->ActivarProyectil(Salida3, FRotator(200.f, 0.f, 0.f));
    }
}