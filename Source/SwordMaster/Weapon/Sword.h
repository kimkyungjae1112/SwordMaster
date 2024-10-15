// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class SWORDMASTER_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	ASword();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> MeshComp;

};
