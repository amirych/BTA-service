#include "bta_service.h"
#include "pf_table.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BTA_service *w = new BTA_service;
    w->show();

//    PF_table pp(210);

//    pp.show();

//    pp.setNewPos(310);

//    Fits_viewer ff("/home/timur/zz10.fts");
//    ff.show();

    return a.exec();
}
