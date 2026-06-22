#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Arma.generated.h"

class ANaveJugador;

// ============================================================================
//  Arma  ->  COMPONENTE BASE del PATRON DECORATOR
//  Define la interfaz de "un arma": saber Disparar(). La nave guarda un UArma
//  y los power-ups la ENVUELVEN con decoradores para anadirle efectos.
// ============================================================================
UCLASS(Abstract)
class IMPACTOESPACIAL_API UArma : public UObject
{
    GENERATED_BODY()

public:
    // Dispara desde la posicion dada. La nave da acceso a su piscina de balas.
    virtual void Disparar(ANaveJugador* Nave, FVector Salida)
        PURE_VIRTUAL(UArma::Disparar, );
};

// Arma CONCRETA basica: dispara una sola bala recta (el comportamiento por defecto).
UCLASS()
class IMPACTOESPACIAL_API UArmaBasica : public UArma
{
    GENERATED_BODY()

public:
    virtual void Disparar(ANaveJugador* Nave, FVector Salida) override;
};
