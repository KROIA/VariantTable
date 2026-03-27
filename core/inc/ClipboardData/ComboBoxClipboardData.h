#pragma once

#include "VariantTable_base.h"
#include "ClipboardData.h"
#include <QVariant>

namespace VariantTable
{
	class VARIANT_TABLE_API ComboBoxClipboardData : public ClipboardData
	{
	public:
		ComboBoxClipboardData()
			: ClipboardData()
		{
		}
		ComboBoxClipboardData(const ComboBoxClipboardData& other)
			: ClipboardData(other)
			, m_hasText(other.m_hasText)
			, m_hasSelectedIndex(other.m_hasSelectedIndex)
			, m_text(other.m_text)
			, m_selectedIndex(other.m_selectedIndex)
		{
		}
		ComboBoxClipboardData(ComboBoxClipboardData&& other) noexcept
			: ClipboardData(std::move(other))
			, m_hasText(other.m_hasText)
			, m_hasSelectedIndex(other.m_hasSelectedIndex)
			, m_text(std::move(other.m_text))
			, m_selectedIndex(other.m_selectedIndex)
		{
		}
		~ComboBoxClipboardData() = default;

		ComboBoxClipboardData& operator=(const ComboBoxClipboardData& other)
		{
			if (this != &other)
			{
				ClipboardData::operator=(other);
				m_hasText = other.m_hasText;
				m_hasSelectedIndex = other.m_hasSelectedIndex;
				m_text = other.m_text;
				m_selectedIndex = other.m_selectedIndex;
			}
			return *this;
		}
		ComboBoxClipboardData& operator=(ComboBoxClipboardData&& other) noexcept
		{
			if (this != &other)
			{
				ClipboardData::operator=(std::move(other));
				m_hasText = other.m_hasText;
				m_hasSelectedIndex = other.m_hasSelectedIndex;
				m_text = std::move(other.m_text);
				m_selectedIndex = other.m_selectedIndex;
			}
			return *this;
		}

		bool operator==(const ComboBoxClipboardData& other) const
		{
			return m_hasText == other.m_hasText &&
				m_hasSelectedIndex == other.m_hasSelectedIndex &&
				m_text == other.m_text &&
				m_selectedIndex == other.m_selectedIndex;
		}
		bool operator!=(const ComboBoxClipboardData& other) const
		{
			return !(*this == other);
		}

		void setText(const QStringList& text)
		{
			m_text = text;
			m_hasText = true;
		}
		void setSelectedIndex(int selectedIndex)
		{
			m_selectedIndex = selectedIndex;
			m_hasSelectedIndex = true;
		}

		bool hasText() const { return m_hasText; }
		bool hasSelectedIndex() const { return m_hasSelectedIndex; }

		const QStringList& getText() const { return m_text; }
		int getSelectedIndex() const { return m_selectedIndex; }

	private:
		bool m_hasText = false;
		bool m_hasSelectedIndex = false;

		QStringList m_text;
		int m_selectedIndex = -1;
	};
}