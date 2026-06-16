// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/Kraken.h"
#include "Proyectiles/ProyectilBase.h"
#include "Kismet/GameplayStatics.h" // para apuntar al jugador

AKraken::AKraken()
{
    Vida = 2500.f;
    Velocidad = 800.f;
    TiempoEntreDisparos = 0.52f;
    ContadorDisparo = 0;
    SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

    // Cargar mesh del Kraken
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshKraken(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
    if (MeshKraken.Succeeded())
    {
        MallaEnemigo->SetStaticMesh(MeshKraken.Object);
    }
}

void AKraken::BeginPlay()
{
    Super::BeginPlay();
}

void AKraken::Disparar()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    // Punto de salida SIEMPRE fuera del cuerpo (hacia el jugador, -X)
    const FVector Salida = GetActorLocation() + FVector(-350.f, 0.f, 0.f);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = this;

    // Pequeña función interna para no repetir el spawn en cada disparo
    auto LanzarBala = [&](const FRotator& Rot, float TiempoVida)
        {
            AProyectilBase* P = Mundo->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida, Rot, Params);
            if (P)
            {
                P->EstablecerTiempoVida(TiempoVida);
                P->ActivarProyectil(Salida, Rot);
            }
        };

    // Alternamos entre 3 ataques para que el jefe se sienta variado
    const int32 Fase = ContadorDisparo % 3;
    ContadorDisparo++;

    switch (Fase)
    {
        // ===== FASE 0: DIRIGIDO A TI (3 balas que van hacia tu posición) =====
    case 0:
    {
        APawn* Jugador = UGameplayStatics::GetPlayerPawn(Mundo, 0);
        if (!Jugador) { LanzarBala(FRotator(180.f, 0.f, 0.f), 5.f); break; }

        FVector Dir = (Jugador->GetActorLocation() - Salida).GetSafeNormal();
        FRotator RotAim = Dir.Rotation();

        LanzarBala(RotAim, 5.f);                        // al centro (a donde estás)
        LanzarBala(RotAim + FRotator(12.f, 0.f, 0.f), 5.f);  // un poco arriba
        LanzarBala(RotAim + FRotator(-12.f, 0.f, 0.f), 5.f); // un poco abajo
        break;
    }

    // ===== FASE 1: ABANICO AMPLIO (5 balas, te obliga a esquivar) =====
    case 1:
    {
        const float Angulos[] = { 150.f, 165.f, 180.f, 195.f, 210.f };
        for (float Pitch : Angulos)
        {
            LanzarBala(FRotator(Pitch, 0.f, 0.f), 5.f);
        }
        break;
    }

    // ===== FASE 2: DOBLE RÁFAGA ALTA Y BAJA (presiona arriba/abajo) =====
    case 2:
    {
        LanzarBala(FRotator(165.f, 0.f, 0.f), 4.f); // arriba-izquierda
        LanzarBala(FRotator(195.f, 0.f, 0.f), 4.f); // abajo-izquierda
        break;
    }
    }
}