/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskStatusIndicatorSkinlet.h"
#include "QskStatusIndicator.h"
#include "QskGraphic.h"
#include "QskGraphicTextureFactory.h"
#include "QskAspect.h"
#include "QskSkin.h"
#include "QskTextureNode.h"
#include "QskFunctions.h"

QskStatusIndicatorSkinlet::QskStatusIndicatorSkinlet( QskSkin* skin ):
    Inherited( skin )
{
    setNodeRoles( { GraphicRole } );
}

QskStatusIndicatorSkinlet::~QskStatusIndicatorSkinlet() = default;

QRectF QskStatusIndicatorSkinlet::subControlRect(
    const QskSkinnable* skinnable, QskAspect::Subcontrol subControl ) const
{
    const auto label = static_cast< const QskStatusIndicator* >( skinnable );

    if ( subControl == QskStatusIndicator::Graphic )
    {
        return graphicRect( label );
    }

    return Inherited::subControlRect( skinnable, subControl );
}

QRect QskStatusIndicatorSkinlet::graphicRect(
    const QskStatusIndicator* indicator ) const
{
    // maybe we should have a common base class for
    // QskStatusIndicator and QskGraphicLabel

    // PreserveAspectFit/PreserveAspectCrop

    const QRect graphicRect = indicator->contentsRect().toAlignedRect();

    QSizeF sz = indicator->graphic( indicator->status() ).defaultSize();
    sz.scale( graphicRect.size(), Qt::KeepAspectRatio );

    return qskAlignedRect( graphicRect,
        ( int )sz.width(), ( int )sz.height(), Qt::AlignCenter );
}

QSGNode* QskStatusIndicatorSkinlet::updateSubNode(
    const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto indicator = static_cast< const QskStatusIndicator* >( skinnable );

    switch( nodeRole )
    {
        case GraphicRole:
            return updateGraphicNode( indicator, node );

        default:
            return nullptr;
    }
}

QSGNode* QskStatusIndicatorSkinlet::updateGraphicNode(
    const QskStatusIndicator* indicator, QSGNode* node ) const
{
    const auto subControl = QskStatusIndicator::Graphic;

    const QRectF rect = subControlRect( indicator, subControl );
    const Qt::Alignment alignment = indicator->flagHint< Qt::Alignment >(
        subControl | QskAspect::Alignment, Qt::AlignCenter );

    const int status = indicator->status();

    node = QskSkinlet::updateGraphicNode( indicator, node,
        indicator->graphic( status ), indicator->graphicFilter( status ),
        rect, alignment );

    return node;
}

#include "moc_QskStatusIndicatorSkinlet.cpp"
