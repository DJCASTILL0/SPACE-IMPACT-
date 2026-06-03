// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImpactoEspacialGameModeBase.generated.h"

UCLASS()
class IMPACTOESPACIAL_API AImpactoEspacialGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AImpactoEspacialGameModeBase();
    // Clase del menú (se asigna en BP_ModoJuego)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseMenu;
public:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseGameOver;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> ClaseVictoria;
protected:
    virtual void BeginPlay() override;

    
};