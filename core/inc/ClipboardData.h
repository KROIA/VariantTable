#pragma once

#include "VariantTable_base.h"

namespace VariantTable
{
	class ClipboardData
	{
	public:
		ClipboardData() = default;
		ClipboardData(const ClipboardData& other) = default;
		ClipboardData(ClipboardData&&) noexcept = default;
		virtual ~ClipboardData() = default;

		ClipboardData& operator=(const ClipboardData& other) = default;
		ClipboardData& operator=(ClipboardData&& other) noexcept = default;

	};
}