// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/ImpactoEspacialGameModeBase.h"
#include "UI/PantallaMenu.h"
#include "Nucleo/MapaEstelar.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Jugador/NaveJugador.h"
#include "Kismet/GameplayStatics.h"

AImpactoEspacialGameModeBase::AImpactoEspacialGameModeBase()
{
    // Usar nuestra nave C++ como jugador por defecto
    DefaultPawnClass = ANaveJugador::StaticClass();
}

void AImpactoEspacialGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (ClaseMenu)
    {
        // Si hay menú, mostrarlo
        UUserWidget* Menu = CreateWidget<UUserWidget>(GetWorld(), ClaseMenu);
        if (Menu)
        {
            Menu->AddToViewport();

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
    else
    {
        // Si NO hay menú, activar controles del juego
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = false;
            PC->SetInputMode(FInputModeGameOnly());
        }
    }

    // Spawnear el mapa estelar
    GetWorld()->SpawnActor<AMapaEstelar>(AMapaEstelar::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}