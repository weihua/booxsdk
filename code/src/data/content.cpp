
#include <QBuffer>
#include "onyx/data/content.h"


namespace content
{


QDataStream & operator<< ( QDataStream & out, const Book & book )
{
    out << book.bookid;
    out << book.url;
    out << book.internalid;
    return out;
}

QDataStream & operator>> ( QDataStream & in, Book & book )
{
    in >> book.bookid;
    in >> book.url;
    in >> book.internalid;
    return in;
}

bool save( QByteArray & data, const QVector<Book> & books )
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << books;
    return true;
}

bool load( QByteArray & data, QVector<Book> & books )
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream >> books;
    return true;
}

DownloadItem::DownloadItem()
: size(0)
, downloaded(0)
{
}

DownloadItem::~DownloadItem()
{
}

QDataStream & operator<< ( QDataStream & out, const DownloadItem & item )
{
    out << item.url;
    out << item.md5;
    out << item.size;

    out << item.path;
    out << item.downloaded;
    out << item.downloaded_md5;
    return out;
}

QDataStream & operator>> ( QDataStream & in, DownloadItem & item )
{
    in >> item.url;
    in >> item.md5;
    in >> item.size;

    in >> item.path;
    in >> item.downloaded;
    in >> item.downloaded_md5;
    return in;
}

bool save( QByteArray & data, const DownloadList & list )
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << list;
    return true;
}

bool load( QByteArray & data, DownloadList & list )
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream >> list;
    return true;
}

}
