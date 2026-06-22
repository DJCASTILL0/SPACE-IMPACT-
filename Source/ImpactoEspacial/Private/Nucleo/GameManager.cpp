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
#include "Nucleo/EstadoJuego.h"
#include "Engine/Engine.h"

UGameManager::UGameManager()
{
    EstadoObjeto = nullptr;         // STATE: se asigna al crear la instancia
    PuntuacionTotal = 0;
    OleadaActual = 0;
    EnemigosRestantesEnOleada = 0;
    bJefeDerrotado = false;
    MundoActual = nullptr;
}

// SINGLETON: definicion de la unica instancia (variable estatica de la clase).
UGameManager* UGameManager::Instancia = nullptr;

UGameManager* UGameManager::ObtenerInstancia(UWorld* Mundo)
{
    // SINGLETON: si aun no existe, la creamos UNA sola vez.
    // La creamos en el "paquete transitorio" (no atada a ningun nivel) y la
    // protegemos del recolector de basura con AddToRoot(), para que viva toda la
    // partida y NUNCA quede como puntero colgado al cambiar de nivel.
    if (!Instancia)
    {
        Instancia = NewObject<UGameManager>(GetTransientPackage());
        Instancia->AddToRoot();
        Instancia->CambiarEstado(UEstadoMenu::StaticClass()); // estado inicial
    }

    // El mundo actual cambia en cada nivel: lo actualizamos en cada acceso.
    if (Mundo)
    {
        Instancia->MundoActual = Mundo;
    }
    return Instancia;
}

// STATE: devuelve el tipo del estado actual (delegando en el objeto de estado).
EEstadoJuego UGameManager::ObtenerEstado() const
{
    return EstadoObjeto ? EstadoObjeto->Tipo() : EEstadoJuego::MenuPrincipal;
}

// STATE: intercambia el estado: crea el objeto del nuevo estado y entra en el.
void UGameManager::CambiarEstado(TSubclassOf<UEstadoJuego> NuevoEstado)
{
    if (!NuevoEstado) return;
    EstadoObjeto = NewObject<UEstadoJuego>(this, NuevoEstado);
    if (EstadoObjeto)
    {
        EstadoObjeto->Entrar(this);
    }
}

// Arranca una partida: deja todo a cero y lanza la primera oleada.
void UGameManager::IniciarJuego()
{
    CambiarEstado(UEstadoJugando::StaticClass());   // STATE
    PuntuacionTotal = 0;
    OleadaActual = 0;
    bJefeDerrotado = false;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("�JUEGO INICIADO!"));

    IniciarNuevaOleada();
}

void UGameManager::PausarJuego()
{
    // STATE: NO decidimos aqui con un if; delegamos en el estado actual.
    // Solo EstadoJugando hara algo; el resto ignora la accion.
    if (EstadoObjeto) EstadoObjeto->Pausar(this);
}

void UGameManager::ReanudarJuego()
{
    // STATE: el estado actual decide si reanudar (solo lo hace EstadoPausado).
    if (EstadoObjeto) EstadoObjeto->Reanudar(this);
}

// Pasa a estado GameOver y muestra el widget de Game Over (una sola vez).
void UGameManager::GameOver()
{
    if (ObtenerEstado() == EEstadoJuego::GameOver) return;
    CambiarEstado(UEstadoGameOver::StaticClass());   // STATE

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
    // STATE: pasa al estado de victoria.
    CambiarEstado(UEstadoVictoria::StaticClass());
}

// Suma puntos al marcador global.
void UGameManager::SumarPuntos(int32 Cantidad)
{
    PuntuacionTotal += Cantidad;
}

// Sube de oleada y calcula cuántos enemigos tendrá (crece con la oleada).
void UGameManager::IniciarNuevaOleada()
{
    OleadaActual++;
    EnemigosRestantesEnOleada = 5 + (OleadaActual * 3);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            FString::Printf(TEXT("�Oleada %d! Enemigos: %d"), OleadaActual, EnemigosRestantesEnOleada));
}

// Resta 1 a los enemigos que faltan; al llegar a 0 toca el jefe.
void UGameManager::EnemigoEliminado()
{
    if (EnemigosRestantesEnOleada > 0)
    {
        EnemigosRestantesEnOleada--;

        if (EnemigosRestantesEnOleada <= 0)
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("�Oleada completada!"));
        }
    }
}

void UGameManager::EstablecerEnemigosEnOleada(int32 Cantidad)
{
    EnemigosRestantesEnOleada = Cantidad;
}

// Marca el jefe como vencido y avanza al siguiente nivel.
void UGameManager::JefeDerrotado(UWorld* Mundo)
{
    bJefeDerrotado = true;
    IniciarSiguienteNivel();
}

// Deja todos los contadores a cero (al empezar una partida nueva).
void UGameManager::ResetearJuego()
{
    PuntuacionTotal = 0;
    OleadaActual = 0;
    EnemigosRestantesEnOleada = 0;
    bJefeDerrotado = false;
    CambiarEstado(UEstadoMenu::StaticClass());   // STATE
}

// Fija directamente el nivel (lo usa la nave al empezar en el nivel 1).
void UGameManager::EstablecerNivel(int32 NuevoNivel)
{
    OleadaActual = NuevoNivel;
    bJefeDerrotado = false;
    EnemigosRestantesEnOleada = 10;
    CambiarEstado(UEstadoJugando::StaticClass());   // STATE
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
            FString::Printf(TEXT("NIVEL %d"), OleadaActual));
}

// Sube un nivel, reinicia el conteo y reactiva el generador de enemigos.
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
// Guarda la puntuación en el GameInstance y abre otro mapa.
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