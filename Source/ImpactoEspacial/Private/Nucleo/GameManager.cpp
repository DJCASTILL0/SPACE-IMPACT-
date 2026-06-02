// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/GameManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Inicializar la instancia estática a nullptr
UGameManager* UGameManager::Instancia = nullptr;

UGameManager::UGameManager()
{
    EstadoActual = EEstadoJuego::MenuPrincipal;
    PuntuacionTotal = 0;
    OleadaActual = 0;
    EnemigosRestantesEnOleada = 0;
    bJefeDerrotado = false;
}

UGameManager* UGameManager::ObtenerInstancia(UWorld* Mundo)
{
    if (!Instancia)
    {
        Instancia = NewObject<UGameManager>(Mundo);
        // NO llamar a AddToRoot()
        // Instancia->AddToRoot(); // ? QUITAR ESTO
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
    EstadoActual = EEstadoJuego::GameOver;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("¡GAME OVER!"));
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
            FString::Printf(TEXT("Puntuación final: %d"), PuntuacionTotal));
    }
}

void UGameManager::VictoriaJuego()
{
    EstadoActual = EEstadoJuego::Victoria;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("¡VICTORIA! ¡JEFE DERROTADO!"));
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
            FString::Printf(TEXT("Puntuación final: %d"), PuntuacionTotal));
    }
}

void UGameManager::SumarPuntos(int32 Cantidad)
{
    PuntuacionTotal += Cantidad;
}

void UGameManager::IniciarNuevaOleada()
{
    OleadaActual++;
    EnemigosRestantesEnOleada = 5 + (OleadaActual * 3); // Cada oleada tiene más enemigos

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

    if (OleadaActual >= 3)
    {
        VictoriaJuego();
    }
    else
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                FString::Printf(TEXT("¡Nivel %d completado!"), OleadaActual));

        // Pasar al siguiente nivel después de 3 segundos
        FTimerHandle TimerSiguienteNivel;
        Mundo->GetTimerManager().SetTimer(TimerSiguienteNivel, this,
            &UGameManager::IniciarSiguienteNivel, 3.0f, false);
    }
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
    EnemigosRestantesEnOleada = 10; // 10 enemigos por nivel

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
}