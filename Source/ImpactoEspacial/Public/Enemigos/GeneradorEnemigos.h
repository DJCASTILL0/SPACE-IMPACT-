#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneradorEnemigos.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AGeneradorEnemigos : public AActor
{
	GENERATED_BODY()

public:
	AGeneradorEnemigos();
	void ReanudarGeneracion();

protected:
	virtual void BeginPlay() override;

	// Función que se llamará repetidamente
	void AparecerEnemigo();


	FTimerHandle TemporizadorSpawn;
	float TiempoEntreEnemigos;
};