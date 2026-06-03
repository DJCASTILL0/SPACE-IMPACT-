#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PantallaVictoria.generated.h"

UCLASS()
class IMPACTOESPACIAL_API UPantallaVictoria : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextoPuntuacion;

    UPROPERTY(meta = (BindWidget))
    class UButton* BotonMenu;

    UFUNCTION()
    void AlPresionarMenu();
};