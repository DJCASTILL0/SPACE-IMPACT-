#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "Kraken.generated.h"

// ============================================================================
//  Kraken  ->  JEFE FINAL (Nivel 3)
//  Hereda de JefeBase. Es el m�s duro: alterna 3 ataques distintos usando un
//  contador (dirigido a ti, abanico amplio, y r�faga alta/baja).
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AKraken : public AJefeBase
{
    GENERATED_BODY()

public:
    AKraken();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Disparar() override; // Elige el ataque seg�n ContadorDisparo

private:
    int32 ContadorDisparo; // alterna entre los distintos ataques
};