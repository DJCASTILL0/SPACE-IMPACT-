#include "Enemigos/GeneradorEnemigos.h"
#include "Enemigos/FabricaEnemigos.h"
#include "Enemigos/Dragon.h"
#include "Enemigos/PezMonstruo.h"
#include "Enemigos/Kraken.h"
#include "Engine/Engine.h"
#include "Nucleo/GameManager.h"
#include "Components/BillboardComponent.h"
#include "TimerManager.h"

AGeneradorEnemigos::AGeneradorEnemigos()
{
	PrimaryActorTick.bCanEverTick = false;
	TiempoEntreEnemigos = 0.5f; // Aparece un enemigo cada 2 segundos
	// Esto crea un icono de "Sprite" que ver�s en el editor para poder pincharlo y moverlo
	UBillboardComponent* IconoEditor = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconoEditor"));
	RootComponent = IconoEditor;
}

void AGeneradorEnemigos::BeginPlay()
{
    Super::BeginPlay();

    // COMENTAR ESTO para que no empiece solo:
    // GetWorldTimerManager().SetTimer(TemporizadorSpawn, this, &AGeneradorEnemigos::AparecerEnemigo, TiempoEntreEnemigos, true);
}

void AGeneradorEnemigos::AparecerEnemigo()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    UGameManager* GM = UGameManager::ObtenerInstancia(Mundo);
    if (!GM) return;
    // DEBUG
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
            FString::Printf(TEXT("Enemigos restantes: %d, JefeDerrotado: %s"),
                GM->ObtenerEnemigosRestantes(),
                GM->ObtenerJefeDerrotado() ? TEXT("SI") : TEXT("NO")));
    // Si ya no quedan enemigos, spawnear jefe seg�n nivel
    if (GM->EsMomentoDelJefe())
    {
        FVector UbicacionJefe = GetActorLocation() + FVector(500.f, 0.f, 0.f);

        // El generador NO conoce las clases concretas de jefe: se lo pide a la fábrica.
        UFabricaEnemigos::GenerarJefe(Mundo, GM->ObtenerNivelActual(), UbicacionJefe);

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("�JEFE APARECIDO!"));

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

void AGeneradorEnemigos::ReanudarGeneracion()
{
    // Reiniciar el temporizador de spawn
    GetWorldTimerManager().SetTimer(TemporizadorSpawn, this,
        &AGeneradorEnemigos::AparecerEnemigo, TiempoEntreEnemigos, true);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Generador reanudado"));
}