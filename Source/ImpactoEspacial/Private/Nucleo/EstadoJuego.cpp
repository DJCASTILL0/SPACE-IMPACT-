#include "Nucleo/EstadoJuego.h"
#include "Engine/Engine.h"

// Pequena ayuda para mostrar un mensaje en pantalla al entrar a cada estado.
static void MensajeEstado(const FString& Texto, FColor Color)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, Color, Texto);
    }
}

// Cada estado define QUE pasa al entrar en el (comportamiento propio del State).
void UEstadoMenu::Entrar(UGameManager* GM)
{
    MensajeEstado(TEXT("Estado: MENU PRINCIPAL"), FColor::White);
}

void UEstadoJugando::Entrar(UGameManager* GM)
{
    MensajeEstado(TEXT("Estado: JUGANDO"), FColor::Green);
}

// STATE: solo el estado JUGANDO responde a "pausar" (pasa a PAUSADO).
void UEstadoJugando::Pausar(UGameManager* GM)
{
    if (GM) GM->CambiarEstado(UEstadoPausado::StaticClass());
}

void UEstadoPausado::Entrar(UGameManager* GM)
{
    MensajeEstado(TEXT("Estado: PAUSADO"), FColor::Yellow);
}

// STATE: solo el estado PAUSADO responde a "reanudar" (vuelve a JUGANDO).
void UEstadoPausado::Reanudar(UGameManager* GM)
{
    if (GM) GM->CambiarEstado(UEstadoJugando::StaticClass());
}

void UEstadoGameOver::Entrar(UGameManager* GM)
{
    MensajeEstado(TEXT("Estado: GAME OVER"), FColor::Red);
}

void UEstadoVictoria::Entrar(UGameManager* GM)
{
    MensajeEstado(TEXT("Estado: VICTORIA"), FColor::Cyan);
}
