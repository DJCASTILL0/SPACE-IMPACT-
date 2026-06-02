
#include "Enemigos/EnemigoBase.h"
#include "Nucleo/GameManager.h"
#include "Enemigos/JefeBase.h"
#include "Jugador/NaveJugador.h"
#include "Enemigos/GeneradorEnemigos.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
AEnemigoBase::AEnemigoBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ComponenteColision = CreateDefaultSubobject<UBoxComponent>(TEXT("ColisionEnemigo"));
	ComponenteColision->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	RootComponent = ComponenteColision;
	// Perfil para asegurar que choque con la nave
	ComponenteColision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	// Enlazar el evento de impacto
	ComponenteColision->OnComponentHit.AddDynamic(this, &AEnemigoBase::AlChocar);


	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	MallaEnemigo->SetupAttachment(RootComponent);

	// Placeholder: Un cubo para los enemigos
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CuboAsset.Succeeded()) { MallaEnemigo->SetStaticMesh(CuboAsset.Object); }

	Vida = 100.f;
	Velocidad = 400.f;
}

void AEnemigoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Usamos la Estrategia para movernos
	if (EstrategiaActual)
	{
		FVector NuevaPos = EstrategiaActual->CalcularNuevaPosicion(GetActorLocation(), DeltaTime, Velocidad);
		SetActorLocation(NuevaPos);
	}
}

float AEnemigoBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DanioRecibido = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Vida -= DanioRecibido;

	if (Vida <= 0.0f)
	{
		ANaveJugador* MiNave = Cast<ANaveJugador>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (MiNave)
		{
			MiNave->SumarPuntos(100);
		}

		UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
		if (GM)
		{
			GM->SumarPuntos(100);
			GM->EnemigoEliminado();
			// Si el enemigo era un jefe, notificar al GameManager
			if (this->IsA(AJefeBase::StaticClass()))
			{
				GM->JefeDerrotado(GetWorld());  // Pasar el mundo

				// Buscar el generador y reanudarlo cuando empiece el siguiente nivel
				TArray<AActor*> Generadores;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeneradorEnemigos::StaticClass(), Generadores);
				if (Generadores.Num() > 0)
				{
					AGeneradorEnemigos* Gen = Cast<AGeneradorEnemigos>(Generadores[0]);
					if (Gen)
					{
						// Reanudar después de 4 segundos (dar tiempo al mensaje)
						FTimerHandle TimerReanudar;
						GetWorld()->GetTimerManager().SetTimer(TimerReanudar, Gen,
							&AGeneradorEnemigos::ReanudarGeneracion, 4.0f, false);
					}
				}
			}
			// Si era un jefe, avisar al GameManager
			if (this->IsA(AJefeBase::StaticClass()))
			{
				GM->JefeDerrotado(GetWorld());
			}
		}

		Destroy();
	}

	return DanioRecibido;
}

void AEnemigoBase::AlChocar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		ANaveJugador* NaveChocada = Cast<ANaveJugador>(OtherActor);

		if (NaveChocada)
		{
			UGameplayStatics::ApplyDamage(NaveChocada, 50.0f, nullptr, this, UDamageType::StaticClass());

			// Solo los enemigos normales se autodestruyen, los jefes no
			if (!this->IsA(AJefeBase::StaticClass()))
			{
				Destroy();
			}
		}
	}
}