#pragma once

#include "VariantTable_base.h"
#include <QWidget>
#include <QVariant>
#include <QMargins>
#include <QBoxLayout>
#include <Qt>

namespace VariantTable
{
	/**
	 * @brief Persistable layout configuration for editor widgets that host a QBoxLayout.
	 *
	 * Stored on the CellDataBase subclass so the configuration survives across editor
	 * widget recreations performed by the Qt model/view framework.
	 */
	struct VARIANT_TABLE_API BoxLayoutSettings
	{
		int spacing = 0;                                         ///< Spacing between layout items (px).
		QMargins contentsMargins = QMargins(0, 0, 0, 0);         ///< Outer margins of the layout.
		QBoxLayout::Direction direction = QBoxLayout::TopToBottom; ///< Stack direction.
		bool addTrailingStretch = true;                          ///< If true, items pack to the start with a stretch at the end.

		/**
		 * @brief Apply the settings to @p layout.
		 * @param layout Target box layout (no-op if null).
		 *
		 * Removes any existing trailing stretch, then re-applies spacing / margins /
		 * direction and optionally appends a single trailing stretch.
		 */
		void apply(QBoxLayout* layout) const
		{
			if (!layout) return;
			layout->setSpacing(spacing);
			layout->setContentsMargins(contentsMargins);
			layout->setDirection(direction);

			// Strip any pre-existing trailing stretch before (optionally) re-adding one.
			if (layout->count() > 0)
			{
				QLayoutItem* last = layout->itemAt(layout->count() - 1);
				if (last && last->spacerItem() && !last->widget())
					delete layout->takeAt(layout->count() - 1);
			}
			if (addTrailingStretch)
				layout->addStretch(1);
		}
	};

	class VARIANT_TABLE_API CellWidgetBase : public QWidget
	{
		Q_OBJECT
	public:
		explicit CellWidgetBase(QWidget* parent = nullptr)
			: QWidget(parent)
		{}
		CellWidgetBase(const CellWidgetBase&) = delete;
		CellWidgetBase& operator=(const CellWidgetBase&) = delete;

		virtual ~CellWidgetBase() = default;

		virtual void setData(const QVariant& data) = 0;
		virtual QVariant getData() const = 0;

		void setAlignment(Qt::Alignment alignment)
		{
			if (m_alignment == alignment) return;
			m_alignment = alignment;
			onAlignmentChanged(alignment);
		}
		Qt::Alignment getAlignment() const { return m_alignment; }

	protected:
		virtual void onAlignmentChanged(Qt::Alignment /*alignment*/) {}

	private:
		Qt::Alignment m_alignment{ Qt::AlignTop };
	};
}
