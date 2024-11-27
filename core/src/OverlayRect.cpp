#include "OverlayRect.h"
#include "TableView.h"
#include <QPainter>
#include <cmath>

namespace VariantTable
{
    namespace Internal
    {
        // Function to calculate a rainbow color based on parameter t
        QColor getRainbowColor(float t) {
            // Normalize t to range [0, 1]
            t = std::fmod(t, 1.0f);
            if (t < 0.0f) {
                t += 1.0f;
            }

            // Compute hue based on t (hue ranges from 0 to 360 degrees)
            float hue = t * 360.0f;

            // Convert HSV to QColor (HSV: Hue [0-360], Saturation [0-1], Value [0-1])
            return QColor::fromHsvF(hue / 360.0, 1.0, 1.0); // Full saturation and brightness
        }

        void OverlayRect::Rect::draw(QPainter& painter)
        {
            QPen pen(color);
            pen.setWidth(width);
            painter.setPen(pen);
            painter.drawRect(rect);
        }
        void OverlayRect::Rect::update()
        {
            int alpha = color.alpha();
            switch (mode)
            {
                case Mode::fadeOut:
                    alpha -= speed;
                    if (alpha <= 0)
                    {
                        finished = true;
                    }
                    break;
                case Mode::fadeIn:
                    alpha += speed;
                    if (alpha >= 255)
                    {
                        finished = true;
                    }
                    break;
                case Mode::fadeInOut:
                {
					if (time < 50)
					{
						alpha = time * 255/50;						
					}
					else if (time >= 50 && time < 100)
					{
						alpha = 255-(time-50)*255/50;						
					}
					else
					{
                        finished = true;
					}					
					break;  
                }
                case Mode::fadeInOutInfinity:
                {
                    if (time < 50)
                    {
                        alpha = time * 255 / 50;
                    }
                    else if (time >= 50 && time < 100)
                    {
                        alpha = 255-(time - 50) * 255 / 50;
                    }
                    else
                    {
                        time = 0;
                    }
                    break;
                }

            }
            if (alpha >= 255)
            {
                alpha = 255;
            }else if (alpha <= 0)
            {
                alpha = 0;
            }
            color.setAlpha(alpha);

			//color = getRainbowColor(time / 100);
            time += speed;
        }
      

        void OverlayRect::RowRect::update()
        {
            Rect::update();
            rect = table->getRowRect(m_row);
			int _width = rect.width() - width;
			int _height = rect.height() - width;
            rect.setX(rect.x() + width / 2);
			rect.setY(rect.y() + width / 2);
            rect.setWidth(_width);
			rect.setHeight(_height);
        }
        void OverlayRect::ColumnRect::update()
        {
            Rect::update();
            rect = table->getColumnRect(m_column);
            int _width = rect.width() - width;
            int _height = rect.height() - width;
            rect.setX(rect.x() + width / 2);
            rect.setY(rect.y() + width / 2);
            rect.setWidth(_width);
            rect.setHeight(_height);
        }
		void OverlayRect::CellRect::update()
		{
			Rect::update();
			rect = table->getCellRect(m_row, m_column);
			int _width = rect.width() - width;
			int _height = rect.height() - width;
			rect.setX(rect.x() + width / 2);
			rect.setY(rect.y() + width / 2);
			rect.setWidth(_width);
			rect.setHeight(_height);
		}


        OverlayRect::OverlayRect(TableView* tableView)
            : QWidget(tableView)
            , m_tableView(tableView)
        {
            //setAttribute(Qt::WA_OpaquePaintEvent);
            // Set the overlay to have a transparent background
            setAttribute(Qt::WA_OpaquePaintEvent, false); // Disable opaque painting
            setAttribute(Qt::WA_TransparentForMouseEvents); // Allow mouse events to pass through

            m_timer = new QTimer(this);
            connect(m_timer, &QTimer::timeout, this, &OverlayRect::updateFade);
            m_timer->start(50);
        }

        void OverlayRect::paintEvent(QPaintEvent* event) 
        {
            event;
            QPainter painter(this);
            QPen pen; // Red color with fading alpha      

            pen.setJoinStyle(Qt::MiterJoin);
            for (auto rect : m_rects)
            {
				pen.setColor(rect->color);
                pen.setWidth(rect->width); // Border thickness
                painter.setPen(pen);
                painter.drawRect(rect->rect);
            }
        }

        void OverlayRect::updateFade()
        {
            for (size_t i = 0; i < m_rects.size(); ++i)
            {
                if (m_rects[i]->finished)
                {
					m_rects.erase(m_rects.begin() + i);
					--i;
                    continue;
                }
				m_rects[i]->update();
            }
            update();
        }

    }
}