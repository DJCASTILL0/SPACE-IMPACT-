#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EstrategiaMovimiento.generated.h"

UCLASS(Abstract)
class IMPACTOESPACIAL_API UEstrategiaMovimiento : public UObject
{
	GENERATED_BODY()

public:
	// M�todo ABSTRACTO del patr�n Strategy: es un contrato puro, sin cuerpo por
	// defecto. Cada estrategia concreta (Lineal, ZigZag, Diagonal, Perseguidor)
	// est� OBLIGADA a implementarlo. As� la base es una verdadera interfaz.
	virtual FVector CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad)
		PURE_VIRTUAL(UEstrategiaMovimiento::CalcularNuevaPosicion, return PosicionActual;);
};