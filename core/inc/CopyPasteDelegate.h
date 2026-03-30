#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QModelIndex>

namespace VariantTable
{
	class CopyPasteDelegate
	{
	public:
		CopyPasteDelegate() = default;
		CopyPasteDelegate(const CopyPasteDelegate& other) = default;
		CopyPasteDelegate(CopyPasteDelegate&&) noexcept = default;
		virtual ~CopyPasteDelegate() = default;

		CopyPasteDelegate& operator=(const CopyPasteDelegate& other) = default;
		CopyPasteDelegate& operator=(CopyPasteDelegate&& other) noexcept = default;

		virtual bool copyCell(CellDataBasePtr cellData, const QModelIndex& index) = 0;
		virtual bool pasteCell(CellDataBasePtr cellData, const QModelIndex& index) = 0;
	};
}