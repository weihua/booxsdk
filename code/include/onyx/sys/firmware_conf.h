// Authors: jacky

/// Public header of the system configuration library.

#ifndef SYS_FIRMWARE_CONF_H__
#define SYS_FIRMWARE_CONF_H__

#include "onyx/base/base.h"
#include <QString>
#include <QtSql/QtSql>

namespace sys
{

class FirmwareConfig
{
public:
    FirmwareConfig();
    ~FirmwareConfig();

private:
    friend class SystemConfig;

    static bool makeSureTableExist(QSqlDatabase& db);
    static void askMeForUpdate(QSqlDatabase& db,bool askme );
    static bool askMeForUpdate(QSqlDatabase& db);

    static void manulUpdate(QSqlDatabase& db,bool byhand);
    static bool manulUpdate(QSqlDatabase& db);

    static void installUpdate(QSqlDatabase& db, bool install);
    static bool installUpdate(QSqlDatabase& db);

};

};

#endif  // SYS_FIRMWARE_CONF_H__
