#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneradorEnemigos.generated.h"

// ============================================================================
//  GeneradorEnemigos
//  Actor que colocas en el nivel. Con un temporizador va "soltando" enemigos
//  (pidi�ndoselos a la F�brica) y, cuando ya no quedan, suelta al jefe.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AGeneradorEnemigos : public AActor
{
	GENERATED_BODY()

public:
	AGeneradorEnemigos();
	void ReanudarGeneracion();   // Vuelve a activar el temporizador de spawn

protected:
	virtual void BeginPlay() override;

	// Funci�n que se llama repetidamente por el temporizador: crea un enemigo o el jefe.
	void AparecerEnemigo();


	FTimerHandle TemporizadorSpawn; // Temporizador que llama a AparecerEnemigo
	float TiempoEntreEnemigos;      // Segundos entre cada aparici�n
};