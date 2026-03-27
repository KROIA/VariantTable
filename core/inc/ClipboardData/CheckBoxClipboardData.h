#pragma once

#include "VariantTable_base.h"
#include "ClipboardData.h"
#include <QVariant>

namespace VariantTable
{
	class VARIANT_TABLE_API CheckBoxClipboardData : public ClipboardData
	{
	public:
		CheckBoxClipboardData()
			: ClipboardData()
		{
		}
		CheckBoxClipboardData(const CheckBoxClipboardData& other)
			: ClipboardData(other)
			, m_hasText(other.m_hasText)
			, m_hasCheckedState(other.m_hasCheckedState)
			, m_text(other.m_text)
			, m_checkedState(other.m_checkedState)
		{
		}
		CheckBoxClipboardData(CheckBoxClipboardData&& other) noexcept
			: ClipboardData(std::move(other))
			, m_hasText(other.m_hasText)
			, m_hasCheckedState(other.m_hasCheckedState)
			, m_text(std::move(other.m_text))
			, m_checkedState(other.m_checkedState)
		{
		}
		~CheckBoxClipboardData() = default;

		CheckBoxClipboardData& operator=(const CheckBoxClipboardData& other)
		{
			if (this != &other)
			{
				ClipboardData::operator=(other);
				m_hasText = other.m_hasText;
				m_hasCheckedState = other.m_hasCheckedState;
				m_text = other.m_text;
				m_checkedState = other.m_checkedState;
			}
			return *this;
		}
		CheckBoxClipboardData& operator=(CheckBoxClipboardData&& other) noexcept
		{
			if (this != &other)
			{
				ClipboardData::operator=(std::move(other));
				m_hasText = other.m_hasText;
				m_hasCheckedState = other.m_hasCheckedState;
				m_text = std::move(other.m_text);
				m_checkedState = other.m_checkedState;
			}
			return *this;
		}

		bool operator==(const CheckBoxClipboardData& other) const
		{
			return m_hasText == other.m_hasText &&
				m_hasCheckedState == other.m_hasCheckedState &&
				m_text == other.m_text &&
				m_checkedState == other.m_checkedState;
		}
		bool operator!=(const CheckBoxClipboardData& other) const
		{
			return !(*this == other);
		}

		void setText(const QString& text)
		{
			m_text = text;
			m_hasText = true;
		}
		void setCheckedState(bool checked)
		{
			m_checkedState = checked;
			m_hasCheckedState = true;
		}

		bool hasText() const { return m_hasText; }
		bool hasCheckedState() const { return m_hasCheckedState; }

		const QString& getText() const { return m_text; }
		bool getCheckedState() const { return m_checkedState; }
		
	private:
		bool m_hasText = false;
		bool m_hasCheckedState = false;

		QString m_text;
		bool m_checkedState = false;
	};
}