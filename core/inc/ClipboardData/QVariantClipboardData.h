#pragma once

#include "VariantTable_base.h"
#include "ClipboardData.h"
#include <QVariant>

namespace VariantTable
{
	class VARIANT_TABLE_API QVariantClipboardData : public ClipboardData
	{
	public:
		QVariantClipboardData() = default;
		QVariantClipboardData(const QVariant& data)
			: m_data(data){}
		QVariantClipboardData(const QVariantClipboardData& other)
			: ClipboardData(other)
			, m_data(other.m_data)
		{}
		QVariantClipboardData(QVariantClipboardData&& other) noexcept
			: ClipboardData(std::move(other)) 
			, m_data(std::move(other.m_data))
		{}

		~QVariantClipboardData() = default;

		QVariantClipboardData& operator=(const QVariantClipboardData& other)
		{
			if (this != &other)
			{
				ClipboardData::operator=(other);
				m_data = other.m_data;
			}
			return *this;
		}
		QVariantClipboardData& operator=(QVariantClipboardData&& other) noexcept
		{
			if (this != &other)
			{
				ClipboardData::operator=(std::move(other));
				m_data = std::move(other.m_data);
			}
			return *this;
		}

		bool operator==(const QVariantClipboardData& other) const
		{
			return m_data == other.m_data;
		}
		bool operator!=(const QVariantClipboardData& other) const
		{
			return !(*this == other);
		}

		void setData(const QVariant& data) { m_data = data; }
		const QVariant& getData() const { return m_data; }
	private:
		QVariant m_data;
	};
}