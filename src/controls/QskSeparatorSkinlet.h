/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_SEPARATOR_SKINLET_H
#define QSK_SEPARATOR_SKINLET_H

#include "QskGlobal.h"
#include "QskSkinlet.h"

class QskSeparator;

class QSK_EXPORT QskSeparatorSkinlet : public QskSkinlet
{
    Q_GADGET

    using Inherited = QskSkinlet;

public:
    enum NodeRole
    {
        PanelRole,
    };

    Q_INVOKABLE QskSeparatorSkinlet( QskSkin* = nullptr );
    virtual ~QskSeparatorSkinlet();

    virtual QRectF subControlRect( const QskSkinnable*,
        QskAspect::Subcontrol ) const override;

protected:
    virtual QSGNode* updateSubNode( const QskSkinnable*,
        quint8 nodeRole, QSGNode* ) const override;

private:
    QRectF panelRect( const QskSeparator* ) const;
    QSGNode* updatePanelNode( const QskSeparator*, QSGNode* ) const;
};

#endif

