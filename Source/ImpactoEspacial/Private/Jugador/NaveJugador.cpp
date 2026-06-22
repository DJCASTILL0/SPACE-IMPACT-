// Copyright Epic Games, Inc. All Rights Reserved.

#include "Jugador/NaveJugador.h"
#include "Nucleo/GameManager.h"
#include "Nucleo/FachadaJuego.h"
#include "Armas/Arma.h"
#include "Armas/DecoradorArma.h"
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

	// FACADE: una sola llamada inicia la partida
	// (reinicia el GameManager, pone nivel 1 y arranca el generador de enemigos).
	UFachadaJuego::IniciarPartida(GetWorld());

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

	// DECORATOR: arma inicial = basica (1 bala). Los power-ups la envolveran.
	ArmaActual = NewObject<UArmaBasica>(this);
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

void ANaveJugador::LanzarProyectil(FVector Ubicacion, FRotator Rotacion)
{
	// OBJECT POOL: saca una bala dormida de la piscina y la enciende.
	AProyectilBase* Bala = ObtenerProyectilDePiscina();
	if (Bala)
	{
		Bala->ActivarProyectil(Ubicacion, Rotacion);
	}
}

void ANaveJugador::EfectuarDisparo()
{
	const FVector UbicacionSalida = GetActorLocation() + FVector(80.0f, 0.0f, 0.0f);

	// DECORATOR: el arma actual decide COMO dispara. Si es la basica, 1 bala;
	// si esta envuelta por un decorador (power-up), anade su efecto encima.
	if (ArmaActual)
	{
		ArmaActual->Disparar(this, UbicacionSalida);
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

			// FACADE: una sola llamada hace GameOver + mostrar widget + liberar cursor.
			UFachadaJuego::TerminarPartida(GetWorld(), ClaseGameOver);

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
	// DECORATOR: ENVOLVEMOS el arma actual con un decorador de disparo triple.
	// (Se podrian apilar varios power-ups envolviendo uno sobre otro.)
	UDecoradorDisparoTriple* Triple = NewObject<UDecoradorDisparoTriple>(this);
	Triple->Envolver(ArmaActual);
	ArmaActual = Triple;

	bTieneDisparoMultiple = true;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("�DISPARO MULTIPLE ACTIVADO!"));

	GetWorldTimerManager().SetTimer(ManejadorTemporizadorPoder, this, &ANaveJugador::DesactivarDisparoMultiple, 8.0f, false);
}

void ANaveJugador::DesactivarDisparoMultiple()
{
	// DECORATOR: quitamos una capa, volviendo al arma que estaba envuelta.
	if (UDecoradorArma* Deco = Cast<UDecoradorArma>(ArmaActual))
	{
		ArmaActual = Deco->ArmaEnvuelta;
	}

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
	// FACADE: muestra la pantalla de victoria y libera el cursor en una llamada.
	UFachadaJuego::MostrarVictoria(GetWorld(), ClaseVictoria);
}

