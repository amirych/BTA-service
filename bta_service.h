#ifndef BTA_SERVICE_H
#define BTA_SERVICE_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QGroupBox>
#include <QComboBox>
#include <QTextEdit>
#include <QSlider>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include "../fits_viewer/fits_viewer.h"
#include "pf_table.h"


#define BTA_SERVICE_NEXP_MAX_DIGITS 3
#define BTA_SERVICE_FILE_MAX_CHARS 20


class BTA_service : public QMainWindow
{
    Q_OBJECT

public:
    BTA_service(QWidget *parent = 0);
    ~BTA_service();

protected slots:
    void resizeEvent(QResizeEvent *event);

private slots:
    void changeFrameType(QString type);
    void changeBinX(int index);
    void changeBinY(int index);

    void newTablePos(const QString &pos);
private:
    /*     Functions       */

    void setupUI();

                /*     GUI ELEMENTS    */

    QFrame *central_widget;

            /*  left panel  */

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
    QLabel *pf_table_new_label;
    QLineEdit *pf_table_new_input_field;
//    QCheckBox *pf_table_step_check;
    QLabel *pf_table_step_label;
    QLabel *pf_table_stop_label;
    QLabel *pf_table_start_label;
    QLineEdit *pf_table_step_input_field;
    QLineEdit *pf_table_start_input_field;
    QLineEdit *pf_table_stop_input_field;

    QPushButton *pf_table_rotation_button;


            /*  right panel  */

    // FITS file viewer controls
    QLabel *FITS_filename_label;
    Fits_viewer *fits_viewer_widget;
    QPushButton *load_file_button;
    QDoubleSpinBox *image_cuts_low;
    QDoubleSpinBox *image_cuts_high;
    QLabel *current_pixel_coords_label;
    QLabel *current_pixel_value_label;
    QPushButton *plot_region_button;
    QPushButton *region_stat_button;
    QPushButton *region_centroid_button;
    QPushButton *seeing_button;
    QLabel *psf_coords_label;
    QLabel *psf_fwhm_label;

    // image zoom and data reduction controls
    Fits_viewer *zoom_widget;
    QLabel *mirror_axis_label;
    QLabel *table_axis_label;
    QLineEdit *mirror_axis_X_input_field;
    QLineEdit *mirror_axis_Y_input_field;
    QLineEdit *table_axis_X_input_field;
    QLineEdit *table_axis_Y_input_field;

    QLabel *data_reduc_label;

    QGroupBox *axes_calc_root_widget;
    QLabel *mirror_file_label;
    QLineEdit *mirror_file_input_field;
    QLabel *mirror_file_Nexp_label;
    QLineEdit *mirror_file_Nexp_input_field;
    QLabel *mirror_reduc_result_label;
    QLabel *table_file_label;
    QLineEdit *table_file_input_field;
    QLabel *table_file_Nexp_label;
    QLineEdit *table_file_Nexp_input_field;
    QLabel *table_reduc_result_label;
    QPushButton *run_reduc_button;

    QGroupBox *pointing_residual_root_widget;
    QLabel *pointing_file_label;
    QLineEdit *pointing_file_input_field;
    QLabel *acs_coords_label;
    QLineEdit *acs_alpha_input_field;
    QLineEdit *acs_delta_input_field;
    QLabel *real_alpha_label;
    QLineEdit *real_alpha_input_field;
    QLabel *real_coords_label;
    QLineEdit *real_delta_input_field;
    QPushButton *pointing_residual_calc_button;
    QPushButton *pointing_residual_load_button;
    QLabel *alpha_coord_label;
    QLabel *delta_coord_label;
    QLabel *residual_label;
    QLineEdit *alpha_residual;
    QLineEdit *delta_residual;

};

#endif // BTA_SERVICE_H
