#include <QApplication>
#include <QTableView>
#include <QVBoxLayout>
#include <qgridlayout.h>
#include <QPushButton>
#include <QToolTip>
#include <QFile>
#include <QTimer>

#include "VariantTable.h"


int main(int argc, char* argv[]) 
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	QApplication app(argc, argv);
	
	/*QFile file("dark-style.css"); // Adjust path as needed
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		QString styleSheet = file.readAll();
		app.setStyleSheet(styleSheet);
		file.close();
	}
	else {
		qDebug() << "Failed to load stylesheet.";
	}*/
	VariantTable::TableView* tableView = new VariantTable::TableView;

	VariantTable::CellDataBasePtr boolCell = VariantTable::CheckBox::create("Test", true);
	VariantTable::CellDataBasePtr radioBox = VariantTable::RadioButton::create(QStringList{ "RadioA" ,"RadioB"});
	VariantTable::CellDataBasePtr progressBar = VariantTable::ProgressBar::create(0, 100, 0);
	auto button = VariantTable::PushButton::create("Button");
	connect(button, &VariantTable::PushButton::clicked, [radioBox]() {
		qDebug() << "Button clicked";
		radioBox->setEditable(!radioBox->isEditable());
		VariantTable::IconManager::Theme theme = VariantTable::IconManager::getTheme();
		if (theme == VariantTable::IconManager::Theme::color)
			VariantTable::IconManager::setTheme(VariantTable::IconManager::Theme::black);
		else
			VariantTable::IconManager::setTheme(VariantTable::IconManager::Theme::color);
			 
			
			});

	tableView->getModel()->setCellData(0, 0, VariantTable::CheckBox::create("Test", true));
	tableView->getModel()->setCellData(1, 0, VariantTable::RadioButton::create(QStringList{ "RadioA" ,"RadioB" }));
	tableView->getModel()->setCellData(2, 0, VariantTable::CheckBoxList::create(QStringList{ "Box1" ,"Box2", "Box3"}));
	tableView->getModel()->setCellData(3, 0, VariantTable::ComboBox::create(QStringList{ "Option1" ,"Option2", "Option3"}));
	tableView->getModel()->setCellData(4, 0, VariantTable::LineEdit::create("Hello World"));
	tableView->getModel()->setCellData(0, 1, VariantTable::TextEdit::create("Hello\nWorld"));
	tableView->getModel()->setCellData(0, 2, VariantTable::TimeEdit::create(QTime::currentTime()));
	tableView->getModel()->setCellData(0, 3, VariantTable::DateEdit::create(QDate::currentDate()));
	tableView->getModel()->setCellData(0, 4, VariantTable::DateTimeEdit::create(QDateTime::currentDateTimeUtc()));
	tableView->getModel()->setCellData(6, 0, progressBar);
	tableView->getModel()->setCellData(5, 0, button);
	tableView->getModel()->setCellData(5, 5, boolCell);
	tableView->getModel()->setCellData(5, 6, boolCell);

	bool ret1 = tableView->getModel()->setHeaderData(0, Qt::Horizontal, "Column 1", Qt::DisplayRole);
	tableView->getModel()->setHeaderData(2, Qt::Horizontal, "Column 3");
	ret1;

	QTimer timer;
	QObject::connect(&timer, &QTimer::timeout, [tableView, progressBar]() {
		VariantTable::ProgressBar* bar = dynamic_cast<VariantTable::ProgressBar*>(progressBar.get());
		if (bar)
		{
			bar->setValue(bar->getValue() + 1);
			if (bar->getValue() >= bar->getMaximum())
				bar->setValue(bar->getMinimum());
		}
		});
	timer.start(100);

	

	tableView->resize(800, 600);
	tableView->resizeRowToContents(0);

	QWidget* mainWidget = new QWidget;
	QGridLayout* mainLayout = new QGridLayout;
	mainWidget->setLayout(mainLayout);

	QWidget* widget = new QWidget;
	QVBoxLayout* layout = new QVBoxLayout;
	QPushButton* upButton = new QPushButton("Up");
	QPushButton* downButton = new QPushButton("Down");
	layout->addWidget(upButton);
	layout->addWidget(downButton);
	widget->setLayout(layout);

	QObject::connect(upButton, &QPushButton::clicked, [tableView]() {
		VariantTable::Model* model = tableView->getModel();
		model->moveRow(5, 0);
		});

	QObject::connect(downButton, &QPushButton::clicked, [tableView]() {
		VariantTable::Model* model = tableView->getModel();
		
		});
	{
		QPushButton* selectionButton = new QPushButton("Select Column");
		QObject::connect(selectionButton, &QPushButton::clicked, [tableView]() {
			tableView->highlightColumn(5);
		//	tableView->scrollToColumn(5);
			tableView->deselectColumn(5);
						 });
		layout->addWidget(selectionButton);
	}
	{
		QPushButton* selectionButton = new QPushButton("Select Row");
		QObject::connect(selectionButton, &QPushButton::clicked, [tableView]() {
			tableView->highlightRow(5);
		//	tableView->scrollToRow(5);
			tableView->selectRow(5);
						 });
		layout->addWidget(selectionButton);
	}
	{
		QPushButton* selectionButton = new QPushButton("Select Cell");
		QObject::connect(selectionButton, &QPushButton::clicked, [tableView]() {
			tableView->highlightCell(2,2);
						 });
		layout->addWidget(selectionButton);
	}


	mainLayout->addWidget(widget, 0, 0);
	mainLayout->addWidget(tableView, 0, 1);

	// Select column 1
	//tableView->selectColumn(6);

	mainWidget->show();
	mainWidget->resize(300, 300);

	layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

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