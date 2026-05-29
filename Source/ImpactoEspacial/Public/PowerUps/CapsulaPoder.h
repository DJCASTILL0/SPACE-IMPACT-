// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapsulaPoder.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class IMPACTOESPACIAL_API ACapsulaPoder : public AActor
{
	GENERATED_BODY()

public:
	ACapsulaPoder();

protected:
	// --- COMPONENTES ---
	UPROPERTY(VisibleAnywhere)
	USphereComponent* ComponenteColision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MallaCapsula;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ComponenteMovimiento;

	// --- FUNCIÓN DE SUPERPOSICIÓN ---
	// Usaremos Overlap en lugar de Hit para que la nave la "atraviese" y la recoja suavemente
	UFUNCTION()
	void AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};