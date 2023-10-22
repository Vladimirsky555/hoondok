#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("hoondok.ru");

    //Подсветка поиска
    highlighter = new QRegexpHighlighter(ui->tab_2);
    highlighter->setDocument(ui->txtres->document());
    highlighter->setPattern(ui->txtsearch->text());

    // Подсветка цитат
    highlighter2 = new QRegexpHighlighter(ui->tab_3);
    highlighter2->setDocument(ui->txtres_quote->document());
    highlighter2->setPattern(ui->txtsearch_quote->text());

	f_dbcon = 0;
	db_connect();
    loadBooknames();
    loadLocations();

    ui->tab->setCurrentIndex(0);
    ui->rrus_search->setChecked(true);

    ui->rrus->setChecked(true);
    ui->chbIO->setChecked(true);
    loadYears();

    loadPreachers();
    loadMonthes();

    tw_reset();

    QString currentPath = QDir::currentPath();
    QDir dir_current(currentPath);

    //Создаём папку quotes, если её нет
    QDir dir_quotes("quotes");
    if(!dir_quotes.exists()){
        dir_current.mkpath("quotes");
    }

	setObjects_Start_StyleSheet();

    QPixmap pix(":/images/pix.png");
    ui->lblPix->setPixmap(pix.scaledToWidth(250));

    // Функционал переподключения к базе данных
    timer = new QTimer();
    timer->setInterval(300000); // Проверка каждые 5 минут
    timer->start();  

    QObject::connect(timer, &QTimer::timeout, [&]() {
        f_dbcon = db->dbclose();
        db->connectdb();
        if (!db->opendb()) {f_dbcon = 0;} else {f_dbcon = 1;}
    });
}

MainWindow::~MainWindow()
{
	if(f_dbcon != 0) {f_dbcon = db->dbclose();}
	delete ui;
}

void MainWindow::setObjects_Start_StyleSheet()
{
	func f; QString Obj_T = "", Obj_N = "";
	QList<QWidget *> allWdt = this->findChildren<QWidget *>();

	for (int i=0; i<allWdt.count();i++) {

		Obj_T = allWdt[i]->metaObject()->className();
		Obj_N = allWdt[i]->objectName();

		allWdt[i]->setStyleSheet(f.F_StyleSheet(Obj_N, Obj_T));
		allWdt[i]->installEventFilter(this);
    }
}



// ****************************
// СОЕДИНЕНИЕ С БАЗОЙ
// ****************************
void MainWindow::db_connect()
{
    if (f_dbcon == 0) {
        db = new DataBase();
        db->connectdb();
        if (!db->opendb()) {
            f_dbcon = 0;
            ui->statusBar->showMessage(se_db_notconn);
            QMessageBox::critical(this, ecap, tr(s_psf14) + tr(se_db_notconn));
            return;
        }
    }
    f_dbcon = 1;
    ui->statusBar->showMessage(se_db_conn);
}

void MainWindow::on_cmdrestart_clicked()
{
    loadBooknames();
}

void MainWindow::loadYears()
{
	func f;
    ui->cbxyears->clear();
    QString sqq = "", s_year = "";
    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    if(ui->chbIO->isChecked() && ui->rrus->isChecked()){
        sqq = "select distinct year from " + tr(s_tbl_speeches_tfs_rus) + " order by year desc";
    } else if(ui->chbIO->isChecked() && ui->reng->isChecked()) {
        sqq = "select distinct year from " + tr(s_tbl_speeches_tfs_eng) + " order by year desc";
    } else if(ui->chbIM->isChecked() && ui->rrus->isChecked()){
        sqq = "select distinct year from " + tr(s_tbl_speeches_tms_rus) + " order by year desc";
    } else if(ui->chbIM->isChecked() && ui->reng->isChecked()){
        sqq = "select distinct year from " + tr(s_tbl_speeches_tms_eng) + " order by year desc";
    }

	if(!f.checkrequest(&qqd, sqq)) return;

    while (qqd.next()){
        s_year = qqd.value("year").toString();
        ui->cbxyears->addItem(s_year);
	}


	// Года для служб текстов на русском
    sqq = "select distinct year from " + tr(s_tbl_sstext) + " order by year desc";
	if(!f.checkrequest(&qqd, sqq)) return;

	while (qqd.next()){
		s_year = qqd.value("year").toString();
		ui->cbxyears_sstxt->addItem(s_year);
    }   

    // Года для служб видео на русском
    sqq = "select distinct year(date) as yr from " + tr(s_tbl_ssvideo) + " order by date desc";
    if(!f.checkrequest(&qqd, sqq)) return;

    ui->cbxssyears->addItem("---");
    while (qqd.next()){
        s_year = qqd.value("yr").toString();
        ui->cbxssyears->addItem(s_year);
    }
}


