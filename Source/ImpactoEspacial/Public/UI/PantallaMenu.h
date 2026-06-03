// PantallaMenu.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaMenu.generated.h"

UCLASS()
class IMPACTOESPACIAL_API UPantallaMenu : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Botones (se enlazan con el editor)
    UPROPERTY(meta = (BindWidget))
    class UButton* BotonJugar;

    UPROPERTY(meta = (BindWidget))
    class UButton* BotonSalir;

    UFUNCTION()
    void AlPresionarJugar();

    UFUNCTION()
    void AlPresionarSalir();
};