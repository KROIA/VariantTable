#pragma once

#include "VariantTable_base.h"
#include <QWidget>
#include <QTimer>



namespace VariantTable
{
    class TableView;
    namespace Internal
    {
        class OverlayRect : public QWidget
        {
            Q_OBJECT
            public:

            enum class Mode
            {
                fadeOut,
				fadeIn,
                fadeInOut,
				fadeInOutInfinity
            };
            class Rect
            {
                public:
				Rect(TableView* table) : table(table) {}
				virtual ~Rect() = default;
                QRect rect;
				QColor color;
                unsigned int width = 4;
				Mode mode = Mode::fadeOut;
                bool finished = false;
                float speed = 1;
                
                void draw(QPainter& painter);
                virtual void update();

                protected:
				TableView* table;
				float time = 0;
            };
			class RowRect : public Rect
			{
				public:
				RowRect(TableView* table, int row)
                    : Rect(table)
                    , m_row(row)
                {}
                void update() override;
                private:
				int m_row;
			};
			class ColumnRect : public Rect
			{
				public:
				ColumnRect(TableView* table, int column)
					: Rect(table)
					, m_column(column)
				{}
                void update() override;
				private:
				int m_column;
			};
			class CellRect : public Rect
			{
				public:
				CellRect(TableView* table, int row, int column)
					: Rect(table)
					, m_row(row)
					, m_column(column)
				{}
				void update() override;
				private:
				int m_row;
				int m_column;
			};
            
            OverlayRect(TableView* tableView);

			void add(const std::shared_ptr<Rect>& rect)
			{
				m_rects.push_back(rect);
			}
            //void start();
            void clear()
			{
				m_rects.clear();
			}

            protected:
            void paintEvent(QPaintEvent* event) override;

            private slots:
            void updateFade();

            private:
			//bool m_enabled = false;
            //int m_column = -1;
            //int m_fadeAlpha = 255;

            TableView* m_tableView;

            QTimer* m_timer;

			std::vector<std::shared_ptr<Rect>> m_rects;
        };
    }
}