/*
 * ZhihuNao.h
 *
 *  Created on: 2014年9月5日
 *      Author: qGG
 */

#ifndef ZHIHUNAO_H_
#define ZHIHUNAO_H_
#include <bb/cascades/Image>
#include <src/CacheHtmlView.h>
#include "Nao.h"

typedef QList< QPair<QString,QString> > QDict;

class ZhihuNao: public QObject
{
    Q_OBJECT
public:
    ZhihuNao(QObject* parent=0);


public slots:
    static QString dateToOffset(QString date, int offset);
    static QString todayDate();
    static void initCacheFolder();
    static void cleanFolder(QString path);

    void getNewsList(QString date="");
    void getListImg(QString id, QString url, QVariantList indexPath);
    void getNewsContent(QString id, bool offline=false);
    void getNewsImg(QString id, QString url, bool offline=false);

    void offlineDownloadNews(QVariantList idlist);     //离线下载,使用webview的缓存功能
    void cacheContentImgByView(QString id);

    void cacheContentImg(QString id);           //离线下载,自己管理缓存,不用

signals:
    void getNewsListOk(QVariant list, QString date);
    void getNewsListError(QString msg);
    void getListImgOk(QString imageUrl, QVariantList indexPath, QString id);
    void getListImgError(QString msg);
    void getNewsImgOk(QString imageUrl, QString id);
    void getNewsImgError(QString msg);
    void getNewsContentOk(QVariant content);
    void getNewsContentError(QString msg);

    void offlineDownloadNewsOk();

private slots:
    void onNewsList();
    void onListImg();
    void onNewsImg();
    void onNewsContent();

    void onCacheHtmlImgOk(QVariant addtion);   //webview缓存成功

    void getContentImg(QString id, QString name, QString url);
    void onContentImg();

public:
    static const QString ListCachePre;
    static const QString ListImgCachePre;
    static const QString NewsImgCachePre;
    static const QString NewsContentCachePre;

private:

    CacheHtmlView  m_cacheView;   //离线下载用, 加载html后会自动缓存图片, loadingChanged 0101
    QVariantList m_toViewList;
    int m_taskNum;
    int m_processingNum;

};

#endif /* ZHIHUNAO_H_ */
