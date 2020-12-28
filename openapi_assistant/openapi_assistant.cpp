#include "openapi_assistant.h"
#include "ui_openapi_assistant.h"
#include <QtNetwork>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTime>
#include <QUuid>
#include <QByteArray>
#include <QMessageAuthenticationCode>
#include <QHash>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>

QJsonObject update_startime_json;
QJsonObject update_endtime_json;
QJsonObject update_span_json;
QJsonObject update_counter_json;
int licenseForm;
int licStartTimeType_cloud;
int licStartTimeType_slock;
int flag_start;
int flag_end;
QByteArray AppId = "044b79df99e245f8ac547f892b197b59";
QByteArray SecretId = "fdfee335c3b34bfe991dffb6016a4d31";
QByteArray ttp;
QByteArray uuidStr;

openapi_assistant::openapi_assistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::openapi_assistant)
{
    ui->setupUi(this);
    setWindowTitle("openapi assistant");

    //可以拖动滚动条
    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea_hard->setWidgetResizable(false);
    ui->scrollArea_product->setWidgetResizable(false);
    ui->scrollArea_template->setWidgetResizable(false);

    //竖滚动条和横滚动条都可以一直显示
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_hard->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_hard->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_product->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_product->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_template->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea_template->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //设置主界面的实际界面
    ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
    ui->scrollArea_hard->setWidget(ui->scrollAreaWidgetContents_4);
    ui->scrollArea_product->setWidget(ui->scrollAreaWidgetContents_5);
    ui->scrollArea_template->setWidget(ui->scrollAreaWidgetContents_6);

    //实际界面参数
    ui->scrollAreaWidgetContents->setGeometry(0,0,5000,5000);
    ui->scrollAreaWidgetContents_4->setGeometry(0,0,5000,5000);
    ui->scrollAreaWidgetContents_5->setGeometry(0,0,5000,5000);
    ui->scrollAreaWidgetContents_6->setGeometry(0,0,5000,5000);

    //设置label参数
    ui->ret_label->setGeometry(0,0,5000,5000);
    ui->ret_label_1->setGeometry(0,0,5000,5000);
    ui->ret_label_2->setGeometry(0,0,5000,5000);
    ui->ret_label_3->setGeometry(0,0,5000,5000);

    //设置默认显示页面
    ui->stackedWidget->setCurrentWidget(ui->user_page);
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    ui->stackedWidget_6->setCurrentWidget(ui->page_11);
    ui->stackedWidget_7->setCurrentWidget(ui->page_13);

    //设置默认参数
    ui->isForever_edit->setEnabled(false);
    ui->isForever_edit->setText("false");
    ui->isForever_edit_2->setText("false");
    ui->startime_edit->setEnabled(false);
    ui->endtime_edit->setEnabled(false);
    ui->span_edit->setEnabled(false);
    ui->counter_edit->setEnabled(false);

    ui->isForever_edit_2->setEnabled(false);
    ui->startime_edit_2->setEnabled(false);
    ui->endtime_edit_2->setEnabled(false);
    ui->span_edit_2->setEnabled(false);
    ui->counter_edit_2->setEnabled(false);
}

openapi_assistant::~openapi_assistant()
{
    delete ui;
}

/*
    @QMap:以"键-值"的形式保存数据;
    @QByteArray:字节数组;
    用法：
     byte.resize(5);
    byte[0]=1;
    ...
*/

//post请求
QString openapi_assistant::httpPost(QString url,QByteArray data,QHash<QString,QString>headers)
{
    QNetworkRequest request;
    QNetworkAccessManager *m_accessManager = new QNetworkAccessManager();
    QNetworkReply *reply = NULL;
    QString ret;
    QByteArray reply_info;
    QString code;

    //设置请求的url
    request.setUrl(QUrl(url));

    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    if((headers.isEmpty()==false)&&headers.size()>0)
    {
        QByteArray key,value;
        QHash<QString,QString>::iterator iter = headers.begin();
        while(iter != headers.end())
        {
            key = iter.key().toUtf8();
            value = iter.value().toUtf8();
            request.setRawHeader(key,value);
            iter++;
        }
    }

    //post请求
    reply = m_accessManager->post(request,data);
    //添加事件循环机制
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();

    //获取http响应状态码
    QVariant variant =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    code = variant.toInt();

    if(code == 200)
    {
         reply_info = reply->readAll();
         ret = reply_info;
         delete reply;
         delete m_accessManager;
         return ret;
    }
    else
    {
         return "post_http error";
    }

}