void MainWindow::loadPreachers()
{
    func f; QString sqq = "", s_preacher = "";
    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    sqq = "select distinct preacher from " + tr(s_tbl_ssvideo) + " order by preacher";
    if(!f.checkrequest(&qqd, sqq)) return;

    while (qqd.next()){
        s_preacher = qqd.value("preacher").toString();
        ui->cbxpreacher->addItem(s_preacher);
    }
}

void MainWindow::loadLocations()
{
    func f; QString sqq = "", s_location = "";
    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    sqq = "select distinct location from " + tr(s_tbl_ssvideo) + " order by location";
    if(!f.checkrequest(&qqd, sqq)) return;

    while (qqd.next()){
        s_location = qqd.value("location").toString();
        ui->cbxlocation->addItem(s_location);
    }
}

void MainWindow::loadMonthes()
{
    ui->cbxssmonths->addItem("---");
    ui->cbxssmonths->addItem("Январь");
    ui->cbxssmonths->addItem("Февраль");
    ui->cbxssmonths->addItem("Март");
    ui->cbxssmonths->addItem("Апрель");
    ui->cbxssmonths->addItem("Май");
    ui->cbxssmonths->addItem("Июнь");
    ui->cbxssmonths->addItem("Июль");
    ui->cbxssmonths->addItem("Август");
    ui->cbxssmonths->addItem("Сентябрь");
    ui->cbxssmonths->addItem("Октябрь");
    ui->cbxssmonths->addItem("Ноябрь");
    ui->cbxssmonths->addItem("Декабрь");
}

QString MainWindow::quoteMaker(QString pattern)
{
	func f;
	QString s_result = "", s_text = "";
	QStringList strList; // список абзацев одного текста

	int cnt = 1;

	for(int i = 0; i < items.count(); i++){

		s_text = items.at(i).content;
		QRegExp rx_("</p>");
		strList = s_text.split(rx_);
		for(int j = 0; j < strList.count(); j++){
			QRegExp rx(pattern);
			if(!f.checkRegExp(rx))return "";
			int pos = 0;
			QString str = strList[j];

			//Проверка, чтобы исключить вывод одной цитаты,
			//в которой искомое слово повторяется, несколько раз
			int check = 0;
			while((pos = rx.indexIn(str, pos)) != -1){
				pos += rx.matchedLength();
				check++;
				if(check > 1)continue;
				cnt++;

				s_result += "<b><span style=\"color:#800000\">";
				s_result += QString::number(cnt);
				s_result += ": </span></b>";
				s_result += str;
				s_result += "<b><span style=\"color:#483D8B\"><br>(";
				s_result += items.at(i).rusname + ", ";

				if(items.at(i).name == "tfs_rus" || items.at(i).name == "tms_rus"){
					s_result += items.at(i).title + ", ";
				} else {
					s_result += items.at(i).title;
				}

				// Для речей ИО и ИМ добавляем год
				if(items.at(i).name == "tfs_rus" || items.at(i).name == "tms_rus"){
				   s_result += QString::number(items.at(i).year);
				}

				s_result +=   ")</span></b>";
				s_result += "<br />";
				s_result += "<br />";
				s_result += "<br />";
			}
		}

		strList.clear();
	}

	return s_result;
}

