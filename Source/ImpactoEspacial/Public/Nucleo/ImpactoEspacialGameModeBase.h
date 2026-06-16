// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImpactoEspacialGameModeBase.generated.h"

// ============================================================================
//  GameMode
//  Define las "reglas" del nivel: qu� pawn controla el jugador (la nave) y qu�
//  widgets de UI usar. En BeginPlay muestra el men� (si hay) y crea el mapa.
//  Las clases de widget se asignan desde el editor en BP_ModoJuego.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API AImpactoEspacialGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AImpactoEspacialGameModeBase();
    // Clase del men� (se asigna en BP_ModoJuego)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseMenu;
public:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseGameOver; // Widget de pantalla Game Over

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseVictoria; // Widget de pantalla Victoria
protected:
    virtual void BeginPlay() override; // Se ejecuta al cargar el nivel


};