#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    //Автоматически переводит системные диалоги
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    // Стили для всего проекта
//    QFile styleFile(":/qss/styles.qss");
//    styleFile.open(QFile::ReadOnly | QFile::Text);
//    QTextStream styleStream(&styleFile);
//    QString styleSheet = styleStream.readAll();
//    qApp->setStyleSheet(styleSheet);

	MainWindow w;
	w.show();

	return a.exec();
}
