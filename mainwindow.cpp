#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->export_progress->hide();
    ui->search_line->hide();

    ui->btn_question_add_fill->setEnabled(false);
    ui->btn_question_add_vidpov->setEnabled(false);

    ui->recent_label->hide();

    conf = new config();

    questions = new Questions();

    // встановлення налашутвань для експорту
    questions->setExportOption(conf->questionSymbol, conf->answerSymbol);

    // налаштування останніх файлів
    recentFiles();

    // шлях до програми
    path = QDir::currentPath();

    createTempDir(path);

    connect(ui->listWidget->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotRecent(QModelIndex)));
}

void MainWindow::recentFiles(){
    ui->recent1->hide();
    ui->recent2->hide();
    ui->recent3->hide();
    ui->recent4->hide();
    ui->recent5->hide();
    ui->recent6->hide();
    ui->recent7->hide();

    if (conf->recentFiles.size() == 0)
        ui->recent_label->show();

    qDebug() << conf->recentFiles.size();

    for(int i = 0; i < conf->recentFiles.size(); i++)
    {
        QFileInfo inf(conf->recentFiles.at(i));
        QString data = inf.birthTime().toString("dd.MM.yyyy");

        if (data != "")
        {
            switch (i) {
            case 0:
                ui->recent1->setText(inf.fileName() + " (" + data  + ")\n" + conf->recentFiles.at(i));
                ui->recent1->show();
                break;
            case 1:
                ui->recent2->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent2->show();
                break;
            case 2:
                ui->recent3->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent3->show();
                break;
            case 3:
                ui->recent4->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent4->show();
                break;
            case 4:
                ui->recent5->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent5->show();
                break;
            case 5:
                ui->recent6->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent6->show();
                break;
            case 6:
                ui->recent7->setText(inf.fileName() + " (" + data + ")\n" + conf->recentFiles.at(i));
                ui->recent7->show();
                break;
            default:
                break;
            }
        }
    }
}

