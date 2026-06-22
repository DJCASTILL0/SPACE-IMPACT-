#include "Armas/DecoradorArma.h"
#include "Jugador/NaveJugador.h"

void UDecoradorDisparoTriple::Disparar(ANaveJugador* Nave, FVector Salida)
{
    // 1) Primero hace lo que haga el arma ENVUELTA (p. ej. la bala central).
    if (ArmaEnvuelta)
    {
        ArmaEnvuelta->Disparar(Nave, Salida);
    }

    // 2) Y ADEMAS anade lo suyo: 2 balas en diagonal (lo "extra" del decorador).
    if (Nave)
    {
        Nave->LanzarProyectil(Salida + FVector(0.f, 0.f, 30.f), FRotator(0.f, 15.f, 0.f));
        Nave->LanzarProyectil(Salida + FVector(0.f, 0.f, -30.f), FRotator(0.f, -15.f, 0.f));
    }
}
