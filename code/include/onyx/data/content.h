#ifndef ONYX_CONTENT_H_
#define ONYX_CONTENT_H_

#include <QtCore/QtCore>

namespace content
{

/// Represent a single book.
struct Book
{
    QString bookid;
    QString url;
    int internalid;
};
typedef QVector<Book> Books;

QDataStream & operator<< ( QDataStream & out, const Book & book );
QDataStream & operator>> ( QDataStream & in, Book & book );

bool save( QByteArray & data, const Books & books );
bool load( QByteArray & data, Books & books );


struct DownloadItem
{
public:
    DownloadItem();
    ~DownloadItem();

public:
    QString url;
    QString md5;        ///< md5 value from server side.
    int size;           ///< Total size.

    QString path;       ///< Local file path.
    int downloaded;     ///< Downloaded file size must be less than size.
    QString downloaded_md5; ///< Md5 value of downloaded part.
};
typedef QVector<DownloadItem> DownloadList;

QDataStream & operator<< ( QDataStream & out, const DownloadItem & item );
QDataStream & operator>> ( QDataStream & in, DownloadItem & item );

bool save( QByteArray & data, const DownloadList & list );
bool load( QByteArray & data, DownloadList & list );

struct Firmware
{
    QString version;
    QString finger_print;
    QString change_log;
    QString priority;
    DownloadList list;

    Firmware & operator = (const Firmware &rhs)
    {
        if (this != &rhs)
        {
            version = rhs.version;
            finger_print = rhs.finger_print;
            change_log = rhs.change_log;
            priority = rhs.priority;
            list = rhs.list;
        }
        return *this;
    }
};

typedef QVector<Firmware> Firmwares;


}   // namespace content

#endif
