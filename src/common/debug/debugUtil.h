#pragma once

#include "stdafx.h"

namespace DBG
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr)) throw;
	}
}