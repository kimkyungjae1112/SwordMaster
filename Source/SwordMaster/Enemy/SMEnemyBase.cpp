// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBase.h"

float ASMEnemyBase::GetDetectRadius()
{
	return 0.0f;
}

float ASMEnemyBase::GetPatrolRadius()
{
	return 0.0f;
}

float ASMEnemyBase::GetAttackRange()
{
	return 0.0f;
}

void ASMEnemyBase::SetAttackFinished(const FOnAttackFinished& InOnAttackFinished)
{
}

void ASMEnemyBase::AttackByAI()
{
}

void ASMEnemyBase::AttackEndTiming()
{
}

void ASMEnemyBase::DefaultAttackHitCheck()
{
}
