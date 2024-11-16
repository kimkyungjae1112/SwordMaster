// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"
#include "Components/BoxComponent.h"

ASword::ASword()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlap);
	Box->SetCollisionProfileName(TEXT("Sword"));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(Box);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


