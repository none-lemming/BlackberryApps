/*
 * ZhihuNao.cpp
 *
 *  Created on: 2014年9月5日
 *      Author: qGG
 */

#include <src/ZhihuNao.h>
#include <bb/data/JsonDataAccess>
#include <bb/utility/ImageConverter>
#include <QDate>
#include <QDir>

using namespace bb::cascades;
using namespace bb::data;

const QString LastNewsPre = "http://news-at.zhihu.com/api/3/stories/latest";
const QString BeforeNewsPre = "http://news-at.zhihu.com/api/3/stories/before/";
const QString NewsContentPre = "http://news-at.zhihu.com/api/3/story/";

const QString ZhihuNao::ListCachePre = "./data/List/";
const QString ZhihuNao::ListImgCachePre = "./data/listImg/";
const QString ZhihuNao::NewsImgCachePre = "./data/newsImg/";
const QString ZhihuNao::NewsContentCachePre = "./data/html/";

ZhihuNao::ZhihuNao(QObject* parent)
    :   QObject(parent)
    ,   m_taskNum(0)
    ,   m_processingNum(0)
{
    qDebug()<<connect(&m_cacheView, SIGNAL(cacheHtmlOk(QVariant)), this, SLOT(onCacheHtmlImgOk(QVariant)));
    qDebug()<<"network:"<<Nao::isNetworkEnable();
}

QString ZhihuNao::todayDate()
{
    return QDateTime::currentDateTime().toString("yyyyMMdd");
}

QString ZhihuNao::dateToOffset(QString date, int offset)
{
    return QDate::fromString(date, "yyyyMMdd").addDays(offset).toString("yyyyMMdd");
}

void ZhihuNao::initCacheFolder()
{
    QDir dir("./data");
    dir.mkdir("List");
    dir.mkdir("listImg");
    dir.mkdir("newsImg");
    dir.mkdir("html");
}

void ZhihuNao::cleanFolder(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            cleanFolder(fi.absoluteFilePath());
    }
}

void ZhihuNao::getNewsList(QString date)
{
    QString url;
    if (date.isEmpty())
        url = LastNewsPre;
    else
    {
        if (!Nao::isNetworkEnable() && QFile::exists(ListCachePre+date))
        {
            qDebug("retrun list cache");
            JsonDataAccess jda;
            QVariantMap map = jda.load(ListCachePre+date).toMap();
            emit getNewsListOk(map["stories"], map["date"].toString());
            return ;
        }
        if (date >= todayDate())//超过今天还是请求last
            url = LastNewsPre;
        else
            url = BeforeNewsPre + dateToOffset(date, 1);
    }

    qDebug()<<url;
    ReplyWrap* wrap = Nao::GET(url);
    connect(wrap, SIGNAL(finished()), this, SLOT(onNewsList()));
}

