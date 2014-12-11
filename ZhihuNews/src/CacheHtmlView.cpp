/*
 * CacheHtmlView.cpp
 *
 *  Created on: 2014年9月7日
 *      Author: qGG
 */

#include <src/CacheHtmlView.h>
#include <bb/cascades/WebLoadRequest>
using namespace bb::cascades;

CacheHtmlView::CacheHtmlView()
    :   m_caching(false)
{
    connect(this, SIGNAL(loadingChanged (bb::cascades::WebLoadRequest*)), this, SLOT(onLoadingChanged(bb::cascades::WebLoadRequest*)));
}

bool CacheHtmlView::isCaching()
{
    return m_caching;
}

void CacheHtmlView::cacheHtml(QString body, QVariant addtion)
{
    if (m_caching)
        return;
    m_caching = true;
    m_addtion = addtion;
    this->setHtml(body);
    //qDebug()<<body;
}

void CacheHtmlView::onLoadingChanged(WebLoadRequest *loadRequest)
{
    static int count = 0;
    count++;
    //qDebug()<<"status: "+QString::number(loadRequest->status());
    if (count >= 2)
    {
        //qDebug()<<"cache ok";
        count = 0;
        m_caching = false;
        emit cacheHtmlOk(m_addtion);
    }
}
