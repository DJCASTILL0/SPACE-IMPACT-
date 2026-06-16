// Copyright Epic Games, Inc. All Rights Reserved.

#include "Jugador/NaveJugador.h"
#include "Nucleo/GameManager.h"
#include "Proyectiles/ProyectilBase.h" // Necesario para gestionar la piscina
#include "Proyectiles/ProyectilMisil.h"
#include "Proyectiles/ProyectilLaserVertical.h"
#include "Proyectiles/ProyectilLaser.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UI/PantallaHUD.h"
#include "PowerUps/CapsulaPoder.h"
#include "Enemigos/PezMonstruo.h"
#include "Enemigos/Dragon.h"
#include "Enemigos/GeneradorEnemigos.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ANaveJugador::ANaveJugador()
{
	// Permitir que el Tick se ejecute
	PrimaryActorTick.bCanEverTick = true;

	// 1. Configurar Colision Base
	ComponenteColision = CreateDefaultSubobject<USphereComponent>(TEXT("ComponenteColision"));
	ComponenteColision->InitSphereRadius(40.0f);
	ComponenteColision->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = ComponenteColision;

	// 2. Configurar Malla (Cubo temporal)
	MallaNave = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaNave"));
	MallaNave->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RecursoMalla(TEXT("/Game/Jugador/nave/Rocket.Rocket"));
	if (RecursoMalla.Succeeded()) { MallaNave->SetStaticMesh(RecursoMalla.Object); }
	MallaNave->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	// 3. Brazo de C�mara (SpringArm)
	BrazoCamara = CreateDefaultSubobject<USpringArmComponent>(TEXT("BrazoCamara"));
	BrazoCamara->SetupAttachment(RootComponent);
	BrazoCamara->TargetArmLength = 800.0f;
	BrazoCamara->SetUsingAbsoluteRotation(true);
	BrazoCamara->bDoCollisionTest = false;
	BrazoCamara->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// 4. C�mara Lateral Ortogr�fica
	CamaraLateral = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraLateral"));
	CamaraLateral->SetupAttachment(BrazoCamara, USpringArmComponent::SocketName);
	CamaraLateral->ProjectionMode = ECameraProjectionMode::Orthographic;
	CamaraLateral->OrthoWidth = 2048.0f;

	// 5. Componente de Movimiento
	ComponenteMovimiento = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("ComponenteMovimiento"));

	// 6. Asignar Valores Iniciales
	VidaMaxima = 150.0f;
	VidasTotales = 3;
	VelocidadMovimiento = 900.0f;
	ComponenteMovimiento->MaxSpeed = VelocidadMovimiento;

	VelocidadDisparo = 0.15f; // Cadencia de tiro
	bEstaDisparando = false;
	TamanoPiscina = 100; // �Aumentamos la piscina para el disparo triple!
	bTieneDisparoMultiple = false;
	
}

void ANaveJugador::BeginPlay()
{
	Super::BeginPlay();
	// Guardar ubicacion inicial
	UbicacionInicial = GetActorLocation();
	VidaActual = VidaMaxima;
	PuntosActuales = 0;

	UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
	if (GM)
	{
		GM->ResetearJuego();
		GM->EstablecerNivel(1);
		GM->EstablecerEnemigosEnOleada(10);
	}

	// --- ACTIVAR EL GENERADOR DE ENEMIGOS ---
	TArray<AActor*> Generadores;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeneradorEnemigos::StaticClass(), Generadores);
	if (Generadores.Num() > 0)
	{
		AGeneradorEnemigos* Gen = Cast<AGeneradorEnemigos>(Generadores[0]);
		if (Gen)
		{
			Gen->ReanudarGeneracion();
		}
	}

	// --- CREAR EL HUD ---
	if (ClaseHUD)
	{
		MiHUD = CreateWidget<UPantallaHUD>(GetWorld(), ClaseHUD);
		if (MiHUD)
		{
			MiHUD->AddToViewport();

			// El HUD (observador) se suscribe a los eventos de la nave (sujeto).
			MiHUD->Observar(this);

			// PUBLICAMOS los valores iniciales para que el HUD se pinte al arrancar.
			OnSaludCambiada.Broadcast(VidaActual, VidaMaxima);
			OnVidasCambiadas.Broadcast(VidasTotales);
			OnPuntosCambiados.Broadcast(PuntosActuales);
		}
	}

	// --- INICIALIZAR OBJECT POOL ---
	UWorld* Mundo = GetWorld();
	if (Mundo)
	{
		for (int32 i = 0; i < TamanoPiscina; i++)
		{
			AProyectilBase* Proyectil = Mundo->SpawnActor<AProyectilBase>(
				AProyectilBase::StaticClass(),
				FVector(-5000.f, -5000.f, -5000.f),
				FRotator::ZeroRotator
			);
			if (Proyectil)
			{
				Proyectil->SetInstigator(this);
				Proyectil->DesactivarProyectil();
				PiscinaProyectiles.Add(Proyectil);
			}
		}
	}
}

