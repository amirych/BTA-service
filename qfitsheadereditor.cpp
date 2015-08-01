#include "qfitsheadereditor.h"


QFitsHeaderEditor::QFitsHeaderEditor(QString fits_filename, QStringList keynames, QWidget *parent): QWidget(parent),
    Fits_filename(""), Fits_file(NULL), currentError(QFITSHEADEREDITOR_ERROR_OK)
{
    if ( !fits_filename.isEmpty() ) {
        readFitsHeader(keynames);
    } else {
        Fits_filename = fits_filename;
    }

    setupUI();
}


QFitsHeaderEditor::QFitsHeaderEditor(QString fits_filename, QWidget *parent):
    QFitsHeaderEditor(fits_filename,QStringList(),parent)
{

}


QFitsHeaderEditor::QFitsHeaderEditor(QWidget *parent): QFitsHeaderEditor("",parent)
{

}


            /*    Public methods     */

int QFitsHeaderEditor::getCurrentError() const
{
    return currentError;
}


            /*    Private methods    */

void QFitsHeaderEditor::setupUI()
{

}


void QFitsHeaderEditor::readFitsHeader(QStringList &keynames)
{

}

void QFitsHeaderEditor::saveFitsHeader()
{

}

