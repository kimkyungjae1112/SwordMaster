// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"
#include "Components/BoxComponent.h"

ASword::ASword()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(Root);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);
	Box->OnComponentHit.AddDynamic(this, &ASword::OnHit);
	Box->SetCollisionProfileName(TEXT("Sword"));
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}


