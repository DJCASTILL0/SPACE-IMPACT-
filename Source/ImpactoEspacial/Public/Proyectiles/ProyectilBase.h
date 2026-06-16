// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

// ============================================================================
//  ProyectilBase  ->  pieza del PATR�N OBJECT POOL
//  La bala b�sica. En vez de crearse y destruirse, se "enciende" (ActivarProyectil)
//  y se "apaga" (DesactivarProyectil) para reutilizarse desde la piscina de la nave.
//  Es la clase padre de los proyectiles especiales (L�ser, Misil...).
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AProyectilBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USphereComponent* ComponenteColision;   // Esfera que detecta el impacto

	UPROPERTY()
	UStaticMeshComponent* MallaProyectil;    // Malla visible de la bala

	// Manejador para que la bala vuelva a la piscina si no choca con nada
	FTimerHandle TemporizadorDesactivacion;
	float TiempoVida;                        // Segundos hasta apagarse sola

public:
	AProyectilBase();

	// L�gica de Piscina (Object Pool)
	bool bEstaActivo;                        // �Est� en uso o "dormida" en la piscina?
	virtual void ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion); // Enciende la bala
	void DesactivarProyectil();              // Apaga la bala y la devuelve a la piscina
	UPROPERTY()
	UProjectileMovementComponent* ComponenteMovimiento; // Mueve la bala automáticamente
	void EstablecerTiempoVida(float NuevoTiempo) { TiempoVida = NuevoTiempo; }
protected:
	virtual void BeginPlay() override;

	// En ProyectilBase.h

	//UFUNCTION()
	//virtual void AlImpactar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// Se llama cuando la bala se superpone con otro actor: le aplica da�o.
	UFUNCTION()
	void AlSuperponer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	float CantidadDano;   // Da�o que hace esta bala al impactar
};