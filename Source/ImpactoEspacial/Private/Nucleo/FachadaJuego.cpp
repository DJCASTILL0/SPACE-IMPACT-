#include "Nucleo/FachadaJuego.h"
#include "Nucleo/GameManager.h"
#include "Enemigos/GeneradorEnemigos.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UFachadaJuego::IniciarPartida(UWorld* Mundo)
{
    if (!Mundo) return;

    // 1) Reinicia el "cerebro" del juego y deja el nivel 1 listo.
    UGameManager* GM = UGameManager::ObtenerInstancia(Mundo);
    if (GM)
    {
        GM->ResetearJuego();
        GM->EstablecerNivel(1);
        GM->EstablecerEnemigosEnOleada(10);
    }

    // 2) Activa el generador de enemigos del nivel.
    TArray<AActor*> Generadores;
    UGameplayStatics::GetAllActorsOfClass(Mundo, AGeneradorEnemigos::StaticClass(), Generadores);
    if (Generadores.Num() > 0)
    {
        if (AGeneradorEnemigos* Gen = Cast<AGeneradorEnemigos>(Generadores[0]))
        {
            Gen->ReanudarGeneracion();
        }
    }
}

void UFachadaJuego::TerminarPartida(UWorld* Mundo, TSubclassOf<UUserWidget> WidgetGameOver)
{
    if (!Mundo) return;

    // 1) Estado del juego.
    if (UGameManager* GM = UGameManager::ObtenerInstancia(Mundo))
    {
        GM->GameOver();
    }

    // 2) Muestra el widget de Game Over.
    if (WidgetGameOver)
    {
        if (UUserWidget* W = CreateWidget<UUserWidget>(Mundo, WidgetGameOver))
        {
            W->AddToViewport(100);
        }
    }

    // 3) Libera el cursor para poder pulsar los botones del menu.
    if (APlayerController* PC = Mundo->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UFachadaJuego::MostrarVictoria(UWorld* Mundo, TSubclassOf<UUserWidget> WidgetVictoria)
{
    if (!Mundo) return;

    if (WidgetVictoria)
    {
        if (UUserWidget* W = CreateWidget<UUserWidget>(Mundo, WidgetVictoria))
        {
            W->AddToViewport(100);
        }
    }

    if (APlayerController* PC = Mundo->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}
