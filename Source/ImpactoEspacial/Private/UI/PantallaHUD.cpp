// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PantallaHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPantallaHUD::ActualizarSalud(float SaludActual, float SaludMaxima)
{
	if (BarraSalud)
	{
		// La barra de progreso funciona de 0.0 a 1.0 (Porcentajes)
		BarraSalud->SetPercent(SaludActual / SaludMaxima);
	}
}

void UPantallaHUD::ActualizarVidas(int32 Vidas)
{
	if (TextoVidas)
	{
		// Convertimos el número entero a un Texto que Unreal pueda dibujar
		TextoVidas->SetText(FText::FromString(FString::Printf(TEXT("Vidas : %d"), Vidas)));
	}
}

void UPantallaHUD::ActualizarPuntos(int32 Puntos)
{
	if (TextoPuntos)
	{
		TextoPuntos->SetText(FText::FromString(FString::Printf(TEXT("Puntaje: %d"), Puntos)));
	}
}