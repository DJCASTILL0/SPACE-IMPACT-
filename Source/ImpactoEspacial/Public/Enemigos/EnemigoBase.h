// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EnemigoBase.generated.h"

// ============================================================================
//  EnemigoBase  ->  usa el PATR�N STRATEGY para moverse
//  Clase padre de todos los enemigos. Tiene su colisi�n, su malla y una
//  "estrategia de movimiento" intercambiable. En su Tick le pide a la estrategia
//  la nueva posici�n; as� cada enemigo se mueve distinto sin cambiar esta clase.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AEnemigoBase : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY()
    class UBoxComponent* ComponenteColision;       // Caja que detecta choques con la nave

    UPROPERTY()
    class UStaticMeshComponent* MallaEnemigo;       // Malla visible principal

    UPROPERTY()
    class UStaticMeshComponent* MallaSecundaria;    // Mallas extra (formas compuestas)

    UPROPERTY()
    class UStaticMeshComponent* MallaTerciaria;

protected:
    // STRATEGY: puntero a la estrategia de movimiento actual (Lineal, ZigZag...).
    UPROPERTY()
    UEstrategiaMovimiento* EstrategiaActual;

    // Se llama autom�ticamente cuando el enemigo choca con algo (evento Observer de Unreal).
    UFUNCTION()
    void AlChocar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
    AEnemigoBase();
    virtual void Tick(float DeltaTime) override;   // Cada frame: mueve seg�n la estrategia

    // STRATEGY: cambia la estrategia de movimiento (incluso en pleno juego).
    void EstablecerEstrategia(UEstrategiaMovimiento* NuevaEstrategia) { EstrategiaActual = NuevaEstrategia; }

    float Vida;
    float Velocidad;

    // Seguro de muerte: evita que la l�gica de muerte (puntos, subir de nivel,
    // Destroy) se ejecute varias veces si llegan varios impactos en el mismo
    // frame. Sin esto, un jefe pod�a "subir" 2-3 niveles de golpe.
    bool bMuerto = false;

    void ConfigurarApariencia(FColor ColorPrincipal, FColor ColorSecundario); // Colorea las mallas
public:
    void EstablecerMesh(const TCHAR* RutaMesh); // Carga una malla desde una ruta de Content
    // Recibe da�o; si la vida llega a 0, da puntos y se destruye.
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};