#include "Enemigos/FabricaEnemigos.h"
#include "Enemigos/EnemigoBase.h"
#include "Enemigos/EstrategiaMovimientoLineal.h"
#include "Enemigos/EstrategiaMovimientoZigZag.h"
#include "Enemigos/EstrategiaMovimientoPerseguidor.h"
#include "Enemigos/EstrategiaMovimientoDiagonal.h"
#include "Enemigos/JefeBase.h"
#include "Enemigos/Dragon.h"
#include "Enemigos/PezMonstruo.h"
#include "Enemigos/Kraken.h"

AEnemigoBase* UFabricaEnemigos::GenerarEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Ubicacion)
{
    if (!Mundo) return nullptr;

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemigoBase* NuevoEnemigo = Mundo->SpawnActor<AEnemigoBase>(AEnemigoBase::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);

    if (NuevoEnemigo)
    {
        UEstrategiaMovimientoLineal* Lineal = NewObject<UEstrategiaMovimientoLineal>(NuevoEnemigo);
        UEstrategiaMovimientoZigZag* ZigZag = NewObject<UEstrategiaMovimientoZigZag>(NuevoEnemigo);
        UEstrategiaMovimientoPerseguidor* Perseguidor = NewObject<UEstrategiaMovimientoPerseguidor>(NuevoEnemigo);

        switch (Tipo)
        {
        case ETipoEnemigo::Zangano:
            NuevoEnemigo->Vida = 100.f;
            NuevoEnemigo->Velocidad = 400.f;
            NuevoEnemigo->EstablecerEstrategia(Lineal);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(0.8f));
            break;

        case ETipoEnemigo::Cazador:
            NuevoEnemigo->Vida = 100.f;
            NuevoEnemigo->Velocidad = 300.f;
            NuevoEnemigo->EstablecerEstrategia(ZigZag);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(0.8f));
            break;

        case ETipoEnemigo::Tanque:
            NuevoEnemigo->Vida = 100.f;
            NuevoEnemigo->Velocidad = 600.f;
            NuevoEnemigo->EstablecerEstrategia(Lineal);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(0.6f));
            break;

        case ETipoEnemigo::Interceptor:
            NuevoEnemigo->Vida = 100.f;
            NuevoEnemigo->Velocidad = 500.f;
            NuevoEnemigo->EstablecerEstrategia(Perseguidor);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(0.8f));
            break;

        case ETipoEnemigo::Buso:
        {
            UEstrategiaMovimientoDiagonal* Diagonal = NewObject<UEstrategiaMovimientoDiagonal>(NuevoEnemigo);
            NuevoEnemigo->Vida = 100.f;
            NuevoEnemigo->Velocidad = 400.f;
            NuevoEnemigo->EstablecerEstrategia(Diagonal);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(0.8f));
        }
        break;

        case ETipoEnemigo::TanqueNavi:
        {
            UEstrategiaMovimientoLineal* LinealTanque = NewObject<UEstrategiaMovimientoLineal>(NuevoEnemigo);
            NuevoEnemigo->Vida = 120.f;
            NuevoEnemigo->Velocidad = 300.f;
            NuevoEnemigo->EstablecerEstrategia(LinealTanque);
            NuevoEnemigo->EstablecerMesh(TEXT("/Game/Jugador/bbghast/Ghast.Ghast"));
            NuevoEnemigo->SetActorScale3D(FVector(1.0f));
        }
        break;
        }
    }
    return NuevoEnemigo;
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