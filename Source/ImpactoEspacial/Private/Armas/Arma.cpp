#include "Armas/Arma.h"
#include "Jugador/NaveJugador.h"

void UArmaBasica::Disparar(ANaveJugador* Nave, FVector Salida)
{
    // Una bala recta usando la piscina de la nave (Object Pool).
    if (Nave)
    {
        Nave->LanzarProyectil(Salida, FRotator::ZeroRotator);
    }
}
