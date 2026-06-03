#pragma once

#include "CoreMinimal.h"
#include "Enemigos/JefeBase.h"
#include "Kraken.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AKraken : public AJefeBase
{
    GENERATED_BODY()

public:
    AKraken();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Disparar() override;
};