void MainWindow::export_to_doc(QString pattern)
{
	QAxObject *word = new QAxObject("Word.Application", nullptr);
	 if (word == nullptr) {
		 QMessageBox::critical(this, ecap, tr(se_word_notfound));
		 return;
	 }

	 QAxObject *documents = word->querySubObject("Documents()");
	 QAxObject *newdoc = documents->querySubObject("Add()");
	 QAxObject *active_doc = word->querySubObject("ActiveDocument");

	 //заголовок
	 QAxObject *range = newdoc->querySubObject("Range()");
	 QString title = "Цитаты на основе ключевого слова: " + pattern;
	 range->setProperty("Text", title);
	 range->dynamicCall("InsertParagraphAfter()");
	 range->dynamicCall("InsertParagraphAfter()");

	 QAxObject *font = range->querySubObject("Font");
	 font->setProperty("Size", 14);
	 font->setProperty("Name", "Times New Roman");
	 QAxObject *alignment = range->querySubObject("ParagraphFormat");
	 alignment->setProperty("SpaceAfter", 0);
	 //заголовок

	 //текст
	 QAxObject *content = newdoc->querySubObject("Content()");
	 content->dynamicCall("InsertAfter(Text)", ui->txtres_quote->toPlainText());
	 //текст

	 QVariant vff(0);	// формат файла (0 - doc)
	 QString s_fndoc1 = QCoreApplication::applicationDirPath() + sos + s_path + sos + pattern +  s_sdoc;
	 s_fndoc1.replace(sos, "\\");
	 word->dynamicCall("SetDisplayAlerts(bool)", false);
	 active_doc->querySubObject("SaveAs(const QString&, const QVariant&)", s_fndoc1, vff);

	 word->querySubObject("ActiveDocument")->dynamicCall("Close()");
	 word->dynamicCall("Quit()");

	 delete alignment;delete font;delete range;delete active_doc;
	 delete newdoc;delete documents;delete word;

	 QString path = QCoreApplication::applicationDirPath() + sos + s_path;
	 QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


void MainWindow::on_chbIO_clicked(bool checked)
{
    if(checked){
        ui->chbIM->setChecked(false);
    }

    loadYears();
}

void MainWindow::on_chbIM_clicked(bool checked)
{
    if(checked){
        ui->chbIO->setChecked(false);
    }

    loadYears();
}

void MainWindow::on_rrus_clicked()
{
    loadYears();
}

void MainWindow::on_reng_clicked()
{
    loadYears();
}


// ****************************
// СЛУЖБЫ ВИДЕО
// ****************************
void MainWindow::on_btnRefresh_clicked()
{
    tw_show();
}

void MainWindow::tw_reset()
{
    ui->tw->setRowCount(0);
    ui->tw->clear();
    ui->tw->repaint();

    QStringList headhor;
    headhor.append("№");
    headhor.append("Дата");
    headhor.append("Место");
    headhor.append("Пастор");
    headhor.append("Комментарий");
    headhor.append("link");
    headhor.append("id");

    ui->tw->setColumnCount(7);
    ui->tw->setHorizontalHeaderLabels(headhor);
    ui->tw->setShowGrid(true);
    ui->tw->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw->verticalHeader()->setVisible(false);

//    ui->tw->setStyleSheet(
//                "QHeaderView{background: rgb(220,220,220)}"
//                "QHeaderView::section{background-color: rgb(220,220,220); "
//                "font-weight: bold; "
//                "font: bold 12px; "
//                "font-family: Microsoft Sans Serif;}"
//                "QLineEdit{background-color: rgb(255,255,255);}"
//                "QTableWidget{background-color: rgb(255,255,255); }"
//                );

    ui->tw->setColumnWidth(0,10);
    ui->tw->setColumnWidth(1,100);
    ui->tw->setColumnWidth(2,150);
    ui->tw->setColumnWidth(3,150);
    ui->tw->setColumnWidth(4,550);
    ui->tw->setColumnWidth(5,0); ui->tw->hideColumn(5);
    ui->tw->setColumnWidth(6,0); ui->tw->hideColumn(6);
}

void MainWindow::tw_show()
{
    func f;
    QSqlQuery qqd(QSqlDatabase::database(sdbd));
    QString s_date = "", s_location = "", s_preacher = "", s_description = "", s_link = "", s_id = "";

    ui->tw->setRowCount(0);

    QString sqq = "select * from " + tr(s_tbl_ssvideo) + " order by id";
    if(!f.checkrequest(&qqd, sqq)) return;

    qint32 i = 0;
    while (qqd.next()) {
        s_date = qqd.value("date").toString();
        s_location = qqd.value("location").toString();
        s_preacher = qqd.value("preacher").toString();
        s_description = qqd.value("description").toString();
        s_link = qqd.value("link").toString();
        s_id = qqd.value("id").toString();

        ui->tw->insertRow(i);
        for (int j = 0; j < 7; j++)
        {
            if (j == 0) {
                ui->tw->setItem(i,j, new QTableWidgetItem(QString::number(i+1)));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
            if (j == 1) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_date));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
            if (j == 2) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_location));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 3) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_preacher));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 4) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_description));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 5) { ui->tw->setItem(i,j, new QTableWidgetItem(s_link)); }
            if (j == 6) { ui->tw->setItem(i,j, new QTableWidgetItem(s_id)); }
        }

        ui->tw->setRowHeight(i,60);

        i++;
    }
}