/*
    QString::number(int,int)
    功能：将数字（整数、浮点数、有符号，无符号等）转换为QString类型
    第一个参数：待转换的数字
    第二个参数：转化进制
*/
void openapi_assistant::http_init()
{
    //SenseTimestamp UTC时间戳
    qint64 currenTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString ttp_tmp = QString::number(currenTime,10);
    ttp = ttp_tmp.toUtf8();

    //SenseNonce 唯一随机数
    QUuid id = QUuid::createUuid();
    QString uuidStr_tmp = id.toString();
    uuidStr_tmp.remove("{").remove("}").remove("-");
    uuidStr = uuidStr_tmp.toUtf8();

}
//添加用户
void openapi_assistant::on_addUser_clicked()
{
    //获取UTC时间戳和唯一随机数
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/addUser";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("userAccount",ui->userAccount->text());
    json.insert("nickname",ui->nickname->text());
    json.insert("passwd",ui->passwd->text());
    json.insert("accountType",ui->accountType->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/addUser",bodyBts,headerMap);

    //显示输出结果
    ui->ret_label->setText(result);
}


void openapi_assistant::on_issueAddLicense_clicked()
{

    ui->stackedWidget_3->setCurrentIndex(PAGE_ZREO);
}


//查找用户
void openapi_assistant::on_findUser_clicked()
{

    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/findUsers";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("userAccount",ui->userAccount->text());
    json.insert("nickname",ui->nickname->text());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/findUsers",bodyBts,headerMap);

    //将result转成json
    QJsonObject obj = QstringToJson(result);

    //解析json数据
    if(obj.contains("data"))
    {
        QJsonValue value = obj.take("data");
        if(value.isObject())
        {
            QJsonObject obj = value.toObject();
            if(obj.contains("users"))
            {
                QJsonValue value = obj.take("users");
                if(value.isArray())
                {
                    QJsonArray array = value.toArray();
                    QJsonObject obj = array[0].toObject();
                    if(obj.contains("userId"))
                    {
                        QJsonValue value = obj.take("userId");
                        QString userId = value.toString();
                        ui->userID->setText(userId);
                    }
                }
            }
        }
    }

    //显示输出结果
    ui->ret_label->setText(result);

}


//删除用户
void openapi_assistant::on_deleteUser_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/deleteUser";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("userId",ui->userID->text());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/deleteUser",bodyBts,headerMap);

    //显示输出结果
    ui->ret_label->setText(result);

}


//字符串转换JSON
QJsonObject openapi_assistant::QstringToJson(QString jsonString)
{
    QTextCodec::codecForName("UTF-8");
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8().data());
    if(jsonDocument.isNull())
    {
        qDebug()<< "String NULL"<< jsonString.toUtf8().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

void openapi_assistant::on_user_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_lock_hard_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_isForever_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->isForever_edit->clear();
        ui->isForever_edit->setText("true");
        ui->isForever_edit->setEnabled(true);
        ui->stackedWidget_2->setCurrentIndex(PAGE_ONE);

    }
    else if(arg1 == 0)
    {
        ui->isForever_edit->clear();
        ui->isForever_edit->setText("false");
        ui->isForever_edit->setEnabled(false);//文本框不可选，内容变成灰色
        ui->stackedWidget_2->setCurrentIndex(PAGE_ZREO);
    }
}

void openapi_assistant::on_startime_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        flag_start = 1;
        ui->startime_edit->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        flag_start = 0;
        ui->startime_edit->setEnabled(false);
    }

}

