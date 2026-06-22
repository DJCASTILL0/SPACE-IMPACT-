#include "Enemigos/FabricaEnemigos.h"
#include "Enemigos/EnemigoBase.h"
#include "Enemigos/EstrategiaMovimientoLineal.h"
#include "Enemigos/EstrategiaMovimientoZigZag.h"
#include "Enemigos/EstrategiaMovimientoPerseguidor.h"
#include "Enemigos/EstrategiaMovimientoDiagonal.h"
#include "Enemigos/ConstructorEnemigo.h"
#include "Enemigos/JefeBase.h"
#include "Enemigos/Dragon.h"
#include "Enemigos/PezMonstruo.h"
#include "Enemigos/Kraken.h"

AEnemigoBase* UFabricaEnemigos::GenerarEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Ubicacion)
{
    if (!Mundo) return nullptr;

    // Malla comun para los enemigos normales.
    const TCHAR* Malla = TEXT("/Game/Jugador/bbghast/Ghast.Ghast");

    // La FABRICA decide el tipo; el BUILDER lo arma paso a paso.
    switch (Tipo)
    {
    case ETipoEnemigo::Zangano:
        return FConstructorEnemigo().ConVida(100.f).ConVelocidad(400.f)
            .ConEstrategia(UEstrategiaMovimientoLineal::StaticClass())
            .ConMalla(Malla).ConEscala(0.8f).Construir(Mundo, Ubicacion);

    case ETipoEnemigo::Cazador:
        return FConstructorEnemigo().ConVida(100.f).ConVelocidad(300.f)
            .ConEstrategia(UEstrategiaMovimientoZigZag::StaticClass())
            .ConMalla(Malla).ConEscala(0.8f).Construir(Mundo, Ubicacion);

    case ETipoEnemigo::Tanque:
        return FConstructorEnemigo().ConVida(100.f).ConVelocidad(600.f)
            .ConEstrategia(UEstrategiaMovimientoLineal::StaticClass())
            .ConMalla(Malla).ConEscala(0.6f).Construir(Mundo, Ubicacion);

    case ETipoEnemigo::Interceptor:
        return FConstructorEnemigo().ConVida(100.f).ConVelocidad(500.f)
            .ConEstrategia(UEstrategiaMovimientoPerseguidor::StaticClass())
            .ConMalla(Malla).ConEscala(0.8f).Construir(Mundo, Ubicacion);

    case ETipoEnemigo::Buso:
        return FConstructorEnemigo().ConVida(100.f).ConVelocidad(400.f)
            .ConEstrategia(UEstrategiaMovimientoDiagonal::StaticClass())
            .ConMalla(Malla).ConEscala(0.8f).Construir(Mundo, Ubicacion);

    case ETipoEnemigo::TanqueNavi:
        return FConstructorEnemigo().ConVida(120.f).ConVelocidad(300.f)
            .ConEstrategia(UEstrategiaMovimientoLineal::StaticClass())
            .ConMalla(Malla).ConEscala(1.0f).Construir(Mundo, Ubicacion);
    }

    return nullptr;
}

AJefeBase* UFabricaEnemigos::GenerarJefe(UWorld* Mundo, int32 Nivel, FVector Ubicacion)
{
    if (!Mundo) return nullptr;

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AJefeBase* NuevoJefe = nullptr;

    // La fábrica decide qu� jefe concreto crear seg�n el nivel.
    switch (Nivel)
    {
    case 1:
        NuevoJefe = Mundo->SpawnActor<ADragon>(ADragon::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);
        break;
    case 2:
        NuevoJefe = Mundo->SpawnActor<APezMonstruo>(APezMonstruo::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);
        break;
    case 3:
        NuevoJefe = Mundo->SpawnActor<AKraken>(AKraken::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);
        break;
    }

    return NuevoJefe;
}