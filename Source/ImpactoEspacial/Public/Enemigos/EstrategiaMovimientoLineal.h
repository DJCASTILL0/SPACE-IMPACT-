#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EstrategiaMovimientoLineal.generated.h"

UCLASS()
class IMPACTOESPACIAL_API UEstrategiaMovimientoLineal : public UEstrategiaMovimiento
{
	GENERATED_BODY()

public:
	// Sobrescribimos la función para que se mueva en línea recta a la izquierda
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) override;
};