// зміна вибраного запитання
void MainWindow::slotRecent(QModelIndex index)
{
    TempImg.clear(); // назви тисчасових зображень
    
    // очищає зображення запитання
    ui->question_img->clear();

    // очищає лейаут від вісіх елементів
    int zx = ui->question_layout->count();
    for (int i = 0; i < zx; i++)
    {
        int zz =  ui->question_layout->itemAt(0)->layout()->count();
        //        qDeleteAll(ui->question_layout->itemAt(0)->layout()->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly));
        for (int j = 0; j < zz;j++)
        {
            QLayoutItem *item = ui->question_layout->itemAt(0)->layout()->itemAt(j);
            ui->question_layout->itemAt(0)->layout()->itemAt(j)->widget()->hide();
            delete item;
        }
        delete ui->question_layout->itemAt(0)->layout();
    }
    
    // додає існуючі елементи


    question q = questions->getQuestion(ui->listWidget->model()->data(ui->listWidget->model()->index(index.row(), 0)).toString());
    //qDebug() << "11" << ui->listWidget->model()->data(ui->listWidget->model()->index(index.row(), 0)).toString();
    auto hb = new QHBoxLayout(); // горизонтальний бокс

    // якшо в запитанні картинка
    //qDebug () << q.ques;
    if (q.ques.contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
    {
        TempImg.push_back(q.ques.mid(q.ques.indexOf("/media/") + 7,
                                     q.ques.indexOf("alt") - (q.ques.indexOf("/media/") + 9)));

        ui->question_img->setPixmap(path + "/temp/media/" + TempImg[0]);

        // текст без картинки
        ui->line_question_edit->setText(q.ques.remove(q.ques.indexOf(QRegExp("<img style"), 0), q.ques.indexOf(QRegExp(" />"), 0) + 2));
    } else
    {
        TempImg.push_back("");
        ui->line_question_edit->setText(q.ques);
    }

    switch (q.type)
    {
    case 0 : {  // з 1 прав відповіддю
        for (int i = 0; i < q.answers.size(); i++)
        {
            hb = new QHBoxLayout();

            // з картинокою / без
            if (q.answers.at(i).contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
            {
                // тимчасова картинка
                TempImg.push_back(q.answers.at(i).mid(q.answers.at(i).indexOf("/media/") + 7,
                                                      q.answers.at(i).indexOf("alt") - (q.answers.at(i).indexOf("/media/") + 9)));

                // текст без картинки
                QString str = QString(q.answers.at(i)).remove(q.answers.at(i).indexOf(QRegExp("<img style"), 0), q.answers.at(i).indexOf(QRegExp(" />"), 0) + 2).mid(1);


                QRadioButton * rb = new QRadioButton();
                if (q.answers.at(i)[0] == '+')
                    rb->setChecked(true);

                // jbj name - для того шоб потім знайти їх
                rb->setObjectName("rb"+ ui->listWidget->currentItem()->text() + QString(i));
                hb->addWidget(rb);

                // якшо питання і з текстом і картинкою
                if (str != "")
                {
                    QLineEdit * le = new QLineEdit(str);
                    le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(i));
                    hb->addWidget(le);
                }

                // картинка
                QLabel *l = new QLabel ();
                QPixmap *p = new QPixmap ();
                p->load(path + "/temp/media/" + TempImg[i + 1]);
                l->setPixmap(*p);
                l->setScaledContents(false);

                hb->addWidget(l);
            } else
            {
                TempImg.push_back("");  // нема картинки

                QRadioButton * rb = new QRadioButton();
                if (q.answers.at(i)[0] == '+')
                    rb->setChecked(true);
                rb->setObjectName("rb"+ ui->listWidget->currentItem()->text() + QString(i));
                QLineEdit * le = new QLineEdit(q.answers.at(i).mid(1, q.answers.at(i).length() - 1));
                le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(i));
                hb->addWidget(rb);
                hb->addWidget(le);
            }

            // додає всі елементи створені елементи
            ui->question_layout->addLayout(hb);
        }

        break;
    }
    case 1: {
        for (int i = 0; i < q.answers.size(); i++)
        {
            hb = new QHBoxLayout();
            //            QLabel *l = new QLabel ("heel");
            //            auto cb = new QRadioButton(q.answers.at(i));
            //            qDebug() << "<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/.*  alt\\=\"image\" />";
            //            qDebug() << q.answers.at(i).indexOf(QRegExp("<img style"), 0);
            //            qDebug() << q.answers.at(i).indexOf(QRegExp(" />"), 0);


            if (q.answers.at(i).contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
            {
                // тимчасова картинка
                TempImg.push_back(q.answers.at(i).mid(q.answers.at(i).indexOf("/media/") + 7,
                                                      q.answers.at(i).indexOf("alt") - (q.answers.at(i).indexOf("/media/") + 9)));

                // текст без картинки
                QString str = QString(q.answers.at(i)).remove(q.answers.at(i).indexOf(QRegExp("<img style"), 0), q.answers.at(i).indexOf(QRegExp(" />"), 0) + 2).mid(1);


                QCheckBox * cb = new QCheckBox();
                if (q.answers.at(i)[0] == '+')
                    cb->setChecked(true);

                // jbj name - для того шоб потім знайти їх
                cb->setObjectName("cb"+ ui->listWidget->currentItem()->text() + QString(i));
                hb->addWidget(cb);

                // якшо питання і з текстом і картинкою
                if (str != "")
                {
                    QLineEdit * le = new QLineEdit(str);
                    le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(i));
                    hb->addWidget(le);
                }

                // картинка
                QLabel *l = new QLabel ();
                QPixmap *p = new QPixmap ();
                p->load(path + "/temp/media/" + TempImg[i + 1]);
                l->setPixmap(*p);
                l->setScaledContents(false);

                hb->addWidget(l);
            } else {
                TempImg.push_back("");  // нема картинки

                QCheckBox * cb = new QCheckBox();
                if (q.answers.at(i)[0] == '+')
                    cb->setChecked(true);
                cb->setObjectName("cb"+ ui->listWidget->currentItem()->text() + QString(i));
                QLineEdit * le = new QLineEdit(q.answers.at(i).mid(1, q.answers.at(i).length() - 1));
                le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(i));
                hb->addWidget(cb);
                hb->addWidget(le);
            }
            ui->question_layout->addLayout(hb);
        }
        break;
    }
        // інші типи питань

    }
}

MainWindow::~MainWindow()
{
    conf->updateConfig();

    deleteTempDir(path + "/temp");
    delete ui;
}

// на 1 стор створити новий
void MainWindow::on_btn_create_new_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// додати запитання з 1 прав відповіддю
void MainWindow::on_btn_question_add_one_clicked()
{
    QString name = QString::number(ui->listWidget->count());

    // проста перевірка чи питання існує
    if (questions->getQuestion(QString::number(ui->listWidget->count())).ques != "")
        for (int i = ui->listWidget->count() + 1; true; i ++)
            if (questions->getQuestion(QString::number(i)).ques == "")
            { name = QString::number(i); break; }


    QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/one_ans.png"), name);

    questions->addQuestion(name,* new question(0));

    ui->listWidget->addItem(item);
}

// даодати запитання з бащатьма правльними відповіддями
void MainWindow::on_btn_question_add_multi_clicked()
{
    QString name = QString::number(ui->listWidget->count());

    // проста перевірка чи питання існує
    if (questions->getQuestion(QString::number(ui->listWidget->count())).ques != "")
        for (int i = ui->listWidget->count(); true; i ++)
            if (questions->getQuestion(QString::number(i)).ques == "")
            { name = QString::number(i); break; }

    QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/multi_ans.png"), name);
    
    questions->addQuestion(name,* new question(1));

    ui->listWidget->addItem(item);
}

