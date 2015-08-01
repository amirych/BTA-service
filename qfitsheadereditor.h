#ifndef QFITSHEADEREDITOR_H
#define QFITSHEADEREDITOR_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QStringList>

#include <fitsio.h>


class QFitsHeaderEditor : public QWidget
{
    Q_OBJECT

public:
    explicit QFitsHeaderEditor(QWidget *parent = nullptr);
    QFitsHeaderEditor(QString fits_filename, QWidget *parent);
    QFitsHeaderEditor(QString fits_filename, QStringList keynames, QWidget *parent);


signals:
    void error(int err);

private:
    void setupUI();
    void saveFitsHeader();
    void readFitsHeader(QStringList &keynames);


    QString Fits_filename;
    fitsfile *Fits_file;

    QPushButton *load_file;
    QPushButton *cancel;
    QPushButton *quit;
};

#endif // QFITSHEADEREDITOR_H