void MainWindow::on_btnSearch_clicked()
{
    func f;
    QSqlQuery qqd(QSqlDatabase::database(sdbd));
    QString sqq = "", s_uslov = "";
    QString s_year = "", s_month = "";
    QString s_date = "", s_location = "", s_preacher = "", s_description = "", s_link = "", s_id = "";

    ui->tw->setRowCount(0);

    // ----- формирование текста запроса
    s_year = ui->cbxssyears->currentText().trimmed();
    if (s_year != "---") {
        s_uslov += " and year(date) = '" + s_year + "'";
    }

    s_month = f.convertMonthes(ui->cbxssmonths->currentText());
    if (s_month != "") {
        s_uslov += " and SUBSTRING(date, 6, 2)  = '" + s_month + "'";
    }

    s_location = ui->cbxlocation->currentText().trimmed();
    if (s_location != "") {
        s_uslov += " and location = '" + s_location + "'";
    }

    s_preacher = ui->cbxpreacher->currentText().trimmed();
    if (s_preacher != "") {
        s_uslov += " and preacher = '" + s_preacher + "'";
    }

    s_description = ui->txtpattern->text();
    if (s_description != "") {
        s_uslov += " and description like '%" + s_description + "%'";
    }

    sqq = "select * from " + tr(s_tbl_ssvideo) + " where 1 " + s_uslov + " order by id";
    if(!f.checkrequest(&qqd, sqq)) return;

    qint32 i = 0;
    while (qqd.next()) {
        s_date = qqd.value("date").toString();
        s_location = qqd.value("location").toString();
        s_preacher = qqd.value("preacher").toString();
        s_description = qqd.value("description").toString();
        s_link = qqd.value("link").toString();
        s_id = qqd.value("id").toString();

        ui->tw->insertRow(i);
        for (int j = 0; j < 7; j++)
        {
            if (j == 0) {
                ui->tw->setItem(i,j, new QTableWidgetItem(QString::number(i+1)));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
            if (j == 1) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_date));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
            if (j == 2) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_location));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 3) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_preacher));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 4) {
                ui->tw->setItem(i,j, new QTableWidgetItem(s_description));
                ui->tw->item(i,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            if (j == 5) { ui->tw->setItem(i,j, new QTableWidgetItem(s_link)); }
            if (j == 6) { ui->tw->setItem(i,j, new QTableWidgetItem(s_id)); }
        }

        ui->tw->setRowHeight(i,60);

        i++;
    }
}



void MainWindow::on_btnClear_clicked()
{
    ui->cbxssyears->setCurrentText("---");
    ui->cbxssmonths->setCurrentText("---");
    ui->cbxlocation->setCurrentText("");
    ui->cbxpreacher->setCurrentText("");
    ui->txtpattern->clear();
    ui->tw->setRowCount(0);
}

void MainWindow::on_tw_cellClicked(int row, int col)
{
    Q_UNUSED(col)
    QString s_alias = ui->tw->item(row,5)->text();
    QString s_link = "https://youtu.be/" + s_alias;

    if(s_link != ""){
      QDesktopServices::openUrl(QUrl(s_link));
    }
}

