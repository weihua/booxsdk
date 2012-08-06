#include "onyx/data/annotation.h"

namespace anno
{

Annotation::Annotation()
{
}

Annotation::Annotation(const Annotation & right)
    : title_(right.title_)
    , data_(right.data_)
    , rect_list_(right.rect_list_)
    , page_(right.page_)
    , update_time_(right.update_time_)
{
}

Annotation::Annotation(const QString & title, const QVariant & data)
    : title_(title)
    , data_(data)
{
}

Annotation::~Annotation(void)
{
}

// begins from 1000
static int Data_Version = 1000;

QDataStream & operator << ( QDataStream & out, const Annotation & annotation )
{
    out << Data_Version;
    out << annotation.data();
    out << annotation.title();
    out << annotation.rect_list();
    out << annotation.page();
    out << annotation.update_time();
    return out;
}

QDataStream & operator >> ( QDataStream & in, Annotation & annotation )
{
    int ver;
    in >> ver;
    if (ver == 1000) {
        in >> annotation.mutable_data();
        in >> annotation.mutable_title();
        in >> annotation.mutable_rect_list();
        in >> annotation.mutable_page();
        in >> annotation.mutable_update_time();
    }
    return in;
}

}
