// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

// Declaraci�n anticipada para el Patr�n Object Pool
class AProyectilBase;
class USoundBase;
class UArma;   // PATRON DECORATOR: arma de la nave (definida en Armas/Arma.h)

// --- PATR�N OBSERVER ---
// La nave es el SUJETO. Estos delegados son los "eventos" que la nave PUBLICA
// (Broadcast) cuando cambian sus estad�sticas. Los OBSERVADORES (el HUD) se
// suscriben con AddDynamic y reaccionan solos, sin que la nave los conozca.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaludCambiada, float, VidaActual, float, VidaMaxima);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVidasCambiadas, int32, Vidas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuntosCambiados, int32, Puntos);

// ============================================================================
//  NaveJugador
//  El Pawn que controla el jugador. Aqu� vive:
//   - El movimiento y la c�mara lateral.
//   - El disparo, que usa el PATR�N OBJECT POOL (PiscinaProyectiles) para
//     reciclar balas en vez de crear/destruir.
//   - El SUJETO del PATR�N OBSERVER: publica eventos de salud/vidas/puntos a
//     los que se suscribe el HUD.
//   - Las habilidades especiales (misil, l�ser, l�ser vertical) y los power-ups.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API ANaveJugador : public APawn
{
	GENERATED_BODY()

protected:
	// --- COMPONENTES (C++ Puro protegido en memoria) ---
	UPROPERTY()
	class USphereComponent* ComponenteColision;

	UPROPERTY()
	class UStaticMeshComponent* MallaNave;

	UPROPERTY()
	class USpringArmComponent* BrazoCamara;

	UPROPERTY()
	class UCameraComponent* CamaraLateral;

	UPROPERTY()
	class UFloatingPawnMovement* ComponenteMovimiento;

	// --- PATR�N OBJECT POOL (Piscina de Objetos) ---
	UPROPERTY()
	TArray<AProyectilBase*> PiscinaProyectiles;

	int32 TamanoPiscina;

	// Funci�n para buscar un proyectil disponible en la piscina
	AProyectilBase* ObtenerProyectilDePiscina();

	// --- PATR�N DECORATOR ---
	// Arma actual de la nave. Empieza como UArmaBasica y los power-ups la
	// ENVUELVEN con decoradores (p. ej. disparo triple).
	UPROPERTY()
	UArma* ArmaActual;

protected:
	// La clase del Widget que queremos usar (la seleccionaremos en Unreal)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPantallaHUD> ClaseHUD;

	// Referencia a la pantalla ya creada en memoria
	UPROPERTY()
	class UPantallaHUD* MiHUD;

	// Puntuaci�n
	int32 PuntosActuales;

public:
	ANaveJugador();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// --- HABILIDADES ESPECIALES (Teclas 1, 2, 3) ---
	void ActivarMisil();
	void ActivarLaserRecto();
	void ActivarLaserVertical();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// --- ENTRADAS DE MOVIMIENTO ---
	void MoverDerecha(float Valor);
	void MoverArriba(float Valor);

	// --- MEC�NICA DE DISPARO ---
	void IniciarDisparo();
	void DetenerDisparo();
	void EfectuarDisparo();
	void UsarHabilidad1();
	void UsarHabilidad2();
	void UsarHabilidad3();
public:
	// --- ESTAD�STICAS ---
	float VidaMaxima;
	float VidaActual;
	int32 VidasTotales;
	float VelocidadMovimiento;

	// Variables l�gicas de disparo
	float VelocidadDisparo;
	bool bEstaDisparando;
	FTimerHandle ManejadorTemporizadorDisparo;
	// Handles del disparo en abanico (deben ser miembros para poder cancelarlos
	// al destruirse la nave; si fueran locales con lambda, seguir�an disparando
	// sobre un 'this' ya liberado -> crash).
	FTimerHandle ManejadorAbanicoArriba;
	FTimerHandle ManejadorAbanicoAbajo;
	// --- SISTEMA DE POWER-UPS ---
	bool bTieneDisparoMultiple;
	FTimerHandle ManejadorTemporizadorPoder;

	void SumarPuntos(int32 Cantidad);

	// El arma (Decorator) usa esto para lanzar una bala reciclada de la piscina (Object Pool).
	void LanzarProyectil(FVector Ubicacion, FRotator Rotacion);

	// --- EVENTOS DEL PATR�N OBSERVER (el HUD se suscribe a estos) ---
	UPROPERTY()
	FOnSaludCambiada OnSaludCambiada;

	UPROPERTY()
	FOnVidasCambiadas OnVidasCambiadas;

	UPROPERTY()
	FOnPuntosCambiados OnPuntosCambiados;

	// Funciones para activar y desactivar el poder (deben ser p�blicas para que la c�psula las llame)
	void ActivarDisparoMultiple();
	void DesactivarDisparoMultiple();
	// Getters puros
	FORCEINLINE class USphereComponent* GetComponenteColision() const { return ComponenteColision; }
	FORCEINLINE class UCameraComponent* GetCamaraLateral() const { return CamaraLateral; }
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ClaseGameOver;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ClaseVictoria;
	// --- HABILIDADES ESPECIALES ---
public:
	void MostrarVictoria();
private:
	FVector UbicacionInicial;
};