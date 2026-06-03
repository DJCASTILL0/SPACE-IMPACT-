// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PantallaVictoria.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Nucleo/GameManager.h"
#include "Kismet/GameplayStatics.h"

void UPantallaVictoria::NativeConstruct()
{
    Super::NativeConstruct();

    UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
    if (GM && TextoPuntuacion)
    {
        TextoPuntuacion->SetText(FText::FromString(
            FString::Printf(TEXT("¡VICTORIA!\nPuntuación: %d"), GM->ObtenerPuntuacion())
        ));
    }

    if (BotonMenu)
    {
        BotonMenu->OnClicked.AddDynamic(this, &UPantallaVictoria::AlPresionarMenu);
    }
}

void UPantallaVictoria::AlPresionarMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Nivel_Menu")));
}