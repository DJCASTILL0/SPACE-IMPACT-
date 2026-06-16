#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoLineal.generated.h"

// STRATEGY concreta: movimiento en LÍNEA RECTA hacia la izquierda.
UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoLineal : public UEstrategiaMovimiento
{
	GENERATED_BODY()

public:
	// Sobrescribimos la funci�n para que se mueva en l�nea recta a la izquierda
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};