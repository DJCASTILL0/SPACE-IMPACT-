#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Nucleo/GameManager.h"   // para el enum EEstadoJuego
#include "EstadoJuego.generated.h"

class UGameManager;

// ============================================================================
//  EstadoJuego  ->  PATRON STATE
//  Cada estado del juego es una CLASE con su propio comportamiento. El
//  GameManager guarda el estado actual como objeto y delega en el; al cambiar
//  de estado se intercambia el objeto (CambiarEstado()).
//  Esta es la version "de verdad" del patron State (no el simple enum + switch).
// ============================================================================
UCLASS(Abstract)
class IMPACTOESPACIAL_API UEstadoJuego : public UObject
{
    GENERATED_BODY()

public:
    // Se ejecuta al ENTRAR en este estado (comportamiento propio de cada uno).
    virtual void Entrar(UGameManager* GM) {}

    // Acciones que dependen del ESTADO. Por defecto no hacen nada; cada estado
    // concreto decide si responde (esto es el corazon del patron State).
    virtual void Pausar(UGameManager* GM) {}
    virtual void Reanudar(UGameManager* GM) {}

    // A que valor del enum corresponde este estado (compatibilidad / consulta).
    virtual EEstadoJuego Tipo() const
        PURE_VIRTUAL(UEstadoJuego::Tipo, return EEstadoJuego::MenuPrincipal;);
};

// --- Estados concretos ---

UCLASS()
class IMPACTOESPACIAL_API UEstadoMenu : public UEstadoJuego
{
    GENERATED_BODY()
public:
    virtual void Entrar(UGameManager* GM) override;
    virtual EEstadoJuego Tipo() const override { return EEstadoJuego::MenuPrincipal; }
};

UCLASS()
class IMPACTOESPACIAL_API UEstadoJugando : public UEstadoJuego
{
    GENERATED_BODY()
public:
    virtual void Entrar(UGameManager* GM) override;
    virtual void Pausar(UGameManager* GM) override;   // jugando -> pausado
    virtual EEstadoJuego Tipo() const override { return EEstadoJuego::Jugando; }
};

UCLASS()
class IMPACTOESPACIAL_API UEstadoPausado : public UEstadoJuego
{
    GENERATED_BODY()
public:
    virtual void Entrar(UGameManager* GM) override;
    virtual void Reanudar(UGameManager* GM) override; // pausado -> jugando
    virtual EEstadoJuego Tipo() const override { return EEstadoJuego::Pausado; }
};

UCLASS()
class IMPACTOESPACIAL_API UEstadoGameOver : public UEstadoJuego
{
    GENERATED_BODY()
public:
    virtual void Entrar(UGameManager* GM) override;
    virtual EEstadoJuego Tipo() const override { return EEstadoJuego::GameOver; }
};

UCLASS()
class IMPACTOESPACIAL_API UEstadoVictoria : public UEstadoJuego
{
    GENERATED_BODY()
public:
    virtual void Entrar(UGameManager* GM) override;
    virtual EEstadoJuego Tipo() const override { return EEstadoJuego::Victoria; }
};