void openapi_assistant::on_endtime_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        flag_end = 1;
        ui->endtime_edit->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        flag_end = 0;
        ui->endtime_edit->setEnabled(false);
    }
}

void openapi_assistant::on_span_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->span_edit->setEnabled(true);

    }
    else if(arg1 == 0)
    {
        ui->span_edit->setEnabled(false);
    }
}

void openapi_assistant::on_counter_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->counter_edit->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->counter_edit->setEnabled(false);
    }
}

//字符串转换UTC时间戳
long openapi_assistant::QStringToQdataTime(QString datatime)
{
    QDateTime time_tmp;
    time_tmp = QDateTime::fromString(datatime, "yyyy/M/d H:mm");
    long time = time_tmp.toTime_t();
    return time;
}



void openapi_assistant::on_isForever_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->isForever_edit_2->clear();
        ui->isForever_edit_2->setText("true");
        ui->isForever_edit_2->setEnabled(true);
        ui->stackedWidget_4->setCurrentIndex(PAGE_ONE);

    }
    else if(arg1 == 0)
    {
        ui->isForever_edit_2->clear();
        ui->isForever_edit_2->setText("false");
        ui->isForever_edit_2->setEnabled(false);//文本框不可选，内容变成灰色
        ui->stackedWidget_4->setCurrentIndex(PAGE_ZREO);
    }
}

void openapi_assistant::on_issueUpdateLicense_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_issueDeleteLicense_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(PAGE_TWO);
}

void openapi_assistant::on_issueClearLicense_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(PAGE_THREE);
}

void openapi_assistant::on_startime_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->startime_edit_2->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->startime_edit_2->setEnabled(false);
    }

}

void openapi_assistant::on_endtime_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->endtime_edit_2->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->endtime_edit_2->setEnabled(false);
    }
}

void openapi_assistant::on_span_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->span_edit_2->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->span_edit_2->setEnabled(false);
    }
}

void openapi_assistant::on_counter_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->counter_edit_2->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->counter_edit_2->setEnabled(false);
    }
}

//添加许可
void openapi_assistant::on_addlicense_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/dlm/issueAddLicense";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    qint64 startTime,endTime,span,counter;
    startTime = QStringToQdataTime(ui->startime_edit->text());
    endTime = QStringToQdataTime(ui->endtime_edit->text());
    QJsonObject json;
    json.insert("pin",ui->pin->text());
    json.insert("shellNumber",ui->shellNumber->text());
    json.insert("licenseId",ui->licenseid->text().toInt());
    json.insert("force",ui->force->text());
    json.insert("isForever",ui->isForever_edit->text());

    if(ui->isForever_edit->text()=="false")
    {
        if(flag_start == 1)
        {
            json.insert("startTime",startTime);
        }

        if(flag_end == 1)
        {
            json.insert("endTime",endTime);
        }
    }

    if(!ui->span_edit->text().isEmpty())
    {
        span = ui->span_edit->text().toInt();
        json.insert("span",span);

    }

    if(!ui->counter_edit->text().isEmpty())
    {
        counter = ui->counter_edit->text().toInt();
        json.insert("counter",counter);
    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/dlm/issueAddLicense",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_1->setText(result);
}

