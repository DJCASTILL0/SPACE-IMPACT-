// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyectiles/ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AProyectilBase::AProyectilBase()
{
	// Optimización: Desactivamos el Tick porque el ProjectileMovementComponent ya se encarga de la posición
	PrimaryActorTick.bCanEverTick = false;

	// Estado inicial para la piscina
	bEstaActivo = false;
	TiempoVida = 2.0f; // La bala vuelve a la piscina tras 2 segundos de vuelo
	// 1. Configurar Colisión
	ComponenteColision = CreateDefaultSubobject<USphereComponent>(TEXT("ComponenteColision"));
	ComponenteColision->InitSphereRadius(15.0f);
	ComponenteColision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ComponenteColision->OnComponentBeginOverlap.AddDynamic(this, &AProyectilBase::AlSuperponer);
	RootComponent = ComponenteColision;

	// 2. Configurar Malla Visual
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaProyectil"));
	MallaProyectil->SetupAttachment(RootComponent);
	MallaProyectil->SetCollisionProfileName(TEXT("NoCollision"));
	MallaProyectil->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RecursoEsfera(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (RecursoEsfera.Succeeded())
	{
		MallaProyectil->SetStaticMesh(RecursoEsfera.Object);
	}

	// 3. Componente de Movimiento
	ComponenteMovimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ComponenteMovimiento"));
	ComponenteMovimiento->UpdatedComponent = ComponenteColision;
	ComponenteMovimiento->InitialSpeed = 1500.f;
	ComponenteMovimiento->MaxSpeed = 1500.f;
	ComponenteMovimiento->bRotationFollowsVelocity = true;
	ComponenteMovimiento->ProjectileGravityScale = 0.f; // Sin gravedad en el espacio

	// Restringir al plano 2D (Eje Y bloqueado)
	ComponenteMovimiento->bConstrainToPlane = true;
	ComponenteMovimiento->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// 4. Estadísticas
	CantidadDano = 25.0f;

	// IMPORTANTE: Ponemos LifeSpan en 0 para que Unreal NO destruya el objeto.
	// Nosotros lo gestionaremos con la piscina.
	InitialLifeSpan = 0.0f;
}

void AProyectilBase::BeginPlay()
{
	Super::BeginPlay();
}



void AProyectilBase::ActivarProyectil(FVector NuevaUbicacion, FRotator NuevaRotacion)
{
	bEstaActivo = true;
	SetActorLocation(NuevaUbicacion);
	SetActorRotation(NuevaRotacion);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	ComponenteMovimiento->Activate();
	ComponenteMovimiento->Velocity = NuevaRotacion.Vector() * ComponenteMovimiento->InitialSpeed;
	
	GetWorldTimerManager().SetTimer(TemporizadorDesactivacion, this, &AProyectilBase::DesactivarProyectil, TiempoVida, false);

	// if (GetOwner())
// {
//     ComponenteColision->IgnoreActorWhenMoving(GetOwner(), true);
// }
}

void AProyectilBase::DesactivarProyectil()
{
	// Limpiamos el temporizador por si acaso la bala desactivó por choque antes de tiempo
	GetWorldTimerManager().ClearTimer(TemporizadorDesactivacion);

	bEstaActivo = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	ComponenteMovimiento->Deactivate();

	SetActorLocation(FVector(-10000.f, -10000.f, -10000.f));
}



void AProyectilBase::AlSuperponer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		UGameplayStatics::ApplyDamage(OtherActor, CantidadDano, GetInstigatorController(), this, UDamageType::StaticClass());
		DesactivarProyectil();
	}
}