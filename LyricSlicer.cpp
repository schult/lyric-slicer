#include "LyricSlicer.h"

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>

#include "AboutDialog.h"
#include "ui_LyricSlicer.h"

static QString ConvertQuotes(QString str)
{
    QRegularExpression double_quotes("\"([^\"]*)\"");
    str.replace(double_quotes, "“\\1”");
    str.replace('"', "“");
    str.replace('\'', "’");
    return str;
}

static QString PreviewOf(QString text)
{
    const QRegularExpression trailing_punctuation("[,.!?;:]*$");
    auto preview = text.split('\n').first();
    preview.remove(trailing_punctuation);
    preview.append("…");
    return preview;
}

LyricSlicer::LyricSlicer(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::LyricSlicer>())
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::NoContextMenu);
    ui->toolBar->insertWidget(ui->actionExport, ui->widgetSlideGroupSize);
    ui->scrollContents->setBackgroundRole(QPalette::Base);

    const int font_id = QFontDatabase::addApplicationFont(":/font/LinBiolinum_R.otf");
    const QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
    m_lyrics_font = QFont(family);
    m_preview_font = QFont(family);
    m_lyrics_font.setPixelSize(80);
    m_preview_font.setPixelSize(48);

    UpdateExportReady();
    ReadSettings();
}

LyricSlicer::~LyricSlicer()
{
}

