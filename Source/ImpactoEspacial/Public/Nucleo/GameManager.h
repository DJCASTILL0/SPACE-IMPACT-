// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "GameManager.generated.h"

class UEstadoJuego;   // STATE: clase base de los estados (definida en EstadoJuego.h)

// Valores de estado del juego. El comportamiento real lo dan las clases de
// estado (patron State); este enum se usa solo para consultar en que estado se esta.
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

    // STATE: el estado actual es un OBJETO (no un enum); se intercambia con CambiarEstado().
    UPROPERTY()
    UEstadoJuego* EstadoObjeto;

    int32 PuntuacionTotal;          // Puntos acumulados
    int32 OleadaActual;             // Nivel/oleada actual (1, 2, 3...)
    int32 EnemigosRestantesEnOleada;// Cu�ntos enemigos faltan por matar
    bool bJefeDerrotado;            // �Ya se venci� al jefe de este nivel?

    UGameManager();                 // Constructor PRIVADO (rasgo del Singleton)

    // SINGLETON: la UNICA instancia de la clase.
    // (Los UObject de Unreal ya no son copiables, asi que no hace falta borrar
    //  el constructor de copia: GENERATED_BODY() ya lo gestiona.)
    static UGameManager* Instancia;

public:
    // Punto de acceso �nico del Singleton: devuelve siempre el mismo GameManager.
    static UGameManager* ObtenerInstancia(UWorld* Mundo);

    void CargarNivel(FName NombreNivel); // Cambia de mapa guardando antes la puntuaci�n
    void IniciarJuego();
    void PausarJuego();
    void ReanudarJuego();
    void GameOver();                // Pasa a estado GameOver y muestra su pantalla
    void VictoriaJuego();
    EEstadoJuego ObtenerEstado() const;   // devuelve el tipo del estado actual

    // STATE: cambia el estado del juego creando el objeto de estado indicado.
    void CambiarEstado(TSubclassOf<UEstadoJuego> NuevoEstado);

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