// додати питання - заповнити бланк
void MainWindow::on_btn_question_add_fill_clicked()
{
    QString name = QString::number(ui->listWidget->count());

    // проста перевірка чи питання існує
    if (questions->getQuestion(QString::number(ui->listWidget->count())).ques != "")
        for (int i = ui->listWidget->count(); true; i ++)
            if (questions->getQuestion(QString::number(i)).ques == "")
            { name = QString::number(i); break; }

    QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/fillblank.png"), name);
    
    questions->addQuestion(name, * new question(2));

    ui->listWidget->addItem(item);
}

// додати питання відповідність
void MainWindow::on_btn_question_add_vidpov_clicked()
{
    QString name = QString::number(ui->listWidget->count());

    // проста перевірка чи питання існує
    if (questions->getQuestion(QString::number(ui->listWidget->count())).ques != "")
        for (int i = ui->listWidget->count(); true; i ++)
            if (questions->getQuestion(QString::number(i)).ques == "")
            { name = QString::number(i); break; }

    QListWidgetItem *item = new QListWidgetItem(QIcon(":/img/vidpov.png"), name);
    
    questions->addQuestion(name, * new question(3));

    ui->listWidget->addItem(item);
}

// кнопка видалити питання
void MainWindow::on_btn_del_question_clicked()
{
    if (!ui->listWidget->selectedItems().empty()) {
        questions->map.remove(ui->listWidget->currentItem()->text());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    } else QMessageBox::information(0, "Info", "Не вибрано запитання");
}

