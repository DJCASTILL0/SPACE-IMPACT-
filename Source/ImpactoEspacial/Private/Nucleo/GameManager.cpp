// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/GameManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Nucleo/ImpactoEspacialGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Nucleo/SpaceImpactGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enemigos/GeneradorEnemigos.h"
#include "Jugador/NaveJugador.h"
#include "Engine/Engine.h"

UGameManager* UGameManager::Instancia = nullptr;

UGameManager::UGameManager()
{
    EstadoActual = EEstadoJuego::MenuPrincipal;
    PuntuacionTotal = 0;
    OleadaActual = 0;
    EnemigosRestantesEnOleada = 0;
    bJefeDerrotado = false;
    MundoActual = nullptr;
}

UGameManager* UGameManager::ObtenerInstancia(UWorld* Mundo)
{
    if (!Instancia)
    {
        Instancia = NewObject<UGameManager>(Mundo);
        Instancia->MundoActual = Mundo;
    }
    return Instancia;
}

void UGameManager::IniciarJuego()
{
    EstadoActual = EEstadoJuego::Jugando;
    PuntuacionTotal = 0;
    OleadaActual = 0;
    bJefeDerrotado = false;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("¡JUEGO INICIADO!"));

    IniciarNuevaOleada();
}

void UGameManager::PausarJuego()
{
    if (EstadoActual == EEstadoJuego::Jugando)
    {
        EstadoActual = EEstadoJuego::Pausado;
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("JUEGO PAUSADO"));
    }
}

void UGameManager::ReanudarJuego()
{
    if (EstadoActual == EEstadoJuego::Pausado)
    {
        EstadoActual = EEstadoJuego::Jugando;
    }
}

void UGameManager::GameOver()
{
    if (EstadoActual == EEstadoJuego::GameOver) return;
    EstadoActual = EEstadoJuego::GameOver;

    // Buscar el GameMode para obtener las clases de Widgets
    AImpactoEspacialGameModeBase* GM = Cast<AImpactoEspacialGameModeBase>(
        UGameplayStatics::GetGameMode(MundoActual));

    if (GM && GM->ClaseGameOver && MundoActual)
    {
        UUserWidget* WidgetGO = CreateWidget<UUserWidget>(MundoActual, GM->ClaseGameOver);
        if (WidgetGO)
        {
            WidgetGO->AddToViewport(100);
            APlayerController* PC = MundoActual->GetFirstPlayerController();
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}


void UGameManager::VictoriaJuego()
{
    // Ya no se usa, la victoria se muestra desde EnemigoBase
}

void UGameManager::SumarPuntos(int32 Cantidad)
{
    PuntuacionTotal += Cantidad;
}

void UGameManager::IniciarNuevaOleada()
{
    OleadaActual++;
    EnemigosRestantesEnOleada = 5 + (OleadaActual * 3);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            FString::Printf(TEXT("¡Oleada %d! Enemigos: %d"), OleadaActual, EnemigosRestantesEnOleada));
}

void UGameManager::EnemigoEliminado()
{
    if (EnemigosRestantesEnOleada > 0)
    {
        EnemigosRestantesEnOleada--;

        if (EnemigosRestantesEnOleada <= 0)
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("¡Oleada completada!"));
        }
    }
}

void UGameManager::EstablecerEnemigosEnOleada(int32 Cantidad)
{
    EnemigosRestantesEnOleada = Cantidad;
}

void UGameManager::JefeDerrotado(UWorld* Mundo)
{
    bJefeDerrotado = true;
    IniciarSiguienteNivel();
}

void UGameManager::ResetearJuego()
{
    PuntuacionTotal = 0;
    OleadaActual = 0;
    EnemigosRestantesEnOleada = 0;
    bJefeDerrotado = false;
    EstadoActual = EEstadoJuego::MenuPrincipal;
}

void UGameManager::EstablecerNivel(int32 NuevoNivel)
{
    OleadaActual = NuevoNivel;
    bJefeDerrotado = false;
    EnemigosRestantesEnOleada = 10;
    EstadoActual = EEstadoJuego::Jugando;
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
            FString::Printf(TEXT("NIVEL %d"), OleadaActual));
}

void UGameManager::IniciarSiguienteNivel()
{
    OleadaActual++;
    bJefeDerrotado = false;
    EnemigosRestantesEnOleada = 10;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
            FString::Printf(TEXT("NIVEL %d"), OleadaActual));

    // Reanudar generador
    TArray<AActor*> Generadores;
    UGameplayStatics::GetAllActorsOfClass(MundoActual, AGeneradorEnemigos::StaticClass(), Generadores);
    if (Generadores.Num() > 0)
    {
        AGeneradorEnemigos* Gen = Cast<AGeneradorEnemigos>(Generadores[0]);
        if (Gen) Gen->ReanudarGeneracion();
    }
}
void UGameManager::CargarNivel(FName NombreNivel)
{
    if (!MundoActual) return;

    USpaceImpactGameInstance* GI = Cast<USpaceImpactGameInstance>(MundoActual->GetGameInstance());
    if (GI)
    {
        GI->GuardarPuntuacion(PuntuacionTotal, OleadaActual);
    }

    UGameplayStatics::OpenLevel(MundoActual, NombreNivel);
}