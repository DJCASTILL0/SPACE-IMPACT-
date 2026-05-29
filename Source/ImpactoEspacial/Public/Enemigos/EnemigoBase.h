#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemigos/EstrategiaMovimiento.h"
#include "EnemigoBase.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AEnemigoBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UBoxComponent* ComponenteColision;

	UPROPERTY()
	class UStaticMeshComponent* MallaEnemigo;

	// Referencia al Patrón Strategy
	UPROPERTY()
	UEstrategiaMovimiento* EstrategiaActual;
	// Función para detectar choques
	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	AEnemigoBase();
	virtual void Tick(float DeltaTime) override;

	// Función para asignar la estrategia (Inyección de dependencia)
	void EstablecerEstrategia(UEstrategiaMovimiento* NuevaEstrategia) { EstrategiaActual = NuevaEstrategia; }

	// Estadísticas
	float Vida;
	float Velocidad;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};