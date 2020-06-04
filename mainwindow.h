#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QFileDialog>
#include <JlCompress.h>
#include <QStandardItemModel>
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include <QThread>
#include <QLineEdit>
#include <QFileInfo>
#include <QMessageBox>


#include <word.h>
#include "questions.h"
#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createTempDir(QString);  // тимчасовий каталог із запитаннями
    void deleteTempDir(QString);  // видаляє тимчасову директорію

                            //  tesCreator.exe
                            //  temp
                            //  |-media
                            //  | |-image1.png
                            //  | |-image2.png
                            //  |-gift.txt
                            //  tmp
                            //  |-word

    void openFile(QString);


private slots:
    void slotRecent(QModelIndex index);

    void on_btn_create_new_clicked();
    void on_btn_question_add_one_clicked();
    void on_btn_question_add_multi_clicked();
    void on_btn_question_add_fill_clicked();
    void on_btn_question_add_vidpov_clicked();
    void on_btn_del_question_clicked();
    void on_btn_view_mode_clicked();
    void on_btn_edite_mode_clicked();
    void on_btn_export_clicked();
//    void on_btn_question_add_img_clicked();
    void on_btn_clear_questions_clicked();
    void on_btn_add_answer_t_clicked();
    void on_btn_add_answer_tp_clicked();
    void on_btn_add_answer_p_clicked();
    void on_btn_add_question_img_clicked();
    void on_btn_del_answer_clicked();
    void on_btn_save_question_clicked();
    void on_search_line_textChanged(const QString &arg1);
    void on_btn_search_clicked();
    void on_btn_save_exp_config_clicked();
    void on_in_symbol_at_textChanged(const QString &arg1);
    void on_in_symbol_af_textChanged(const QString &arg1);
    void on_btn_exp_clicked();
    void on_btn_export_setting_clicked();
    void on_btn_save_clicked();
    void on_btn_open_clicked();
    void on_recent1_clicked();
    void on_recent2_clicked();
    void on_recent3_clicked();
    void on_recent4_clicked();
    void on_recent5_clicked();
    void on_recent6_clicked();
    void on_recent7_clicked();
    void on_btn_go_edit_clicked();
    void on_commandLinkButton_2_clicked();
    void on_btn_import_clicked();
    void on_commandLinkButton_clicked();
    void on_btn_go_home_clicked();

private:
    Ui::MainWindow *ui;
    Questions *questions;   // для роботи із запитаннями
    config *conf;           // настройки для роботи програми

    QString path = "";  // шлях до папки з виконуваною програмою

    QVector <QString> TempImg;  // назви файлів доданих картинок

   // bool createdTemp = false;   // тимчасова папка створена

    void recentFiles();
};
#endif // MAINWINDOW_H