void ANaveJugador::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Cancelar TODOS los temporizadores de esta nave antes de que se libere la
	// memoria. Sin esto, los timers del disparo en abanico (lambdas que capturan
	// 'this') podr�an ejecutarse tras el Destroy() -> acceso a memoria liberada.
	if (UWorld* Mundo = GetWorld())
	{
		FTimerManager& Temporizadores = Mundo->GetTimerManager();
		// Timers atados a 'this' por nombre de funci�n (disparo, poder).
		Temporizadores.ClearAllTimersForObject(this);
		// Los timers con lambda NO se atan a 'this', hay que limpiarlos por handle.
		Temporizadores.ClearTimer(ManejadorAbanicoArriba);
		Temporizadores.ClearTimer(ManejadorAbanicoAbajo);
	}

	Super::EndPlay(EndPlayReason);
}

void ANaveJugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANaveJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enlaces de Movimiento
	PlayerInputComponent->BindAxis("MoverDerecha", this, &ANaveJugador::MoverDerecha);
	PlayerInputComponent->BindAxis("MoverArriba", this, &ANaveJugador::MoverArriba);

	// Enlaces de Disparo
	PlayerInputComponent->BindAction("Disparar", IE_Pressed, this, &ANaveJugador::IniciarDisparo);
	PlayerInputComponent->BindAction("Disparar", IE_Released, this, &ANaveJugador::DetenerDisparo);
	// Vinculamos la acci�n de Disparo Especial
	PlayerInputComponent->BindAction("Habilidad1", IE_Pressed, this, &ANaveJugador::UsarHabilidad1);
	PlayerInputComponent->BindAction("Habilidad2", IE_Pressed, this, &ANaveJugador::UsarHabilidad2);
	PlayerInputComponent->BindAction("Habilidad3", IE_Pressed, this, &ANaveJugador::UsarHabilidad3);
}

void ANaveJugador::MoverDerecha(float Valor)
{
	if (Valor != 0.0f)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Valor);
	}
}

void ANaveJugador::MoverArriba(float Valor)
{
	if (Valor != 0.0f)
	{
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Valor);
	}
}

void ANaveJugador::IniciarDisparo()
{
	bEstaDisparando = true;
	// Disparo inmediato y repetici�n por temporizador
	GetWorldTimerManager().SetTimer(ManejadorTemporizadorDisparo, this, &ANaveJugador::EfectuarDisparo, VelocidadDisparo, true, 0.0f);
}

void ANaveJugador::DetenerDisparo()
{
	bEstaDisparando = false;
	GetWorldTimerManager().ClearTimer(ManejadorTemporizadorDisparo);
}

AProyectilBase* ANaveJugador::ObtenerProyectilDePiscina()
{
	for (AProyectilBase* Proyectil : PiscinaProyectiles)
	{
		// Si el proyectil no est� en uso (est� dormido), lo devolvemos
		if (Proyectil && !Proyectil->bEstaActivo)
		{
			return Proyectil;
		}
	}
	// Si todos est�n ocupados, devolvemos nulo (o podr�amos expandir la piscina)
	return nullptr;
}

void ANaveJugador::EfectuarDisparo()
{
	const FVector UbicacionSalida = GetActorLocation() + FVector(80.0f, 0.0f, 0.0f);

	
	if (bTieneDisparoMultiple)
	{
		// Peque�o retraso entre cada bala para que no se solapen
		AProyectilBase* BalaCentro = ObtenerProyectilDePiscina();
		if (BalaCentro) BalaCentro->ActivarProyectil(UbicacionSalida, FRotator::ZeroRotator);

		// Usar Timer para disparar las otras 2 con retraso.
		// Usamos handles miembro (no locales) para poder cancelarlos en EndPlay
		// y evitar que la lambda se ejecute sobre una nave ya destruida.
		GetWorldTimerManager().SetTimer(ManejadorAbanicoArriba, [this, UbicacionSalida]()
			{
				AProyectilBase* BalaArriba = ObtenerProyectilDePiscina();
				if (BalaArriba) BalaArriba->ActivarProyectil(UbicacionSalida + FVector(0.f, 0.f, 30.f), FRotator(0.f, 15.f, 0.f));
			}, 0.05f, false);

		GetWorldTimerManager().SetTimer(ManejadorAbanicoAbajo, [this, UbicacionSalida]()
			{
				AProyectilBase* BalaAbajo = ObtenerProyectilDePiscina();
				if (BalaAbajo) BalaAbajo->ActivarProyectil(UbicacionSalida + FVector(0.f, 0.f, -30.f), FRotator(0.f, -15.f, 0.f));
			}, 0.1f, false);
	}
	else
	{
		AProyectilBase* BalaNormal = ObtenerProyectilDePiscina();
		if (BalaNormal) BalaNormal->ActivarProyectil(UbicacionSalida, FRotator::ZeroRotator);
	}
}

