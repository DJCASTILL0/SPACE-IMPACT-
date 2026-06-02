// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/Kraken.h"
#include "Nucleo/GameManager.h"
#include "Proyectiles/ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AKraken::AKraken()
{
    Vida = 2500.f;
    Velocidad = 800.f;
    TiempoEntreDisparos = 0.52f;

    SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));

    // Crear el OJO (punto débil)
    OjoColision = CreateDefaultSubobject<USphereComponent>(TEXT("OjoColision"));
    OjoColision->InitSphereRadius(80.f);
    OjoColision->SetupAttachment(RootComponent);
    OjoColision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    OjoColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    OjoColision->OnComponentBeginOverlap.AddDynamic(this, &AKraken::RecibirDanioEnOjo);
}

void AKraken::BeginPlay()
{
    Super::BeginPlay();
}

void AKraken::Disparar()
{
    FVector Ubicacion = GetActorLocation();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = this;

    // Disparo en abanico de 5 proyectiles
    float Angulos[5] = { 220.f, 200.f, 180.f, 160.f, 140.f };
    float OffsetsZ[5] = { -160.f, -80.f, 0.f, 80.f, 160.f };

    for (int i = 0; i < 5; i++)
    {
        FVector Salida = Ubicacion + FVector(-150.f, 0.f, OffsetsZ[i]);
        AProyectilBase* Proy = GetWorld()->SpawnActor<AProyectilBase>(AProyectilBase::StaticClass(), Salida, FRotator::ZeroRotator, Params);
        if (Proy)
        {
            Proy->EstablecerTiempoVida(5.0f);
            Proy->ActivarProyectil(Salida, FRotator(Angulos[i], 0.f, 0.f));
        }
    }
}

void AKraken::RecibirDanioEnOjo(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AProyectilBase* Proyectil = Cast<AProyectilBase>(OtherActor);
        if (Proyectil && Proyectil->bEstaActivo)
        {
            // Daño x3 al ojo
            float Danio = Proyectil->CantidadDano * 2.f;
            Vida -= Danio;
            Proyectil->DesactivarProyectil();

            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
                    FString::Printf(TEXT("¡GOLPE EN EL OJO! -%.0f DAÑO"), Danio));

            if (Vida <= 0.f)
            {
                UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
                if (GM)
                {
                    GM->SumarPuntos(500);
                    GM->JefeDerrotado(GetWorld());
                }
                Destroy();
            }
        }
    }
}