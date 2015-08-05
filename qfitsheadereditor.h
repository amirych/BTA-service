#ifndef QFITSHEADEREDITOR_H
#define QFITSHEADEREDITOR_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QStringList>

#include <fitsio.h>

#define QFITSHEADEREDITOR_ERROR_OK 0

class QFitsHeaderEditor : public QDialog
{
    Q_OBJECT

public:
    explicit QFitsHeaderEditor(QWidget *parent = nullptr, Qt::WindowFlags flag = 0);
    QFitsHeaderEditor(QString fits_filename, QWidget *parent = nullptr, Qt::WindowFlags flag = 0);
    QFitsHeaderEditor(QString fits_filename, QStringList keynames, QWidget *parent = nullptr, Qt::WindowFlags flag = 0);

    ~QFitsHeaderEditor();

    int getCurrentError() const;

signals:
    void error(int err);

private:
    void setupUI();
    void saveFitsHeader();
    void readFitsHeader(QStringList &keynames);


    QString Fits_filename;
    fitsfile *Fits_file;

    int currentError;

    QTableWidget *editor;

    QPushButton *load_file;
    QPushButton *cancel;
    QPushButton *quit;
};

#endif // QFITSHEADEREDITOR_H
