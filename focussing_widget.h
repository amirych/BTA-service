#ifndef FOCUSSING_WIDGET_H
#define FOCUSSING_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "fits_viewer.h"

class focussing_widget : public QDialog
{
public:
    explicit focussing_widget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    focussing_widget(double start_value, double stop_value, double step_value, QWidget *parent = 0, Qt::WindowFlags f = 0);

    ~focussing_widget();

private:
    void setupUI();

    Fits_viewer *viewer;
    QLineEdit *start_focus_edit_field;
    QLineEdit *stop_focus_edit_field;
    QLineEdit *step_focus_edit_field;
    QLineEdit *window_size_edit_field;
    QPushButton *run_button;
    QPushButton *stop_button;
    QPushButton *quit_button;


};

#endif // FOCUSSING_WIDGET_H
