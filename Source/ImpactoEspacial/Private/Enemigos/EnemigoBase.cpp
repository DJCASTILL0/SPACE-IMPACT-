
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
	ComponenteColision->InitBoxExtent(FVector(90.f, 90.f, 90.f));
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
	// Segunda malla (para formas compuestas)
	MallaSecundaria = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaSecundaria"));
	MallaSecundaria->SetupAttachment(RootComponent);

	// Tercera malla (para formas compuestas)
	MallaTerciaria = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaTerciaria"));
	MallaTerciaria->SetupAttachment(RootComponent);
	Vida = 100.f;
	Velocidad = 400.f;
}

void AEnemigoBase::ConfigurarApariencia(FColor ColorPrincipal, FColor ColorSecundario)
{
	// Crear material din�mico para colorear
	UMaterialInstanceDynamic* MaterialPrincipal = MallaEnemigo->CreateAndSetMaterialInstanceDynamic(0);
	if (MaterialPrincipal)
	{
		MaterialPrincipal->SetVectorParameterValue(FName("BaseColor"), ColorPrincipal);
	}

	UMaterialInstanceDynamic* MaterialSecundario = MallaSecundaria->CreateAndSetMaterialInstanceDynamic(0);
	if (MaterialSecundario)
	{
		MaterialSecundario->SetVectorParameterValue(FName("BaseColor"), ColorSecundario);
	}

	UMaterialInstanceDynamic* MaterialTerciario = MallaTerciaria->CreateAndSetMaterialInstanceDynamic(0);
	if (MaterialTerciario)
	{
		MaterialTerciario->SetVectorParameterValue(FName("BaseColor"), ColorSecundario);
	}
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

	if (Vida <= 0.0f && !bMuerto)
	{
		// A partir de aqu� el enemigo ya est� muerto: marcamos el seguro y
		// apagamos la colisi�n para no recibir m�s impactos en este frame.
		// Esto evita que la l�gica de muerte (puntos / subir de nivel) corra
		// varias veces y haga saltar al jefe siguiente (Dragon -> Kraken).
		bMuerto = true;
		SetActorEnableCollision(false);

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

			if (this->IsA(AJefeBase::StaticClass()))
			{
				if (GM->ObtenerNivelActual() >= 3)
				{
					if (MiNave) MiNave->MostrarVictoria();
				}
				else
				{
					GM->JefeDerrotado(GetWorld());
					// No reanudar aqu�, lo har� IniciarSiguienteNivel
				}
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

void AEnemigoBase::EstablecerMesh(const TCHAR* RutaMesh)
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, RutaMesh);
	if (Mesh)
	{
		MallaEnemigo->SetStaticMesh(Mesh);
	}
}