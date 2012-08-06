#include "onyx/data/bookmark.h"

using namespace cms;

namespace vbf
{

Bookmark::Bookmark()
{
}

Bookmark::Bookmark(const Bookmark & right)
    : title_(right.title_)
    , data_(right.data_)
    , update_time_(right.update_time_)
{
}

Bookmark::Bookmark(const QString & title, const QVariant & data)
: title_(title)
, data_(data)
{
}

Bookmark::~Bookmark(void)
{
}

// begins from 1000
static int Data_Version = 1000;

QDataStream & operator<< ( QDataStream & out, const Bookmark & bookmark )
{
    out << Data_Version;
    out << bookmark.title();
    out << bookmark.data();
    out << bookmark.update_time();
    return out;
}

QDataStream & operator>> ( QDataStream & in, Bookmark & bookmark )
{
    int ver;
    in >> ver;
    if (ver == 1000) {
        in >> bookmark.mutable_title();
        in >> bookmark.mutable_data();
        in >> bookmark.mutable_update_time();
    }
    return in;
}

bool removeBookmark(Bookmarks & bookmarks, const Bookmark & bookmark)
{
    BookmarksIter pos = std::find(bookmarks.begin(), bookmarks.end(), bookmark);
    if (pos != bookmarks.end())
    {
        bookmarks.erase(pos);
        return true;
    }
    return false;
}


/// Store bookmark for the document.
bool loadBookmarks(cms::ContentManager & db,
                   const QString &doc_path,
                   Bookmarks & bookmarks)
{
    // Retrieve document information.
    QFileInfo info(doc_path);
    ContentNode node;
    node.mutable_name() = info.fileName();
    node.mutable_location() = info.path();
    node.mutable_size() = info.size();
    if (!db.getContentNode(node))
    {
        db.createContentNode(node);
        return true;
    }

    // Read blob from the database.
    cms_blob data;
    db.getBookmarks(node.id(), data);
    if (data.size() <= 0)
    {
        return true;
    }

    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream >> bookmarks;
    return true;
}

/// Load bookmarks for the document.
bool saveBookmarks(cms::ContentManager & db,
                   const QString &doc_path,
                   const Bookmarks & bookmarks)
{

    // Retrieve document information.
    QFileInfo info(doc_path);
    ContentNode node;
    node.mutable_name() = info.fileName();
    node.mutable_location() = info.path();
    node.mutable_size() = info.size();
    if (!db.getContentNode(node))
    {
        db.createContentNode(node);
    }

    // Store the bookmarks.
    cms_blob data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << bookmarks;
    db.updateBookmarks(node.id(), data);
    return true;
}

}