// режим перегляду
void MainWindow::on_btn_view_mode_clicked()
{
    ui->mode->setCurrentIndex(1);
    ui->btn_view_mode->setStyleSheet("background: #7D87AF;");
    ui->btn_edite_mode->setStyleSheet("QPushButton { background: rgb(240, 240, 240); border: none; } :hover {background: #7D87AF; }");
    ui->btn_del_question->setEnabled(false);
    ui->btn_export->setEnabled(false);

    ui->btn_add_answer_p->setEnabled(false);
    ui->btn_add_answer_tp->setEnabled(false);
    ui->btn_add_answer_t->setEnabled(false);
    ui->btn_add_question_img->setEnabled(false);
    ui->btn_del_answer->setEnabled(false);


    int zx = ui->question_view_layout->count();
    for (int i = 0; i < zx;)
    {
        int zz =  ui->question_view_layout->itemAt(0)->layout()->count();
        for (int j = 0; j < zz;j++)
        {
            QLayoutItem *item = ui->question_view_layout->itemAt(0)->layout()->itemAt(j);
            ui->question_view_layout->itemAt(0)->layout()->itemAt(j)->widget()->hide();
            delete item;
        }
        delete ui->question_view_layout->itemAt(0)->layout();
        i++;
    }

    foreach (QString key, questions->map.keys())
    {
        question q = questions->map.value(key);

        auto hb = new QHBoxLayout(); // горизонтальний бокс

        // якшо питання містить картинку
        if (q.ques.contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
        {
            QString img = q.ques.mid(q.ques.indexOf("/media/") + 7,
                                     q.ques.indexOf("alt") - (q.ques.indexOf("/media/") + 9));

            QString str = q.ques.remove(q.ques.indexOf(QRegExp("<img style"), 0), q.ques.indexOf(QRegExp(" />"), 0) + 3);

            // номер і запитання більшимм жирним шрифтом + синій фон
            QLabel *lab = new QLabel ( key + ": " +  str);
            lab->setStyleSheet("background: #7D87AF;");
            QFont f( "Segoe UI", 11, QFont::Bold);
            lab->setFont(f);
            hb->addWidget(lab);

            // додаєм текст
            ui->question_view_layout->addLayout(hb);

            // очищаєм від попередніх значень
            hb = new QHBoxLayout();

            // картинка
            QLabel *l = new QLabel ();
            QPixmap *p = new QPixmap ();
            p->load(path + "/temp/media/" + img);
            l->setPixmap(*p);
            l->setScaledContents(false);

            hb->addWidget(l);
            ui->question_view_layout->addLayout(hb);
        }else
        {
            QLabel *lab = new QLabel (key + ": " + q.ques);
            lab->setStyleSheet("background: #7D87AF;");
            QFont f( "Segoe UI", 9, QFont::Bold);
            lab->setFont(f);
            hb->addWidget(lab);
            ui->question_view_layout->addLayout(hb);
        }

        switch (q.type)
        {
        case 0 : {
            for (int i = 0; i < q.answers.size(); i++)
            {
                hb = new QHBoxLayout();
                //                qDebug() << "<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/.*  alt\\=\"image\" />";
                //                qDebug() << q.answers.at(i).indexOf(QRegExp("<img style"), 0);
                //                qDebug() << q.answers.at(i).indexOf(QRegExp(" />"), 0);


                if (q.answers.at(i).contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
                {
                    QString img = q.answers.at(i).mid(q.answers.at(i).indexOf("/media/") + 7,
                                                      q.answers.at(i).indexOf("alt") - (q.answers.at(i).indexOf("/media/") + 9));

                    QString str = QString(q.answers.at(i)).remove(q.answers.at(i).indexOf(QRegExp("<img style"), 0), q.answers.at(i).indexOf(QRegExp(" />"), 0) + 2).mid(1);

                    QRadioButton *rb = new QRadioButton(str);
                    if (q.answers.at(i)[0] == '+')
                        rb->setChecked(true);
                    hb->addWidget(rb);

                    QLabel *l = new QLabel ();
                    QPixmap *p = new QPixmap ();
                    p->load(path + "/temp/media/" + img);
                    l->setPixmap(*p);
                    l->setScaledContents(false);

                    hb->addWidget(l);
                } else
                {
                    QRadioButton *rb = new QRadioButton(QString(q.answers.at(i)).mid(1,q.answers.at(i).length() - 1));
                    if (q.answers.at(i)[0] == '+')
                        rb->setChecked(true);
                    hb->addWidget(rb);
                }
                ui->question_view_layout->addLayout(hb);
            }
            break;
        }
        case 1: {
            for (int i = 0; i < q.answers.size(); i++)
            {
                hb = new QHBoxLayout();

                if (q.answers.at(i).contains("<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/"))
                {
                    QString img = q.answers.at(i).mid(q.answers.at(i).indexOf("/media/") + 7,
                                                      q.answers.at(i).indexOf("alt") - (q.answers.at(i).indexOf("/media/") + 9));

                    QString str = QString(q.answers.at(i)).remove(q.answers.at(i).indexOf(QRegExp("<img style"), 0), q.answers.at(i).indexOf(QRegExp(" />"), 0) + 2).mid(1);

                    QCheckBox *cb = new QCheckBox(str);
                    if (q.answers.at(i)[0] == '+')
                        cb->setChecked(true);

                    hb->addWidget(cb);


                    QLabel *l = new QLabel ();
                    QPixmap *p = new QPixmap ();
                    p->load(path + "/temp/media/" + img);
                    l->setPixmap(*p);
                    l->setScaledContents(false);

                    hb->addWidget(l);
                } else
                {
                    QCheckBox *cb = new QCheckBox(q.answers.at(i).mid(1, q.answers.at(i).length() - 1));
                    if (q.answers.at(i)[0] == '+')
                        cb->setChecked(true);
                    hb->addWidget(cb);
                }
                ui->question_view_layout->addLayout(hb);
            }

            break;
        }
            // інші типи запитань
        }
    }
}

// режим редагування
void MainWindow::on_btn_edite_mode_clicked()
{
    ui->mode->setCurrentIndex(0);
    ui->btn_view_mode->setStyleSheet("QPushButton { background: rgb(240, 240, 240); border: none; } :hover {background: #7D87AF; }");
    ui->btn_edite_mode->setStyleSheet("background: #7D87AF;");
    ui->btn_del_question->setEnabled(true);
    ui->btn_export->setEnabled(true);

    ui->btn_add_answer_p->setEnabled(true);
    ui->btn_add_answer_tp->setEnabled(true);
    ui->btn_add_answer_t->setEnabled(true);
    ui->btn_add_question_img->setEnabled(true);
    ui->btn_del_answer->setEnabled(true);
}

// кнопка експорту
void MainWindow::on_btn_export_clicked()
{
//    ui->export_progress->show();

    QString filename = QFileDialog::getOpenFileName(0, "Open docx", conf->openExportPath, "*.docx");

    if (filename != "")
    {
        word *w = new word();

        // Обрізка шляху. без файлу - тільки назва папки
        int pos = filename.lastIndexOf(QChar('/'));
        conf->openExportPath = filename.left(pos);

        questions->setQuestionFromXml(w->readDocx(filename));

        //    qDebug() << q->getQuestion("1").answers.at(2);
        //    qDebug() << q->getQuestion("1").type;
        //        qDebug() << q->getQuestion("2").type;

        ui->listWidget->clear();

        foreach (QString key, questions->map.keys())
        {

            questions->map.value(key);

            QListWidgetItem *item;
            switch (questions->map[key].type) {
            case 0: item = new QListWidgetItem(QIcon(":/img/one_ans.png"), key);break;
            case 1: item = new QListWidgetItem(QIcon(":/img/multi_ans.png"), key);break;
            case 2: item = new QListWidgetItem(QIcon(":/img/fillblank.png"), key);break;
            case 3: item = new QListWidgetItem(QIcon(":/img/vidpov.png"), key);break;
            }
            ui->listWidget->addItem(item);
        }

        //        for (int i = 0; i <= 100; i ++)
        //        {
        //            QThread::msleep(20);
        //            ui->export_progress->setValue(i);
        //        }

        // видалення тимчасових файлів
        deleteTempDir(path + "/tmp");

        ui->export_progress->hide();
    } else QMessageBox::information(0, "Info", "Не вибрано файл");
}

// стерти всі запитання
void MainWindow::on_btn_clear_questions_clicked()
{
    for (int i = ui->listWidget->count(); i >= 0; i --)
    {
        delete ui->listWidget->takeItem(ui->listWidget->row( ui->listWidget->item(i)));
    }
    questions->map.clear();

    QDir dir(path + "/temp/media");

    // видалення всіх забражень
    QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for(int file = 0; file < files.count(); file++)
    {
        dir.remove(files.at(file).fileName());
    }
}

// додати запитання - тільки текст
void MainWindow::on_btn_add_answer_t_clicked()
{
    if (!ui->listWidget->selectedItems().empty())
    {
        question q = questions->getQuestion(ui->listWidget->currentItem()->text());

        TempImg.push_back("");

        auto hb = new QHBoxLayout(); // горизонтальний бокс

        switch (q.type)
        {
        case 0 : {
            QRadioButton * rb = new QRadioButton();
            rb->setObjectName("rb"+ ui->listWidget->currentItem()->text() + QString(ui->question_layout->count()));
            QLineEdit * le = new QLineEdit();
            le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(ui->question_layout->count()));
            hb->addWidget(rb);
            hb->addWidget(le);
            break;
        }
        case 1: {
            QCheckBox * cb = new QCheckBox();
            cb->setObjectName("cb"+ ui->listWidget->currentItem()->text() + QString(ui->question_layout->count()));
            QLineEdit * le = new QLineEdit();
            le->setObjectName("le"+ ui->listWidget->currentItem()->text() + QString(ui->question_layout->count()));
            hb->addWidget(cb);
            hb->addWidget(le);

            break;
        }
            // інші типи запитань
        }
        ui->question_layout->addLayout(hb);
    } else qDebug() << "Not selected question";
}

// додати запитання - текс + картинка
void MainWindow::on_btn_add_answer_tp_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open img"), conf->openImgPath, tr("Data files (*.png *.jpg *.bmp)"));

    if (fileName == "")
        QMessageBox::information(0, "Info", "Не вибрано зображення");
    else {
        question q = questions->getQuestion(ui->listWidget->currentItem()->text());

        // Обрізка шляху. без файлу - тільки назва папки
        int pos = fileName.lastIndexOf(QChar('/'));
        conf->openImgPath = fileName.left(pos);

        q.answers.push_back("");

        QString name = QFileInfo(fileName).fileName();

        TempImg.push_back(name);
        QFile::copy(fileName, path + "/temp/media/" + name);


        auto hb = new QHBoxLayout(); // горизонтальний бокс

        switch (q.type)
        {
        case 0 : {
            hb->addWidget(new QRadioButton());
            hb->addWidget(new QLineEdit());
            break;
        }
        case 1: {
            hb->addWidget(new QCheckBox());
            hb->addWidget(new QLineEdit());
            break;
        }
        }

        QPixmap img(fileName); // <- path to image file
        QLabel *i = new QLabel();
        i->setPixmap(img);
        i->setScaledContents(false);
        hb->addWidget(i);

        ui->question_layout->addLayout(hb);
    }
}

