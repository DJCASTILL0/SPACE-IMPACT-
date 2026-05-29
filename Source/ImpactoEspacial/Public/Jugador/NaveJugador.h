// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

// Declaración anticipada para el Patrón Object Pool
class AProyectilBase;
class USoundBase;
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

	// --- PATRÓN OBJECT POOL (Piscina de Objetos) ---
	UPROPERTY()
	TArray<AProyectilBase*> PiscinaProyectiles;

	int32 TamanoPiscina;

	// Función para buscar un proyectil disponible en la piscina
	AProyectilBase* ObtenerProyectilDePiscina();

protected:
	// La clase del Widget que queremos usar (la seleccionaremos en Unreal)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPantallaHUD> ClaseHUD;

	// Referencia a la pantalla ya creada en memoria
	UPROPERTY()
	class UPantallaHUD* MiHUD;

	// Puntuación
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

	// --- ENTRADAS DE MOVIMIENTO ---
	void MoverDerecha(float Valor);
	void MoverArriba(float Valor);

	// --- MECÁNICA DE DISPARO ---
	void IniciarDisparo();
	void DetenerDisparo();
	void EfectuarDisparo();
	void UsarHabilidad1();
	void UsarHabilidad2();
	void UsarHabilidad3();
public:
	// --- ESTADÍSTICAS ---
	float VidaMaxima;
	float VidaActual;
	int32 VidasTotales;
	float VelocidadMovimiento;

	// Variables lógicas de disparo
	float VelocidadDisparo;
	bool bEstaDisparando;
	FTimerHandle ManejadorTemporizadorDisparo;
	// --- SISTEMA DE POWER-UPS ---
	bool bTieneDisparoMultiple;
	FTimerHandle ManejadorTemporizadorPoder;

	void SumarPuntos(int32 Cantidad);

	// Funciones para activar y desactivar el poder (deben ser públicas para que la cápsula las llame)
	void ActivarDisparoMultiple();
	void DesactivarDisparoMultiple();
	// Getters puros
	FORCEINLINE class USphereComponent* GetComponenteColision() const { return ComponenteColision; }
	FORCEINLINE class UCameraComponent* GetCamaraLateral() const { return CamaraLateral; }
	// --- HABILIDADES ESPECIALES ---
};