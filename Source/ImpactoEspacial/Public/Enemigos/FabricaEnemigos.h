#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enemigos/EnemigoBase.h"
#include "FabricaEnemigos.generated.h"

// Declaraci�n anticipada (el tipo concreto solo se necesita en el .cpp)
class AJefeBase;

// Los tipos de enemigo normal que la fábrica sabe crear.
UENUM()
enum class ETipoEnemigo : uint8 {
    Zangano,        // 0
    Cazador,        // 1
    Tanque,         // 2
    Interceptor,    // 3
    Buso,           // 4
    TanqueNavi      // 5
};

// ============================================================================
//  FabricaEnemigos  ->  PATR�N FACTORY (F�brica)
//  �nico lugar que sabe CREAR enemigos y jefes. Quien necesita uno solo pide el
//  tipo/nivel y recibe el objeto ya configurado (vida, velocidad, estrategia,
//  malla). As� el resto del c�digo no conoce las clases concretas.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API UFabricaEnemigos : public UObject
{
    GENERATED_BODY()

public:
    // Fábrica de enemigos normales (decide el tipo concreto seg�n el enum).
    static AEnemigoBase* GenerarEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Ubicacion);

    // Fábrica de JEFES: �nico punto de creaci�n de los jefes seg�n el nivel.
    // As� ning�n cliente instancia un jefe concreto a mano.
    static AJefeBase* GenerarJefe(UWorld* Mundo, int32 Nivel, FVector Ubicacion);
};