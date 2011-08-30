#include "onyx/cms/media_info_manager.h"

#include "onyx/base/device.h"
#include "onyx/sys/sys_conf.h"
#include <QVector>

using namespace sys;

namespace cms
{

MediaInfoManager::MediaInfoManager()
{

}

MediaInfoManager::~MediaInfoManager()
{
}

void MediaInfoManager::recurseCollect(const QString &sub_dir,
        const QStringList &name_filters, QStringList &path_list)
{
    QDir dir(sub_dir);
    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
    for (QFileInfoList::iterator itr = dirList.begin();
                itr != dirList.end(); ++itr)
    {
        recurseCollect(itr->absoluteFilePath(), name_filters, path_list);
    }

    QDir::Filters filters = QDir::Files|QDir::NoDotAndDotDot|QDir::CaseSensitive;
    QFileInfoList target_files = dir.entryInfoList(name_filters, filters);
    for (QFileInfoList::iterator itr = target_files.begin();
            itr != target_files.end(); ++itr)
    {
        QString item = itr->absoluteFilePath();
        if (!path_list.contains(item))
        {
            path_list.push_back(item);
        }
    }
}

QStringList MediaInfoManager::getFullFilter(const QStringList &lower_filter)
{
    QStringList full_filter(lower_filter);
    for (int i = 0; i < lower_filter.size(); i++)
    {
        full_filter.push_back(lower_filter.at(i).toUpper());
    }
    return full_filter;
}

void MediaInfoManager::setFilterForBooks(QStringList &filter)
{
    // get extensions from service configuration
    QStringList temp;
    Service service;
    SystemConfig conf;
    conf.nabooReaderService(service);
    temp << service.mutable_extensions();
    conf.onyxReaderService(service);
    temp << service.mutable_extensions();
    conf.htmlReaderService(service);
    temp << "chm";
    conf.webBrowserService(service);
    temp << service.mutable_extensions();
    temp << "djv" << "djvu";

    int size = temp.size();
    for (int i=0; i<size; i++)
    {
        QString ext_item = temp.at(i);
        ext_item.prepend("*.");
        filter.push_back(ext_item);
    }
}

QStringList MediaInfoManager::mediaInfo(MediaType type)
{
    MediaDB db;
    return db.list(type);
}

void MediaInfoManager::mergeList(MediaDB &db, MediaType type,
        QStringList &new_list, bool is_sd_card)
{
    MediaInfoList origin = db.list(type);
    int size = origin.size();
    for (int i=0; i<size; i++)
    {
        QString prefix(LIBRARY_ROOT);
        if (is_sd_card)
        {
            prefix = SDMMC_ROOT;
        }

        if (!origin.at(i).startsWith(prefix))
        {
            new_list.push_front(origin.at(i));
        }
    }
}

void MediaInfoManager::update(bool is_sd_card)
{
    MediaDB db;

    // collect pictures
    QStringList pictures_list;
    QStringList lower_filter("*.png");
    lower_filter << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif"
            << "*.tif" << "*.tiff";
    QStringList full_filter = getFullFilter(lower_filter);
    if (is_sd_card)
    {
        recurseCollect(SDMMC_ROOT, full_filter, pictures_list);
    }
    else
    {
        recurseCollect(LIBRARY_ROOT, full_filter, pictures_list);
    }

    mergeList(db, PICTURES, pictures_list, is_sd_card);
    db.update(PICTURES, pictures_list);

    // collect books
    QStringList books_list;
    lower_filter.clear();
    setFilterForBooks(lower_filter);
    full_filter = getFullFilter(lower_filter);
    if (is_sd_card)
    {
        recurseCollect(SDMMC_ROOT, full_filter, books_list);
    }
    else
    {
        recurseCollect(LIBRARY_ROOT, full_filter, books_list);
    }

    mergeList(db, BOOKS, books_list, is_sd_card);
    db.update(BOOKS, books_list);

    // collect music
    QStringList music_list;
    lower_filter.clear();
    lower_filter << "*.mp3" << "*.wav";
    full_filter = getFullFilter(lower_filter);
    if (is_sd_card)
    {
        recurseCollect(SDMMC_ROOT, full_filter, music_list);
    }
    else
    {
        recurseCollect(LIBRARY_ROOT, full_filter, music_list);
    }

    mergeList(db, MUSIC, music_list, is_sd_card);
    db.update(MUSIC, music_list);
}

}   // namespace cms
