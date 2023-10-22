#include "func.h"


func::func(){}

bool func::checkrequest(QSqlQuery *qry, QString sqq)
{
	if (!qry->exec(sqq)) {
		qDebug() << se_q_err << qry->lastError().text() << sqq;
		QMessageBox::critical(nullptr, ecap, QString::fromUtf8(s_psf14) + se_q_err + QString::fromUtf8(sbr) +
							  qry->lastError().text() + QString::fromUtf8(sbr) + sqq);
		return false;
	}

	return true;
}

bool func::checkRegExp(QRegExp rx)
{
	if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
		return true;
	} else {
		return false;
	}
}

QString func::F_StyleSheet(QString obj_name, QString obj_class)
{
    QString str = "";

    if (obj_class == "QTabWidget") {
		//Цвета набора вкладок:
        str = "QTabWidget::pane {border: 1px solid rgb(255, 0, 0); border-radius: 3px; background: rgb(255, 255, 255); font: Arial Narrow; font-size: 14px;}";
		//Цвета заголовков  вкладок:
        str += "QTabBar::tab {color: blue; background: rgb(220, 255, 255); border: 1px solid rgb(0, 0, 255); border-radius: 3px; padding: 2px; "
				"font: Arial Narrow; font-size: 14px; min-height: 20px; min-width: 50px;}";
		//Цвет  фона заголовка вкладки под указателем мыши:
        str += "QTabBar::tab:selected, QTabBar::tab:hover {color: rgb(0,0,200); background: rgb(205, 255, 255); font: Arial Narrow;"
				"font-size: 16px;}";
		//Цвет границ заголовков вкладок
        str += "QTabBar::tab:selected {color: rgb(120,40,40); border-color: rgb(0, 0, 255); font: Arial Narrow;  font-weight: bold; font-size: 14px;}";
	   //'Выпячивание' открытой вкладки:
        str += "QTabBar::tab:!selected {margin-top: 2px; font: Arial Narrow;font-size: 14px;}";
	}

    if (obj_class == "QPlainTextEdit") {
        str = "QPlainTextEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
					"font: Arial Narrow; font-size: 12px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
	}

    if (obj_class == "QTextEdit") {
        str = "QTextEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
					"font: Arial Narrow; font-size: 14px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
	}

    if (obj_class == "QGroupBox") {
        str = "QGroupBox{color: rgb(0,0,100); background-color: rgb(200,220,255); "
					"font: Arial Narrow; font-size: 14px; border: 2px solid rgb(150,50,50); border-radius: 6px;}";
	}

    if (obj_class == "QComboBox") {
        str = "QComboBox{color: rgb(0,0,100); background-color: rgb(230,250,250); "
					"font: Arial Narrow; font-size: 14px;  border:transparent;}";
	}

    if(obj_class == "QTableWidget"){
        str = "QTableWidget {background-color: rgb(255,255,255);}";
        str += "QHeaderView{background: rgb(0,0,255); color: rgb(255,255,255);}";
        str += "QHeaderView::section{background-color: rgb(0,0,255);"
               "font-weight: bold;font: bold 14px;font-family: Tahoma;}";
    }

    if (obj_class == "QLineEdit") {
        str = "QLineEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
					"font: Arial Narrow; font-size: 14px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
        if (obj_name.left(6) == "LE_Btn" || obj_name == "") {
            str="QLineEdit{color: rgb(0,0,100); background-color: transparent; "
					"font: Arial Narrow; font-size: 14px; font-weight: bold; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
		}
	}

    if (obj_class == "QLabel") {
        str = "QLabel {color: rgb(0,0,100); font: Arial Narrow; font-size: 15px; "
              "background-color: transparent; border: none;}";
	}

    if (obj_class == "QToolButton") {

        str = "QToolButton {color: white; font: Arial Narrow; font-size: 18px; border: 1px solid rgb(150,50,50); border-radius: 5px;"
                        "background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1 rgba(0, 50, 200, 255), stop:0.1 rgba(0, 50,200, 90));}"
                        "QToolButton:disabled {color: #707070; background-color: #f0f0f0;}";
	}

    if ( obj_class == "QPushButton") {
            str = "QPushButton {color: white; padding-left: 10px; padding-right: 10px; font: Arial Narrow; font-size: 18px; border: 1px solid rgb(150,50,50); border-radius: 5px;"
					"background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1 rgba(0, 50, 200, 255), stop:0.1 rgba(0, 50,200, 90));}"
					"QPushButton:disabled {color: #707070; background-color: #f0f0f0;}";
	}

    if (obj_class == "Header") {
        str = "QHeaderView::section {font: Arial Narrow; font-size: 12px; font-weight: Light; background-color: rgb(230,230,230);"
				"border: 1px solid blue;}";
	}

    if ( obj_class == "QRadioButton") {
        str = "QRadioButton{font: Arial Narrow; font-size: 16px;}";
        str += "QRadioButton::indicator {width:16px; height:16px;}";
        str += "QRadioButton::indicator:checked {image: url(:/images/radiobutton_03_32_checked2.png);}";
        str += "QRadioButton::indicator:unchecked {image: url(:/images/radiobutton_03_32_unchecked.png);}";
        str += "QRadioButton::indicator:checked:disabled {image: url(:/images/radiobutton_03_32_checked2_disabled.png)}";
	}

    if ( obj_class == "QCheckBox") {
        str = "QCheckBox{font: Arial Narrow; font-size: 16px;}";
        str += "QCheckBox::indicator {width:16px; height:16px;}";
        str += "QCheckBox::indicator:checked {image: url(:/images/checkbox-05-32-checked.png);}";
        str += "QCheckBox::indicator:unchecked {image: url(:/images/checkbox-05-32-unchecked.png);}";
	}

    return str;
}

QString func::convertMonthes(QString month)
{
    QString res = "";
    if(month == "---"){res = "";}
    else if(month == "Январь"){res = "01";}
    else if(month == "Февраль"){res = "02";}
    else if(month == "Март"){res = "03";}
    else if(month == "Апрель"){res = "04";}
    else if(month == "Май"){res = "05";}
    else if(month == "Июнь"){res = "06";}
    else if(month == "Июль"){res = "07";}
    else if(month == "Август"){res = "08";}
    else if(month == "Сентябрь"){res = "09";}
    else if(month == "Октябрь"){res = "10";}
    else if(month == "Ноябрь"){res = "11";}
    else if(month == "Декабрь"){res = "12";}
    return res;
}








