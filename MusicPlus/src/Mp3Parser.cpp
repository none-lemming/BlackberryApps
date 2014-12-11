#include "Mp3Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <QTextCodec>

Mp3Parser::Mp3Parser()
    : m_v1(false)
    , m_v2(false)
    , m_v2Size(0)
    , m_filePath(NULL)
{
}

Mp3Parser::~Mp3Parser()
{
	 if (m_filePath != NULL)
		 delete [] m_filePath;
}

void Mp3Parser::setSource(const QString &filePath)
{
	qDebug()<<filePath;
    if (m_filePath != NULL)
        delete [] m_filePath;
    QByteArray path = filePath.toLocal8Bit();
    m_filePath = new char[path.size()+1];
    m_filePath[path.size()] = 0;
    strcpy(m_filePath, path.data());
    prepare();
}

void Mp3Parser::prepare()
{
    FILE *fp=NULL;
    m_v1 = false;
    m_v2 = false;
    m_v2Size = 0;
    fp=fopen(m_filePath,"rb");
    if(!fp)
    {
    	qDebug()<<m_filePath;
    	qDebug()<<"unable to open mp3 file";
        return;
    }

    //判断v2标签
    char tag[3] = {};
    fread(tag,3,1,fp);
    if(0 == strncmp(tag,"ID3",3))
        m_v2 = true;

    //判断v1标签
    fseek(fp, 128, SEEK_END);
    fread(tag,3,1,fp);
    if(0 == strncmp(tag,"TAG",3))
        m_v1 = true;

    //提取v2大小
    if (m_v2)
    {
        fseek(fp,6,SEEK_SET);
        uchar size[4];
        fread(size, 4, 1, fp);
        m_v2Size = 0;
        for (int i=0; i<4; i++)
        {
            m_v2Size = m_v2Size<<7;
            m_v2Size += size[i];
        }
    }

    fclose(fp);
}

bool Mp3Parser::findFramHeader(const char* frameId, int& offset, int& size)
{
    if (!existId3v2())
        return false;

    FILE *fp=NULL;
    fp=fopen(m_filePath,"rb");

    char id[4] = {};
    uchar len[4] = {};
    fseek(fp, 10, SEEK_SET);
    while(ftell(fp) < v2TagSize()+10)
    {
        //qDebug()<<ftell(fp);
        fread(id, 4, 1, fp);
        fread(len, 4, 1, fp);
        fseek(fp, 2, SEEK_CUR);
        size = 0;
        //size = len[0]*0x1000000 + len[1]*0x10000 + len[2]*0x100 + len[3];
        for (int i=0; i<4; i++)
        {
            size = size<<8;
            size += len[i];
        }
        //printf("%c%c%c%c %d\n", id[0], id[1], id[2], id[3], size);
        if (0 == strncmp(id, frameId, 4))
        {
            //qDebug()<<int(len[0])<<" "<<int(len[1])<<" "<<len[2]<<" "<<(len[3]);
            offset = ftell(fp);
            fclose(fp);
            return true;
        }
        else if (id[0] == 0)
            break;

        fseek(fp, size, SEEK_CUR);
    }

    fclose(fp);
    return false;
}

QByteArray Mp3Parser::getImage()
{
    int offset,size;
    QByteArray ret;
    if (!findFramHeader("APIC", offset, size))
        return ret;

    int beginOffset;
    uchar begin[2] = {0xFF,0xD8};
    //ushort end = 0xFFD9;
    uchar buff[2] = {};
    FILE *fp=NULL;
    fp=fopen(m_filePath,"rb");
    fseek(fp , offset, SEEK_CUR);
    //寻找图片开始标志
    //qDebug()<<"begin";
    //qDebug()<<"size:"<<size;
    //qDebug()<<"offset:"<<offset;
    while (true)
    {
        fread(&buff, 2, 1, fp);
        if (begin[0]==buff[0] && begin[1]==buff[1])
        {
//            int i = buff[0];
//            printf("%x\n", (i<<8)+buff[1]);
//            i = begin[0];
//            printf("%x\n", (i<<8)+begin[1]);
            beginOffset = ftell(fp) - 2;
            break;
        }
        fseek(fp, -1, SEEK_CUR);
        if (ftell(fp) >= offset + size)
            return ret;
    }

    //qDebug()<<"end";
    //qDebug()<<"size:"<<offset+size-beginOffset;
    fseek(fp, beginOffset, SEEK_SET);
    ret.resize(offset+size-beginOffset);
    fread(ret.data(), offset+size-beginOffset, 1, fp);
    //qDebug()<<"size:"<<ret.size();

    return ret;
}

QString Mp3Parser::getString(const QString &frameID)
{
    int offset,size;
    if (!findFramHeader(frameID.toLocal8Bit().data(), offset, size))
        return QString();

    FILE *fp=NULL;
    char code;
    fp=fopen(m_filePath,"rb");
    fseek(fp, offset, SEEK_SET);
    fread(&code, 1, 1, fp);
    qDebug()<<"code:"<<(int)code;

    QString ret;
	if (code==1 || code==2)
	{
		ushort* str = new ushort[size/2];
		fread(str, 2, size/2, fp);
		ret = QString::fromUtf16(str, size/2);
		delete [] str;
	}
	else if (code==0 || code==3)
	{
		char* str = new char[size-1];
		fread(str, 1, size-1, fp);
		qDebug()<<"-----------";
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
		ret = QString::fromLocal8Bit(str, size-1);
		qDebug()<<"ret:"<<size-1<<ret;
		delete [] str;
	}

    fclose(fp);
    return ret;
}

QString Mp3Parser::getTitle()
{
    return getString("TIT2");
}

QString Mp3Parser::getAlbum()
{
    return getString("TALB");
}

QString Mp3Parser::getArtist()
{
    QString ret = getString("TPE2");
    if (ret.isEmpty())
        ret = getString("TPE1");
    return ret;
}

bool Mp3Parser::existId3v1()
{
    return m_v1;
}

bool Mp3Parser::existId3v2()
{
    return m_v2;
}

int Mp3Parser::v2TagSize()
{
    return m_v2Size;
}
