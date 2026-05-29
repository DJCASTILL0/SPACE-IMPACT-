// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyectiles/ProyectilLaser.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AProyectilLaser::AProyectilLaser()
{
    ComponenteMovimiento->InitialSpeed = 3000.f;
    ComponenteMovimiento->MaxSpeed = 3000.f;
    CantidadDano = 200.f;
    TiempoVida = 0.5f;

    MallaProyectil->SetWorldScale3D(FVector(0.8f, 0.1f, 0.1f));

    // Overlap para atravesar enemigos
    ComponenteColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ComponenteColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectilLaser::AlSuperponerLaser);
}

void AProyectilLaser::BeginPlay()
{
    Super::BeginPlay();
}

void AProyectilLaser::ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion)
{
    bEstaActivo = true;
    SetActorLocation(NuevaUbicacion);
    SetActorRotation(NuevaRotacion);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    ComponenteMovimiento->Activate();
    ComponenteMovimiento->Velocity = NuevaRotacion.Vector() * ComponenteMovimiento->InitialSpeed;

    GetWorldTimerManager().SetTimer(TemporizadorDesactivacion, this, &AProyectilLaser::DesactivarProyectil, TiempoVida, false);
}

void AProyectilLaser::AlSuperponerLaser(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherActor != GetInstigator()))
    {
        UGameplayStatics::ApplyDamage(OtherActor, CantidadDano, GetInstigatorController(), this, UDamageType::StaticClass());
    }
}