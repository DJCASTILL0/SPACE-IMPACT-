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
	// La magia del BindWidget: Si creas una barra en el editor que se llame EXACTAMENTE "BarraSalud", C++ la controlará.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BarraSalud;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextoVidas;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextoPuntos;

public:
	// Funciones que llamaremos desde la nave para actualizar la pantalla
	void ActualizarSalud(float SaludActual, float SaludMaxima);
	void ActualizarVidas(int32 Vidas);
	void ActualizarPuntos(int32 Puntos);
};