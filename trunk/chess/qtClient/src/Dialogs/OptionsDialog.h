/*
 * OptionsDialog.h
 *
 *  Created on: Jul 9, 2010
 *      Author: ssy
 */

#ifndef OPTIONSDIALOG_H_
#define OPTIONSDIALOG_H_

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>

#include <MyDialog.h>


class OptionsDialog: public MyDialog
{
    Q_OBJECT
public:
    OptionsDialog(QWidget *parent = 0);
private:

    QLabel* serverNameLabel;
    QLabel* serverPortLabel;
    QLabel* languageLabel;

    QLineEdit* serverNameEdit;
    QLineEdit* serverPortEdit;
    QComboBox* languageComboBox;

    QPushButton* okButton;
    QPushButton* exitButton;

    QGridLayout* edits;

    QHBoxLayout* box;
    QVBoxLayout* upperLayout;
    QHBoxLayout* lowerLayout;
    QVBoxLayout* layout;

    int mLanguageIndex;

private slots:

    void onOkClicked();
    void onExitClicked();
};

#endif /* OPTIONSDIALOG_H_ */