//更新许可
void openapi_assistant::on_updatelicense_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/dlm/issueUpdateLicense";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    qint64 startTime,endTime,span,counter;
    QJsonObject json;
    json.insert("pin",ui->pin_1->text());
    json.insert("shellNumber",ui->shellNumber_1->text());
    json.insert("licenseId",ui->licenseId_1->text().toInt());
    json.insert("isForever",ui->isForever_edit_2->text());

    if(ui->isForever_edit_2->text()=="false")
    {

        QJsonValue value_star = update_startime_json.take("op");
        QJsonValue value_end = update_endtime_json.take("op");
        QString startime = value_star.toString();
        QString endtime = value_end.toString();
        if(startime=="set")
        {
            startTime = QStringToQdataTime(ui->startime_edit_2->text());
            update_startime_json.insert("value",startTime);
            update_startime_json.insert("op","set");
            json.insert("startTime",update_startime_json);

        }
        else if(startime=="inc")
        {
            startTime = ui->update_edit->text().toInt();
            update_startime_json.insert("value",startTime);
            update_startime_json.insert("op","inc");
            json.insert("startTime",update_startime_json);
        }
        else
        {
            update_startime_json.insert("op","disable");
            update_startime_json.insert("value","");
            json.insert("startTime",update_startime_json);
        }


        if(endtime=="set")
        {
            endTime = QStringToQdataTime(ui->endtime_edit_2->text());
            update_endtime_json.insert("value",endTime);
            update_endtime_json.insert("op","set");
            json.insert("endTime",update_endtime_json);
        }
        else if(endtime=="inc")
        {
            endTime = ui->update_edit2->text().toInt();
            update_endtime_json.insert("value",endTime);
            update_endtime_json.insert("op","inc");
            json.insert("endTime",update_endtime_json);
        }
        else
        {
            update_endtime_json.insert("op","disable");
            update_endtime_json.insert("value","");
            json.insert("endTime",update_endtime_json);
        }
    }

    if(!ui->span_edit_2->text().isEmpty())
    {
        span = ui->span_edit_2->text().toInt();
        update_span_json.insert("value",span);
        json.insert("span",update_span_json);

    }
    else
    {
        update_span_json.insert("value","");
        update_span_json.insert("op","disable");
        json.insert("span",update_span_json);
    }

    if(!ui->counter_edit_2->text().isEmpty())
    {
        counter = ui->counter_edit_2->text().toInt();
        update_counter_json.insert("value",counter);
        json.insert("counter",update_counter_json);
    }
    else
    {
        update_counter_json.insert("op","disable");
        update_counter_json.insert("value","");
        json.insert("counter",update_counter_json);
    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/dlm/issueUpdateLicense",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_1->setText(result);
}

void openapi_assistant::on_set_button_clicked()
{
    update_startime_json.insert("op","set");
    ui->stackedWidget_7->setCurrentIndex(PAGE_ZREO);
    ui->startime_edit_2->setEnabled(true);
    ui->update_edit->setEnabled(true);

}

void openapi_assistant::on_add_button_clicked()
{

    update_startime_json.insert("op","inc");
    ui->stackedWidget_7->setCurrentIndex(PAGE_ONE);
    ui->startime_edit_2->setEnabled(true);
    ui->update_edit->setEnabled(true);
}

void openapi_assistant::on_disable_button_clicked()
{
    update_startime_json.insert("op","disable");
    ui->stackedWidget_7->setCurrentIndex(PAGE_ONE);
    ui->startime_edit_2->setEnabled(false);
    ui->update_edit->setEnabled(false);
}

void openapi_assistant::on_set_button1_clicked()
{
    update_endtime_json.insert("op","set");
    ui->stackedWidget_6->setCurrentIndex(PAGE_ZREO);
    ui->update_edit2->setEnabled(true);
    ui->endtime_edit_2->setEnabled(true);
}

void openapi_assistant::on_add_button1_clicked()
{
    update_endtime_json.insert("op","inc");
    ui->stackedWidget_6->setCurrentIndex(PAGE_ONE);
    ui->update_edit2->setEnabled(true);
    ui->endtime_edit_2->setEnabled(true);
}

void openapi_assistant::on_disable_button1_clicked()
{
    update_endtime_json.insert("op","disable");
    ui->stackedWidget_6->setCurrentIndex(PAGE_ONE);
    ui->update_edit2->setEnabled(false);
    ui->endtime_edit_2->setEnabled(false);
}

void openapi_assistant::on_set_button2_clicked()
{
    update_span_json.insert("op","set");
     ui->span_edit_2->setEnabled(true);
}

void openapi_assistant::on_add_button2_clicked()
{
    update_span_json.insert("op","inc");
    ui->span_edit_2->setEnabled(true);
}

