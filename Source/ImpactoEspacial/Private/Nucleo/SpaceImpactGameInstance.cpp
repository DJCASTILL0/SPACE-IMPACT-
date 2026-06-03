// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/SpaceImpactGameInstance.h"

void USpaceImpactGameInstance::GuardarPuntuacion(int32 Puntos, int32 Nivel)
{
    PuntuacionFinal = Puntos;
    NivelAlcanzado = Nivel;
}

void USpaceImpactGameInstance::ResetearDatos()
{
    PuntuacionFinal = 0;
    NivelAlcanzado = 0;
}