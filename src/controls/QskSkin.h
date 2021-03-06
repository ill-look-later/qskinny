/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_SKIN_H
#define QSK_SKIN_H

#include "QskGlobal.h"
#include "QskAspect.h"

#include <QObject>
#include <QColor>

#include <unordered_map>
#include <set>
#include <memory>
#include <type_traits>

class QskControl;
class QskSkinnable;
class QskSkinlet;

class QskColorFilter;
class QskGraphic;
class QskGraphicProvider;

class QskAnimationHint;

class QVariant;

class QSK_EXPORT QskSkin : public QObject
{
    Q_OBJECT

    using Inherited = QObject;

public:
    enum SkinFontRole
    {
        DefaultFont = 0,

        TinyFont,
        SmallFont,
        MediumFont,
        LargeFont,
        HugeFont
    };

    Q_ENUM( SkinFontRole )

    QskSkin( QObject* parent = nullptr );
    virtual ~QskSkin();

    template<typename Control, typename Skinlet> void declareSkinlet();

    virtual void resetColors( const QColor& accent );

    void setColor( QskAspect::Aspect, Qt::GlobalColor );
    void setColor( QskAspect::Aspect, QRgb );
    void setColor( QskAspect::Aspect, const QColor& );

    QColor color( QskAspect::Aspect ) const;
    QColor baseColor() const;

    void setMetric( QskAspect::Aspect, qreal metric );
    qreal metric( QskAspect::Aspect ) const;

    void setAnimation( QskAspect::Aspect, QskAnimationHint );
    QskAnimationHint animation( QskAspect::Aspect ) const;

    void setSkinHint( QskAspect::Aspect, const QVariant& hint );
    void removeSkinHint( QskAspect::Aspect );

    const QVariant& skinHint( QskAspect::Aspect ) const;

    void setGraphicFilter( int graphicRole, const QskColorFilter& );
    void resetGraphicFilter( int graphicRole );
    QskColorFilter graphicFilter( int graphicRole ) const;

    void setFont( int fontRole, const QFont& );
    void resetFont( int fontRole );
    QFont font( int fontRole ) const;

    void setupFonts( const QString& family,
        int weight = -1, bool italic = false );

    virtual QskGraphic symbol( int symbolType ) const;

    void addGraphicProvider( const QString& providerId, QskGraphicProvider* );
    QskGraphicProvider* graphicProvider( const QString& providerId ) const;
    bool hasGraphicProvider() const;

    virtual const int *dialogButtonLayout( Qt::Orientation ) const;

    QskSkinlet* skinlet( const QskSkinnable* );

    const std::set< QskAspect::Aspect >& animatorAspects( QskAspect::Subcontrol ) const;
    const std::unordered_map< QskAspect::Aspect, QVariant >& skinHints() const;
    const std::unordered_map< int, QFont >& fonts() const;
    const std::unordered_map< int, QskColorFilter >& graphicFilters() const;

private:
    void declareSkinlet( const QMetaObject* controlMetaObject,
        const QMetaObject* skinMetaObject );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

template<typename Control, typename Skinlet>
inline void QskSkin::declareSkinlet()
{
    Q_STATIC_ASSERT( ( std::is_base_of< QskControl, Control >::value ) );
    Q_STATIC_ASSERT( ( std::is_base_of< QskSkinlet, Skinlet >::value ) );
    declareSkinlet( &Control::staticMetaObject, &Skinlet::staticMetaObject );
}

#endif