void openapi_assistant::on_disable_button2_clicked()
{
    update_span_json.insert("op","disable");
    ui->span_edit_2->setEnabled(false);
}

void openapi_assistant::on_set_button3_clicked()
{
    update_counter_json.insert("op","set");
    ui->counter_edit_2->setEnabled(true);
}

void openapi_assistant::on_add_button3_clicked()
{
    update_counter_json.insert("op","inc");
    ui->counter_edit_2->setEnabled(true);
}

void openapi_assistant::on_disable_button3_clicked()
{
    update_counter_json.insert("op","disable");
    ui->counter_edit_2->setEnabled(false);
}

//删除指定许可
void openapi_assistant::on_deletelicense_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/dlm/issueDeleteLicense";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("pin",ui->pin_2->text());
    json.insert("shellNumber",ui->shellNumber_2->text());
    json.insert("licenseId",ui->licenseId_2->text().toInt());

    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/dlm/issueDeleteLicense",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_1->setText(result);
}

//清空锁内许可
void openapi_assistant::on_clearlicense_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/dlm/issueClearAllLicenses";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("pin",ui->pin_3->text());
    json.insert("shellNumber",ui->shellNumber_3->text());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/dlm/issueClearAllLicenses",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_1->setText(result);
}

void openapi_assistant::on_product_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_TWO);
}

void openapi_assistant::on_template_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_THREE);
}

void openapi_assistant::on_addproduct_clicked()
{
    ui->stackedWidget_5->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_modifyproduct_clicked()
{
    ui->stackedWidget_5->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_deleteproduct_clicked()
{
    ui->stackedWidget_5->setCurrentIndex(PAGE_TWO);
}



//添加产品
void openapi_assistant::on_add_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/addProduct";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    QJsonArray array;
    QString test;
    int n,size;
    json.insert("licenseId",ui->licenseId->text().toInt());
    json.insert("productName",ui->productName->text());
    test = ui->licenseForm->text();
    n = test.toInt();
    size = test.size();
    if(size == 1)
    {
        array.append(n);
    }
    else if(size == 2)
    {
        for(int i = 0;i<size;i++)
        {
            if(i == 0)
            {
                array.append(n/10%10);
            }
            else
            {
                array.append(n/1%10);
            }
        }
    }
    else if(size == 3)
    {
        for(int i = 0;i<size;i++)
        {
            if(i == 0)
            {
                array.append(n/100%10);
            }
            else if(i == 1)
            {
                array.append(n/10%10);
            }
            else
            {
                array.append(n/1%10);
            }
        }
    }
    json.insert("licenseForm",array);
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/addProduct",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_2->setText(result);
}

//修改产品
void openapi_assistant::on_modify_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/modifyProduct";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    QJsonArray array;
    QString test;
    int n,size;
    json.insert("licenseId",ui->licenseId_4->text().toInt());
    json.insert("productName",ui->productName_2->text());
    test = ui->licenseForm_2->text();
    n = test.toInt();
    size = test.size();
    if(size == 1)
    {
        array.append(n);
    }
    else if(size == 2)
    {
        for(int i = 0;i<size;i++)
        {
            if(i == 0)
            {
                array.append(n/10%10);
            }
            else
            {
                array.append(n/1%10);
            }
        }
    }
    else if(size == 3)
    {
        for(int i = 0;i<size;i++)
        {
            if(i == 0)
            {
                array.append(n/100%10);
            }
            else if(i == 1)
            {
                array.append(n/10%10);
            }
            else
            {
                array.append(n/1%10);
            }
        }
    }
    json.insert("licenseForm",array);
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/modifyProduct",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_2->setText(result);
}

//删除产品
void openapi_assistant::on_delete_2_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/deleteProduct";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("licenseId",ui->licenseId_3->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/deleteProduct",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_2->setText(result);
}

