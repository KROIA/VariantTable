#pragma once

#include "VariantTable_base.h"
#include <QIcon>
#include <QMap>


namespace VariantTable
{
	class VARIANT_TABLE_API IconManager
	{
		IconManager();
		static IconManager& getInstance();
		public:
		enum class Theme
		{
			black,
			color
		};

		static void setTheme(Theme theme);
		static Theme getTheme();
		static const QIcon &getIcon(const QString& name);



		private:
		void loadIcons();
		QString getThemePath(Theme t);
		void loadTheme(const QString& theme);
		Theme m_theme;
		QMap<QString, QIcon> m_icons;
		static const QStringList s_themes;


		
	};
}