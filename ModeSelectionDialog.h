#ifndef MODESELECTIONDIALOG_H
#define MODESELECTIONDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>

/**
 * @brief Dialog for selecting between Image Classification and Object Detection modes
 * 
 * Displays on application startup to allow user to choose which mode to use.
 */
class ModeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        ImageClassification,
        ObjectDetection
    };

    explicit ModeSelectionDialog(QWidget *parent = nullptr);
    ~ModeSelectionDialog();
    
    Mode selectedMode() const { return m_selectedMode; }

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    
    QRadioButton *classificationRadio;
    QRadioButton *detectionRadio;
    QPushButton *okButton;
    QPushButton *cancelButton;
    
    Mode m_selectedMode;
};

#endif // MODESELECTIONDIALOG_H

