
#include "onyx/sys/firmware_conf.h"
#include "onyx/base/device.h"

namespace sys
{

FirmwareConfig::FirmwareConfig()
{
}

FirmwareConfig::~FirmwareConfig()
{
}

bool FirmwareConfig::makeSureTableExist(QSqlDatabase& database)
{
    QSqlQuery query(database);
    bool ok = query.exec("create table if not exists firmware_conf ("
                         "key text primary key, "
                         "value int "
                         ")");
    if (ok)
    {
        return query.exec("create index if not exists key_index on firmware_conf (key) ");
    }
    return false;
}

void FirmwareConfig::askMeForUpdate(QSqlDatabase& db,bool askme)
{
    QSqlQuery query(db);
    query.prepare( "INSERT OR REPLACE into firmware_conf (key, value) values(?, ?)");
    query.addBindValue("non-critical");
    query.addBindValue(askme?1:0);
    query.exec();
}
bool FirmwareConfig::askMeForUpdate(QSqlDatabase& db)
{
    bool ret=true;
    QSqlQuery query(db);
    query.prepare( "select value from firmware_conf where key = ?");
    query.addBindValue("non-critical");
    if (query.exec() && query.next())
    {
        ret = query.value(0).toInt();
    }
    return ret;
}

void FirmwareConfig::manulUpdate(QSqlDatabase& db,bool byhand)
{
    QSqlQuery query(db);
    query.prepare( "INSERT OR REPLACE into firmware_conf (key, value) values(?, ?)");
    query.addBindValue("byhand");
    query.addBindValue(byhand?1:0);
    query.exec();

}
bool FirmwareConfig::manulUpdate(QSqlDatabase& db)
{
    bool ret=false;
    QSqlQuery query(db);
    query.prepare( "select value from firmware_conf where key = ?");
    query.addBindValue("byhand");
    if (query.exec() && query.next())
    {
        ret = query.value(0).toInt();
    }
    return ret;
}

void FirmwareConfig::installUpdate(QSqlDatabase& db, bool install)
{
    QSqlQuery query(db);
    query.prepare( "INSERT OR REPLACE into firmware_conf (key, value) values(?, ?)");
    query.addBindValue("install");
    query.addBindValue(install? 1 : 0);
    query.exec();
}

bool FirmwareConfig::installUpdate(QSqlDatabase& db)
{
    bool install = false;
    QSqlQuery query(db);
    query.prepare( "select value from firmware_conf where key = ?");
    query.addBindValue("install");
    if (query.exec() && query.next())
    {
        install = query.value(0).toBool();
    }
    return install;
}


}
