/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskTextLabel.h"
#include "QskAspect.h"
#include "QskTextOptions.h"
#include "QskSkinRenderer.h"

#include <QFontMetricsF>
#include <QtMath>

QSK_SUBCONTROL( QskTextLabel, Text )

class QskTextLabel::PrivateData
{
public:
    PrivateData( const QString& txt ):
        effectiveTextFormat( QskTextOptions::AutoText ),
        text( txt )
    {
    }

    QskTextOptions textOptions;

    QskTextOptions::TextFormat effectiveTextFormat;
    QString text;
};

QskTextLabel::QskTextLabel( QQuickItem* parent ):
    QskTextLabel( QString(), parent )
{
}

QskTextLabel::QskTextLabel( const QString& text, QQuickItem* parent ):
    Inherited( parent ),
    m_data( new PrivateData( text ) )
{
    setSizePolicy( QskSizePolicy::Minimum, QskSizePolicy::Fixed );
}

QskTextLabel::~QskTextLabel()
{
}

void QskTextLabel::setText( const QString& text )
{
    if ( text == m_data->text )
        return;

    m_data->text = text;
    m_data->effectiveTextFormat = QskTextOptions::AutoText;

    resetImplicitSize();
    update();

    Q_EMIT textChanged( text );
}

QString QskTextLabel::text() const
{
    return m_data->text;
}

void QskTextLabel::setTextOptions( const QskTextOptions& options )
{
    if ( options == m_data->textOptions )
        return;

    if ( options.format() != m_data->textOptions.format() )
        m_data->effectiveTextFormat = QskTextOptions::AutoText;

#if 0
    // we are killing user settings of the policy this way ??

    const QskSizePolicy::Policy policy = ( options.wrapMode() == QTextOption::NoWrap )
        ? QskSizePolicy::Minimum : QskSizePolicy::Preferred;

    setSizePolicy( policy, sizePolicy().verticalPolicy() );
#endif

    m_data->textOptions = options;

    resetImplicitSize();
    update();

    Q_EMIT textOptionsChanged();
}

QskTextOptions QskTextLabel::textOptions() const
{
    return m_data->textOptions;
}

Qt::Alignment QskTextLabel::alignment() const
{
    return flagHint< Qt::Alignment >(
        Text | QskAspect::Alignment, Qt::AlignLeft | Qt::AlignTop );
}

void QskTextLabel::setAlignment( Qt::Alignment alignment )
{
    if ( alignment == this->alignment() )
        return;

    const auto subControl = effectiveSubcontrol( Text );
    setFlagHint( subControl | QskAspect::Alignment, alignment );

    if ( m_data->text.isEmpty() )
        update();

    Q_EMIT alignmentChanged();
}

QFont QskTextLabel::font() const 
{
    return effectiveFont( QskTextLabel::Text );
}

bool QskTextLabel::isRichText() const
{
    if ( m_data->effectiveTextFormat == QskTextOptions::AutoText )
    {
        // caching the rich text evaluation
        const bool isRichRext = m_data->textOptions.isRichText( m_data->text );
        m_data->effectiveTextFormat = isRichRext ? QskTextOptions::RichText : QskTextOptions::PlainText;
    }

    return m_data->effectiveTextFormat;
}

QSizeF QskTextLabel::contentsSizeHint() const
{
    if ( !m_data->text.isEmpty() )
    {
        QskTextOptions options = textOptions();
        options.setFormat( isRichText() ? QskTextOptions::RichText : QskTextOptions::PlainText );

        return QskSkinRenderer::textSize(
            this, m_data->text, options, QskTextLabel::Text );
    }

    return QSizeF( 0, 0 );
}

qreal QskTextLabel::heightForWidth( qreal width ) const
{
    const qreal lineHeight = QFontMetricsF( effectiveFont( Text ) ).height();

    if ( m_data->text.isEmpty() ||
        ( m_data->textOptions.wrapMode() == QskTextOptions::NoWrap ) )
    {
        return lineHeight;
    }

    qreal maxHeight = std::numeric_limits< qreal >::max();
    if ( maxHeight / lineHeight > m_data->textOptions.maximumLineCount() )
    {
        // be careful with overflows
        maxHeight = m_data->textOptions.maximumLineCount() * lineHeight;
    }

    QskTextOptions options = textOptions();
    options.setFormat( isRichText() ? QskTextOptions::RichText : QskTextOptions::PlainText );

    const QSizeF size = QskSkinRenderer::textSize( this,
        QSizeF( width, maxHeight ), m_data->text, options, QskTextLabel::Text );

    return qCeil( size.height() );
}

qreal QskTextLabel::widthForHeight( qreal height ) const
{
    if ( m_data->text.isEmpty() ||
        ( m_data->textOptions.wrapMode() == QskTextOptions::NoWrap ) )
    {
        return Inherited::widthForHeight( height );
    }

    const qreal maxWidth = std::numeric_limits< qreal >::max();

    QskTextOptions options = textOptions();
    options.setFormat( isRichText() ? QskTextOptions::RichText : QskTextOptions::PlainText );

    const QSizeF size = QskSkinRenderer::textSize( this, 
        QSizeF( maxWidth, height ), m_data->text, options, QskTextLabel::Text );

    return qCeil( size.width() );
}

void QskTextLabel::changeEvent( QEvent* event )
{
    switch( event->type() )
    {
        case QEvent::LocaleChange:
        {
            if ( !m_data->text.isEmpty() )
            {
                // maybe Qt::LayoutDirection has changed
                update();
            }

            break;
        }
        default:
            break;
    }

    Inherited::changeEvent( event );
}

#include "moc_QskTextLabel.cpp"
