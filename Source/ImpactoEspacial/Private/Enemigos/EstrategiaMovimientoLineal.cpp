



#include "Enemigos/EstrategiaMovimientoLineal.h"

FVector UEstrategiaMovimientoLineal::CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad)
{
	
	FVector Direccion = FVector(-1.0f, 0.0f, 0.0f);

	return PosicionActual + (Direccion * Velocidad * DeltaTime);
}