// додати запитання - тільки картинка
void MainWindow::on_btn_add_answer_p_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open img"), conf->openImgPath, tr("Data files (*.png *.jpg *.bmp)"));

    if (fileName == "")
        QMessageBox::information(0, "Info", "Не вибрано зображення");
    else {
        // Обрізка шляху. без файлу - тільки назва папки
        int pos = fileName.lastIndexOf(QChar('/'));
        conf->openImgPath = fileName.left(pos);

        QString name = QFileInfo(fileName).fileName();

        TempImg.push_back(name);
        QFile::copy(fileName, path + "/temp/media/" + name);

        QPixmap img(fileName); // <- path to image file
        QLabel *i = new QLabel();
        i->setPixmap(img);
        i->setScaledContents(false);

        question q = questions->getQuestion(ui->listWidget->currentItem()->text());

        auto hb = new QHBoxLayout(); // горизонтальний бокс

        switch (q.type)
        {
        case 0 : {
            hb->addWidget(new QRadioButton());

            hb->addWidget(i);
            break;
        }
        case 1: {
            hb->addWidget(new QCheckBox());

            hb->addWidget(i);
            break;
        }
        }

        ui->question_layout->addLayout(hb);
    }
}

// кнопка додати зображення до запитання
void MainWindow::on_btn_add_question_img_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open img"), conf->openImgPath, tr("Data files (*.png *.jpg *.bmp)"));

    if (fileName == "")
         QMessageBox::information(0, "Info", "Не вибрано зображення");
    else {
        // Обрізка шляху. без файлу - тільки назва папки
        int pos = fileName.lastIndexOf(QChar('/'));
        conf->openImgPath = fileName.left(pos);

        QString name = QFileInfo(fileName).fileName();

        TempImg[0] = name;
        QFile::copy(fileName, path + "/temp/media/" + name);

        QPixmap img(fileName); // <- path to image file
        ui->question_img->setPixmap(img);
        ui->question_img->setScaledContents(false);
    }
}

