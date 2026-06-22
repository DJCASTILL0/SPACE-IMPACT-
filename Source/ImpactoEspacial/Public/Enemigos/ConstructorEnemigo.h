#pragma once

#include "CoreMinimal.h"
#include "Enemigos/EstrategiaMovimiento.h"

class AEnemigoBase;

// ============================================================================
//  ConstructorEnemigo  ->  PATRON BUILDER
//  Construye un AEnemigoBase PASO A PASO con metodos encadenados, en vez de
//  configurarlo todo de golpe. Ejemplo:
//      FConstructorEnemigo().ConVida(100).ConVelocidad(400)
//          .ConEstrategia(UEstrategiaMovimientoLineal::StaticClass())
//          .ConMalla(TEXT("/Game/..."))
//          .Construir(Mundo, Ubicacion);
//  La Fabrica de enemigos lo usa por dentro (combo Factory + Builder).
// ============================================================================
class IMPACTOESPACIAL_API FConstructorEnemigo
{
public:
    // Pasos de construccion: cada uno devuelve *this para poder encadenarlos.
    FConstructorEnemigo& ConVida(float NuevaVida);
    FConstructorEnemigo& ConVelocidad(float NuevaVelocidad);
    FConstructorEnemigo& ConEscala(float NuevaEscala);
    FConstructorEnemigo& ConMalla(const FString& Ruta);
    FConstructorEnemigo& ConEstrategia(TSubclassOf<UEstrategiaMovimiento> Clase);

    // Paso FINAL: crea el enemigo en el mundo ya configurado y lo devuelve.
    AEnemigoBase* Construir(UWorld* Mundo, FVector Ubicacion);

private:
    float Vida = 100.f;
    float Velocidad = 400.f;
    float Escala = 1.0f;
    FString RutaMalla;
    TSubclassOf<UEstrategiaMovimiento> ClaseEstrategia;
};
