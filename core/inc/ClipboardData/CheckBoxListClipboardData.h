#pragma once

#include "VariantTable_base.h"
#include "ClipboardData.h"
#include <QVariant>

namespace VariantTable
{
	class VARIANT_TABLE_API CheckBoxListClipboardData : public ClipboardData
	{
	public:
		CheckBoxListClipboardData()
			: ClipboardData()
		{
		}
		CheckBoxListClipboardData(const CheckBoxListClipboardData& other)
			: ClipboardData(other)
			, m_hasText(other.m_hasText)
			, m_hasCheckedState(other.m_hasCheckedState)
			, m_text(other.m_text)
			, m_checkedState(other.m_checkedState)
		{
		}
		CheckBoxListClipboardData(CheckBoxListClipboardData&& other) noexcept
			: ClipboardData(std::move(other))
			, m_hasText(other.m_hasText)
			, m_hasCheckedState(other.m_hasCheckedState)
			, m_text(std::move(other.m_text))
			, m_checkedState(other.m_checkedState)
		{
		}
		~CheckBoxListClipboardData() = default;

		CheckBoxListClipboardData& operator=(const CheckBoxListClipboardData& other)
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
		CheckBoxListClipboardData& operator=(CheckBoxListClipboardData&& other) noexcept
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

		bool operator==(const CheckBoxListClipboardData& other) const
		{
			return m_hasText == other.m_hasText &&
				m_hasCheckedState == other.m_hasCheckedState &&
				m_text == other.m_text &&
				m_checkedState == other.m_checkedState;
		}
		bool operator!=(const CheckBoxListClipboardData& other) const
		{
			return !(*this == other);
		}

		void setText(const QVector<QString>& text)
		{
			m_text = text;
			m_hasText = true;
		}
		void setCheckedState(const QVector<bool>& checked)
		{
			m_checkedState = checked;
			m_hasCheckedState = true;
		}

		bool hasText() const { return m_hasText; }
		bool hasCheckedState() const { return m_hasCheckedState; }

		const QVector<QString>& getText() const { return m_text; }
		const QVector<bool>& getCheckedState() const { return m_checkedState; }

	private:
		bool m_hasText = false;
		bool m_hasCheckedState = false;

		QVector<QString> m_text;
		QVector<bool> m_checkedState;
	};
}