// кнопка видалити останню відповідь
void MainWindow::on_btn_del_answer_clicked()
{

    if (ui->question_layout->count() != 0) {
        int zz = ui->question_layout->itemAt(ui->question_layout->count() - 1)->layout()->count();

        for (int i = 0; i < zz; i++)
        {
            QLayoutItem *item = ui->question_layout->itemAt(ui->question_layout->count() - 1)->layout()->itemAt(i);
            ui->question_layout->itemAt(ui->question_layout->count() - 1)->layout()->itemAt(i)->widget()->hide();
            delete item;
        }
        delete ui->question_layout->itemAt(ui->question_layout->count() - 1)->layout();
    };

    QFile df (path + "/temp/media/" + TempImg.last());
    df.remove();

    TempImg.removeLast();
}

// кнопка зберегти запитання
void MainWindow::on_btn_save_question_clicked()
{
    if (!ui->listWidget->selectedItems().empty()) {
        question *q = new question(questions->getQuestion(ui->listWidget->currentItem()->text()).type);

        q->clear();

        if (TempImg[0] != "")
        {
            q->ques = ui->line_question_edit->toPlainText() +
                    "<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/media/" + TempImg[0] + "\" alt\\=\"image\" />";
        } else
            q->ques = ui->line_question_edit->toPlainText();

        int zx = ui->question_layout->count();
        for (int i = 0; i < zx; i ++)
        {
            QString img = "";
            if (TempImg[i + 1] != "")
                img = "<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/media/" + TempImg[i + 1] + "\" alt\\=\"image\" />";

            switch (q->type)
            {
            case 0: {
                QRadioButton *rb = qobject_cast<QRadioButton*> (ui->question_layout->itemAt(i)->layout()->itemAt(0)->widget());
                QLineEdit *le = qobject_cast<QLineEdit*> (ui->question_layout->itemAt(i)->layout()->itemAt(1)->widget());


                //            QRadioButton *rb = findChild<QRadioButton*> ("rb" + ui->listWidget->currentItem()->text() + QString(i));
                //            QLineEdit *le = findChild<QLineEdit*> ("le" + ui->listWidget->currentItem()->text() + QString(i));
                if (le != nullptr)
                {
                    if (rb->isChecked())
                    {
                        q->answers.push_back("+" + le->text() + img);
                        q->ra ++;
                    }
                    else {
                        q->wa ++;
                        q->answers.push_back("-" + le->text() + img);
                    }
                } else rb->isChecked() ? q->answers.push_back("+" + img) : q->answers.push_back("-" + img);
                break;
            }
            case 1: {
                QCheckBox *cb = qobject_cast<QCheckBox*> (ui->question_layout->itemAt(i)->layout()->itemAt(0)->widget());
                QLineEdit *le = qobject_cast<QLineEdit*> (ui->question_layout->itemAt(i)->layout()->itemAt(1)->widget());


                if (le != nullptr)
                {
                    if (cb->isChecked())
                    {
                        q->answers.push_back("+" + le->text() + img);
                        q->ra ++;
                    }
                    else {
                        q->wa ++;
                        q->answers.push_back("-" + le->text() + img);
                    }
                } else cb->isChecked() ? q->answers.push_back("+" + img) : q->answers.push_back("-" + img);
                break;
            }
            case 2: {

            }
            case 3: {

            }
            default: qDebug () << "error";
            }
        }

        //qDebug() << ui->listWidget->currentItem()->text() << " - " << q->answers.at(0);
        //qDebug () << q->answers.size();
        questions->setQuestion(ui->listWidget->currentItem()->text(), *q);
        //    qDebug() << ui->listWidget->currentItem()->text();
        //        qDebug () << questions->getQuestion(ui->listWidget->currentItem()->text()).answers.at(0);
        //    ui->btn_save_question->setText("Збережено");
    } else  QMessageBox::information(0, "Info", "Не вибрано запитання");
}

// створює тисчасову папку для запитань
void MainWindow::createTempDir(QString _path)
{
    QDir *dir = new QDir();

    dir->mkpath(_path + "/temp/media");
}

// видаляє тимчсові папки
void MainWindow::deleteTempDir(QString dir_path)
{
    QDir dir(dir_path);

    //First delete any files in the current directory
    QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for(int file = 0; file < files.count(); file++)
    {
        dir.remove(files.at(file).fileName());
    }

    //Now recursively delete any child directories
    QFileInfoList dirs = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for(int dir = 0; dir < dirs.count(); dir++)
    {
        this->deleteTempDir(dirs.at(dir).absoluteFilePath());
    }

    //Finally, remove empty parent directory
    dir.rmdir(dir.path());
}

