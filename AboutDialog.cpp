#include "AboutDialog.h"

#include <QFile>
#include <QTextStream>

#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QFile qt_license(":/qt/LICENSE.txt");
    if (qt_license.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream qt_stream(&qt_license);
        ui->editQtLicense->setText(qt_stream.readAll());
    }

    QFile font_license(":/font/LICENSE.txt");
    if (font_license.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream font_stream(&font_license);
        ui->editFontLicense->setText(font_stream.readAll());
    }

    QFile icon_license(":/icons/LICENSE.txt");
    if (icon_license.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream icon_stream(&icon_license);
        ui->editIconLicense->setText(icon_stream.readAll());
    }
}

AboutDialog::~AboutDialog()
{
}
