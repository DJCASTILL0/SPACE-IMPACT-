// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameManager.generated.h"

// Los posibles estados del juego (una "m�quina de estados" simple con enum).
UENUM()
enum class EEstadoJuego : uint8
{
    MenuPrincipal,
    Jugando,
    Pausado,
    GameOver,
    Victoria
};

// ============================================================================
//  GameManager  ->  PATR�N SINGLETON
//  Es el "cerebro" del juego: lleva la puntuaci�n, el nivel/oleada actual,
//  cu�ntos enemigos faltan y el estado del juego. Hay UNA sola instancia y se
//  accede a ella desde cualquier parte con ObtenerInstancia().
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API UGameManager : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    UWorld* MundoActual;            // El mundo (nivel) en el que estamos ahora

    EEstadoJuego EstadoActual;      // En qu� estado est� el juego (jugando, game over...)
    int32 PuntuacionTotal;          // Puntos acumulados
    int32 OleadaActual;             // Nivel/oleada actual (1, 2, 3...)
    int32 EnemigosRestantesEnOleada;// Cu�ntos enemigos faltan por matar
    bool bJefeDerrotado;            // �Ya se venci� al jefe de este nivel?

    UGameManager();                 // Constructor PRIVADO (rasgo del Singleton)

public:
    // Punto de acceso �nico del Singleton: devuelve siempre el mismo GameManager.
    static UGameManager* ObtenerInstancia(UWorld* Mundo);

    void CargarNivel(FName NombreNivel); // Cambia de mapa guardando antes la puntuaci�n
    void IniciarJuego();
    void PausarJuego();
    void ReanudarJuego();
    void GameOver();                // Pasa a estado GameOver y muestra su pantalla
    void VictoriaJuego();
    EEstadoJuego ObtenerEstado() const { return EstadoActual; }

    void SumarPuntos(int32 Cantidad);
    int32 ObtenerPuntuacion() const { return PuntuacionTotal; }

    void IniciarNuevaOleada();
    void EnemigoEliminado();        // Resta 1 a los enemigos que faltan
    void EstablecerEnemigosEnOleada(int32 Cantidad);
    int32 ObtenerOleadaActual() const { return OleadaActual; }
    int32 ObtenerEnemigosRestantes() const { return EnemigosRestantesEnOleada; }

    int32 ObtenerNivelActual() const { return OleadaActual; }
    void EstablecerNivel(int32 NuevoNivel);
    void IniciarSiguienteNivel();   // Sube de nivel y reanuda el generador

    // Es momento del jefe cuando ya no quedan enemigos y no se ha vencido a�n.
    UFUNCTION()
    bool EsMomentoDelJefe() const { return EnemigosRestantesEnOleada <= 0 && !bJefeDerrotado; }
    void JefeDerrotado(UWorld* Mundo); // Marca el jefe vencido y pasa al siguiente nivel
    bool ObtenerJefeDerrotado() const { return bJefeDerrotado; }

    void ResetearJuego();           // Deja todo a cero (al empezar una partida)

};