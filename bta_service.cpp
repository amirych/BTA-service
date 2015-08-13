#include "bta_service.h"

#include <iostream>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QFont>
#include <QFontMetrics>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QMargins>
#include <QFileDialog>

#include <QStringList>

static QStringList expTypeItems = QStringList({"obj","bias","dark","rotation"});

static QStringList binValues = QStringList({"1","2","3","4","5"});



                /*      Constructors      */

BTA_service::BTA_service(QWidget *parent)
    : QMainWindow(parent), foc_dialog(nullptr),
      currentFITS_filename(""),
      isFocussingMode(false), currentSelectedRegion()
{
    setupUI();

//    foc_dialog = new focussing_widget(88.5,92.5,0.5,this);

    // connect UI signals

    connect(fits_viewer_widget,SIGNAL(SelectedRegion(QRectF)),this,SLOT(setSelectedRegion(QRectF)));
    connect(fits_viewer_widget,SIGNAL(DeselectRegion()),this,SLOT(setSelectedRegion()));
    connect(plot_region_button,SIGNAL(clicked()),this,SLOT(plotRegion()));

    connect(focussing_button,SIGNAL(clicked()),this,SLOT(showFocussingDialog()));
}

BTA_service::~BTA_service()
{
}


        /*     Private slots    */


void BTA_service::changeFrameType(QString type)
{
    if ( type == "bias" ) {
        exp_time->setText("0");
        exp_time->setEnabled(false);
    } else {
        exp_time->setEnabled(true);
    }

    if ( type == "rotation" ) { // activate PF table controls for sequence of positions
        pf_table_stop_input_field->setEnabled(true);
        pf_table_stop_label->setEnabled(true);
        pf_table_start_input_field->setEnabled(true);
        pf_table_start_label->setEnabled(true);
        pf_table_step_input_field->setEnabled(true);
        pf_table_step_label->setEnabled(true);

        pf_table_new_label->setEnabled(false);
        pf_table_new_input_field->setEnabled(false);
    } else {
        pf_table_stop_input_field->setEnabled(false);
        pf_table_stop_label->setEnabled(false);
        pf_table_start_input_field->setEnabled(false);
        pf_table_start_label->setEnabled(false);
        pf_table_step_input_field->setEnabled(false);
        pf_table_step_label->setEnabled(false);

        pf_table_new_label->setEnabled(true);
        pf_table_new_input_field->setEnabled(true);
    }
}


void BTA_service::newTablePos(const QString &pos)
{
    pf_table_widget->setNewPos(pos.toDouble());
}


void BTA_service::changeBinX(int index)
{
}

void BTA_service::changeBinY(int index)
{
}



void BTA_service::openFileInViewer()
{
    QString working_path = current_path_input_field->text();
    working_path = working_path.trimmed();

    if ( working_path.isEmpty() ) working_path = ".";

    currentFITS_filename = QFileDialog::getOpenFileName(this,"Open FITS file ...",working_path,BTA_SERVICE_FITS_FILE_FILTER);

    fits_viewer_widget->LoadFile(currentFITS_filename);
    if ( fits_viewer_widget->getCurrentError() != FITS_VIEWER_ERROR_OK) {
        qDebug() << "fv: " << fits_viewer_widget->getCurrentError();
        // TODO ...
        return;
    }

    FITS_filename_label->setText(currentFITS_filename);

    double lc,hc;

    fits_viewer_widget->GetImageMinMax(&lc,&hc);
    image_cuts_low->setRange(lc,hc);
    image_cuts_high->setRange(lc,hc);

    fits_viewer_widget->GetCurrentCuts(&lc,&hc);

    showImageScaling(lc,hc);

}


void BTA_service::showImagePoint(double xpos, double ypos, double val)
{
    current_pixel_coords_label->setText("Pixel: [" + QString::number(xpos,'f',1) + ", " + QString::number(ypos,'f',1) + "]");
    current_pixel_value_label->setText("Value: " + QString::number(val,'g'));
}


