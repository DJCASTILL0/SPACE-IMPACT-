#include "Armas/DecoradorArma.h"
#include "Jugador/NaveJugador.h"
#include "TimerManager.h"

void UDecoradorDisparoTriple::Disparar(ANaveJugador* Nave, FVector Salida)
{
    if (!Nave) return;

    // 1) Bala CENTRAL inmediata (la dispara el arma envuelta).
    if (ArmaEnvuelta)
    {
        ArmaEnvuelta->Disparar(Nave, Salida);
    }

    // 2) Las otras 2 balas (arriba y abajo) con un pequeno RETRASO. Eso da el
    //    efecto de multidisparo escalonado (como el original del proyecto).
    //    Usamos un puntero DEBIL a la nave: si la nave se destruye antes de que
    //    salte el temporizador, simplemente no hacemos nada (evita crashes).
    TWeakObjectPtr<ANaveJugador> NaveDebil(Nave);

    FTimerHandle TimerArriba;
    Nave->GetWorldTimerManager().SetTimer(TimerArriba, [NaveDebil, Salida]()
    {
        if (NaveDebil.IsValid())
            NaveDebil->LanzarProyectil(Salida + FVector(0.f, 0.f, 30.f), FRotator(0.f, 15.f, 0.f));
    }, 0.05f, false);

    FTimerHandle TimerAbajo;
    Nave->GetWorldTimerManager().SetTimer(TimerAbajo, [NaveDebil, Salida]()
    {
        if (NaveDebil.IsValid())
            NaveDebil->LanzarProyectil(Salida + FVector(0.f, 0.f, -30.f), FRotator(0.f, -15.f, 0.f));
    }, 0.1f, false);
}
