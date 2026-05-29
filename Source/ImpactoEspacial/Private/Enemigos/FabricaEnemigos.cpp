// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemigos/FabricaEnemigos.h"
#include "Enemigos/EstrategiaMovimientoLineal.h"
#include "Enemigos/EstrategiaMovimientoZigZag.h"
#include "Enemigos/EstrategiaMovimientoDiagonal.h"
#include "Enemigos/EstrategiaMovimientoPerseguidor.h" // <--- Agregamos la nueva

AEnemigoBase* UFabricaEnemigos::GenerarEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Ubicacion)
{
	if (!Mundo) return nullptr;

	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEnemigoBase* NuevoEnemigo = Mundo->SpawnActor<AEnemigoBase>(AEnemigoBase::StaticClass(), Ubicacion, FRotator::ZeroRotator, Parametros);

	if (NuevoEnemigo)
	{
		// Creamos las estrategias
		UEstrategiaMovimientoLineal* Lineal = NewObject<UEstrategiaMovimientoLineal>(NuevoEnemigo);
		UEstrategiaMovimientoZigZag* ZigZag = NewObject<UEstrategiaMovimientoZigZag>(NuevoEnemigo);
		UEstrategiaMovimientoPerseguidor* Perseguidor = NewObject<UEstrategiaMovimientoPerseguidor>(NuevoEnemigo);

		switch (Tipo)
		{
		case ETipoEnemigo::Zangano: // Dorito
			NuevoEnemigo->Vida = 100.f;
			NuevoEnemigo->Velocidad = 400.f; // 1x
			NuevoEnemigo->EstablecerEstrategia(Lineal);
			NuevoEnemigo->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			break;

		case ETipoEnemigo::Cazador: // Pétalo
			NuevoEnemigo->Vida = 100.f;
			NuevoEnemigo->Velocidad = 300.f; // 0.75x
			NuevoEnemigo->EstablecerEstrategia(ZigZag);
			NuevoEnemigo->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			break;

		case ETipoEnemigo::Tanque: // Misil (rápido, recto)
			NuevoEnemigo->Vida = 100.f;
			NuevoEnemigo->Velocidad = 600.f; // 1.5x
			NuevoEnemigo->EstablecerEstrategia(Lineal);
			NuevoEnemigo->SetActorScale3D(FVector(0.8f, 0.8f, 0.8f));
			break;

		case ETipoEnemigo::Interceptor: // Nave
			NuevoEnemigo->Vida = 100.f;
			NuevoEnemigo->Velocidad = 500.f; // 1.25x
			NuevoEnemigo->EstablecerEstrategia(Perseguidor);
			NuevoEnemigo->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			break;
		case ETipoEnemigo::Buso:
		{
			UEstrategiaMovimientoDiagonal* Diagonal = NewObject<UEstrategiaMovimientoDiagonal>(NuevoEnemigo);
			NuevoEnemigo->Vida = 100.f;
			NuevoEnemigo->Velocidad = 400.f;
			NuevoEnemigo->EstablecerEstrategia(Diagonal);
			NuevoEnemigo->SetActorScale3D(FVector(1.0f, 0.5f, 1.0f));
		}
		break;

		case ETipoEnemigo::TanqueNavi:
		{
			UEstrategiaMovimientoLineal* LinealTanque = NewObject<UEstrategiaMovimientoLineal>(NuevoEnemigo);
			NuevoEnemigo->Vida = 120.f;
			NuevoEnemigo->Velocidad = 300.f; // 0.75x
			NuevoEnemigo->EstablecerEstrategia(LinealTanque);
			NuevoEnemigo->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f)); // Mediano
		}
		break;
		}
	}
	return NuevoEnemigo;
}