// ****************************
// СЛУЖБЫ ТЕКСТЫ
// ****************************
void MainWindow::loadSStextes(QString year)
{
	QSqlQueryModel *model = new QSqlQueryModel(this);

	model->setQuery("select title, date, id from " + tr(s_tbl_sstext) +
					" WHERE YEAR(date) = '" + year + "' order by id", QSqlDatabase::database(sdbd));

	QTableView *table = new QTableView;
	table->verticalHeader()->setVisible(false);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);

	QComboBox *wgt_cbx = ui->cbxsstxt;
	wgt_cbx->setModel(model);
	wgt_cbx->setView(table);
	table->setModel(model);

	model->setHeaderData(0,Qt::Horizontal,"Название");
	model->setHeaderData(1,Qt::Horizontal,"Дата");
	model->setHeaderData(2,Qt::Horizontal,"id");

	table->setColumnWidth(0,800);
	table->setColumnWidth(1,200);
	table->setColumnWidth(2,0); table->hideColumn(2);

	table->setFixedWidth(1000);
	table->verticalHeader()->setDefaultSectionSize(30);

	table->setFont(QFont("Tahoma", 12));
	wgt_cbx->addItem("---");
}

void MainWindow::on_cbxyears_sstxt_activated(const QString &arg)
{
	loadSStextes(arg);
}

void MainWindow::on_cbxsstxt_activated(int index)
{
	func f;
	QString sqq = "", s_content = "", s_year = "";
	QString s_id = ui->cbxsstxt->model()->index(index,2).data(Qt::DisplayRole).toString();// ID

	QSqlQuery qqd(QSqlDatabase::database(sdbd));

	sqq = "select year(date) as yr, content from " + tr(s_tbl_sstext) + " where id = '" + s_id + "'";
	if(!f.checkrequest(&qqd, sqq)) return;

	qqd.first();
	s_content = qqd.value("content").toString();
	s_year = qqd.value("yr").toString();

	// Формирование ссылки на hoondok.ru
	gs_link_ss = "https://hoondok.ru/ss/" + s_year + "/" + s_id;

	ui->cmdlinksstxtrus->setToolTip(gs_link_ss);
	ui->txtsstxt->setHtml(s_content);
}

void MainWindow::on_cmdlinksstxtrus_clicked()
{
	if(gs_link_ss != ""){
	  QDesktopServices::openUrl(QUrl(gs_link_ss));
	}
}


// ****************************
// ЦИТАТЫ
// ****************************
void MainWindow::on_txtsearch_quote_returnPressed()
{
    QString s_pattern = ui->txtsearch_quote->text();
    loadfromDBbyPattern(s_pattern);
}

void MainWindow::loadfromDBbyPattern(QString pattern)
{
	func f;
    items.clear();
    QString sqq = "", s_result = "";
    int cnt = 0;
    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    QStringList tblList;
    tblList << tr(s_tbl_books) << tr(s_tbl_speeches_tfs_rus) << tr(s_tbl_speeches_tms_rus);

    for(int i = 0; i < tblList.count(); i++)
    {
        sqq =  "select name,rusname,title,year,alias,content from " + tblList.at(i) +
        " where content like '" + "%" + pattern + "%" + "' order by id";
		if(!f.checkrequest(&qqd, sqq)) return;

        while (qqd.next()) {
            item itm;
            itm.name = qqd.value("name").toString().trimmed();
            itm.rusname = qqd.value("rusname").toString().trimmed();
            itm.title = qqd.value("title").toString().trimmed();
            itm.year = qqd.value("year").toInt();
            itm.alias = qqd.value("alias").toString().trimmed();
            itm.content = qqd.value("content").toString().trimmed();
            cnt++;
            items.append(itm);
        }
    }

    s_result = quoteMaker(pattern);
    ui->txtres_quote->setText(s_result);
}

void MainWindow::on_cmdclear_clicked()
{
    ui->txtsearch_quote->clear();
    ui->txtres_quote->clear();
}


// ****************************
// КНИГИ
// ****************************
void MainWindow::loadBooknames()
{
    QString s_content = "content";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select distinct rusname from " + tr(s_tbl_books), QSqlDatabase::database(sdbd));// + " where alias='" + s_content + "' order by id"
    QTableView *table = new QTableView;
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QComboBox *wgt_cbx = ui->cbxbooks;
    wgt_cbx->setModel(model);
    wgt_cbx->setView(table);
    table->setModel(model);

    model->setHeaderData(0,Qt::Horizontal,"Название");
    table->setColumnWidth(0,600);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->setFixedWidth(600);
    table->setFont(QFont("Tahoma", 12));
    wgt_cbx->addItem("---");
}