float ANaveJugador::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DanioRecibido = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Dentro de TakeDamage, cada vez que restes vida:
	VidaActual -= DanioRecibido;
	// PUBLICAMOS el cambio: el HUD suscrito se actualiza solo (Observer).
	OnSaludCambiada.Broadcast(VidaActual, VidaMaxima);
	OnVidasCambiadas.Broadcast(VidasTotales);

	// Verificamos si la nave explot�
	if (VidaActual <= 0.0f)
	{
		VidasTotales--; // Perdemos una vida

		if (VidasTotales > 0)
		{
			// REAPARECER (Respawn temporal)
			VidaActual = VidaMaxima;
			SetActorLocation(UbicacionInicial);

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Nave destruida. Vidas restantes: %d"), VidasTotales));
		}
		else
		{
			// GAME OVER
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GAME OVER - TE QUEDASTE SIN VIDAS"));

			// Notificar al GameManager
			UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
			if (GM)
			{
				GM->GameOver();
			}

			// Mostrar Widget Game Over
			if (ClaseGameOver)
			{
				UUserWidget* WidgetGO = CreateWidget<UUserWidget>(GetWorld(), ClaseGameOver);
				if (WidgetGO)
				{
					WidgetGO->AddToViewport(100);

					APlayerController* PC = GetWorld()->GetFirstPlayerController();
					if (PC)
					{
						PC->bShowMouseCursor = true;
						PC->SetInputMode(FInputModeUIOnly());
					}
				}
			}

			Destroy();
		}
	}
	else
	{
		// Si sobrevivimos al golpe, mostramos la vida restante
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("�Impacto! Vida actual: %f"), VidaActual));
	}

	return DanioRecibido;
}

void ANaveJugador::SumarPuntos(int32 Cantidad)
{
	PuntosActuales += Cantidad;
	// PUBLICAMOS el cambio de puntos (el HUD suscrito reacciona).
	OnPuntosCambiados.Broadcast(PuntosActuales);

	// Sincronizar con el GameManager
	UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
	if (GM)
	{
		GM->SumarPuntos(Cantidad);
	}
}

void ANaveJugador::ActivarDisparoMultiple()
{
	bTieneDisparoMultiple = true;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("�DISPARO EN ABANICO ACTIVADO!"));

	GetWorldTimerManager().SetTimer(ManejadorTemporizadorPoder, this, &ANaveJugador::DesactivarDisparoMultiple, 8.0f, false);
}

void ANaveJugador::DesactivarDisparoMultiple()
{
	bTieneDisparoMultiple = false;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Poder agotado..."));
}

void ANaveJugador::UsarHabilidad1()
{
	ActivarMisil();
}

void ANaveJugador::UsarHabilidad2()
{
	ActivarLaserRecto();
}

void ANaveJugador::UsarHabilidad3()
{
	ActivarLaserVertical();
}

void ANaveJugador::ActivarLaserRecto()
{
	const FVector UbicacionSalida = GetActorLocation() + FVector(80.0f, 0.0f, 0.0f);

	AProyectilLaser* Laser = GetWorld()->SpawnActor<AProyectilLaser>(
		AProyectilLaser::StaticClass(),
		UbicacionSalida,
		FRotator::ZeroRotator
	);

	if (Laser)
	{
		Laser->SetInstigator(this);
		Laser->ActivarProyectil(UbicacionSalida, FRotator::ZeroRotator);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("�L�SER RECTO!"));
	}
}

void ANaveJugador::ActivarMisil()
{
	const FVector UbicacionSalida = GetActorLocation() + FVector(80.0f, 0.0f, 0.0f);

	AProyectilMisil* Misil = GetWorld()->SpawnActor<AProyectilMisil>(
		AProyectilMisil::StaticClass(),
		UbicacionSalida,
		FRotator::ZeroRotator
	);

	if (Misil)
	{
		Misil->SetInstigator(this);
		Misil->ActivarProyectil(UbicacionSalida, FRotator::ZeroRotator);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("�MISIL OSCILATORIO!"));
	}
}

void ANaveJugador::ActivarLaserVertical()
{
	const FVector UbicacionSalida = GetActorLocation() + FVector(80.0f, 0.0f, 0.0f);

	AProyectilLaserVertical* LaserV = GetWorld()->SpawnActor<AProyectilLaserVertical>(
		AProyectilLaserVertical::StaticClass(),
		UbicacionSalida,
		FRotator::ZeroRotator
	);

	if (LaserV)
	{
		LaserV->SetInstigator(this);
		LaserV->ActivarProyectil(UbicacionSalida, FRotator::ZeroRotator);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("�L�SER VERTICAL!"));
	}
}

void ANaveJugador::MostrarVictoria()
{
	// Mostrar Widget Victoria
	if (ClaseVictoria)
	{
		UUserWidget* WidgetV = CreateWidget<UUserWidget>(GetWorld(), ClaseVictoria);
		if (WidgetV)
		{
			WidgetV->AddToViewport(100);

			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

