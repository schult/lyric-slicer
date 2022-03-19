#include "SlideWidget.h"

#include <QPainter>

#include "ui_SlideWidget.h"

SlideWidget::SlideWidget(QWidget *parent) :
    QWidget(parent),
    ui(std::make_unique<Ui::SlideWidget>()),
    m_pixmap(1280, 720)
{
    ui->setupUi(this);
    DrawPixmap();
}

SlideWidget::~SlideWidget()
{
}

QString SlideWidget::GetName() const
{
    return ui->labelName->text();
}

QPixmap SlideWidget::GetPixmap() const
{
    return m_pixmap;
}

void SlideWidget::SetName(const QString &name)
{
    ui->labelName->setText(name);
}

void SlideWidget::SetLyrics(const QString &lyrics)
{
    if(m_lyrics == lyrics) return;
    m_lyrics = lyrics;
    DrawPixmap();
}

void SlideWidget::SetPreview(const QString &preview)
{
    if(m_preview == preview) return;
    m_preview = preview;
    if(m_show_preview) DrawPixmap();
}

void SlideWidget::SetLyricsFont(const QFont &lyrics_font)
{
    if(m_lyrics_font == lyrics_font) return;
    m_lyrics_font = lyrics_font;
    DrawPixmap();
}

void SlideWidget::SetPreviewFont(const QFont &preview_font)
{
    if(m_preview_font == preview_font) return;
    m_preview_font = preview_font;
    DrawPixmap();
}

void SlideWidget::SetPreviewVisible(bool show_preview)
{
    if(m_show_preview == show_preview) return;
    m_show_preview = show_preview;
    DrawPixmap();
}

void SlideWidget::resizeEvent(QResizeEvent *)
{
    Update();
}

void SlideWidget::DrawPixmap()
{
    QPainter painter(&m_pixmap);

    painter.fillRect(m_pixmap.rect(), Qt::black);
    painter.setPen(Qt::white);

    const QRect text_rect = m_pixmap.rect().translated(0, -30);
    painter.setFont(m_lyrics_font);
    painter.drawText(text_rect, Qt::AlignCenter, m_lyrics);

    if(m_show_preview)
    {
        const QRect preview_rect = m_pixmap.rect().marginsRemoved(QMargins(30, 30, 30, 30));
        painter.setFont(m_preview_font);
        painter.drawText(preview_rect, Qt::AlignRight|Qt::AlignBottom, m_preview);
    }

    Update();
}

void SlideWidget::Update()
{
    ui->labelPixmap->setPixmap(m_pixmap.scaledToWidth(width(), Qt::SmoothTransformation));
}
