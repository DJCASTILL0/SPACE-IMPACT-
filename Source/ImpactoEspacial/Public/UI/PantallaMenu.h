// PantallaMenu.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaMenu.generated.h"

// ============================================================================
//  PantallaMenu  (widget del men� principal)
//  En NativeConstruct conecta los botones a sus funciones (suscripci�n a
//  OnClicked = Observer de Unreal). Jugar carga el nivel; Salir cierra el juego.
// ============================================================================
UCLASS()
class IMPACTOESPACIAL_API UPantallaMenu : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override; // Se llama al crear el widget: engancha botones

    // Botones (se enlazan autom�ticamente con los del editor por su nombre)
    UPROPERTY(meta = (BindWidget))
    class UButton* BotonJugar;

    UPROPERTY(meta = (BindWidget))
    class UButton* BotonSalir;

    UFUNCTION()
    void AlPresionarJugar();   // Carga el nivel de juego

    UFUNCTION()
    void AlPresionarSalir();   // Cierra el juego
};