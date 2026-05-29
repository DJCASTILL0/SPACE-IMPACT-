#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EstrategiaMovimiento.generated.h"

UCLASS(Abstract)
class IMPACTOESPACIAL_API UEstrategiaMovimiento : public UObject
{
	GENERATED_BODY()

public:
	// Función pura que cada estrategia implementará (Línea recta, zigzag, etc.)
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad) { return PosicionActual; }
};