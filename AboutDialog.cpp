#include "AboutDialog.h"

#include <QFile>
#include <QTextStream>

#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

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
