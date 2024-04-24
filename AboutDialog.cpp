#include "AboutDialog.h"

#include <QFile>
#include <QTextStream>

#include "ui_AboutDialog.h"

#ifndef VERSION_STRING
#define VERSION_STRING "--"
#endif

#ifndef BUILD_NUMBER_STRING
#define BUILD_NUMBER_STRING "--"
#endif

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentWidget(ui->tabApp);

    auto version_font = ui->labelVersion->font();
    version_font.setBold(true);
    version_font.setPointSize(version_font.pointSize() * 2);
    ui->labelVersion->setFont(version_font);
    ui->labelVersion->setText(QString("Version %1").arg(VERSION_STRING));
    ui->labelBuild->setText(QString("Build %1").arg(BUILD_NUMBER_STRING));

    QFile app_license(":/LICENSE.txt");
    if (app_license.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream app_stream(&app_license);
        ui->editAppLicense->setText(app_stream.readAll());
    }

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
