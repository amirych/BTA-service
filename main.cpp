#include "bta_service.h"
#include "pf_table.h"
#include "qfitsheadereditor.h"
#include "focussing_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BTA_service *w = new BTA_service;
    w->show();

//    focussing_widget *ff = new focussing_widget(88.5,92.5,0.5);
//    ff->show();
//    ff->raise();
//    ff->activateWindow();
//    ff->exec();

//    QFitsHeaderEditor *ee = new QFitsHeaderEditor();

//    ee->show();

//    PF_table pp(210);

//    pp.show();

//    pp.setNewPos(310);

//    Fits_viewer ff("/home/timur/zz10.fts");
//    ff.show();

    return a.exec();
}