void LyricSlicer::closeEvent(QCloseEvent *event)
{
    if(m_modified)
    {
        switch(PromptToSave())
        {
        case QMessageBox::Save:
            ui->actionSave->trigger();
            break;
        case QMessageBox::Discard:
            m_modified = false;
            break;
        default:
            break;
        }
    }

    if(!m_modified)
    {
        WriteSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void LyricSlicer::on_editTitle_textChanged(const QString &)
{
    m_modified = true;
    UpdateSlideNames();
}

void LyricSlicer::on_editLyrics_textChanged()
{
    m_modified = true;

    const auto full_text = ConvertQuotes(ui->editLyrics->toPlainText());
    const auto first_line = full_text.split('\n').first().trimmed();
    ui->editTitle->setPlaceholderText(first_line.isEmpty() ? "Title" : first_line);

    const QRegularExpression slide_break("\n{2,}");
    auto slide_texts = full_text.split(slide_break, Qt::SkipEmptyParts);
    SetSlideCount(slide_texts.count());

    SlideWidget *previous_slide = nullptr;
    for(auto slide : m_slides)
    {
        const auto lyrics = slide_texts.takeFirst().trimmed();
        slide->SetLyrics(lyrics);
        if(previous_slide) previous_slide->SetPreview(PreviewOf(lyrics));
        previous_slide = slide;
    }
    if(previous_slide) previous_slide->SetPreview(QString());

    UpdateSlideNames();
}

void LyricSlicer::on_spinSlidesPerVerse_valueChanged(int)
{
    m_modified = true;
    UpdateSlideNames();
}

void LyricSlicer::on_actionNew_triggered(bool)
{
    if(m_modified)
    {
        switch(PromptToSave())
        {
        case QMessageBox::Save:
            ui->actionSave->trigger();
            break;
        case QMessageBox::Discard:
            m_modified = false;
            break;
        default:
            return;
        }
    }

    if(!m_modified)
    {
        ui->editTitle->clear();
        ui->editLyrics->clear();
        ui->spinSlidesPerVerse->setValue(1);
        m_filepath.clear();
        m_modified = false;
    }
}

void LyricSlicer::on_actionOpen_triggered(bool)
{
    if(m_modified)
    {
        switch(PromptToSave())
        {
        case QMessageBox::Save:
            ui->actionSave->trigger();
            break;
        case QMessageBox::Discard:
            m_modified = false;
            break;
        default:
            return;
        }
    }

    if(!m_modified)
    {
        auto filepath = QFileDialog::getOpenFileName(this, "Open", QString(), "Lyric Slicer Song (*.lss)");
        if(!filepath.isEmpty())
        {
            OpenFile(filepath);
        }
    }
}

void LyricSlicer::on_actionSave_triggered(bool)
{
    if(m_filepath.isEmpty())
    {
        ui->actionSaveAs->trigger();
    }
    else
    {
        SaveFile(m_filepath);
    }
}

void LyricSlicer::on_actionSaveAs_triggered(bool)
{
    auto filepath = QFileDialog::getSaveFileName(this, "Save File As", QString(), "Lyric Slicer Song (*.lss)");
    if(!filepath.isEmpty())
    {
        SaveFile(filepath);
    }
}

void LyricSlicer::on_actionExport_triggered(bool)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setLabelText(QFileDialog::Accept, "Export");
    connect(&dialog, &QFileDialog::fileSelected,
            this, &LyricSlicer::ExportTo);
    dialog.exec();
}

void LyricSlicer::on_actionQuit_triggered(bool)
{
    close();
}

void LyricSlicer::on_actionAbout_triggered(bool)
{
    AboutDialog dialog(this);
    dialog.exec();
}

void LyricSlicer::SetSlideCount(int count)
{
    while(m_slides.count() > count)
    {
        delete m_slides.takeLast();
    }

    while(m_slides.count() < count)
    {
        auto slide = new SlideWidget;
        slide->SetName(BaseName() + "_" + SlideId(m_slides.count()));
        slide->SetLyricsFont(m_lyrics_font);
        slide->SetPreviewFont(m_preview_font);
        m_slides.append(slide);
        ui->widgetPages->layout()->addWidget(slide);
    }

    UpdateExportReady();
}

QString LyricSlicer::BaseName() const
{
    auto base_name = ui->editTitle->text().trimmed();
    if(base_name.isEmpty())
    {
        base_name = ui->editTitle->placeholderText().trimmed();
    }

    const QRegularExpression punctuation("[,.!?;:/\\\\“”’\"']");
    const QRegularExpression whitespace("\\s+");
    base_name.remove(punctuation);
    base_name.replace(whitespace, "-");
    return base_name.toLower();
}

QString LyricSlicer::SlideId(int idx) const
{
    const int pages_per_verse = ui->spinSlidesPerVerse->value();
    const int verse = idx / pages_per_verse + 1;
    auto label = QString("%1").arg(verse, 2, 10, QChar('0'));
    if(pages_per_verse > 1)
    {
        label += QChar('a' + (idx % pages_per_verse));
    }
    return label;
}

void LyricSlicer::UpdateSlideNames()
{
    const auto base_name = BaseName();
    for(int i = 0; i < m_slides.count(); ++i)
    {
        m_slides[i]->SetName(base_name + "_" + SlideId(i));
    }
}

void LyricSlicer::UpdateExportReady()
{
    ui->actionExport->setEnabled(!m_slides.isEmpty());
}

QMessageBox::StandardButton LyricSlicer::PromptToSave()
{
    return QMessageBox::question(
        this,
        "Save Changes",
        "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);
}

void LyricSlicer::OpenFile(const QString &path)
{
    QString title;
    int slides_per_verse = 1;
    QString lyrics;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        title = stream.readLine();
        stream >> slides_per_verse;
        (void)stream.readLine();
        lyrics = stream.readAll();

        ui->editTitle->setText(title);
        ui->spinSlidesPerVerse->setValue(slides_per_verse);
        ui->editLyrics->setPlainText(lyrics);
        m_filepath = path;
        m_modified = false;
    }
    else
    {
        const QString msg_format("Could not open file \"%1\"");
        QMessageBox::warning(this, "Error", msg_format.arg(path));
    }
}

void LyricSlicer::SaveFile(const QString &path)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << ui->editTitle->text() << "\n";
        stream << ui->spinSlidesPerVerse->value() << "\n";
        stream << ui->editLyrics->toPlainText();
        m_filepath = path;
        m_modified = false;
    }
    else
    {
        QString msg_format("Could not write to file \"%1\"");
        QMessageBox::warning(this, "Error", msg_format.arg(path));
    }
}

void LyricSlicer::ExportTo(const QString &dir_path)
{
    QDir dir(dir_path);
    if(!dir.isEmpty())
    {
        auto answer = QMessageBox::question(this, "Warning",
            "The selected directory is not empty. Overwrite its contents?");
        if(answer != QMessageBox::Yes) return;
    }

    for(auto page : m_slides)
    {
        const auto page_path = dir.filePath(page->GetName() + ".png");
        const auto pixmap = page->GetPixmap();
        pixmap.save(page_path);
    }
}

void LyricSlicer::WriteSettings() const
{
    QSettings settings("UglyHack.net", "Lyric Slicer");

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitterSizes", ui->splitter->saveState());
    settings.endGroup();
}

void LyricSlicer::ReadSettings()
{
    QSettings settings("UglyHack.net", "Lyric Slicer");

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());
    settings.endGroup();
}
