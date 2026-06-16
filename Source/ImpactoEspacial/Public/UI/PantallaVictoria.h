#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaVictoria.generated.h"

// Pantalla de VICTORIA: muestra la puntuación final y un botón para volver al menú.
UCLASS()
class IMPACTOESPACIAL_API UPantallaVictoria : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override; // Muestra la puntuación y engancha el botón

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextoPuntuacion;   // Texto "¡VICTORIA!" + puntos

    UPROPERTY(meta = (BindWidget))
    class UButton* BotonMenu;            // Botón para volver al menú

    UFUNCTION()
    void AlPresionarMenu();              // Carga el nivel del menú
};