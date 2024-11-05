#include <QApplication>
#include <QTableView>
#include "VariantTable.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	VariantTable::TableView* tableView = new VariantTable::TableView;
	//VariantTable::Model* model = new VariantTable::Model(5,3, tableView);

	VariantTable::BoolCell* boolCell = new VariantTable::BoolCell("Test", true);
	
	tableView->getModel()->setCellData(0, 0, boolCell);

	tableView->resize(800, 600);
	tableView->resizeRowToContents(0);
	tableView->show();

	return app.exec();
}



/*#ifdef QT_ENABLED
#include <QApplication>
#endif
#include <iostream>
#include "VariantTable.h"

#ifdef QT_WIDGETS_ENABLED
#include <QWidget>
#endif

int main(int argc, char* argv[])
{
#ifdef QT_WIDGETS_ENABLED
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#ifdef QT_ENABLED
	QApplication app(argc, argv);
#endif
	VariantTable::Profiler::start();
	VariantTable::LibraryInfo::printInfo();
#ifdef QT_WIDGETS_ENABLED
	QWidget* widget = VariantTable::LibraryInfo::createInfoWidget();
	if (widget)
		widget->show();
#endif
	int ret = 0;
#ifdef QT_ENABLED
	ret = app.exec();
#endif
	VariantTable::Profiler::stop((std::string(VariantTable::LibraryInfo::name) + ".prof").c_str());
	return ret;
}
*/