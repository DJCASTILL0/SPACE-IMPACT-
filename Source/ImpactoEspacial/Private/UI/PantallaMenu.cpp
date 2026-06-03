// PantallaMenu.cpp
#include "UI/PantallaMenu.h"
#include "Components/Button.h"
#include "Nucleo/GameManager.h"
#include "Enemigos/GeneradorEnemigos.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPantallaMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (BotonJugar)
        BotonJugar->OnClicked.AddDynamic(this, &UPantallaMenu::AlPresionarJugar);

    if (BotonSalir)
        BotonSalir->OnClicked.AddDynamic(this, &UPantallaMenu::AlPresionarSalir);
}

void UPantallaMenu::AlPresionarJugar()
{
    // Cargar el nivel del juego
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Minimal_Default")));
}

void UPantallaMenu::AlPresionarSalir()
{
    APlayerController* PC = GetOwningPlayer();
    UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}