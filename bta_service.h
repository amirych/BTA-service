#ifndef BTA_SERVICE_H
#define BTA_SERVICE_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QComboBox>
#include <QTextEdit>
#include <QSlider>
#include <QCheckBox>

#include "../fits_viewer/fits_viewer.h"
#include "pf_table.h"


#define BTA_SERVICE_NEXP_MAX_DIGITS 3


class BTA_service : public QMainWindow
{
    Q_OBJECT

public:
    BTA_service(QWidget *parent = 0);
    ~BTA_service();

private slots:
    void changeFrameType(QString type);
    void changeBinX(int index);
    void changeBinY(int index);

private:
    /*     Functions       */

    void setupUI();

    /*     GUI elements    */

    QFrame *central_widget;

    // current file and exposure control
    QFrame *current_file_root_widget;
    QLabel *obj_name_label;
    QLineEdit *obj_name_input_field;
    QLabel *seeing_label;
    QLineEdit *seeing_input_field;
    QLabel *clouds_label;
    QLineEdit *clouds_input_field;
    QLabel *comment_label;
    QLineEdit *comment_input_field;
    QLabel *current_path_label;
    QLineEdit *current_path_input_field;
    QPushButton *current_path_create_button;
    QLabel *current_file_label;
    QLineEdit *current_file_input_field;
    QLabel *Nexp_label;
    QLineEdit * Nexp_input_field;
    QCheckBox *auto_increment_check;

    QFrame *current_exp_root_widget;
    QComboBox *exp_type_combo;
    QLineEdit *exp_time;
    QLabel *X1_label;
    QLineEdit *X1_input_field;
    QLabel *Y1_label;
    QLineEdit *Y1_input_field;
    QLabel *X2_label;
    QLineEdit *X2_input_field;
    QLabel *Y2_label;
    QLineEdit *Y2_input_field;
    QLabel *binX_label;
    QComboBox *binX_combo;
    QLabel *binY_label;
    QComboBox *binY_combo;
    QPushButton *exp_start_button;
    QPushButton *exp_stop_button;
    QPushButton *ccd_init_button;
    QPushButton *focussing_button;
    QPushButton *header_button;
    QPushButton *app_exit_button;

    QFrame *log_root_widget;
    QLabel *log_label;
    QTextEdit *log_window;

    // mode switching
    QFrame *mode_root_widget;
    QPushButton *mirror_button;
    QPushButton *shack_button;
    QPushButton * calibration_button;

    // telescope shift
    QFrame *tel_shift_root_widget;
    QPushButton *tel_shift_button;
    QLabel *X_shift_label;
    QLineEdit *X_shift_input_field;
    QSlider *X_shift_slider;
    QLabel *Y_shift_label;
    QLineEdit *Y_shift_input_field;
    QSlider *Y_shift_slider;

    // PF table
    QFrame *pf_table_root_widget;
    QLabel *pf_table_label;
    PF_table *pf_table_widget;
//    QLabel *pf_table_current_label;
    QLineEdit *pf_table_current_input_field;
//    QLabel *pf_table_new_label;
    QLineEdit *pf_table_new_input_field;
//    QCheckBox *pf_table_step_check;
//    QLabel *pf_table_step_label;
    QLineEdit *pf_table_step_input_field;
    QLineEdit *pf_table_start_input_field;
    QLineEdit *pf_table_stop_input_field;

    QPushButton *pf_table_rotation_button;

};

#endif // BTA_SERVICE_H
