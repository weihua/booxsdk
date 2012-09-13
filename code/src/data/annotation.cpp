#include "onyx/data/annotation.h"

namespace anno
{

Annotation::Annotation()
    : page_(0)
    , is_public_access_(false)
{
}

Annotation::Annotation(const Annotation & right)
    : title_(right.title_)
    , data_(right.data_)
    , rect_list_(right.rect_list_)
    , page_(right.page_)
    , update_time_(right.update_time_)
    , is_public_access_(right.is_public_access_)
{
}

Annotation::Annotation(const QString & title, const QVariant & data)
    : title_(title)
    , data_(data)
    , page_(0)
    , is_public_access_(false)
{
}

Annotation::~Annotation(void)
{
}

namespace {
const int V_1000 = 1000;
const int V_1001 = 1001;

// current version number
const int Data_Version = V_1001;
}

QDataStream & operator << ( QDataStream & out, const Annotation & annotation )
{
    out << Data_Version;
    if (Data_Version == V_1000) {
        out << annotation.data();
        out << annotation.title();
        out << annotation.rect_list();
        out << annotation.page();
        out << annotation.update_time();
    }
    else if (Data_Version == V_1001) {
        out << annotation.data();
        out << annotation.title();
        out << annotation.rect_list();
        out << annotation.page();
        out << annotation.update_time();
        out << annotation.is_public_access();
    }
    return out;
}

QDataStream & operator >> ( QDataStream & in, Annotation & annotation )
{
    int ver;
    in >> ver;
    if (ver == V_1000) {
        in >> annotation.mutable_data();
        in >> annotation.mutable_title();
        in >> annotation.mutable_rect_list();
        in >> annotation.mutable_page();
        in >> annotation.mutable_update_time();
    }
    else if (ver = V_1001) {
        in >> annotation.mutable_data();
        in >> annotation.mutable_title();
        in >> annotation.mutable_rect_list();
        in >> annotation.mutable_page();
        in >> annotation.mutable_update_time();
        in >> annotation.mutable_is_public_access();
    }
    return in;
}

}
