#include "IconManager.h"

#include <QDirIterator>
#include <QApplication>

#include <QDebug>
#include "Model.h"


static void loadVTResources()
{
	Q_INIT_RESOURCE(vt_icons_color);
	Q_INIT_RESOURCE(vt_icons_black);
}

namespace VariantTable
{
	const QStringList IconManager::s_themes = { "icons_black", "icons_color"};

	

	IconManager::IconManager()
	{
		m_theme = Theme::color;
		// Load QT resourse file
		loadVTResources();
		loadIcons();
	}

	QString IconManager::getThemePath(Theme t)
	{
		switch (t)
		{
			case Theme::color:
				return "icons_color";
		}
		return "icons_black";
	}

	IconManager& IconManager::getInstance()
	{
		static IconManager instance;
		return instance;
	}


	void IconManager::setTheme(Theme theme)
	{
		IconManager& instance = getInstance();
		if (instance.m_theme == theme)
			return;
		instance.m_theme = theme;
		instance.loadIcons();
		Model::iconThemeChanged();
	}
	IconManager::Theme IconManager::getTheme()
	{
		IconManager& instance = getInstance();
		return instance.m_theme;
	}
	const QIcon& IconManager::getIcon(const QString& name)
	{
		IconManager& instance = getInstance();
		if (instance.m_icons.find(name) == instance.m_icons.end())
		{
			static QIcon emptyIcon;
			qWarning() << "IconManager::getIcon: Icon not found: " << name;
			return emptyIcon;
		}
		return instance.m_icons[name];
	}

	
	void IconManager::loadIcons()
	{
		loadTheme(getThemePath(m_theme));
	}

	

	void IconManager::loadTheme(const QString& theme)
	{
		// load all files from the resourse file
		QDirIterator it(":/"+ theme, QDirIterator::Subdirectories);
		QMap<QString, QIcon> icons;
		while (it.hasNext())
		{
			it.next();
			if (it.fileInfo().isFile())
			{
				QIcon icon(it.filePath());
				icons[it.fileName()] = icon;
			}
		}
		m_icons = icons;
	}
}