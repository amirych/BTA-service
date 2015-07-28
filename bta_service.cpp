#include "bta_service.h"

#include <iostream>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpacerItem>
#include <QFont>
#include <QFontMetrics>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QMargins>

#include <QStringList>

static QStringList expTypeItems = QStringList({"obj","bias","flat"});
static QStringList binValues = QStringList({"1","2","3","4","5"});



                /*      Constructors      */

BTA_service::BTA_service(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

BTA_service::~BTA_service()
{

}


        /*     Private slots    */

void BTA_service::changeFrameType(QString type)
{
}


void BTA_service::changeBinX(int index)
{
}

void BTA_service::changeBinY(int index)
{
}


        /*     Private methods     */

void BTA_service::setupUI()
{
    QString str;
    int dw;

    central_widget = new QFrame(this);
    this->setCentralWidget(central_widget);
    central_widget->setFrameShape(QFrame::Box);
    central_widget->setFrameShadow(QFrame::Raised);


            /*  layouts definitions */

    QWidget *left_panel = new QWidget(central_widget);
    QVBoxLayout *left_panel_layout = new QVBoxLayout(left_panel);

    QWidget *right_panel = new QWidget(central_widget);
    QVBoxLayout *right_panel_layout = new QVBoxLayout(right_panel);

    QWidget *file_exp_widget = new QWidget(left_panel);
    QHBoxLayout *file_exp_layout = new QHBoxLayout(file_exp_widget);

    QWidget *shift_table_widget = new QWidget(left_panel);
    QHBoxLayout *shift_table_layout = new QHBoxLayout(shift_table_widget);
    shift_table_layout->setMargin(0);


                                    /*      LEFT PANEL      */


            /* current file and exposure control */

    QFont fn;
    fn = this->font();
    QFontMetrics fnm(fn);
    int label_width = fnm.boundingRect("seeing").width();

    current_file_root_widget = new QFrame(file_exp_widget);
    current_file_root_widget->setFrameShape(QFrame::Box);
    current_file_root_widget->setFrameShadow(QFrame::Plain);

    QVBoxLayout *current_file_layout = new QVBoxLayout(current_file_root_widget);

    // object name

    QHBoxLayout *obj_layout = new QHBoxLayout(current_file_root_widget);
    obj_name_label = new QLabel("object ",current_file_root_widget);
    obj_name_label->resize(label_width,obj_name_label->height());
    obj_name_input_field = new QLineEdit(current_file_root_widget);
    obj_layout->addWidget(obj_name_label);
    obj_layout->addWidget(obj_name_input_field);

    current_file_layout->addLayout(obj_layout);

    // seeing and clouds

    QHBoxLayout *seeing_layout = new QHBoxLayout(current_file_root_widget);
    seeing_label = new QLabel("seeing",current_file_root_widget);
    seeing_input_field = new QLineEdit(current_file_root_widget);
    seeing_input_field->setMaximumWidth(50);
    seeing_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *seeing_val = new QDoubleValidator(0.0,20.0,1,current_file_root_widget);
    seeing_input_field->setValidator(seeing_val);

    QSpacerItem *seeing_space = new QSpacerItem(30,10);
    clouds_label = new QLabel("clouds%",current_file_root_widget);
    clouds_input_field = new QLineEdit(current_file_root_widget);
    clouds_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *clouds_val = new QDoubleValidator(0.0,100.0,1,current_file_root_widget);
    clouds_input_field->setValidator(clouds_val);

    seeing_layout->addWidget(seeing_label);
    seeing_layout->addWidget(seeing_input_field);
    seeing_layout->addSpacerItem(seeing_space);
    seeing_layout->addWidget(clouds_label);
    seeing_layout->addWidget(clouds_input_field);

    current_file_layout->addLayout(seeing_layout);

    // comment field

    QHBoxLayout *comment_layout = new QHBoxLayout(current_file_root_widget);
    comment_label = new QLabel("comment",current_file_root_widget);
    comment_input_field = new QLineEdit(current_file_root_widget);
    comment_layout->addWidget(comment_label);
    comment_layout->addWidget(comment_input_field);

    current_file_layout->addLayout(comment_layout);

    // current path

    QHBoxLayout *current_path_layout = new QHBoxLayout(current_file_root_widget);
    current_path_label = new QLabel("path",current_file_root_widget);
    current_path_input_field = new QLineEdit(current_file_root_widget);
    current_path_create_button = new QPushButton("create",current_file_root_widget);
    current_path_layout->addWidget(current_path_label);
    current_path_layout->addWidget(current_path_input_field);
    current_path_layout->addWidget(current_path_create_button);

    current_file_layout->addLayout(current_path_layout);

    // current filename

    QHBoxLayout *file_layout = new QHBoxLayout(current_file_root_widget);
    current_file_label = new QLabel("file",current_file_root_widget);
    current_file_input_field = new QLineEdit(current_file_root_widget);
    QSpacerItem *file_space = new QSpacerItem(30,10);
    Nexp_label = new QLabel("N exp",current_file_root_widget);

    Nexp_input_field = new QLineEdit(current_file_root_widget);
    str = QString(BTA_SERVICE_NEXP_MAX_DIGITS+1,'0');
    dw = fnm.width(str);
    Nexp_input_field->setMaximumWidth(dw);
    QIntValidator *Nexp_input_field_val = new QIntValidator(1,pow(10,BTA_SERVICE_NEXP_MAX_DIGITS)-1,current_file_root_widget);
    Nexp_input_field->setValidator(Nexp_input_field_val);
//    str = QString(BTA_SERVICE_NEXP_MAX_DIGITS,'0');
//    Nexp_input_field->setInputMask(str); // set mask of digits
    Nexp_input_field->setAlignment(Qt::AlignRight);

    file_layout->addWidget(current_file_label);
    file_layout->addWidget(current_file_input_field);
    file_layout->addSpacerItem(file_space);
    file_layout->addWidget(Nexp_label);
    file_layout->addWidget(Nexp_input_field);

    current_file_layout->addLayout(file_layout);

    // auto increment

    QHBoxLayout *auto_inc_layout = new QHBoxLayout(current_file_root_widget);
    auto_increment_check = new QCheckBox("auto increment file",current_file_root_widget);
    auto_inc_layout->addWidget(auto_increment_check);
    auto_increment_check->setCheckState(Qt::Checked);

    current_file_layout->addLayout(auto_inc_layout);


        /*  frame type, its geometry and exposure control */

//    QIntValidator *coords_val = new QIntValidator(current_exp_root_frame);

    current_exp_root_widget = new QFrame(file_exp_widget);
    current_exp_root_widget->setFrameShape(QFrame::Box);
    current_exp_root_widget->setFrameShadow(QFrame::Plain);

    QVBoxLayout *current_exp_layout = new QVBoxLayout(current_exp_root_widget);

    // frame type and exp time

    QWidget *frame_type_exp = new QWidget(current_exp_root_widget);
    QHBoxLayout *frame_type_layout = new QHBoxLayout(frame_type_exp);
    frame_type_layout->setMargin(0);

    exp_type_combo = new QComboBox(frame_type_exp);
    exp_type_combo->addItems(expTypeItems);
    this->connect(exp_type_combo,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeFrameType(QString)));

    exp_time = new QLineEdit(frame_type_exp);
    QDoubleValidator *exp_valid = new QDoubleValidator(0.0,1000000.0,5,exp_time);
    exp_time->setValidator(exp_valid);
    exp_time->setAlignment(Qt::AlignRight);

    frame_type_layout->addWidget(exp_type_combo,2);
    frame_type_layout->addWidget(exp_time,3);

    current_exp_layout->addWidget(frame_type_exp);

    // X-coordinate options

    dw = fnm.width("WWWWW"); // four digit + 1

    QWidget *X1_X2_binX = new QWidget(current_exp_root_widget);
    QHBoxLayout *X1_X2_binX_layout = new QHBoxLayout(X1_X2_binX);
    X1_X2_binX_layout->setMargin(0);

    X1_label = new QLabel("X1",X1_X2_binX);
    X1_input_field = new QLineEdit(X1_X2_binX);
    X1_input_field->setMaximumWidth(dw);
    X1_input_field->setAlignment(Qt::AlignRight);

    X2_label = new QLabel("X2",X1_X2_binX);
    X2_input_field = new QLineEdit(X1_X2_binX);
    X2_input_field->setMaximumWidth(dw);
    X2_input_field->setAlignment(Qt::AlignRight);

    binX_label = new QLabel("binX",X1_X2_binX);
    binX_combo = new QComboBox(X1_X2_binX);
    binX_combo->addItems(binValues);
//    binX_combo->setEditable(true);

    X1_X2_binX_layout->addWidget(X1_label);
    X1_X2_binX_layout->addWidget(X1_input_field);
    X1_X2_binX_layout->addStretch(2);
    X1_X2_binX_layout->addWidget(X2_label);
    X1_X2_binX_layout->addWidget(X2_input_field);
    X1_X2_binX_layout->addStretch(5);
    X1_X2_binX_layout->addWidget(binX_label);
    X1_X2_binX_layout->addWidget(binX_combo);

//    current_exp_layout->addWidget(X1_X2_binX,1);

    // Y-coordinate options

    QWidget *Y1_Y2_binY = new QWidget(current_exp_root_widget);
    QHBoxLayout *Y1_Y2_binY_layout = new QHBoxLayout(Y1_Y2_binY);
    Y1_Y2_binY_layout->setMargin(0);

    Y1_label = new QLabel("Y1",Y1_Y2_binY);
    Y1_input_field = new QLineEdit(Y1_Y2_binY);
    Y1_input_field->setMaximumWidth(dw);
    Y1_input_field->setAlignment(Qt::AlignRight);

    Y2_label = new QLabel("Y2",Y1_Y2_binY);
    Y2_input_field = new QLineEdit(Y1_Y2_binY);
    Y2_input_field->setMaximumWidth(dw);
    Y2_input_field->setAlignment(Qt::AlignRight);

    binY_label = new QLabel("binY",Y1_Y2_binY);
    binY_combo = new QComboBox(Y1_Y2_binY);
    binY_combo->addItems(binValues);
//    binY_combo->setEditable(true);

    Y1_Y2_binY_layout->addWidget(Y1_label);
    Y1_Y2_binY_layout->addWidget(Y1_input_field);
    Y1_Y2_binY_layout->addStretch(2);
    Y1_Y2_binY_layout->addWidget(Y2_label);
    Y1_Y2_binY_layout->addWidget(Y2_input_field);
    Y1_Y2_binY_layout->addStretch(5);
    Y1_Y2_binY_layout->addWidget(binY_label);
    Y1_Y2_binY_layout->addWidget(binY_combo);

//    current_exp_layout->addWidget(Y1_Y2_binY,1);

    QWidget *frame_geometry = new QWidget(current_file_root_widget);
    QFormLayout *frame_geometry_layout = new QFormLayout(frame_geometry);
    frame_geometry_layout->setMargin(0);


    frame_geometry_layout->addRow(X1_X2_binX);
    frame_geometry_layout->addRow(Y1_Y2_binY);

    current_exp_layout->addWidget(frame_geometry);
    current_exp_layout->addStretch(5);

    // start, stop and init buttons

    QWidget *exp_control_buttons = new QWidget(current_exp_root_widget);
    QHBoxLayout *exp_control_buttons_layout = new QHBoxLayout(exp_control_buttons);
    exp_control_buttons_layout->setMargin(0);

    exp_start_button = new QPushButton("START",exp_control_buttons);
    exp_stop_button = new QPushButton("STOP",exp_control_buttons);
    ccd_init_button = new QPushButton("Initialization CCD",exp_control_buttons);


    exp_control_buttons_layout->addWidget(exp_start_button);
    exp_control_buttons_layout->addWidget(exp_stop_button);
    exp_control_buttons_layout->addWidget(ccd_init_button);

    current_exp_layout->addWidget(exp_control_buttons,2);
//    current_exp_layout->addStretch(1);

    // focus, header and EXIT buttons

    QWidget *focus_header_exit_buttons = new QWidget(current_exp_root_widget);
    QHBoxLayout *focus_header_exit_buttons_layout = new QHBoxLayout(focus_header_exit_buttons);
    focus_header_exit_buttons_layout->setMargin(0);

    focussing_button = new QPushButton("focussing",focus_header_exit_buttons);
    header_button  = new QPushButton("edit header",focus_header_exit_buttons);
    app_exit_button = new QPushButton("EXIT",focus_header_exit_buttons);

    focus_header_exit_buttons_layout->addWidget(focussing_button);
    focus_header_exit_buttons_layout->addWidget(header_button);
    focus_header_exit_buttons_layout->addWidget(app_exit_button);

    current_exp_layout->addWidget(focus_header_exit_buttons,2);
    current_exp_layout->addStretch(1);


    file_exp_layout->setMargin(0);
    file_exp_layout->addWidget(current_file_root_widget);
    file_exp_layout->addWidget(current_exp_root_widget);


        /*  Log window  */

    log_root_widget = new QFrame(left_panel);
    log_root_widget->setFrameShape(QFrame::Box);
    log_root_widget->setFrameShadow(QFrame::Plain);

    QVBoxLayout *log_window_layout = new QVBoxLayout(log_root_widget);

    log_label = new QLabel("<b>SESSION LOG</b>",log_root_widget);
    log_window = new QTextEdit(log_root_widget);
    log_window->setReadOnly(true);

    log_window_layout->addWidget(log_label,0,Qt::AlignHCenter);
    log_window_layout->addWidget(log_window);


        /*  Mode switching  */

    mode_root_widget = new QFrame(left_panel);
    mode_root_widget->setFrameShape(QFrame::Box);
    mode_root_widget->setFrameShadow(QFrame::Plain);

    QHBoxLayout *mode_buttons_layout = new QHBoxLayout(mode_root_widget);

    mirror_button = new QPushButton("MIRROR",mode_root_widget);
    shack_button = new QPushButton("SHACK-HARTMANN",mode_root_widget);
    calibration_button = new QPushButton("CALIBRATION MIRROR",mode_root_widget);

    mode_buttons_layout->addWidget(mirror_button,2);
    mode_buttons_layout->addWidget(shack_button,2);
    mode_buttons_layout->addWidget(calibration_button,2);


        /*  Telescope shift and PF table control  */


    // telescope shift control

    dw = fnm.width("XXXXX");

    tel_shift_root_widget = new QFrame(shift_table_widget);
    tel_shift_root_widget->setFrameShape(QFrame::Box);
    tel_shift_root_widget->setFrameShadow(QFrame::Plain);

//    QVBoxLayout *tel_shift_layout = new QVBoxLayout(tel_shift_root_widget);
    QFormLayout *tel_shift_layout = new QFormLayout(tel_shift_root_widget);
    tel_shift_layout->setFormAlignment(Qt::AlignHCenter|Qt::AlignTop);

    tel_shift_button = new QPushButton("SHIFT, px CCD",tel_shift_root_widget);

//    X_shift_label = new QLabel("X:",tel_shift_root_widget);

    X_shift_input_field = new QLineEdit(tel_shift_root_widget);
    X_shift_input_field->setMaximumWidth(dw);

//    X_shift_slider = new QSlider(Qt::Horizontal,tel_shift_root_widget);
//    X_shift_slider->setSingleStep(1);
//    X_shift_slider->setPageStep(5);
//    X_shift_slider->setMinimum(-200);
//    X_shift_slider->setMaximum(200);


//    Y_shift_label = new QLabel("Y:",tel_shift_root_widget);

    Y_shift_input_field = new QLineEdit(tel_shift_root_widget);
    Y_shift_input_field->setMaximumWidth(dw);

//    Y_shift_slider = new QSlider(Qt::Horizontal,tel_shift_root_widget);
//    Y_shift_slider->setSingleStep(1);
//    Y_shift_slider->setPageStep(5);
//    Y_shift_slider->setMinimum(-200);
//    Y_shift_slider->setMaximum(200);

    tel_shift_layout->addRow(tel_shift_button);
    tel_shift_layout->addRow("X:",X_shift_input_field);
    tel_shift_layout->addRow("Y:",Y_shift_input_field);

    shift_table_layout->addWidget(tel_shift_root_widget,1);


    // PF table control

    pf_table_root_widget = new QFrame(shift_table_widget);
    pf_table_root_widget->setFrameShape(QFrame::Box);
    pf_table_root_widget->setFrameShadow(QFrame::Plain);

    QVBoxLayout *pf_table_layout = new QVBoxLayout(pf_table_root_widget);

    QWidget *pf_table_inner_widget = new QWidget(pf_table_root_widget);
    QHBoxLayout *pf_table_inner_layout = new QHBoxLayout(pf_table_inner_widget);

    QWidget *pf_table_controls_widget = new QWidget(pf_table_inner_widget);
    QFormLayout *pf_table_controls_layout = new QFormLayout(pf_table_controls_widget);

    pf_table_widget = new PF_table(220.0,pf_table_inner_widget);

    pf_table_current_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_current_input_field->setAlignment(Qt::AlignRight);
    pf_table_current_input_field->setReadOnly(true);
    pf_table_new_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_new_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *table_val = new QDoubleValidator(__scale_start_angle__,__scale_stop_angle__,2,pf_table_new_input_field);
    pf_table_new_input_field->setValidator(table_val);

    pf_table_start_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_stop_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_step_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_start_input_field->setAlignment(Qt::AlignRight);
    pf_table_stop_input_field->setAlignment(Qt::AlignRight);
    pf_table_step_input_field->setAlignment(Qt::AlignRight);
    pf_table_start_input_field->setValidator(table_val);
    pf_table_stop_input_field->setValidator(table_val);
    pf_table_step_input_field->setValidator(table_val);

    pf_table_controls_layout->addRow("Current",pf_table_current_input_field);
    pf_table_controls_layout->addRow("New",pf_table_new_input_field);

    pf_table_rotation_button = new QPushButton("Rotation",pf_table_controls_widget);
    pf_table_controls_layout->addRow("    ", pf_table_rotation_button);

    QWidget *empty_widget = new QWidget(pf_table_controls_widget);
//    empty_widget->setGeometry(0,0,100,150);
    empty_widget->setFixedHeight(30);
    pf_table_controls_layout->addRow("",empty_widget);

    pf_table_controls_layout->addRow("Start",pf_table_start_input_field);
    pf_table_controls_layout->addRow("Stop",pf_table_stop_input_field);
    pf_table_controls_layout->addRow("Step",pf_table_step_input_field);

    pf_table_stop_input_field->setEnabled(false);
    pf_table_controls_layout->labelForField(pf_table_stop_input_field)->setEnabled(false);
    pf_table_start_input_field->setEnabled(false);
    pf_table_controls_layout->labelForField(pf_table_start_input_field)->setEnabled(false);
    pf_table_step_input_field->setEnabled(false);
    pf_table_controls_layout->labelForField(pf_table_step_input_field)->setEnabled(false);

    pf_table_inner_layout->addWidget(pf_table_widget);
    pf_table_inner_layout->addWidget(pf_table_controls_widget);

    pf_table_label = new QLabel("<b>PF table control</b>",pf_table_root_widget);

    pf_table_layout->addWidget(pf_table_label,0,Qt::AlignHCenter);
    pf_table_layout->addWidget(pf_table_inner_widget);

    shift_table_layout->addWidget(pf_table_root_widget,3);


    left_panel_layout->addWidget(file_exp_widget);
    left_panel_layout->addWidget(log_root_widget);
    left_panel_layout->addWidget(mode_root_widget);
    left_panel_layout->addWidget(shift_table_widget);


                        /*      RIGHT PANEL      */
}
