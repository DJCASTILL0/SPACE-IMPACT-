#pragma once

#include "CoreMinimal.h"
#include "Armas/Arma.h"
#include "DecoradorArma.generated.h"

// ============================================================================
//  DecoradorArma  ->  DECORADOR BASE del PATRON DECORATOR
//  Es un arma que ENVUELVE a otra arma (ArmaEnvuelta) y delega en ella,
//  pudiendo anadir efectos extra. Se pueden APILAR varios decoradores.
// ============================================================================
UCLASS(Abstract)
class IMPACTOESPACIAL_API UDecoradorArma : public UArma
{
    GENERATED_BODY()

public:
    // El arma que este decorador envuelve (puede ser la basica u otro decorador).
    UPROPERTY()
    UArma* ArmaEnvuelta;

    void Envolver(UArma* Arma) { ArmaEnvuelta = Arma; }
};

// Decorador CONCRETO: ademas de lo que haga el arma envuelta, anade 2 balas
// en diagonal (convierte cualquier arma en "disparo en abanico").
UCLASS()
class IMPACTOESPACIAL_API UDecoradorDisparoTriple : public UDecoradorArma
{
    GENERATED_BODY()

public:
    virtual void Disparar(ANaveJugador* Nave, FVector Salida) override;
};
