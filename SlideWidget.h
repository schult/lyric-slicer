#pragma once

#include <memory>

#include <QWidget>

namespace Ui { class SlideWidget; }

class SlideWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SlideWidget(QWidget *parent = nullptr);
    ~SlideWidget();

    QString GetName() const;
    QPixmap GetPixmap() const;

    void SetName(const QString& name);
    void SetLyrics(const QString& lyrics);
    void SetPreview(const QString& preview);

    void SetLyricsFont(const QFont& lyrics_font);
    void SetPreviewFont(const QFont& preview_font);
    void SetPreviewVisible(bool show_preview);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    void DrawPixmap();
    void Update();

    std::unique_ptr<Ui::SlideWidget> ui;

    QPixmap m_pixmap;

    QString m_lyrics;
    QString m_preview;

    QFont m_lyrics_font;
    QFont m_preview_font;
    bool m_show_preview = true;

};
