#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <memory>

#include <QDialog>

namespace Ui { class AboutDialog; }

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    std::unique_ptr<Ui::AboutDialog> ui;
};

#endif // ABOUTDIALOG_H
