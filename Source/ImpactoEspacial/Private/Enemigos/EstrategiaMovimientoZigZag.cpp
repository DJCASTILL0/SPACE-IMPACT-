
#include "Enemigos/EstrategiaMovimientoZigZag.h"

FVector UEstrategiaMovimientoZigZag::CalcularNuevaPosicion(FVector PosicionActual, float DeltaTime, float Velocidad)
{
	// 1. Movimiento constante hacia la izquierda (Eje X negativo)
	float AvanceX = PosicionActual.X - (Velocidad * DeltaTime);

	// 2. Cálculo del Zig-Zag (Onda Senoidal en el Eje Z)
	UWorld* Mundo = GetWorld();
	float Tiempo = Mundo ? Mundo->GetTimeSeconds() : 0.0f;

	float Frecuencia = 5.0f; // Qué tan rápido sube y baja
	float Amplitud = 600.0f; // Qué tan alto/bajo llega

	// Calculamos la velocidad vertical en este frame usando el Seno del tiempo
	float VelocidadZ = FMath::Sin(Tiempo * Frecuencia) * Amplitud;
	float AvanceZ = PosicionActual.Z + (VelocidadZ * DeltaTime);

	// Devolvemos el vector combinado (Avanza en X, mantiene Y, oscila en Z)
	return FVector(AvanceX, PosicionActual.Y, AvanceZ);
}