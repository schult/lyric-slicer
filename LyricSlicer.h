#pragma once

#include <memory>

#include <QMainWindow>
#include <QMessageBox>

#include "SlideWidget.h"

namespace Ui { class LyricSlicer; }

class LyricSlicer : public QMainWindow
{
    Q_OBJECT

public:
    LyricSlicer(QWidget *parent = nullptr);
    ~LyricSlicer();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_editTitle_textChanged(const QString&);
    void on_editLyrics_textChanged();
    void on_spinSlidesPerVerse_valueChanged(int);

    void on_actionNew_triggered(bool);
    void on_actionOpen_triggered(bool);
    void on_actionSave_triggered(bool);
    void on_actionSaveAs_triggered(bool);
    void on_actionExport_triggered(bool);
    void on_actionQuit_triggered(bool);
    void on_actionAbout_triggered(bool);

private:
    void SetSlideCount(int count);
    QString BaseName() const;
    QString SlideId(int idx) const;

    void UpdateSlideNames();
    void UpdateExportReady();

    QMessageBox::StandardButton PromptToSave();
    void OpenFile(const QString& path);
    void SaveFile(const QString& path);
    void ExportTo(const QString& dir_path);

    void WriteSettings() const;
    void ReadSettings();

    std::unique_ptr<Ui::LyricSlicer> ui;

    bool m_modified = false;
    QString m_filepath;

    QList<SlideWidget*> m_slides;
    QFont m_lyrics_font;
    QFont m_preview_font;
};
