
#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

class SMainMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHud) // Pass info to menuHUD

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class AMenuHUD> OwningHud; // Manual garbage collection(?)

	virtual bool SupportsKeyboardFocus() const override { return true; };
	
	
};
