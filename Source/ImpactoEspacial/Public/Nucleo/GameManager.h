// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameManager.generated.h"

UENUM()
enum class EEstadoJuego : uint8
{
    MenuPrincipal,
    Jugando,
    Pausado,
    GameOver,
    Victoria
};

UCLASS()
class IMPACTOESPACIAL_API UGameManager : public UObject
{
    GENERATED_BODY()

private:
    static UGameManager* Instancia;

    UPROPERTY()
    UWorld* MundoActual;

    EEstadoJuego EstadoActual;
    int32 PuntuacionTotal;
    int32 OleadaActual;
    int32 EnemigosRestantesEnOleada;
    bool bJefeDerrotado;

    UGameManager();

public:
    static UGameManager* ObtenerInstancia(UWorld* Mundo);
    void CargarNivel(FName NombreNivel);
    void IniciarJuego();
    void PausarJuego();
    void ReanudarJuego();
    void GameOver();
    void VictoriaJuego();
    EEstadoJuego ObtenerEstado() const { return EstadoActual; }

    void SumarPuntos(int32 Cantidad);
    int32 ObtenerPuntuacion() const { return PuntuacionTotal; }

    void IniciarNuevaOleada();
    void EnemigoEliminado();
    void EstablecerEnemigosEnOleada(int32 Cantidad);
    int32 ObtenerOleadaActual() const { return OleadaActual; }
    int32 ObtenerEnemigosRestantes() const { return EnemigosRestantesEnOleada; }

    int32 ObtenerNivelActual() const { return OleadaActual; }
    void EstablecerNivel(int32 NuevoNivel);
    void IniciarSiguienteNivel();

    UFUNCTION()
    bool EsMomentoDelJefe() const { return EnemigosRestantesEnOleada <= 0 && !bJefeDerrotado; }
    void JefeDerrotado(UWorld* Mundo);
    bool ObtenerJefeDerrotado() const { return bJefeDerrotado; }

    void ResetearJuego();

};