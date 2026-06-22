#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FachadaJuego.generated.h"

class UUserWidget;

// ============================================================================
//  FachadaJuego  ->  PATRON FACADE (Fachada)
//  Da una interfaz SIMPLE para operaciones que por dentro coordinan varios
//  subsistemas (GameManager, GeneradorEnemigos, UI, PlayerController).
//  Asi la nave (y otros) hacen una sola llamada en vez de orquestar todo a mano.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API UFachadaJuego : public UObject
{
    GENERATED_BODY()

public:
    // Inicia la partida: reinicia el GameManager, pone nivel 1 y arranca el generador.
    static void IniciarPartida(UWorld* Mundo);

    // Termina la partida: pasa a GameOver, muestra el widget y libera el cursor.
    static void TerminarPartida(UWorld* Mundo, TSubclassOf<UUserWidget> WidgetGameOver);

    // Muestra la pantalla de victoria y libera el cursor.
    static void MostrarVictoria(UWorld* Mundo, TSubclassOf<UUserWidget> WidgetVictoria);
};
