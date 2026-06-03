// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/Kraken.h"
#include "Proyectiles/ProyectilBase.h"

AKraken::AKraken()
{
    Vida = 500.f;
    Velocidad = 800.f;
    TiempoEntreDisparos = 0.52f;

    SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));
}

void AKraken::BeginPlay()
{
    Super::BeginPlay();
}

void AKraken::Disparar()
{
    // Sin disparo para evitar crashes
}