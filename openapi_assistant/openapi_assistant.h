#ifndef OPENAPI_ASSISTANT_H
#define OPENAPI_ASSISTANT_H
#define PAGE_ZREO 0
#define PAGE_ONE 1
#define PAGE_TWO 2
#define PAGE_THREE 3
#define PAGE_FOUR 4

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class openapi_assistant; }
QT_END_NAMESPACE

class openapi_assistant : public QWidget
{
    Q_OBJECT

public:
    openapi_assistant(QWidget *parent = nullptr);
    ~openapi_assistant();

private slots:

    void on_addUser_clicked();

    void on_issueAddLicense_clicked();

    void on_findUser_clicked();

    void on_deleteUser_clicked();

    void on_user_clicked();

    void on_lock_hard_clicked();

    void on_isForever_stateChanged(int arg1);

    void on_startime_stateChanged(int arg1);

    void on_endtime_stateChanged(int arg1);

    void on_span_stateChanged(int arg1);

    void on_counter_stateChanged(int arg1);

    void on_isForever_2_stateChanged(int arg1);

    void on_issueUpdateLicense_clicked();

    void on_issueDeleteLicense_clicked();

    void on_issueClearLicense_clicked();

    void on_startime_2_stateChanged(int arg1);

    void on_endtime_2_stateChanged(int arg1);

    void on_span_2_stateChanged(int arg1);

    void on_counter_2_stateChanged(int arg1);

    void on_addlicense_clicked();

    void on_updatelicense_clicked();

    void on_set_button_clicked();

    void on_add_button_clicked();

    void on_disable_button_clicked();

    void on_set_button1_clicked();

    void on_add_button1_clicked();

    void on_disable_button1_clicked();

    void on_set_button2_clicked();

    void on_add_button2_clicked();

    void on_disable_button2_clicked();

    void on_set_button3_clicked();

    void on_add_button3_clicked();

    void on_disable_button3_clicked();

    void on_deletelicense_clicked();

    void on_clearlicense_clicked();

    void on_product_clicked();

    void on_template_2_clicked();

    void on_addproduct_clicked();

    void on_modifyproduct_clicked();

    void on_deleteproduct_clicked();

    void on_add_clicked();

    void on_modify_clicked();

    void on_delete_2_clicked();

    void on_addTemplate_clicked();

    void on_modifyTemplate_clicked();

    void on_deleteTemplate_clicked();

    void on_add_3_clicked();

    void on_delete_4_clicked();

    void on_cloud_clicked();

    void on_slock_clicked();

    void on_specific_time_clicked();

    void on_release_time_clicked();

    void on_specific_time_2_clicked();

    void on_release_time_2_clicked();

    void on_modify_3_clicked();

    void on_cloud_2_clicked();

    void on_slock_3_clicked();

    void on_specific_time_4_clicked();

    void on_release_time_4_clicked();

    void on_specific_time_3_clicked();

    void on_release_time_3_clicked();

private:
    Ui::openapi_assistant *ui;
    QString httpPost(QString url,QByteArray data,QHash<QString,QString>headers);
    QJsonObject QstringToJson(QString jsonString);
    long QStringToQdataTime(QString datatime);
    void http_init();
};
#endif // OPENAPI_ASSISTANT_H