void ZhihuNao::onNewsList()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"onNewsList";
        JsonDataAccess jda;
        QByteArray buff = reply->readAll();
        qDebug()<<buff;
        QVariant response = jda.loadFromBuffer(buff);
        QVariantMap map = response.toMap();
        if (map.contains("date"))
        {
            QString date = map["date"].toString();
            QVariantList newsList = map["stories"].toList();

            emit getNewsListOk(newsList, date);
        }
        else
        {
            emit getNewsListError("onNewsList error: api error");
        }
    }
    else
    {
        qDebug()<<reply->errorString();
        emit getNewsListError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void ZhihuNao::getListImg(QString id, QString url, QVariantList indexPath)
{
    if (QFile::exists(ListImgCachePre+id))
    {
        qDebug("retrun getListImg cache");
        emit getListImgOk("file://"+QDir::currentPath()+"/data/listImg/"+id, indexPath, id);
    }
    else
    {
        qDebug()<<"get listImg: "+id;
        QVariantMap map;
        map["id"] = id;
        map["indexPath"] = indexPath;
        map["id"] = id;
        ReplyWrap* wrap = Nao::GET(url);
        wrap->setAddition(map);
        connect(wrap, SIGNAL(finished()), this, SLOT(onListImg()));
    }
}

void ZhihuNao::onListImg()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        //qDebug()<<"onListImg";
        QByteArray buff = reply->readAll();
        //Image image = Image(bb::utility::ImageConverter::decode(buff));
        QVariantMap map = wrap->addition().toMap();

        QString id = map["id"].toString();
        QFile file(ListImgCachePre+id);
        file.open(QIODevice::WriteOnly);
        file.write(buff);
        file.close();
        emit getListImgOk("file://"+QDir::currentPath()+"/data/listImg/"+id, map["indexPath"].toList(), id);
    }
    else
    {
        qDebug()<<reply->errorString();
        emit getListImgError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void ZhihuNao::getNewsImg(QString id, QString url, bool offline)
{
    if (QFile::exists(NewsImgCachePre+id))
    {
        //qDebug("retrun getNewsImg cache");
        if (!offline)
            emit getNewsImgOk("file://"+QDir::currentPath()+"/data/newsImg/"+id, id);
    }
    else
    {
        QVariantMap map;
        map["id"] = id;
        map["offline"] = offline;
        ReplyWrap* wrap = Nao::GET(url);
        wrap->setAddition(map);
        connect(wrap, SIGNAL(finished()), this, SLOT(onNewsImg()));
    }
}

void ZhihuNao::onNewsImg()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"onNewsImg";
        QByteArray buff = reply->readAll();
        Image image = Image(bb::utility::ImageConverter::decode(buff));
        QVariantMap map = wrap->addition().toMap();
        QString id = map["id"].toString();

        QFile file(NewsImgCachePre+id);
        file.open(QIODevice::WriteOnly);
        qDebug()<<file.write(buff);
        file.close();
        if (!map["offline"].toBool())
            emit getNewsImgOk("file://"+QDir::currentPath()+"/data/newsImg/"+id, id);
    }
    else
    {
        qDebug()<<reply->errorString();
        emit getNewsImgError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void ZhihuNao::getNewsContent(QString id, bool offline)
{
    if (QFile::exists(NewsContentCachePre+id))
    {
        //qDebug("retrun article cache");
        if (offline)
            cacheContentImgByView(id);
        else
        {
            JsonDataAccess jda;
            emit getNewsContentOk(jda.load(NewsContentCachePre+id));
        }
    }
    else
    {
        QVariantMap map;
        map["id"] = id;
        map["offline"] = offline;
        ReplyWrap* wrap = Nao::GET(NewsContentPre+id);
        wrap->setAddition(map);
        connect(wrap, SIGNAL(finished()), this, SLOT(onNewsContent()));
    }
}

void ZhihuNao::onNewsContent()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"onNewsContent";
        JsonDataAccess jda;
        QByteArray buff = reply->readAll();
        //qDebug()<<buff;
        QVariant response = jda.loadFromBuffer(buff);
        QVariantMap map = response.toMap();
        if (map.contains("body"))
        {
            QVariantMap addtion = wrap->addition().toMap();
            bool offline =addtion["offline"].toBool();
            if (!offline)
                emit getNewsContentOk(response);

            QString id = addtion["id"].toString();
            QFile file(NewsContentCachePre+id);
            file.open(QIODevice::WriteOnly);
            qDebug()<<file.write(buff);
            file.close();
            qDebug()<<file.fileName();
            if (offline)
                cacheContentImgByView(id);
        }
        else
        {
            emit getNewsContentError("onNewsContent error: api error");
        }
    }
    else
    {
        qDebug()<<reply->errorString();
        emit getNewsContentError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void ZhihuNao::offlineDownloadNews(QVariantList idlist)
{
    m_taskNum += idlist.size();
    for (int i=0; i<idlist.size(); i++)
    {
        //qDebug()<<"downloadNews: "+idlist.at(i).toString();
        getNewsContent(idlist.at(i).toString(), true);
    }
}

void ZhihuNao::cacheContentImgByView(QString id)
{
    m_toViewList.append(id);
    onCacheHtmlImgOk(QVariant());
}

void ZhihuNao::onCacheHtmlImgOk(QVariant addtion)
{
    if (m_toViewList.size()>0 && !m_cacheView.isCaching())
    {
        qDebug()<<"progress: "<<m_processingNum*100/m_taskNum;
        m_processingNum ++;
        QString id = m_toViewList.first().toString();
        m_toViewList.pop_front();
        JsonDataAccess jda;
        QVariantMap map = jda.load(NewsContentCachePre+id).toMap();
        getNewsImg(id, map["image"].toString(), true);
        m_cacheView.cacheHtml(map["body"].toString(), id);
    }
    else if (m_taskNum == m_processingNum)
    {
        qDebug()<<"offline download ok";
        m_taskNum = 0;
        m_processingNum = 0;
        emit offlineDownloadNewsOk();
    }
}

//------------------------------------------------------------------
void ZhihuNao::cacheContentImg(QString id)
{
    /*JsonDataAccess test;
    json = test.load("./app/native/assets/test.json");
    id = "4141291";*/


    QDir dir("NewsContentCachePre");                                //建立图片文件夹
    dir.mkdir(id);

    JsonDataAccess jda;
    QVariantMap map = jda.load(NewsContentCachePre+id).toMap();
    QString body = map["body"].toString();
    body.replace("\\", "");
    qDebug()<<body;
    QRegExp rx("(<img[^>]*src=\")([^\">]+/)([^\">]+)(\"[^>]*>)");
    int pos = 0;
qDebug()<<"-------------------";
    while (true)                                                    //提取图片链接并下载
    {
        pos = body.indexOf(rx, pos);
        if (pos >= 0)
        {
            getContentImg(id, rx.cap(3), rx.cap(2)+rx.cap(3));
            /*QString url = rx.cap(2);
            qDebug()<<url;
            qDebug()<<rx.cap(3);*/
            pos += rx.cap(0).length();
        }
        else
            break;
    }
qDebug()<<"-------------------";
    QRegExp replaceRx("(<img[^>]*src=\")([^\">]+/)([^\">]+)(\"[^>]*>)");//改写html并保存
    body.replace(replaceRx, QString("\\1%1\\3\\4").arg(NewsContentCachePre+id+"/"));
    qDebug()<<body;
    map["body"] = body;
    jda.save(map, NewsContentCachePre+id);
}

void ZhihuNao::getContentImg(QString id, QString name, QString url)
{
    QVariantMap map;
    map["id"] = id;
    map["name"] = name;
    ReplyWrap* wrap = Nao::GET(url);
    wrap->setAddition(map);
    connect(wrap, SIGNAL(finished()), this, SLOT(onContentImg()));
}

void ZhihuNao::onContentImg()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"onContentImg";
        QByteArray buff = reply->readAll();
        Image image = Image(bb::utility::ImageConverter::decode(buff));
        QVariantMap map = wrap->addition().toMap();

        QFile file(NewsContentCachePre+map["id"].toString()+"/"+map["name"].toString());
        file.open(QIODevice::WriteOnly);
        qDebug()<<file.write(buff);
        file.close();
    }
    else
    {
        qDebug()<<reply->errorString();
    }
    wrap->deleteLater();
    reply->deleteLater();
}