void MainWindow::loadChapters(QString book)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select title, alias from " + tr(s_tbl_books) + " where rusname='" + book + "' order by id",
                    QSqlDatabase::database(sdbd));
    QTableView *table = new QTableView;
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QComboBox *wgt_cbx = ui->cbxchapters;
    wgt_cbx->setModel(model);
    wgt_cbx->setView(table);
    table->setModel(model);

    model->setHeaderData(0,Qt::Horizontal,"Название");
    model->setHeaderData(1,Qt::Horizontal,"Ссылка");
    table->setColumnWidth(0,600);
    table->setColumnWidth(1,0);
    table->setFixedWidth(600);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->hideColumn(1);
    table->setFont(QFont("Tahoma", 12));
	wgt_cbx->addItem("---");
}

void MainWindow::on_cbxbooks_activated(const QString &arg)
{
	gs_link_books = "";
	loadChapters(arg);
}

void MainWindow::on_cbxchapters_activated(int index)
{
	func f;
	QString s_content = "", s_name = "";
    QString s_alias = ui->cbxchapters->model()->index(index,1).data(Qt::DisplayRole).toString();
    QSqlQuery qqd(QSqlDatabase::database(sdbd));
	QString sqq = "select name, content from " + tr(s_tbl_books) + " where alias='" + s_alias + "'";
	if(!f.checkrequest(&qqd, sqq)) return;

    qqd.first();
    s_content = qqd.value("content").toString();
	s_name = qqd.value("name").toString();

	// Формирование ссылки на hoondok.ru
	gs_link_books = "https://hoondok.ru/book/" + s_name + "/" + s_alias;
	ui->cmdlinkbooks->setToolTip(gs_link_books);

    ui->txtbook->setHtml(s_content);
}

void MainWindow::on_cmdlinkbooks_clicked()
{
	if(gs_link_books != ""){
	  QDesktopServices::openUrl(QUrl(gs_link_books));
	}
}


// ****************************
// ПОИСК
// ****************************
void MainWindow::on_txtsearch_returnPressed()
{
	QString s_search = ui->txtsearch->text();
	QSqlQueryModel *model = new QSqlQueryModel(this);

    if(ui->rrus_search->isChecked())
    {
        model->setQuery("select rusname,title,alias,year,name from " + tr(s_tbl_books) +
                        " where content like '" + "%" + s_search + "%" + "'"
                        " UNION SELECT rusname,title,alias,year,name FROM " + tr(s_tbl_speeches_tfs_rus) +
                        " where content like '" + "%" + s_search + "%" + "'"
                        " UNION SELECT rusname,title,alias,year,name FROM " + tr(s_tbl_speeches_tms_rus) +
                        " where content like '" + "%" + s_search + "%" + "'",
                        QSqlDatabase::database(sdbd));
    }
    else
    {
        model->setQuery("select name,title,alias,year from " + tr(s_tbl_speeches_tfs_eng) +
                        " where content like '" + "%" + s_search + "%" + "'"
                        " UNION SELECT name,title,alias,year FROM " + tr(s_tbl_speeches_tms_eng) +
                        " where content like '" + "%" + s_search + "%" + "'",
                        QSqlDatabase::database(sdbd));
    }

	QTableView *table = new QTableView;
	table->verticalHeader()->setVisible(false);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);

	QComboBox *wgt_cbx = ui->cbxres;
	wgt_cbx->setModel(model);
	wgt_cbx->setView(table);
	table->setModel(model);

    if(ui->rrus_search->isChecked())
    {
        model->setHeaderData(0,Qt::Horizontal,"Название");
        model->setHeaderData(1,Qt::Horizontal,"Заголовок");
        model->setHeaderData(2,Qt::Horizontal,"Ссылка");
        model->setHeaderData(3,Qt::Horizontal,"Год");
        model->setHeaderData(4,Qt::Horizontal,"Name");

        table->setColumnWidth(0,400);
        table->setColumnWidth(1,600);
        table->setColumnWidth(2,100); table->hideColumn(2);
        table->setColumnWidth(3,100); table->hideColumn(3);
        table->setColumnWidth(4,100); table->hideColumn(4);

        table->verticalHeader()->setDefaultSectionSize(30);
        table->setFixedWidth(1000);
    }
    else
    {
        model->setHeaderData(0,Qt::Horizontal,"Name");
        model->setHeaderData(1,Qt::Horizontal,"Title");
        model->setHeaderData(2,Qt::Horizontal,"Alias");
        model->setHeaderData(3,Qt::Horizontal,"Year");

        table->setColumnWidth(0,100);
        table->setColumnWidth(1,800);
        table->setColumnWidth(2,100); table->hideColumn(2);
        table->setColumnWidth(3,100); table->hideColumn(3);

        table->verticalHeader()->setDefaultSectionSize(30);
        table->setFixedWidth(1000);
    }

    table->setFont(QFont("Tahoma",12));
}


