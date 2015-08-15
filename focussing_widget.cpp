#include "focussing_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QFrame>

#include <QDebug>

focussing_widget::focussing_widget(double start_value, double stop_value, double step_value, QWidget *parent, Qt::WindowFlags f): QDialog(parent,f)
{

    setupUI();
//    this->setModal(false);
    this->setModal(true);

    start_focus_edit_field->setText(QString::number(start_value));
    stop_focus_edit_field->setText(QString::number(stop_value));
    step_focus_edit_field->setText(QString::number(step_value));

}



focussing_widget::focussing_widget(QWidget *parent, Qt::WindowFlags f): focussing_widget(0.0,0.0,0.0,parent,f)
{

}



focussing_widget::~focussing_widget()
{
    qDebug() << "destr";
}



            /*      Private methods     */

void focussing_widget::setupUI()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    QWidget *range_widget = new QWidget(this);
    QHBoxLayout * range_layout = new QHBoxLayout(range_widget);

    QWidget *buttons_widget = new QWidget(this);
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_widget);

    QFrame *ff = new QFrame(this);
    ff->setFixedSize(512,512);
    ff->setFrameShape(QFrame::Box);
    ff->setFrameShadow(QFrame::Plain);
    QVBoxLayout *ffl = new QVBoxLayout(ff);
    ffl->setAlignment(Qt::AlignHCenter);
    ffl->setMargin(1);
//    viewer = new Fits_viewer("",this);
    viewer = new Fits_viewer("/home/timur/zz10.fts",ff);
    ffl->addWidget(viewer);
//    viewer->setFixedSize(512,512);

//    main_layout->addWidget(viewer);
    main_layout->addWidget(ff);

    start_focus_edit_field = new QLineEdit(range_widget);
    stop_focus_edit_field = new QLineEdit(range_widget);
    step_focus_edit_field = new QLineEdit(range_widget);

    range_layout->addWidget(start_focus_edit_field);
    range_layout->addWidget(stop_focus_edit_field);
    range_layout->addWidget(step_focus_edit_field);

    main_layout->addWidget(range_widget);

    run_button = new QPushButton("Run",buttons_widget);
    stop_button = new QPushButton("Stop",buttons_widget);
    quit_button = new QPushButton("Quit",buttons_widget);

    buttons_layout->addWidget(run_button);
    buttons_layout->addWidget(stop_button);
    buttons_layout->addWidget(quit_button);

    connect(quit_button,SIGNAL(clicked()),this,SLOT(accept()));

    main_layout->addWidget(buttons_widget);

}
