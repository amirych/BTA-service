#include "qfitsheadereditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>


QFitsHeaderEditor::QFitsHeaderEditor(QString fits_filename, QStringList keynames, QWidget *parent, Qt::WindowFlags flag): QDialog(parent,flag),
    Fits_filename(""), Fits_file(NULL), currentError(QFITSHEADEREDITOR_ERROR_OK)
{
//    Fits_filename = "";
//    Fits_file = NULL;
//    currentError = QFITSHEADEREDITOR_ERROR_OK;

    if ( !fits_filename.isEmpty() ) {
        readFitsHeader(keynames);
    } else {
        Fits_filename = fits_filename;
    }

    setModal(false);
    setupUI();
}


QFitsHeaderEditor::QFitsHeaderEditor(QString fits_filename, QWidget *parent, Qt::WindowFlags flag):
    QFitsHeaderEditor(fits_filename,QStringList(),parent,flag)
{

}


QFitsHeaderEditor::QFitsHeaderEditor(QWidget *parent, Qt::WindowFlags flag): QFitsHeaderEditor("",QStringList(),parent, flag)
{

}


QFitsHeaderEditor::~QFitsHeaderEditor()
{
    qDebug() << "FITSHEADEREDITOR destructor";
}

            /*    Public methods     */

int QFitsHeaderEditor::getCurrentError() const
{
    return currentError;
}


            /*    Private methods    */

void QFitsHeaderEditor::setupUI()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    editor = new QTableWidget(this);

    QWidget *control_buttons_widget = new QWidget(this);
    QHBoxLayout *control_buttons_layout = new QHBoxLayout(control_buttons_widget);

    load_file = new QPushButton("Load",control_buttons_widget);
    cancel = new QPushButton("Cancel",control_buttons_widget);
    quit = new QPushButton("Quit",control_buttons_widget);

    control_buttons_layout->addWidget(load_file);
    control_buttons_layout->addWidget(cancel);
    control_buttons_layout->addWidget(quit);


    main_layout->addWidget(editor);
    main_layout->addWidget(control_buttons_widget);
}


void QFitsHeaderEditor::readFitsHeader(QStringList &keynames)
{

}

void QFitsHeaderEditor::saveFitsHeader()
{

}

