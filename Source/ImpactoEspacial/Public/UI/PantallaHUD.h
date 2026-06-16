// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaHUD.generated.h"

// Declaraciones para evitar incluir archivos pesados
class UProgressBar;
class UTextBlock;

UCLASS()
class IMPACTOESPACIAL_API UPantallaHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	// La magia del BindWidget: Si creas una barra en el editor que se llame EXACTAMENTE "BarraSalud", C++ la controlar�.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BarraSalud;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextoVidas;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextoPuntos;

public:
	// El HUD (OBSERVADOR) se suscribe a los eventos de la nave (SUJETO).
	void Observar(class ANaveJugador* Nave);

	// Reaccionan a los eventos PUBLICADOS por la nave.
	// Deben ser UFUNCTION() para poder enlazarse con AddDynamic (patr�n Observer).
	UFUNCTION()
	void ActualizarSalud(float SaludActual, float SaludMaxima);

	UFUNCTION()
	void ActualizarVidas(int32 Vidas);

	UFUNCTION()
	void ActualizarPuntos(int32 Puntos);
};