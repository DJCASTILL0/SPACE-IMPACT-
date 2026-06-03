// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PantallaGameOver.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Nucleo/GameManager.h"
#include "Kismet/GameplayStatics.h"

void UPantallaGameOver::NativeConstruct()
{
    Super::NativeConstruct();

    UGameManager* GM = UGameManager::ObtenerInstancia(GetWorld());
    if (GM && TextoPuntuacion)
    {
        TextoPuntuacion->SetText(FText::FromString(
            FString::Printf(TEXT("%d"), GM->ObtenerPuntuacion())
        ));
    }

    if (BotonMenu)
    {
        BotonMenu->OnClicked.AddDynamic(this, &UPantallaGameOver::AlPresionarMenu);
    }
}

void UPantallaGameOver::AlPresionarMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Nivel_Menu")));
}