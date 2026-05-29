// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/ImpactoEspacialGameModeBase.h"
#include "Jugador/NaveJugador.h" // <--- Importamos nuestra nueva nave

AImpactoEspacialGameModeBase::AImpactoEspacialGameModeBase()
{
	// Le decimos al motor que use nuestra clase C++ pura como la nave por defecto
	DefaultPawnClass = ANaveJugador::StaticClass();
}