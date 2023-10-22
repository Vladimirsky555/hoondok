#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QTableView>
#include <QHeaderView>
#include <QDesktopServices>
#include <QTimer>
#include <QAxObject>
#include <QDir>
#include <QTableWidgetItem>
#include <QDebug>

#include "database.h"
#include "qregexphighlighter.h"
#include "func.h"
#include "gconst.h"

struct item {
    QString name;
    QString rusname;
    QString title;
    int year;
    QString alias;
    QString content;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	DataBase *db;
	qint32 f_dbcon;

    QRegexpHighlighter *highlighter;
    QRegexpHighlighter *highlighter2;

    QTimer *timer;

    QString gs_link_books;
    QString gs_link_speech;
    QString gs_link_search;
	QString gs_link_ss;

    QList<item>items;

public:
	MainWindow(QWidget *parent = nullptr);
	virtual ~MainWindow();

private:
	void db_connect();
	QString quoteMaker(QString pattern);//формирует текст с цитатами
	void export_to_doc(QString pattern);
	void setObjects_Start_StyleSheet();

    void tw_reset();
    void tw_show();

    // Загрузки из БД
    void loadfromDBbyPattern(QString pattern);
	void loadBooknames();
    void loadChapters(QString book);
    void loadSpeeches(QString year);
	void loadSStextes(QString year);
	void loadYears();
    void loadPreachers();
    void loadLocations();

    void loadMonthes();//месяцы для видеослужб

private slots:
    // Книги
	void on_cbxbooks_activated(const QString &arg);
    void on_cbxchapters_activated(int index);
    void on_cbxres_activated(int index);
    void on_cmdrestart_clicked();

    // Речи
    void on_cbxyears_activated(const QString &arg);
    void on_cbxspeech_activated(int index);
    void on_cmdlinkspeech_clicked();

    // Поиск на русском
    void on_txtsearch_returnPressed();
    void on_txtsearch_textChanged(const QString &arg);
    void on_cmdlinksearch_clicked();
    void on_cmdlinkbooks_clicked();

    void on_chbIO_clicked(bool checked);
    void on_chbIM_clicked(bool checked);

    void on_rrus_clicked();
    void on_reng_clicked();

    // Цитаты
    void on_txtsearch_quote_returnPressed();
    void on_txtsearch_quote_textChanged(const QString &arg);
    void on_cmdclear_clicked();

	// Тексты служб на русском
    void on_cmdexport_clicked();
	void on_cbxyears_sstxt_activated(const QString &arg);
	void on_cbxsstxt_activated(int index);
	void on_cmdlinksstxtrus_clicked();

    void on_btnRefresh_clicked();
    void on_btnClear_clicked();
    void on_btnSearch_clicked();
    void on_tw_cellClicked(int row, int col);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
