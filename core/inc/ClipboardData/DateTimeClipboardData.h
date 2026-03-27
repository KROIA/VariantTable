#pragma once

#include "VariantTable_base.h"
#include "ClipboardData.h"
#include <QDate>
#include <QTime>

namespace VariantTable
{
	class VARIANT_TABLE_API DateTimeClipboardData : public ClipboardData
	{
	public:
		DateTimeClipboardData() = default;
		DateTimeClipboardData(const QDateTime& dateTime)
			: m_hasDate(true)
			, m_hasTime(true)
			, m_date(dateTime.date())
			, m_time(dateTime.time())
		{}
		DateTimeClipboardData(const DateTimeClipboardData& other)
			: ClipboardData(other)
			, m_hasDate(other.m_hasDate)
			, m_hasTime(other.m_hasTime)
			, m_date(other.m_date)
			, m_time(other.m_time)
		{}
		DateTimeClipboardData(DateTimeClipboardData&& other) noexcept
			: ClipboardData(std::move(other))
			, m_hasDate(other.m_hasDate)
			, m_hasTime(other.m_hasTime)
			, m_date(other.m_date)
			, m_time(other.m_time)
		{}

		~DateTimeClipboardData() = default;

		DateTimeClipboardData& operator=(const DateTimeClipboardData& other)
		{
			if (this != &other)
			{
				ClipboardData::operator=(other);
				m_hasDate = other.m_hasDate;
				m_hasTime = other.m_hasTime;
				m_date = other.m_date;
				m_time = other.m_time;
			}
			return *this;
		}
		DateTimeClipboardData& operator=(DateTimeClipboardData&& other) noexcept
		{
			if (this != &other)
			{
				ClipboardData::operator=(std::move(other));
				m_hasDate = other.m_hasDate;
				m_hasTime = other.m_hasTime;
				m_date = other.m_date;
				m_time = other.m_time;
			}
			return *this;
		}

		bool operator==(const DateTimeClipboardData& other) const
		{
			return m_hasDate == other.m_hasDate &&
				m_hasTime == other.m_hasTime &&
				m_date == other.m_date &&
				m_time == other.m_time;
		}
		bool operator!=(const DateTimeClipboardData& other) const
		{
			return !(*this == other);
		}

		void setDateTime(const QDateTime& dateTime)
		{
			m_hasDate = true;
			m_hasTime = true;
			m_date = dateTime.date();
			m_time = dateTime.time();
		}
		QDateTime getDateTime() const
		{
			return QDateTime(m_date, m_time);
		}

		void setDate(const QDate& date)
		{
			m_hasDate = true;
			m_date = date;
		}
		const QTime& getTime() const
		{
			return m_time;
		}
		void setTime(const QTime& time)
		{
			m_hasTime = true;
			m_time = time;
		}
		const QDate& getDate() const
		{
			return m_date;
		}

		bool hasDate() const { return m_hasDate; }
		bool hasTime() const { return m_hasTime; }
	private:
		bool m_hasDate = false;
		bool m_hasTime = false;

		QDate m_date;
		QTime m_time;
	};
}