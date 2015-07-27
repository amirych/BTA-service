#include "bta_service.h"
#include "pf_table.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    BTA_service w;
//    w.show();

    PF_table pp(210);

    pp.show();

    return a.exec();
}
