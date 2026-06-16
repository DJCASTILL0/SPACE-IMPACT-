#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapaEstelar.generated.h"

// ============================================================================
//  MapaEstelar
//  Genera el ESCENARIO por c�digo al iniciar: un fondo de estrellas y las
//  paredes (suelo/techo) del t�nel por el que vuela la nave.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AMapaEstelar : public AActor
{
    GENERATED_BODY()

public:
    AMapaEstelar();

protected:
    virtual void BeginPlay() override; // Construye el fondo y el suelo al arrancar

private:
    void CrearFondoEstrellado();       // Crea las 200 estrellas
    void CrearSuelo();                 // Crea las paredes del t�nel

    UPROPERTY()
    TArray<class UStaticMeshComponent*> Estrellas;

    UPROPERTY()
    class UStaticMeshComponent* SueloArriba;

    UPROPERTY()
    class UStaticMeshComponent* SueloAbajo;
private:
    UPROPERTY()
    UStaticMesh* MeshEsfera;

    UPROPERTY()
    UStaticMesh* MeshCubo;
};