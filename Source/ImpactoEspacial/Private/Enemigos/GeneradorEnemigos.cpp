#include "Enemigos/GeneradorEnemigos.h"
#include "Enemigos/FabricaEnemigos.h"
#include "Enemigos/Dragon.h"
#include "Enemigos/PezMonstruo.h"
#include "Enemigos/Kraken.h"
#include "Nucleo/GameManager.h"
#include "Components/BillboardComponent.h"
#include "TimerManager.h"

AGeneradorEnemigos::AGeneradorEnemigos()
{
	PrimaryActorTick.bCanEverTick = false;
	TiempoEntreEnemigos = 2.0f; // Aparece un enemigo cada 2 segundos
	// Esto crea un icono de "Sprite" que verás en el editor para poder pincharlo y moverlo
	UBillboardComponent* IconoEditor = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconoEditor"));
	RootComponent = IconoEditor;
}

void AGeneradorEnemigos::BeginPlay()
{
	Super::BeginPlay();

	// Iniciamos el ciclo de aparición
	GetWorldTimerManager().SetTimer(TemporizadorSpawn, this, &AGeneradorEnemigos::AparecerEnemigo, TiempoEntreEnemigos, true);
}

void AGeneradorEnemigos::AparecerEnemigo()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    UGameManager* GM = UGameManager::ObtenerInstancia(Mundo);
    if (!GM) return;

    // Si ya no quedan enemigos, spawnear jefe según nivel
    if (GM->EsMomentoDelJefe())
    {
        FVector UbicacionJefe = GetActorLocation() + FVector(500.f, 0.f, 0.f);

        switch (GM->ObtenerNivelActual())
        {
        case 1:
            Mundo->SpawnActor<ADragon>(ADragon::StaticClass(), UbicacionJefe, FRotator::ZeroRotator);
            break;
        case 2:
            Mundo->SpawnActor<APezMonstruo>(APezMonstruo::StaticClass(), UbicacionJefe, FRotator::ZeroRotator);
            break;
        case 3:
            Mundo->SpawnActor<AKraken>(AKraken::StaticClass(), UbicacionJefe, FRotator::ZeroRotator);
            break;
        }

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡JEFE APARECIDO!"));

        // Detener el generador temporalmente
        GetWorldTimerManager().ClearTimer(TemporizadorSpawn);
        return;
    }

    // Spawnear enemigo normal
    float AlturaAleatoria = FMath::RandRange(-600.f, 600.f);
    FVector UbicacionSpawn = GetActorLocation() + FVector(0.f, 0.f, AlturaAleatoria);
    ETipoEnemigo TipoAleatorio = static_cast<ETipoEnemigo>(FMath::RandRange(0, 5));
    UFabricaEnemigos::GenerarEnemigo(Mundo, TipoAleatorio, UbicacionSpawn);
}

