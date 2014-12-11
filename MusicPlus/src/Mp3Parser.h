#ifndef MP3PARSER_H
#define MP3PARSER_H
#include <QObject>

class Mp3Parser
{
public:
    Mp3Parser();
    ~Mp3Parser();

    void setSource(const QString& filePath);
    bool existId3v2();
    bool existId3v1();
    int v2TagSize();
    bool findFramHeader(const char* frameId, int& offset, int& size);
    QString getString(const QString& frameID);
    QString getTitle();
    QString getAlbum();
    QString getArtist();
    QByteArray getImage();

private:
    void prepare();

    bool m_v1;
    bool m_v2;

    int m_v2Size;
    char* m_filePath;
};

#endif // MP3PARSER_H