void MainWindow::on_cbxres_activated(int index)
{
	func f;
    QString sqq = "", s_content = "", s_name = "", s_alias = "", s_year = "";

    if(ui->rrus_search->isChecked()){
        s_name = ui->cbxres->model()->index(index,4).data(Qt::DisplayRole).toString();
    } else if(ui->reng_search->isChecked()){
        s_name = ui->cbxres->model()->index(index,0).data(Qt::DisplayRole).toString();
    }

    s_alias = ui->cbxres->model()->index(index,2).data(Qt::DisplayRole).toString();
    s_year = ui->cbxres->model()->index(index,3).data(Qt::DisplayRole).toString();

    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    if(s_name == "tfs_rus"){
        sqq = "select name,content from " + tr(s_tbl_speeches_tfs_rus) + " where alias='" + s_alias + "'";
    } else if(s_name == "tms_rus") {
        sqq = "select name,content from " + tr(s_tbl_speeches_tms_rus) + " where alias='" + s_alias + "'";
    } else if(s_name == "tfs_eng") {
        sqq = "select name,content from " + tr(s_tbl_speeches_tfs_eng) + " where alias='" + s_alias + "'";
    } else if(s_name == "tms_eng") {
        sqq = "select name,content from " + tr(s_tbl_speeches_tms_eng) + " where alias='" + s_alias + "'";
    } else {
        sqq = "select name,content from " + tr(s_tbl_books) + " where alias='" + s_alias + "'";
    }

	if(!f.checkrequest(&qqd, sqq)) return;
    qqd.first();
    s_content = qqd.value("content").toString();

    // Формирование ссылки на hoondok.ru
    if(s_name == "tfs_rus"){
		gs_link_search = "https://hoondok.ru/tfs/rus/" + s_year + "/" + s_alias;
    } else if(s_name == "tms_rus") {
        gs_link_search = "https://hoondok.ru/tms/rus/" + s_year + "/" + s_alias;
    } else if(s_name == "tfs_eng") {
        gs_link_search = "https://hoondok.ru/tfs/eng/" + s_year + "/" + s_alias;
    } else if(s_name == "tms_eng") {
        gs_link_search = "https://hoondok.ru/tms/eng/" + s_year + "/" + s_alias;
    } else {
        gs_link_search = "https://hoondok.ru/book/" + s_name + "/" + s_alias;
    }

	ui->cmdlinksearch->setToolTip(gs_link_search);
    ui->txtres->setHtml(s_content);
}


void MainWindow::on_cmdlinksearch_clicked()
{
	if(gs_link_search != ""){
	  QDesktopServices::openUrl(QUrl(gs_link_search));
	}
}



