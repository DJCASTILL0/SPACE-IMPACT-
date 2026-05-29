// Copyright Epic Games, Inc. All Rights Reserved.

#include "PowerUps/CapsulaPoder.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Jugador/NaveJugador.h"

ACapsulaPoder::ACapsulaPoder()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Colisión (Configurada como "Overlap" o "Gatillo")
	ComponenteColision = CreateDefaultSubobject<USphereComponent>(TEXT("ComponenteColision"));
	ComponenteColision->InitSphereRadius(30.0f);
	// Importante: Queremos que detecte colisiones pero no bloquee físicamente a la nave
	ComponenteColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ComponenteColision->OnComponentBeginOverlap.AddDynamic(this, &ACapsulaPoder::AlSuperponerse);
	RootComponent = ComponenteColision;

	// 2. Malla Visual (Usaremos un cilindro básico para que parezca una batería/cápsula)
	MallaCapsula = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaCapsula"));
	MallaCapsula->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RecursoCilindro(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (RecursoCilindro.Succeeded()) { MallaCapsula->SetStaticMesh(RecursoCilindro.Object); }

	// La hacemos más pequeña y la acostamos un poco
	MallaCapsula->SetWorldScale3D(FVector(0.3f, 0.3f, 0.5f));
	MallaCapsula->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	// Le quitamos la colisión física a la malla para que solo actúe la esfera invisible
	MallaCapsula->SetCollisionProfileName(TEXT("NoCollision"));

	// 3. Movimiento (Para que flote lentamente hacia la izquierda de la pantalla)
	ComponenteMovimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ComponenteMovimiento"));
	ComponenteMovimiento->UpdatedComponent = ComponenteColision;
	ComponenteMovimiento->InitialSpeed = 200.f; // Flota muy lento
	ComponenteMovimiento->MaxSpeed = 200.f;
	ComponenteMovimiento->ProjectileGravityScale = 0.f;

	// Le damos una dirección inicial hacia la izquierda (-X)
	ComponenteMovimiento->Velocity = FVector(-1.0f, 0.0f, 0.0f) * ComponenteMovimiento->InitialSpeed;

	// Desaparece sola si nadie la agarra después de 15 segundos
	InitialLifeSpan = 15.0f;
}

void ACapsulaPoder::AlSuperponerse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificamos si lo que nos tocó fue la Nave
	if (OtherActor != nullptr && OtherActor != this)
	{
		ANaveJugador* Nave = Cast<ANaveJugador>(OtherActor);

		if (Nave)
		{
			// ¡Le damos el poder!
			Nave->ActivarDisparoMultiple();

			// Sumamos unos puntitos extra por recogerla (Opcional)
			Nave->SumarPuntos(50);

			// La cápsula se destruye al ser recogida
			Destroy();
		}
	}
}