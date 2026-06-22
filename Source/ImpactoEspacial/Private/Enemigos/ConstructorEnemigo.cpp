#include "Enemigos/ConstructorEnemigo.h"
#include "Enemigos/EnemigoBase.h"
#include "Engine/World.h"

FConstructorEnemigo& FConstructorEnemigo::ConVida(float NuevaVida)
{
    Vida = NuevaVida;
    return *this;
}

FConstructorEnemigo& FConstructorEnemigo::ConVelocidad(float NuevaVelocidad)
{
    Velocidad = NuevaVelocidad;
    return *this;
}

FConstructorEnemigo& FConstructorEnemigo::ConEscala(float NuevaEscala)
{
    Escala = NuevaEscala;
    return *this;
}

FConstructorEnemigo& FConstructorEnemigo::ConMalla(const FString& Ruta)
{
    RutaMalla = Ruta;
    return *this;
}

FConstructorEnemigo& FConstructorEnemigo::ConEstrategia(TSubclassOf<UEstrategiaMovimiento> Clase)
{
    ClaseEstrategia = Clase;
    return *this;
}

AEnemigoBase* FConstructorEnemigo::Construir(UWorld* Mundo, FVector Ubicacion)
{
    if (!Mundo) return nullptr;

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemigoBase* Enemigo = Mundo->SpawnActor<AEnemigoBase>(
        AEnemigoBase::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);
    if (!Enemigo) return nullptr;

    // Aplicamos paso a paso lo que se fue configurando.
    Enemigo->Vida = Vida;
    Enemigo->Velocidad = Velocidad;
    Enemigo->SetActorScale3D(FVector(Escala));

    if (!RutaMalla.IsEmpty())
    {
        Enemigo->EstablecerMesh(*RutaMalla);
    }

    if (ClaseEstrategia)
    {
        // Crea la estrategia concreta a partir de su clase (Strategy + Builder).
        UEstrategiaMovimiento* Estrategia = NewObject<UEstrategiaMovimiento>(Enemigo, ClaseEstrategia);
        Enemigo->EstablecerEstrategia(Estrategia);
    }

    return Enemigo;
}