// ********
// РЕЧИ
// ********
void MainWindow::loadSpeeches(QString year)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    if(ui->chbIO->isChecked() && ui->rrus->isChecked()){
        model->setQuery("select title, alias from " + tr(s_tbl_speeches_tfs_rus) +
                        " WHERE YEAR(date) = '" + year + "' order by id", QSqlDatabase::database(sdbd));
    } else if(ui->chbIO->isChecked() && ui->reng->isChecked()) {
        model->setQuery("select title, alias from " + tr(s_tbl_speeches_tfs_eng) +
                        " WHERE YEAR(date) = '" + year + "' order by id", QSqlDatabase::database(sdbd));
    } else if(ui->chbIM->isChecked() && ui->rrus->isChecked()){
        model->setQuery("select title, alias from " + tr(s_tbl_speeches_tms_rus) +
                        " WHERE YEAR(date) = '" + year + "' order by id", QSqlDatabase::database(sdbd));
    } else if(ui->chbIM->isChecked() && ui->reng->isChecked()){
        model->setQuery("select title, alias from " + tr(s_tbl_speeches_tms_eng) +
                        " WHERE YEAR(date) = '" + year + "' order by id", QSqlDatabase::database(sdbd));
    }

    QTableView *table = new QTableView;
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QComboBox *wgt_cbx = ui->cbxspeech;
    wgt_cbx->setModel(model);
    wgt_cbx->setView(table);
    table->setModel(model);

    model->setHeaderData(0,Qt::Horizontal,"Название");
    model->setHeaderData(1,Qt::Horizontal,"Ссылка");
    table->setColumnWidth(0,800);
    table->setColumnWidth(1,0);
    table->setFixedWidth(800);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->hideColumn(1);
    table->setFont(QFont("Tahoma", 12));
	wgt_cbx->addItem("---");
}


void MainWindow::on_cbxyears_activated(const QString &arg)
{
    loadSpeeches(arg);
}

void MainWindow::on_cbxspeech_activated(int index)
{
	func f;
    QString sqq = "", s_content = "", s_year = "";
    QString s_alias = ui->cbxspeech->model()->index(index,1).data(Qt::DisplayRole).toString();

    QSqlQuery qqd(QSqlDatabase::database(sdbd));

    if(ui->chbIO->isChecked() && ui->rrus->isChecked()){
        sqq = "select year(date) as yr, content from " + tr(s_tbl_speeches_tfs_rus) +
                " where alias='" + s_alias + "'";
    } else if(ui->chbIO->isChecked() && ui->reng->isChecked()) {
        sqq = "select year(date) as yr, content from " + tr(s_tbl_speeches_tfs_eng) +
                    " where alias='" + s_alias + "'";
    } else if(ui->chbIM->isChecked() && ui->rrus->isChecked()){
        sqq = "select year(date) as yr, content from " + tr(s_tbl_speeches_tms_rus) +
                " where alias='" + s_alias + "'";
    } else if(ui->chbIM->isChecked() && ui->reng->isChecked()){
        sqq = "select year(date) as yr, content from " + tr(s_tbl_speeches_tms_eng) +
                    " where alias='" + s_alias + "'";
    }

	if(!f.checkrequest(&qqd, sqq)) return;

    qqd.first();
    s_content = qqd.value("content").toString();
    s_year = qqd.value("yr").toString();

    // Формирование ссылки на hoondok.ru
    if(ui->chbIO->isChecked() && ui->rrus->isChecked()){
        gs_link_speech = "https://hoondok.ru/tfs/rus/" + s_year + "/" + s_alias;
    } else if(ui->chbIO->isChecked() && ui->reng->isChecked()) {
        gs_link_speech = "https://hoondok.ru/tfs/eng/" + s_year + "/" + s_alias;
    } else if(ui->chbIM->isChecked() && ui->rrus->isChecked()){
        gs_link_speech = "https://hoondok.ru/tms/rus/" + s_year + "/" + s_alias;
    } else if(ui->chbIM->isChecked() && ui->reng->isChecked()){
        gs_link_speech = "https://hoondok.ru/tms/eng/" + s_year + "/" + s_alias;
    }

    ui->cmdlinkspeech->setToolTip(gs_link_speech);
    ui->txtspeech->setHtml(s_content);
}

void MainWindow::on_cmdlinkspeech_clicked()
{
    if(gs_link_speech != ""){
      QDesktopServices::openUrl(QUrl(gs_link_speech));
    }
}




// ****************************
// ЭКСПОРТ В ФАЙЛ
// ****************************
void MainWindow::on_cmdexport_clicked()
{
    QString s_pattern = ui->txtsearch_quote->text();
	export_to_doc(s_pattern);
}



// Подсветка
void MainWindow::on_txtsearch_textChanged(const QString &arg)
{
    highlighter->setPattern(arg);
    highlighter->rehighlight();
}

void MainWindow::on_txtsearch_quote_textChanged(const QString &arg)
{
    highlighter2->setPattern(arg);
    highlighter2->rehighlight();
}





