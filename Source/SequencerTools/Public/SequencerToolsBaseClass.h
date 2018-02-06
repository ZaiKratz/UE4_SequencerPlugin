#pragma once

#include "SequencerToolsBaseClass.generated.h"

#define _LOG(_TEXT_, ...) UE_LOG(LogTemp, Warning, TEXT(_TEXT_), __VA_ARGS__)

UCLASS(Blueprintable, Abstract)
class USequencerToolsBaseClass : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnToolTriggered() {};
	virtual void OnToolClosed() {};
};