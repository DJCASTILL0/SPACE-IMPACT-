#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaGameOver.generated.h"

// Pantalla de GAME OVER: muestra la puntuación y un botón para volver al menú.
UCLASS()
class IMPACTOESPACIAL_API UPantallaGameOver : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override; // Muestra la puntuación y engancha el botón

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextoPuntuacion;   // Texto con los puntos finales

    UPROPERTY(meta = (BindWidget))
    class UButton* BotonMenu;            // Botón para volver al menú

    UFUNCTION()
    void AlPresionarMenu();              // Carga el nivel del menú
};