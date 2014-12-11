/*
 * CacheHtmlView.h
 *
 *  Created on: 2014年9月7日
 *      Author: qGG
 */

#ifndef CACHEHTMLVIEW_H_
#define CACHEHTMLVIEW_H_
#include <bb/cascades/WebView>

class CacheHtmlView: public bb::cascades::WebView
{
    Q_OBJECT
public:
    CacheHtmlView();

public slots:
    bool isCaching();
    void cacheHtml(QString body, QVariant addtion=QVariant());

signals:
    void cacheHtmlOk(QVariant addtion);

private slots:
    void onLoadingChanged(bb::cascades::WebLoadRequest *loadRequest);

private:
    bool m_caching;
    QVariant m_addtion;
};

#endif /* CACHEHTMLVIEW_H_ */