void BTA_service::showImageScaling(double low_cut, double high_cut)
{
    double lc,hc;

    disconnect(image_cuts_low,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));
    disconnect(image_cuts_high,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));

    image_cuts_low->setValue(low_cut);
    image_cuts_high->setValue(high_cut);

    connect(image_cuts_low,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));
    connect(image_cuts_high,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));
}


void BTA_service::changeImageScaling(double val)
{
    double low_cut = val, hight_cut;

    low_cut = image_cuts_low->value();
    hight_cut = image_cuts_high->value();

    // to prevent loop for signals
    disconnect(fits_viewer_widget,SIGNAL(ScalingIsChanged(double,double)),this,SLOT(showImageScaling(double,double)));

    fits_viewer_widget->ScaleImage(low_cut,hight_cut);

    connect(fits_viewer_widget,SIGNAL(ScalingIsChanged(double,double)),this,SLOT(showImageScaling(double,double)));
}



void BTA_service::showFocussingDialog()
{
    if ( foc_dialog != nullptr) delete foc_dialog;

    foc_dialog = new focussing_widget;
    foc_dialog->exec();
    delete foc_dialog;
    foc_dialog = nullptr;
//    foc_dialog->show();
//    foc_dialog->raise();
//    foc_dialog->activateWindow();
}


void BTA_service::setSelectedRegion(QRectF rect)
{
    currentSelectedRegion = rect.normalized();
}


void BTA_service::setSelectedRegion()
{
    currentSelectedRegion = QRectF();
}

