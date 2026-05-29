// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyectiles/ProyectilLaserVertical.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProyectilLaserVertical::AProyectilLaserVertical()
{
    ComponenteMovimiento->InitialSpeed = 800.f;
    ComponenteMovimiento->MaxSpeed = 800.f;
    CantidadDano = 150.f;
    TiempoVida = 3.0f;

    MallaProyectil->SetWorldScale3D(FVector(0.1f, 0.1f, 3.0f));
    ComponenteMovimiento->bConstrainToPlane = false;

    // Overlap para atravesar enemigos
    ComponenteColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ComponenteColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectilLaserVertical::AlSuperponerLaserVertical);
}

void AProyectilLaserVertical::ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion)
{
    bEstaActivo = true;
    SetActorLocation(NuevaUbicacion);
    SetActorRotation(NuevaRotacion);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    ComponenteMovimiento->Activate();
    ComponenteMovimiento->Velocity = FVector(1.0f, 0.0f, 0.0f) * ComponenteMovimiento->InitialSpeed;

    GetWorldTimerManager().SetTimer(TemporizadorDesactivacion, this, &AProyectilLaserVertical::DesactivarProyectil, TiempoVida, false);
}

void AProyectilLaserVertical::AlSuperponerLaserVertical(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherActor != GetInstigator()))
    {
        UGameplayStatics::ApplyDamage(OtherActor, CantidadDano, GetInstigatorController(), this, UDamageType::StaticClass());
    }
}