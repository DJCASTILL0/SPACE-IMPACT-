// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class IMPACTOESPACIAL_API AProyectilBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USphereComponent* ComponenteColision;

	UPROPERTY()
	UStaticMeshComponent* MallaProyectil;

	// Manejador para que la bala vuelva a la piscina si no choca con nada
	FTimerHandle TemporizadorDesactivacion;
	float TiempoVida;

public:
	AProyectilBase();

	// Lógica de Piscina
	bool bEstaActivo;
	virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion);
	void DesactivarProyectil();
	UPROPERTY()
	UProjectileMovementComponent* ComponenteMovimiento;
	void EstablecerTiempoVida(float NuevoTiempo) { TiempoVida = NuevoTiempo; }
protected:
	virtual void BeginPlay() override;

	// En ProyectilBase.h

	//UFUNCTION()
	//virtual void AlImpactar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void AlSuperponer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	float CantidadDano;
};