void openapi_assistant::on_addTemplate_clicked()
{
    ui->stackedWidget_8->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_modifyTemplate_clicked()
{
    ui->stackedWidget_8->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_deleteTemplate_clicked()
{
    ui->stackedWidget_8->setCurrentIndex(PAGE_TWO);
}

//添加模板
void openapi_assistant::on_add_3_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/addTemplate";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("templateName",ui->templatename->text());
    json.insert("licenseId",ui->licenseId_5->text().toInt());
    json.insert("licenseMode",ui->licenseMode->text().toInt());
    json.insert("pricingMode",ui->pricingMode->text());
    json.insert("isForever",ui->isForever_3->text());
    if(licenseForm == 2)//软锁
    {
        if(licStartTimeType_slock == 0)
        {
            qint64 startTime,endTime;
            startTime = QStringToQdataTime(ui->startime_edit_7->text());
            endTime = QStringToQdataTime(ui->endtime_edit_5->text());
            json.insert("licStartTimeType",0);
            json.insert("licenseForm",2);
            json.insert("startTime",startTime);
            json.insert("endTime",endTime);
            json.insert("offlineDays",ui->offlineDays->text().toInt());
            json.insert("licBindLimit",ui->licBindLimit->text().toInt());
            json.insert("licBindMaxLimit",ui->licBindMaxLimit->text().toInt());

        }
        else if(licStartTimeType_slock == 1)
        {
            json.insert("licenseForm",2);
            json.insert("licStartTimeType",1);
            json.insert("offlineDays",ui->offlinedays->text().toInt());
            json.insert("licBindLimit",ui->licbindlimit->text().toInt());
            json.insert("licBindMaxLimit",ui->licbindmaxlimit->text().toInt());
            json.insert("licDuration",ui->licDuration->text().toInt());
        }
    }
    else if(licenseForm == 1)//云锁
    {
        if(licStartTimeType_cloud == 0)
        {
            qint64 startTime,endTime;
            startTime = QStringToQdataTime(ui->startime_edit_3->text());
            endTime = QStringToQdataTime(ui->endtime_edit_3->text());
            json.insert("licStartTimeType",0);
            json.insert("licenseForm",1);
            json.insert("startTime",startTime);
            json.insert("endTime",endTime);
            if(!ui->counter_3->text().isEmpty())
            {
                json.insert("counter",ui->counter_3->text().toInt());
            }
            if(!ui->spanTime->text().isEmpty())
            {
                json.insert("spanTime",ui->spanTime->text().toInt());
            }
        }
        else if(licStartTimeType_cloud == 1)
        {
            json.insert("licStartTimeType",1);
            json.insert("licenseForm",1);
            json.insert("licDuration",ui->licDuration_6->text().toInt());
        }

    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/addTemplate",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_3->setText(result);

}

//删除模板
void openapi_assistant::on_delete_4_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/deleteTemplate";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("templateId",ui->templateId_2->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/deleteTemplate",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_3->setText(result);

}

void openapi_assistant::on_cloud_clicked()
{
    licenseForm = 1;
    ui->stackedWidget_9->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_slock_clicked()
{
    licenseForm = 2;
    ui->stackedWidget_9->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_specific_time_clicked()
{
    licStartTimeType_cloud = 0;//具体时间
    ui->stackedWidget_10->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_release_time_clicked()
{
    licStartTimeType_cloud = 1;//许可发布时间
    ui->stackedWidget_10->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_specific_time_2_clicked()
{
    licStartTimeType_slock = 0;//具体时间
    ui->stackedWidget_11->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_release_time_2_clicked()
{
    licStartTimeType_slock = 1; //许可发布时间
    ui->stackedWidget_11->setCurrentIndex(PAGE_ZREO);
}

//修改模板
void openapi_assistant::on_modify_3_clicked()
{
    http_init();

    //ResourceURI
    QString resourceURI_tmp = "/v2/sv/modifyTemplate";
    QByteArray resourceURI = resourceURI_tmp.toUtf8();

    //BodyContent
    QJsonObject json;
    json.insert("templateName",ui->templatename_3->text());
    json.insert("templateId",ui->templateId->text().toInt());
    json.insert("licenseMode",ui->licenseMode_2->text().toInt());
    json.insert("pricingMode",ui->pricingMode_2->text());
    json.insert("isForever",ui->isForever_4->text());
    if(licenseForm == 2)//软锁
    {
        if(licStartTimeType_slock == 0)
        {
            qint64 startTime,endTime;
            startTime = QStringToQdataTime(ui->startime_edit_8->text());
            endTime = QStringToQdataTime(ui->endtime_edit_6->text());
            json.insert("licStartTimeType",0);
            json.insert("licenseForm",2);
            json.insert("startTime",startTime);
            json.insert("endTime",endTime);
            json.insert("offlineDays",ui->offlineDays_2->text().toInt());
            json.insert("licBindLimit",ui->licBindLimit_2->text().toInt());
            json.insert("licBindMaxLimit",ui->licBindMaxLimit_2->text().toInt());

        }
        else if(licStartTimeType_slock == 1)
        {
            json.insert("licenseForm",2);
            json.insert("licStartTimeType",1);
            json.insert("offlineDays",ui->offlinedays_2->text().toInt());
            json.insert("licBindLimit",ui->licbindlimit_2->text().toInt());
            json.insert("licBindMaxLimit",ui->licbindmaxlimit_2->text().toInt());
            json.insert("licDuration",ui->licDuration_2->text().toInt());
        }
    }
    else if(licenseForm == 1)//云锁
    {
        if(licStartTimeType_cloud == 0)
        {
            qint64 startTime,endTime;
            startTime = QStringToQdataTime(ui->startime_edit_9->text());
            endTime = QStringToQdataTime(ui->endtime_edit_7->text());
            json.insert("licStartTimeType",0);
            json.insert("licenseForm",1);
            json.insert("startTime",startTime);
            json.insert("endTime",endTime);
            if(!ui->counter_3->text().isEmpty())
            {
                json.insert("counter",ui->counter_5->text().toInt());
            }
            if(!ui->spanTime->text().isEmpty())
            {
                json.insert("spanTime",ui->spanTime_2->text().toInt());
            }
        }
        else if(licStartTimeType_cloud == 1)
        {
            json.insert("licStartTimeType",1);
            json.insert("licenseForm",1);
            json.insert("licDuration",ui->licDuration_7->text().toInt());
        }

    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray bodyBts=document.toJson(QJsonDocument::Compact);

    //将内容合并进行hmac-sha256
    QByteArray allSrcData = ttp+uuidStr+resourceURI+bodyBts;
    QString signData = QMessageAuthenticationCode::hash(allSrcData,SecretId,QCryptographicHash::Sha256).toBase64();

    //设置http头信息
    QHash<QString,QString>headerMap;
    headerMap.insert("SenseTimestamp",ttp);
    headerMap.insert("SenseNonce",uuidStr);
    headerMap.insert("SenseAppID",AppId);
    headerMap.insert("SenseSign",signData);

    //调用OpenAPI接口
    QString result = httpPost("https://openapi.senseyun.com/v2/sv/modifyTemplate",bodyBts,headerMap);

    //打印输出结果
    ui->ret_label_3->setText(result);
}

void openapi_assistant::on_cloud_2_clicked()
{
    licenseForm = 1;
    ui->stackedWidget_14->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_slock_3_clicked()
{
    licenseForm = 2;
    ui->stackedWidget_14->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_specific_time_4_clicked()
{
    licStartTimeType_cloud = 0;//具体时间
    ui->stackedWidget_16->setCurrentIndex(PAGE_ZREO);
}

void openapi_assistant::on_release_time_4_clicked()
{
    licStartTimeType_cloud = 1;//许可发布时间
    ui->stackedWidget_16->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_specific_time_3_clicked()
{
    licStartTimeType_slock = 0;//具体时间
    ui->stackedWidget_15->setCurrentIndex(PAGE_ONE);
}

void openapi_assistant::on_release_time_3_clicked()
{
    licStartTimeType_slock = 1;//具体时间
    ui->stackedWidget_15->setCurrentIndex(PAGE_ZREO);
}