void BTA_service::plotRegion()
{
    if ( currentSelectedRegion.isValid() ) {
//        if ( currentSelectedRegion.isValid() && !currentSelectedRegion.isEmpty() ) {
        plotRegion_dialog = new plot_region_widget(currentFITS_filename,currentSelectedRegion,this);
        plotRegion_dialog->resize(500,300);
        plotRegion_dialog->exec();
    }
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
    QHBoxLayout *main_layout = new QHBoxLayout(central_widget);
    main_layout->setMargin(0);

    QWidget *left_panel = new QWidget(central_widget);
    QVBoxLayout *left_panel_layout = new QVBoxLayout(left_panel);

    QWidget *file_exp_widget = new QWidget(left_panel);
    QHBoxLayout *file_exp_layout = new QHBoxLayout(file_exp_widget);

    QWidget *shift_table_widget = new QWidget(left_panel);
    QHBoxLayout *shift_table_layout = new QHBoxLayout(shift_table_widget);
    shift_table_layout->setMargin(0);


    QWidget *right_panel = new QWidget(central_widget);
    QVBoxLayout *right_panel_layout = new QVBoxLayout(right_panel);

    QWidget *fits_image_widget = new QWidget(right_panel);
    QHBoxLayout *fits_image_layout = new QHBoxLayout(fits_image_widget);

    QWidget *zoom_data_reduc = new QWidget(right_panel);
    QHBoxLayout *zoom_data_reduc_layout = new QHBoxLayout(zoom_data_reduc);


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

    QWidget *obj_widget = new QWidget(current_file_root_widget);
    QHBoxLayout *obj_layout = new QHBoxLayout(obj_widget);
    obj_layout->setMargin(0);
    obj_name_label = new QLabel("object ",obj_widget);
    obj_name_label->resize(label_width,obj_name_label->height());
    obj_name_input_field = new QLineEdit(obj_widget);
    obj_layout->addWidget(obj_name_label);
    obj_layout->addWidget(obj_name_input_field);

    current_file_layout->addWidget(obj_widget);

    // seeing and clouds

    QWidget *seeing_widget = new QWidget(current_file_root_widget);
    QHBoxLayout *seeing_layout = new QHBoxLayout(seeing_widget);
    seeing_layout->setMargin(0);
    seeing_label = new QLabel("seeing",seeing_widget);
    seeing_input_field = new QLineEdit(seeing_widget);
    seeing_input_field->setMaximumWidth(50);
    seeing_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *seeing_val = new QDoubleValidator(0.0,20.0,1,seeing_widget);
    seeing_input_field->setValidator(seeing_val);

    clouds_label = new QLabel("clouds%",seeing_widget);
    clouds_input_field = new QLineEdit(seeing_widget);
    clouds_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *clouds_val = new QDoubleValidator(0.0,100.0,1,seeing_widget);
    clouds_input_field->setValidator(clouds_val);

    seeing_layout->addWidget(seeing_label);
    seeing_layout->addWidget(seeing_input_field);
    seeing_layout->addStretch(10);
    seeing_layout->addWidget(clouds_label);
    seeing_layout->addWidget(clouds_input_field);
    seeing_layout->addStretch(0);

    current_file_layout->addWidget(seeing_widget);

    // comment field

    QWidget *comment_widget = new QWidget(current_file_root_widget);
    QHBoxLayout *comment_layout = new QHBoxLayout(comment_widget);
    comment_layout->setMargin(0);
    comment_label = new QLabel("comment",comment_widget);
    comment_input_field = new QLineEdit(comment_widget);
    comment_layout->addWidget(comment_label);
    comment_layout->addWidget(comment_input_field);

    current_file_layout->addWidget(comment_widget);

    // current path

    QWidget *current_path_widget = new QWidget(current_file_root_widget);
    QHBoxLayout *current_path_layout = new QHBoxLayout(current_path_widget);
    current_path_layout->setMargin(0);
    current_path_label = new QLabel("path",current_path_widget);
    current_path_input_field = new QLineEdit(current_path_widget);
    current_path_create_button = new QPushButton("create",current_path_widget);
    current_path_layout->addWidget(current_path_label);
    current_path_layout->addWidget(current_path_input_field);
    current_path_layout->addWidget(current_path_create_button);

    current_file_layout->addWidget(current_path_widget);

    // current filename

    QWidget *file_widget = new QWidget(current_file_root_widget);
    QHBoxLayout *file_layout = new QHBoxLayout(file_widget);
    file_layout->setMargin(0);
    current_file_label = new QLabel("file",file_widget);
    current_file_input_field = new QLineEdit(file_widget);
    str = QString(BTA_SERVICE_FILE_MAX_CHARS,'0');
    fn = current_file_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width(str);
    current_file_input_field->setMinimumWidth(dw);
    Nexp_label = new QLabel("N exp",file_widget);

    Nexp_input_field = new QLineEdit(file_widget);
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
    file_layout->addStretch(10);
    file_layout->addWidget(Nexp_label);
    file_layout->addWidget(Nexp_input_field);
    file_layout->addStretch(0);

    current_file_layout->addWidget(file_widget);

    // auto increment

    auto_increment_check = new QCheckBox("auto increment file",current_file_root_widget);
    auto_increment_check->setCheckState(Qt::Checked);

    current_file_layout->addWidget(auto_increment_check);


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
    connect(app_exit_button,SIGNAL(clicked()),this,SLOT(deleteLater()));

    focus_header_exit_buttons_layout->addWidget(focussing_button);
    focus_header_exit_buttons_layout->addWidget(header_button);
    focus_header_exit_buttons_layout->addWidget(app_exit_button);

    current_exp_layout->addWidget(focus_header_exit_buttons,2);
    current_exp_layout->addStretch(1);


    file_exp_layout->setMargin(0);
    file_exp_layout->addWidget(current_file_root_widget,1);
    file_exp_layout->addWidget(current_exp_root_widget,1);


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
    X_shift_input_field->setAlignment(Qt::AlignRight);
    X_shift_input_field->setText("0");

//    X_shift_slider = new QSlider(Qt::Horizontal,tel_shift_root_widget);
//    X_shift_slider->setSingleStep(1);
//    X_shift_slider->setPageStep(5);
//    X_shift_slider->setMinimum(-200);
//    X_shift_slider->setMaximum(200);


//    Y_shift_label = new QLabel("Y:",tel_shift_root_widget);

    Y_shift_input_field = new QLineEdit(tel_shift_root_widget);
    Y_shift_input_field->setMaximumWidth(dw);
    Y_shift_input_field->setAlignment(Qt::AlignRight);
    Y_shift_input_field->setText("0");

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
    pf_table_controls_layout->setMargin(0);

    QFrame *ff = new QFrame(pf_table_inner_widget);
    ff->setFrameShape(QFrame::Box);
    ff->setFrameShadow(QFrame::Plain);
    ff->setStyleSheet("background-color: white");
    ff->setMinimumSize(120,120);
    QVBoxLayout *ffl = new QVBoxLayout(ff);
    ffl->setAlignment(Qt::AlignCenter);
    ffl->setMargin(0);
    ffl->setSpacing(0);
    ffl->setContentsMargins(0,0,0,0);

//    pf_table_widget = new PF_table(220.0,pf_table_inner_widget);
    pf_table_widget = new PF_table(220.0,ff);
    ffl->addWidget(pf_table_widget);

    pf_table_current_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_current_input_field->setAlignment(Qt::AlignRight);
    pf_table_current_input_field->setReadOnly(true);
    pf_table_new_label = new QLabel("New",pf_table_controls_widget);
    pf_table_new_input_field = new QLineEdit(pf_table_controls_widget);
    pf_table_new_input_field->setAlignment(Qt::AlignRight);
    QDoubleValidator *table_val = new QDoubleValidator(__scale_start_angle__,__scale_stop_angle__,2,pf_table_new_input_field);
    pf_table_new_input_field->setValidator(table_val);
    connect(pf_table_new_input_field,SIGNAL(textEdited(QString)),this,SLOT(newTablePos(QString)));

    pf_table_start_label = new QLabel("Start",pf_table_controls_widget);
    pf_table_stop_label = new QLabel("Stop",pf_table_controls_widget);
    pf_table_step_label = new QLabel("Step",pf_table_controls_widget);
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
    pf_table_controls_layout->addRow(pf_table_new_label,pf_table_new_input_field);

    pf_table_rotation_button = new QPushButton("Rotation",pf_table_controls_widget);
    pf_table_controls_layout->addRow("    ", pf_table_rotation_button);

    QWidget *empty_widget = new QWidget(pf_table_controls_widget);
//    empty_widget->setGeometry(0,0,100,150);
    empty_widget->setFixedHeight(30);
    pf_table_controls_layout->addRow("",empty_widget);

    pf_table_controls_layout->addRow(pf_table_start_label,pf_table_start_input_field);
    pf_table_controls_layout->addRow(pf_table_stop_label,pf_table_stop_input_field);
    pf_table_controls_layout->addRow(pf_table_step_label,pf_table_step_input_field);

    pf_table_stop_input_field->setEnabled(false);
    pf_table_stop_label->setEnabled(false);
    pf_table_start_input_field->setEnabled(false);
    pf_table_start_label->setEnabled(false);
    pf_table_step_input_field->setEnabled(false);
    pf_table_step_label->setEnabled(false);

//    pf_table_inner_layout->addWidget(pf_table_widget,10);
    pf_table_inner_layout->addWidget(ff,10);
    pf_table_inner_layout->addWidget(pf_table_controls_widget,6);

    pf_table_label = new QLabel("<b>PF table control</b>",pf_table_root_widget);

    pf_table_layout->addWidget(pf_table_label,0,Qt::AlignHCenter);
    pf_table_layout->addWidget(pf_table_inner_widget);

    shift_table_layout->addWidget(pf_table_root_widget,2);


    left_panel_layout->addWidget(file_exp_widget);
    left_panel_layout->addWidget(log_root_widget);
    left_panel_layout->addWidget(mode_root_widget);
    left_panel_layout->addWidget(shift_table_widget);



                        /*      RIGHT PANEL      */

    // FITS image viewer and controls

    FITS_filename_label = new QLabel("No image",right_panel);
    fits_viewer_widget = new Fits_viewer(right_panel);
    connect(fits_viewer_widget,SIGNAL(ImagePoint(double,double,double)),this,SLOT(showImagePoint(double,double,double)));
    connect(fits_viewer_widget,SIGNAL(ScalingIsChanged(double,double)),this,SLOT(showImageScaling(double,double)));
    fits_viewer_widget->setFixedSize(520,520);

    QWidget *image_controls_widget = new QWidget(right_panel);
    QVBoxLayout *image_controls_layout = new QVBoxLayout(image_controls_widget);
    image_controls_layout->setAlignment(Qt::AlignHCenter);
    image_controls_layout->setMargin(0);

    load_file_button = new QPushButton("Load file",image_controls_widget);
    connect(load_file_button,SIGNAL(clicked()),this,SLOT(openFileInViewer()));

    image_cuts_low = new QDoubleSpinBox(image_controls_widget);
    image_cuts_high = new QDoubleSpinBox(image_controls_widget);
//    connect(image_cuts_low,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));
//    connect(image_cuts_high,SIGNAL(valueChanged(double)),this,SLOT(changeImageScaling(double)));

    current_pixel_coords_label = new QLabel("Pixel: [0,0]",image_controls_widget);
    fn = current_pixel_coords_label->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width("Pixel: [0000.0, 0000.0]");
    current_pixel_coords_label->setFixedWidth(dw);

    current_pixel_value_label = new QLabel("Value: 0.0",image_controls_widget);
    plot_region_button = new QPushButton("plot region",image_controls_widget);
    region_stat_button = new QPushButton(" region statistics ",image_controls_widget);
    region_centroid_button = new QPushButton(" region centroid ",image_controls_widget);
    seeing_button = new QPushButton("seeing",image_controls_widget);
    psf_coords_label = new QLabel("X: 0.0, Y: 0.0",image_controls_widget);
    psf_fwhm_label = new QLabel("FWHM: 0.0",image_controls_widget);

    image_controls_layout->addWidget(load_file_button);
    image_controls_layout->addWidget(image_cuts_low);
    image_controls_layout->addWidget(image_cuts_high);
    image_controls_layout->addWidget(current_pixel_coords_label);
    image_controls_layout->addWidget(current_pixel_value_label);
    image_controls_layout->addStretch(1);
    image_controls_layout->addWidget(plot_region_button);
    image_controls_layout->addWidget(region_stat_button);
    image_controls_layout->addWidget(region_centroid_button);
    image_controls_layout->addWidget(seeing_button);
    image_controls_layout->addWidget(psf_coords_label);
    image_controls_layout->addWidget(psf_fwhm_label);
    image_controls_layout->addStretch(3);

    fits_image_layout->addWidget(fits_viewer_widget,5);
    fits_image_layout->addWidget(image_controls_widget,1);


        /* zoom and data reduction  */

    // zoom and axes coordinate

    QWidget *zoom_coords = new QWidget(zoom_data_reduc);
    QVBoxLayout *zoom_coords_layout = new QVBoxLayout(zoom_coords);
    zoom_coords_layout->setMargin(0);

    zoom_widget = new Fits_viewer(zoom_coords);
    zoom_widget->setFixedSize(150,150);
//    zoom_widget->setMinimumSize(150,150);

    QWidget *axes_widget = new QWidget(zoom_coords);
    QFormLayout *axes_layout = new QFormLayout(axes_widget);
    axes_layout->setMargin(0);
    axes_layout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    QWidget *mirror_axis_widget = new QWidget(axes_widget);
    QHBoxLayout *mirror_axis_layout = new QHBoxLayout(mirror_axis_widget);
//    mirror_axis_layout->setMargin(0);

    mirror_axis_label = new QLabel("mirror axis",axes_widget);

    mirror_axis_X_input_field = new QLineEdit(mirror_axis_widget);
    mirror_axis_Y_input_field = new QLineEdit(mirror_axis_widget);

    QLabel *tmp_label = new QLabel("X:",mirror_axis_widget);

    mirror_axis_layout->addWidget(tmp_label);
    mirror_axis_layout->addWidget(mirror_axis_X_input_field);
    tmp_label = new QLabel("Y:",mirror_axis_widget);
    mirror_axis_layout->addWidget(tmp_label);
    mirror_axis_layout->addWidget(mirror_axis_Y_input_field);

    QWidget *table_axis_widget = new QWidget(axes_widget);
    QHBoxLayout *table_axis_layout = new QHBoxLayout(table_axis_widget);
//    table_axis_layout->setMargin(0);

    table_axis_label = new QLabel("PF-table axis",axes_widget);

    table_axis_X_input_field = new QLineEdit(table_axis_widget);
    table_axis_Y_input_field = new QLineEdit(table_axis_widget);

    tmp_label = new QLabel("X:",table_axis_widget);
    table_axis_layout->addWidget(tmp_label);
    table_axis_layout->addWidget(table_axis_X_input_field);
    tmp_label = new QLabel("Y:",table_axis_widget);
    table_axis_layout->addWidget(tmp_label);
    table_axis_layout->addWidget(table_axis_Y_input_field);

    axes_layout->addRow("",mirror_axis_label);
    axes_layout->addRow("",mirror_axis_widget);
    axes_layout->addRow("",table_axis_label);
    axes_layout->addRow("",table_axis_widget);


    zoom_coords_layout->addWidget(zoom_widget);
    zoom_coords_layout->addWidget(axes_widget);


    // data reduction
    QFrame *data_reduc_pointing_widget = new QFrame(zoom_data_reduc);
    data_reduc_pointing_widget->setFrameStyle(QFrame::Box | QFrame::Plain);
    QVBoxLayout *data_reduc_pointing_layout = new QVBoxLayout(data_reduc_pointing_widget);
//    data_reduc_pointing_layout->setMargin(0);

    data_reduc_label = new QLabel("<b>Data reduction</b>",data_reduc_pointing_widget);

    axes_calc_root_widget = new QGroupBox("Mirror and PF-table axes calculation",data_reduc_pointing_widget);
    QVBoxLayout *axes_calc_layout = new QVBoxLayout(axes_calc_root_widget);
//    axes_calc_layout->setMargin(0);

    QWidget *mirror_file_widget = new QWidget(axes_calc_root_widget);
    QHBoxLayout *mirror_file_layout = new QHBoxLayout(mirror_file_widget);
//    mirror_file_layout->setMargin(0);
    QMargins marg = mirror_file_layout->contentsMargins();
    marg.setBottom(0);
//    marg.setTop(0);
    mirror_file_layout->setContentsMargins(marg);

    str = QString(BTA_SERVICE_FILE_MAX_CHARS,'0');

    mirror_file_label = new QLabel("mirror file",mirror_file_widget);
    fn = mirror_file_label->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width("PF-table file");
    mirror_file_label->setMinimumWidth(dw);

    mirror_file_input_field = new QLineEdit(mirror_file_widget);
    fn = mirror_file_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width(str);
    mirror_file_input_field->setMinimumWidth(dw);
    mirror_file_Nexp_label = new QLabel("Nexp",mirror_file_widget);
    mirror_file_Nexp_input_field = new QLineEdit(mirror_file_widget);
    fn = mirror_file_Nexp_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width("0000");
    mirror_file_Nexp_input_field->setMaximumWidth(dw);
    mirror_file_Nexp_input_field->setMinimumWidth(dw);
    mirror_reduc_result_label = new QLabel("x = 0.0  y = 0.0",mirror_file_widget);

    mirror_file_layout->addWidget(mirror_file_label);
    mirror_file_layout->addWidget(mirror_file_input_field);
    mirror_file_layout->addWidget(mirror_file_Nexp_label);
    mirror_file_layout->addWidget(mirror_file_Nexp_input_field);
    mirror_file_layout->addStretch(3);
    mirror_file_layout->addWidget(mirror_reduc_result_label);
    mirror_file_layout->addStretch(1);

    QWidget *table_file_widget = new QWidget(axes_calc_root_widget);
    QHBoxLayout *table_file_layout = new QHBoxLayout(table_file_widget);
    marg = table_file_layout->contentsMargins();
//    marg.setBottom(0);
    marg.setTop(0);
    table_file_layout->setContentsMargins(marg);

    table_file_label = new QLabel("PF-table file",table_file_widget);
    table_file_input_field = new QLineEdit(table_file_widget);
    fn = table_file_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width(str);
    table_file_input_field->setMinimumWidth(dw);
    table_file_Nexp_label = new QLabel("Nexp",table_file_widget);
    table_file_Nexp_input_field = new QLineEdit(table_file_widget);
    fn = table_file_Nexp_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width("0000");
    table_file_Nexp_input_field->setMaximumWidth(dw);
    table_file_Nexp_input_field->setMinimumWidth(dw);
    table_reduc_result_label = new QLabel("x = 0.0  y = 0.0",table_file_widget);

    table_file_layout->addWidget(table_file_label);
    table_file_layout->addWidget(table_file_input_field);
    table_file_layout->addWidget(table_file_Nexp_label);
    table_file_layout->addWidget(table_file_Nexp_input_field);
    table_file_layout->addStretch(3);
    table_file_layout->addWidget(table_reduc_result_label);
    table_file_layout->addStretch(1);

    run_reduc_button = new QPushButton("Run",axes_calc_root_widget);

    axes_calc_layout->addWidget(data_reduc_label,0,Qt::AlignCenter);
    axes_calc_layout->addWidget(mirror_file_widget);
    axes_calc_layout->addWidget(table_file_widget);
    axes_calc_layout->addWidget(run_reduc_button,0,Qt::AlignRight);


    pointing_residual_root_widget = new QGroupBox("Telescope pointing mismatch");
    QVBoxLayout *pointing_residual_layout = new QVBoxLayout(pointing_residual_root_widget);
    pointing_residual_layout->setMargin(0);

    QWidget *pointing_residual_file_widget = new QWidget(pointing_residual_root_widget);
    QHBoxLayout *pointing_residual_file_layout = new QHBoxLayout(pointing_residual_file_widget);
    marg = pointing_residual_file_layout->contentsMargins();
    marg.setBottom(0);
//    marg.setTop(0);
    pointing_residual_file_layout->setContentsMargins(marg);

    pointing_file_label = new QLabel("file",pointing_residual_file_widget);
    pointing_file_input_field = new QLineEdit(pointing_residual_file_widget);
    fn = pointing_file_input_field->font();
    fnm = QFontMetrics(fn);
    dw = fnm.width(str);
//    pointing_file_input_field->setFixedWidth(dw);
    pointing_file_input_field->setMinimumWidth(dw);
    pointing_residual_load_button = new QPushButton("Load",pointing_residual_file_widget);
    pointing_residual_calc_button = new QPushButton("Calc",pointing_residual_file_widget);

    pointing_residual_file_layout->addWidget(pointing_file_label);
    pointing_residual_file_layout->addWidget(pointing_file_input_field);
    pointing_residual_file_layout->addWidget(pointing_residual_load_button);
    pointing_residual_file_layout->addWidget(pointing_residual_calc_button);
    pointing_residual_file_layout->addStretch(10);


    QWidget *pointing_coords = new QWidget(pointing_residual_root_widget);
    QGridLayout *pointing_coords_layout = new QGridLayout(pointing_coords);

    acs_coords_label = new QLabel("ACS",pointing_coords);
    real_coords_label = new QLabel("Real",pointing_coords);
    residual_label = new QLabel("ACS-Real",pointing_coords);
    alpha_coord_label = new QLabel("<font size=""+1"">"+QString(QChar(0xb1, 0x03))+"<sub>2000.0</sub></font>",pointing_coords);
    delta_coord_label = new QLabel("<font size=""+1"">"+QString(QChar(0xb4, 0x03))+"<sub>2000.0</sub></font>",pointing_coords);

    acs_alpha_input_field = new QLineEdit(pointing_coords);
    acs_alpha_input_field->setReadOnly(true);
    acs_alpha_input_field->setAlignment(Qt::AlignRight);
    acs_delta_input_field = new QLineEdit(pointing_coords);
    acs_delta_input_field->setReadOnly(true);
    acs_delta_input_field->setAlignment(Qt::AlignRight);
    real_alpha_input_field = new QLineEdit(pointing_coords);
    real_alpha_input_field->setReadOnly(true);
    real_alpha_input_field->setAlignment(Qt::AlignRight);
    real_delta_input_field = new QLineEdit(pointing_coords);
    real_delta_input_field->setReadOnly(true);
    real_delta_input_field->setAlignment(Qt::AlignRight);
    alpha_residual = new QLineEdit(pointing_coords);
    alpha_residual->setReadOnly(true);
    alpha_residual->setAlignment(Qt::AlignRight);
    delta_residual = new QLineEdit(pointing_coords);
    delta_residual->setReadOnly(true);
    delta_residual->setAlignment(Qt::AlignRight);

    acs_alpha_input_field->setText("12:32:56.0");
    acs_delta_input_field->setText("+32:19:45.32");
    real_alpha_input_field->setText("12:32:55.5");
    real_delta_input_field->setText("+32:19:50.43");
    alpha_residual->setText("7.5\"");
    delta_residual->setText("-5.11\"");



    pointing_coords_layout->addWidget(acs_coords_label,0,1,Qt::AlignCenter);
    pointing_coords_layout->addWidget(real_coords_label,0,2,Qt::AlignCenter);
    pointing_coords_layout->addWidget(residual_label,0,3,Qt::AlignCenter);
    pointing_coords_layout->addWidget(alpha_coord_label,1,0);
    pointing_coords_layout->addWidget(delta_coord_label,2,0);
    pointing_coords_layout->addWidget(acs_alpha_input_field,1,1);
    pointing_coords_layout->addWidget(acs_delta_input_field,2,1);
    pointing_coords_layout->addWidget(real_alpha_input_field,1,2);
    pointing_coords_layout->addWidget(real_delta_input_field,2,2);
    pointing_coords_layout->addWidget(alpha_residual,1,3);
    pointing_coords_layout->addWidget(delta_residual,2,3);



    pointing_residual_layout->addWidget(pointing_residual_file_widget);
    pointing_residual_layout->addWidget(pointing_coords);
//    pointing_residual_layout->addWidget(pointing_residual_calc_button,Qt::AlignRight);

    data_reduc_pointing_layout->addWidget(data_reduc_label,0,Qt::AlignHCenter);
    data_reduc_pointing_layout->addWidget(axes_calc_root_widget);
    data_reduc_pointing_layout->addWidget(pointing_residual_root_widget);


    zoom_data_reduc_layout->addWidget(zoom_coords,1);
    zoom_data_reduc_layout->addWidget(data_reduc_pointing_widget,3);


    right_panel_layout->addWidget(FITS_filename_label,0,Qt::AlignHCenter);
    right_panel_layout->addWidget(fits_image_widget,2);
    right_panel_layout->addWidget(zoom_data_reduc,1);


    QFrame *separator_line = new QFrame(central_widget);
    separator_line->setFrameStyle(QFrame::VLine);
    separator_line->setFrameShadow(QFrame::Sunken);
    separator_line->setLineWidth(4);

    main_layout->addWidget(left_panel,2);
    main_layout->addWidget(separator_line);
    main_layout->addWidget(right_panel,2);
}


void BTA_service::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}