// пошук при змінені тексту в полі
void MainWindow::on_search_line_textChanged(const QString &arg1)
{
    if (ui->mode->currentIndex() == 1)
    {
        QList <QLabel*> l = findChildren <QLabel*> ();
        \
        foreach (QLabel *lb, l)
        {
            if (lb->objectName() != "label_4" && lb->objectName() != "question_img")
            {
                //qDebug() << lb->text();
                if (lb->text().contains(arg1))
                {
                    lb->setStyleSheet("QLabel {background: #356F35; color: white;}");
                } else
                {
                    lb->setStyleSheet("QLabel {background: #7D87AF;; color: balack;}");
                }
            }
        }
    }
    else {
        QListWidgetItem* item;

        bool found = false;

        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            item = ui->listWidget->item(i);

            question q = questions->getQuestion(item->text());

            if (q.ques.contains(arg1) || item->text() == arg1)
            {
                if (!found)
                {
                    found = true;
                    ui->listWidget->setCurrentRow(i);
                }

                item->setBackground(QColor("#356F35"));
                item->setTextColor("white");
            } else{
                item->setTextColor("black");
                item->setBackground(QColor(240, 240, 240));
            }
        }

    }
}

// натиснуто на кнопку пошуку
void MainWindow::on_btn_search_clicked()
{
    // якшо вже здійснюється пошук
    if (ui->search_line->isHidden())
        ui->search_line->show();
    else
    {
        // очищення результатів пошуку
        // режим перегляду (стор 1)
        if (ui->mode->currentIndex() == 1)
        {
            QList <QLabel*> l = findChildren <QLabel*> ();
            foreach (QLabel *lb, l)
            {
                if (lb->objectName() != "label_4" && lb->objectName() != "question_img")
                {
                    lb->setStyleSheet("QLabel {background: #7D87AF;; color: balack;}");
                }
            }
        } else { // режим редагування стор 0
            QListWidgetItem* item;

            for(int i = 0; i < ui->listWidget->count(); ++i)
            {
                item = ui->listWidget->item(i);
                item->setTextColor("black");
                item->setBackground(QColor(240, 240, 240));
            }
        }
        ui->search_line->hide();
    }
}

// зберегти налашування експорту
void MainWindow::on_btn_save_exp_config_clicked()
{
    if (ui->in_symbol_q->text() != ui->in_symbol_at->text())
    {
        conf->questionSymbol = ui->in_symbol_q->text().toStdString()[0];
        conf->answerSymbol = ui->in_symbol_at->text().toStdString()[0];

        // встановлення нових настройок
        questions->setExportOption(conf->questionSymbol, conf->answerSymbol);

        ui->stackedWidget->setCurrentIndex(1);
    }else  QMessageBox::information(0, "Info", "Символи для запитань та відповідей не можуть бути однаковими");
}

// при зміні тексту в одному полі змінювати в другому (настройки експорту)
void MainWindow::on_in_symbol_at_textChanged(const QString &arg1)
{
    if (arg1 != ui->in_symbol_q->text())
        ui->in_symbol_af->setText(arg1);
    else {
        ui->in_symbol_at->setText("");
    }
}

// при зміні тексту в одному полі змінювати в другому (настройки експорту)
void MainWindow::on_in_symbol_af_textChanged(const QString &arg1)
{
    if (arg1 != ui->in_symbol_q->text())
        ui->in_symbol_at->setText(arg1);
    else {
        ui->in_symbol_af->setText("");
    }
}

// експорт із налаштувань експорту
void MainWindow::on_btn_exp_clicked()
{
    if (ui->in_symbol_q->text() != ui->in_symbol_at->text())
    {
        conf->questionSymbol = ui->in_symbol_q->text().toStdString()[0];
        conf->answerSymbol = ui->in_symbol_at->text().toStdString()[0];

        // встановлення нових настройок
        questions->setExportOption(conf->questionSymbol, conf->answerSymbol);

        // програмний клік по кнопці в
        ui->btn_export->click();
        ui->stackedWidget->setCurrentIndex(1);

        ui->stackedWidget->setCurrentIndex(1);
    }else  QMessageBox::information(0, "Info", "Символи для запитань та відповідей не можуть бути однаковими");
}

// кнопка експортування при настройках
void MainWindow::on_btn_export_setting_clicked()
{
    ui->in_symbol_q->setText(QString(conf->questionSymbol));
    ui->in_symbol_at->setText(QString(conf->answerSymbol));
    ui->stackedWidget->setCurrentIndex(2);
}

// ЗБЕРЕЖЕННЯ кнопка зберегти
void MainWindow::on_btn_save_clicked()
{
    QFile file(path + "/temp/gift.txt");

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString formated_str = questions->giftQuestion();

        QTextStream stream(&file);

        stream.setCodec("UTF8");    // шоб можна було і укр символи

        stream << formated_str;

        file.close();
        qDebug() << "Writing finished";

        QString fname = QFileDialog::getSaveFileName(nullptr, "Save file", conf->savePath,  "File with test (*.zip)" );

        // Обрізка шляху. без файлу - тільки назва папки
        int pos = fname.lastIndexOf(QChar('/'));
        conf->savePath = fname.left(pos);

        JlCompress::compressDir(fname, path + "/temp");

        conf->recentFiles.push_front(fname);

    }    else  QMessageBox::critical(0, "Помилка", "Не відкрито файл для запису запитань (temp/gift.txt)");;
}

