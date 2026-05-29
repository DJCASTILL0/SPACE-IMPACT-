#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enemigos/EnemigoBase.h"
#include "FabricaEnemigos.generated.h"

UENUM()
enum class ETipoEnemigo : uint8 {
    Zangano,        // 0
    Cazador,        // 1
    Tanque,         // 2
    Interceptor,    // 3
    Buso,           // 4  NUEVO
    TanqueNavi      // 5  NUEVO
};

UCLASS()
class IMPACTOESPACIAL_API UFabricaEnemigos : public UObject
{
	GENERATED_BODY()

public:
	static AEnemigoBase* GenerarEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Ubicacion);
};