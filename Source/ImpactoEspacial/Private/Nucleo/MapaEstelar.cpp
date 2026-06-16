// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nucleo/MapaEstelar.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

AMapaEstelar::AMapaEstelar()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Cargar meshes en el constructor (unico lugar permitido)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> EsferaMesh(TEXT("/Engine/BasicShapes/Sphere"));
    MeshEsfera = EsferaMesh.Object;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("/Engine/BasicShapes/Cube"));
    MeshCubo = CuboMesh.Object;
}

void AMapaEstelar::BeginPlay()
{
    Super::BeginPlay();
    CrearFondoEstrellado();
    CrearSuelo();
}

void AMapaEstelar::CrearFondoEstrellado()
{
    if (!MeshEsfera) return;

    for (int i = 0; i < 200; i++)
    {
        UStaticMeshComponent* Estrella = NewObject<UStaticMeshComponent>(this);
        Estrella->RegisterComponent();
        Estrella->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Estrella->SetStaticMesh(MeshEsfera);

        float X = FMath::RandRange(-3000.f, 3000.f);
        float Z = FMath::RandRange(-1500.f, 1500.f);
        float Y = FMath::RandRange(-100.f, 100.f);
        Estrella->SetRelativeLocation(FVector(X, Y, Z));

        float Tamanio = FMath::RandRange(0.05f, 0.2f);
        Estrella->SetWorldScale3D(FVector(Tamanio));

        FLinearColor ColorEstrella;
        int ColorRandom = FMath::RandRange(0, 2);
        switch (ColorRandom)
        {
        case 0: ColorEstrella = FLinearColor::White; break;
        case 1: ColorEstrella = FLinearColor(0.5f, 0.7f, 1.0f); break;
        case 2: ColorEstrella = FLinearColor(1.0f, 0.9f, 0.5f); break;
        }

        UMaterialInstanceDynamic* Mat = Estrella->CreateAndSetMaterialInstanceDynamic(0);
        if (Mat)
        {
            Mat->SetVectorParameterValue(FName("BaseColor"), ColorEstrella);
        }

        Estrella->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Estrellas.Add(Estrella);
    }
}

void AMapaEstelar::CrearSuelo()
{
    if (!MeshCubo) return;

    // =============================================
    // PARED SUPERIOR (gruesa, como techo de tunel)
    // =============================================
    SueloArriba = NewObject<UStaticMeshComponent>(this);
    SueloArriba->RegisterComponent();
    SueloArriba->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SueloArriba->SetStaticMesh(MeshCubo);
    SueloArriba->SetWorldScale3D(FVector(6000.f, 1.f, 4.0f)); // 4 veces mas gruesa
    SueloArriba->SetRelativeLocation(FVector(0.f, -10.f, 850.f));

    UMaterialInstanceDynamic* MatArriba = SueloArriba->CreateAndSetMaterialInstanceDynamic(0);
    if (MatArriba)
    {
        MatArriba->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.05f, 0.05f, 0.1f));
    }

    // =============================================
    // PARED INFERIOR (gruesa, como suelo de tunel)
    // =============================================
    SueloAbajo = NewObject<UStaticMeshComponent>(this);
    SueloAbajo->RegisterComponent();
    SueloAbajo->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SueloAbajo->SetStaticMesh(MeshCubo);
    SueloAbajo->SetWorldScale3D(FVector(6000.f, 1.f, 4.0f)); // 4 veces mas gruesa
    SueloAbajo->SetRelativeLocation(FVector(0.f, -10.f, -850.f));

    UMaterialInstanceDynamic* MatAbajo = SueloAbajo->CreateAndSetMaterialInstanceDynamic(0);
    if (MatAbajo)
    {
        MatAbajo->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.05f, 0.05f, 0.1f));
    }

    // =============================================
    // BORDE ADICIONAL: Decoracion del tunel
    // =============================================
    for (int i = -3; i <= 3; i++)
    {
        // Luces en las paredes
        UStaticMeshComponent* Luz = NewObject<UStaticMeshComponent>(this);
        Luz->RegisterComponent();
        Luz->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Luz->SetStaticMesh(MeshEsfera);
        Luz->SetWorldScale3D(FVector(0.3f));

        float X = i * 400.f;

        // Luz arriba
        Luz->SetRelativeLocation(FVector(X, 20.f, 780.f));
        UMaterialInstanceDynamic* MatLuz = Luz->CreateAndSetMaterialInstanceDynamic(0);
        if (MatLuz)
        {
            MatLuz->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.8f, 0.2f, 0.2f));
        }
        Luz->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // Luz abajo
        UStaticMeshComponent* LuzAbajo = NewObject<UStaticMeshComponent>(this);
        LuzAbajo->RegisterComponent();
        LuzAbajo->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        LuzAbajo->SetStaticMesh(MeshEsfera);
        LuzAbajo->SetWorldScale3D(FVector(0.3f));
        LuzAbajo->SetRelativeLocation(FVector(X, 20.f, -780.f));

        UMaterialInstanceDynamic* MatLuzAbajo = LuzAbajo->CreateAndSetMaterialInstanceDynamic(0);
        if (MatLuzAbajo)
        {
            MatLuzAbajo->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0.8f, 0.2f, 0.2f));
        }
        LuzAbajo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}