// відкриття і імпорт з gift
void MainWindow::on_btn_open_clicked()
{
    ui->btn_clear_questions->click(); // очищення

    QString filename = QFileDialog::getOpenFileName(0, "Open zip", conf->openPath, "*.zip");

    if (filename == "")
         QMessageBox::information(0, "Info", "Не вибрано файл");
    else {
        // Обрізка шляху. без файлу - тільки назва папки
        int pos = filename.lastIndexOf(QChar('/'));
        conf->openPath = filename.left(pos);

        openFile(filename);
    }
}

// кнопку недавніні натиснуто та відкриття файлу
void MainWindow::on_recent1_clicked()
{ openFile(conf->recentFiles.at(0));}

void MainWindow::on_recent2_clicked()
{ openFile(conf->recentFiles.at(1));}

void MainWindow::on_recent3_clicked()
{ openFile(conf->recentFiles.at(2));}

void MainWindow::on_recent4_clicked()
{ openFile(conf->recentFiles.at(3));}

void MainWindow::on_recent5_clicked()
{ openFile(conf->recentFiles.at(4));}

void MainWindow::on_recent6_clicked()
{ openFile(conf->recentFiles.at(5)); }

void MainWindow::on_recent7_clicked()
{ openFile(conf->recentFiles.at(6));}

// функція відкриття файлу із gift
void MainWindow::openFile(QString pathToFile)
{
    JlCompress::extractDir(pathToFile, path + "/temp");

    QFile file(path + "/temp/gift.txt");
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);

        in.setCodec("UTF8");    // шоб можна було і укр символи

        QString str = in.readAll();
        str.replace("\r", "");
        str.replace("\n", " ");
        //qDebug() << str;

        questions->setQuestionFromGift(str);

        file.close();

        foreach (QString key, questions->map.keys())
        {
            questions->map.value(key);

            QListWidgetItem *item;
            switch (questions->map[key].type) {
            case 0: item = new QListWidgetItem(QIcon(":/img/one_ans.png"), key);break;
            case 1: item = new QListWidgetItem(QIcon(":/img/multi_ans.png"), key);break;
            case 2: item = new QListWidgetItem(QIcon(":/img/fillblank.png"), key);break;
            case 3: item = new QListWidgetItem(QIcon(":/img/vidpov.png"), key);break;
            }
            ui->listWidget->addItem(item);
        }

        ui->stackedWidget->setCurrentIndex(1);

    } else
         QMessageBox::information(0, "Info", "Файл не було відкрито (gift.txt). Можливо його не існує.");
}

// перехід до редагування із допомоги
void MainWindow::on_btn_go_edit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// перехід на сторінку допомога
void MainWindow::on_commandLinkButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// кнопка імпорту на головній сторінці
void MainWindow::on_btn_import_clicked()
{   
    QString filename = QFileDialog::getOpenFileName(0, "Open docx", conf->openExportPath, "*.docx");

    if (filename != "")
    {
        word *w = new word();

        // Обрізка шляху. без файлу - тільки назва папки
        int pos = filename.lastIndexOf(QChar('/'));
        conf->openExportPath = filename.left(pos);

        questions->setQuestionFromXml(w->readDocx(filename));

        // видалення тимчасових файлів
        deleteTempDir(path + "/tmp");

        QFile file(path + "/temp/gift.txt");

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString formated_str = questions->giftQuestion();

            QTextStream stream(&file);

            stream.setCodec("UTF8");    // шоб можна було і укр символи

            stream << formated_str;

            file.close();
            qDebug() << "Writing finished";

            QString fname = QFileDialog::getSaveFileName(nullptr, "Save file", conf->savePath,  "File with test (*.zip)" );

            // Обрізка шляху. без файлу - тільки назва папки
            int pos = fname.lastIndexOf(QChar('/'));
            conf->savePath = fname.left(pos);

            JlCompress::compressDir(fname, path + "/temp");

            conf->recentFiles.push_front(fname);
            QMessageBox::information(0, "Info", "Успішно стоворено");
        }    else  QMessageBox::critical(0, "Помилка", "Не відкрито файл для запису запитань (temp/gift.txt)");;

    } else QMessageBox::information(0, "Info", "Не вибрано файл");
}

// про програму
void MainWindow::on_commandLinkButton_clicked()
{
    QMessageBox::information(0, "Info", "Програму написав для дипломної роботи\nстудент групи КІ-406 тк ТНТУ ім. І. Пулюя\nВасилишин В. В. \ne-mail: vadyvas2812@gmail.com\n2020р");
}

// допомога -> головна
void MainWindow::on_btn_go_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
