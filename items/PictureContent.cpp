/***************************************************************************
 *                                                                         *
 *   This file is part of the Fotowall project,                            *
 *       http://code.google.com/p/fotowall                                 *
 *                                                                         *
 *   Copyright (C) 2007-2009 by Enrico Ros <enrico.ros@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "PictureContent.h"
#include "ButtonItem.h"
#include "CPixmap.h"
#include "PictureProperties.h"
#include "RenderOpts.h"
#include "frames/Frame.h"
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QUrl>

PictureContent::PictureContent(QGraphicsScene * scene, QGraphicsItem * parent)
    : AbstractContent(scene, parent, false)
    , m_photo(0)
    , m_opaquePhoto(false)
    , m_progress(0)
    , m_netWidth(0)
    , m_netHeight(0)
    , m_netReply(0)
{
    // enable frame text
    setFrameTextEnabled(true);
    setFrameText(tr("..."));

    // allow dropping
    setAcceptDrops(true);

    // add flipping buttons
    ButtonItem * bFlipH = new ButtonItem(ButtonItem::FlipH, Qt::blue, QIcon(":/data/action-flip-horizontal.png"), this);
    bFlipH->setToolTip(tr("Flip horizontally"));
    bFlipH->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    connect(bFlipH, SIGNAL(clicked()), this, SIGNAL(flipHorizontally()));
    addButtonItem(bFlipH);

    ButtonItem * bFlipV = new ButtonItem(ButtonItem::FlipV, Qt::blue, QIcon(":/data/action-flip-vertical.png"), this);
    bFlipV->setToolTip(tr("Flip vertically"));
    bFlipV->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    addButtonItem(bFlipV);
    connect(bFlipV, SIGNAL(clicked()), this, SIGNAL(flipVertically()));

    /*ButtonItem * bCrop = new ButtonItem(ButtonItem::Control, Qt::blue, QIcon(":/data/action-scale.png"), this);
    bCrop->setToolTip(tr(""));
    bCrop->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    addButtonItem(bCrop);
    connect(bCrop, SIGNAL(clicked()), this, SIGNAL(toggleCropMode()));*/
}

PictureContent::~PictureContent()
{
    dropNetworkConnection();
    delete m_photo;
}

bool PictureContent::loadPhoto(const QString & fileName, bool keepRatio, bool setName)
{
    dropNetworkConnection();
    delete m_photo;
    m_cachedPhoto = QPixmap();
    m_opaquePhoto = false;
    m_photo = 0;
    m_fileUrl = QString();
    m_netWidth = 0;
    m_netHeight = 0;

    m_photo = new CPixmap(fileName);
    if (m_photo->isNull()) {
        delete m_photo;
        m_photo = 0;
        return false;
    }

    m_opaquePhoto = !m_photo->hasAlpha();
    m_fileUrl = fileName;
    if (keepRatio)
        resetContentsRatio();
    if (setName) {
        QString string = QFileInfo(fileName).fileName().section('.', 0, 0);
        string = string.mid(0, 10);
        setFrameText(string + tr("..."));
    }
    applyPostLoadEffects();

    // notify image change
    emit contentChanged();
    return true;
}

bool PictureContent::loadFromNetwork(const QString & url, QNetworkReply * reply, const QString & title, int width, int height)
{
    dropNetworkConnection();
    delete m_photo;
    m_cachedPhoto = QPixmap();
    m_opaquePhoto = false;
    m_photo = 0;
    m_fileUrl = url;
    m_netWidth = width;
    m_netHeight = height;

    // start a download if not passed as a paramenter
    if (!reply) {
        // the QNAM will be auto-deleted on closure
        QNetworkAccessManager * nam = new QNetworkAccessManager(this);
        QNetworkRequest request(url);
        m_netReply = nam->get(request);
    } else
        m_netReply = reply;

    // set title
    if (!title.isEmpty()) {
        setFrameTextEnabled(true);
        setFrameText(title);
    }

#if QT_VERSION >= 0x040600
    // Immediate Decode: just handle the reply if done
    if (m_netReply->isFinished())
        return slotLoadNetworkData();
#else
    // No Precaching ensures signals to be emitted later
#endif

    // Deferred Decode: listen to the network job
    setAcceptHoverEvents(false);
    setControlsVisible(false);
    m_progress = 0.01;
    connect(m_netReply, SIGNAL(finished()), this, SLOT(slotLoadNetworkData()));
    connect(m_netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkError()));
    connect(m_netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotNetworkProgress(qint64,qint64)));

    // reset size, if got the network one
    if (m_netWidth > 0 && m_netHeight > 0)
        resetContentsRatio();
    return true;
}

void PictureContent::addEffect(const PictureEffect & effect)
{
    if (!m_photo)
        return;

    m_photo->addEffect(effect);
    if (effect.effect == PictureEffect::Crop) {
        QRect actualContentRect = contentsRect();
        float reduceRatio = (float)(effect.croppingRect.width()+effect.croppingRect.height())/
                            (float)(actualContentRect.height() +actualContentRect.width());
        resizeContents(QRect(0,0, (float)effect.croppingRect.width()/reduceRatio, (float)effect.croppingRect.height()/reduceRatio));
    }
#if QT_VERSION >= 0x040500
    else if(effect.effect == PictureEffect::Opacity)
        setOpacity(effect.param);
#endif
    m_cachedPhoto = QPixmap();
    update();
    GFX_CHANGED();

    // notify image change
    emit contentChanged();
}

#include "PropertyEditors.h"
QWidget * PictureContent::createPropertyWidget()
{
    PictureProperties * p = new PictureProperties();

    // connect actions
    PE_AbstractSlider * pe = new PE_AbstractSlider(p->sOpacity, this, "opacity", p);

    // properties link
    //p->bEditShape->setChecked(isShapeEditing());
    //connect(this, SIGNAL(notifyShapeEditing(bool)), p->bEditShape, SLOT(setChecked(bool)));
    //connect(p->bEditShape, SIGNAL(toggled(bool)), this, SLOT(setShapeEditing(bool)));

    return p;
}

bool PictureContent::fromXml(QDomElement & pe)
{
    AbstractContent::fromXml(pe);

    // load picture properties
    QString path = pe.firstChildElement("path").text();

    // build the afterload effects list
    m_afterLoadEffects.clear();
    QDomElement effectsE = pe.firstChildElement("effects");
    for (QDomElement effectE = effectsE.firstChildElement("effect"); effectE.isElement(); effectE = effectE.nextSiblingElement("effect")) {
        PictureEffect fx;
        fx.effect = (PictureEffect::Effect)effectE.attribute("type").toInt();
        fx.param = effectE.attribute("param").toDouble();
        if (fx.effect == PictureEffect::Crop) {
            QString rect = effectE.attribute("croppingRect");
            QStringList coordinates = rect.split(" ");
            if(coordinates.size() >= 3) {
                QRect croppingRect (coordinates.at(0).toInt(), coordinates.at(1).toInt(), coordinates.at(2).toInt(), coordinates.at(3).toInt());
                fx.croppingRect = croppingRect;
            }
        }
#if QT_VERSION >= 0x040500
        else if (fx.effect == PictureEffect::Opacity)
            setOpacity(fx.param);
#endif
        m_afterLoadEffects.append(fx);
    }

    // load Network image
    if (path.startsWith("http", Qt::CaseInsensitive) || path.startsWith("ftp", Qt::CaseInsensitive))
        return loadFromNetwork(path, 0);

    // load Local image
    return loadPhoto(path);
}

void PictureContent::toXml(QDomElement & pe) const
{
    AbstractContent::toXml(pe);
    pe.setTagName("picture");

    // save picture properties
    QDomDocument doc = pe.ownerDocument();
    QDomElement domElement;
    QDomText text;

    // save image url (wether is a local path or remote url)
    domElement = doc.createElement("path");
    pe.appendChild(domElement);
    text = doc.createTextNode(m_fileUrl);
    domElement.appendChild(text);

    // save the effects
    domElement = doc.createElement("effects");
    pe.appendChild(domElement);
    QList<PictureEffect> effectsList = m_afterLoadEffects;
    if (m_photo)
#if QT_VERSION >= 0x040500
        effectsList.append(m_photo->effects());
#else
        foreach(const PictureEffect & effect, m_photo->effects())
            effectsList.append(effect);
#endif
    foreach (const PictureEffect & effect, effectsList) {
        QDomElement effectElement = doc.createElement("effect");
        effectElement.setAttribute("type", effect.effect);
        effectElement.setAttribute("param", effect.param);
        if(effect.effect == PictureEffect::Crop) {
            QString croppingRectStr;
            croppingRectStr = QString::number(effect.croppingRect.x()) + " " + QString::number(effect.croppingRect.y())
                + " " + QString::number(effect.croppingRect.width()) + " " + QString::number(effect.croppingRect.height());

            effectElement.setAttribute("croppingRect", croppingRectStr );
        }
        domElement.appendChild(effectElement);
    }
}

QPixmap PictureContent::renderContent(const QSize & size, Qt::AspectRatioMode ratio) const
{
    if (m_photo)
        return ratioScaledPixmap(m_photo, size, ratio);
    return QPixmap();
}

int PictureContent::contentHeightForWidth(int width) const
{
    if (m_netWidth > 0 && m_netHeight > 0)
        return (m_netHeight * width) / m_netWidth;
    if (!m_photo || m_photo->width() < 1)
        return -1;
    return (m_photo->height() * width) / m_photo->width();
}

bool PictureContent::contentOpaque() const
{
    return m_opaquePhoto;
}

void PictureContent::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    // load the first valid picture
    foreach (const QUrl & url, event->mimeData()->urls()) {
        // handle network drops
        if (url.scheme() == "http" || url.scheme() == "ftp") {
            if (loadFromNetwork(url.toString(), 0)) {
                event->accept();
                return;
            }
        }
        // handle local drops
        if (QFile::exists(url.toLocalFile())) {
            if (loadPhoto(url.toLocalFile(), true, true)) {
                event->accept();
                return;
            }
        }
    }
}

void PictureContent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    emit backgroundMe();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void PictureContent::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // paint parent
    AbstractContent::paint(painter, option, widget);

    // draw progress
    if (m_progress > 0.0 && m_progress < 1.0) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        painter->drawPie(QRect(-10, -10, 20, 20), 90 * 16, (int)(-5760.0 * m_progress));
    }

    // skip if no photo
    if (!m_photo)
        return;

    // blit if opaque picture
#if QT_VERSION >= 0x040500
    //disabled for 4.5 too, since it relies on raster.
    //if (m_opaquePhoto)
    //    painter->setCompositionMode(QPainter::CompositionMode_Source);
#endif

    // draw high-resolution photo when exporting png
    QRect targetRect = contentsRect();
    if (RenderOpts::HQRendering) {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter->drawPixmap(targetRect, *m_photo);
        return;
    }

    // draw photo using caching and deferred rescales
    if (beingTransformed()) {
        if (!m_cachedPhoto.isNull())
            painter->drawPixmap(targetRect, m_cachedPhoto);
    } else {
        if (m_cachedPhoto.isNull() || m_cachedPhoto.size() != targetRect.size())
            m_cachedPhoto = m_photo->scaled(targetRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter->setRenderHints(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(targetRect.topLeft(), m_cachedPhoto);
    }

#if QT_VERSION >= 0x040500
//    if (m_opaquePhoto)
//        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
#endif
}

CPixmap PictureContent::getPhoto() const
{
    return *m_photo;
}

void PictureContent::dropNetworkConnection()
{
    if (m_netReply) {
        m_netReply->disconnect(0,0,0);
#if QT_VERSION >= 0x040600
        if (!m_netReply->isFinished())
#endif
            m_netReply->abort();
        m_netReply->deleteLater();
        m_netReply = 0;
    }
    m_progress = 0.0;
}

void PictureContent::applyPostLoadEffects()
{
    foreach (const PictureEffect & effect, m_afterLoadEffects)
        m_photo->addEffect(effect);
    m_afterLoadEffects.clear();
    update();
    GFX_CHANGED();
}

bool PictureContent::slotLoadNetworkData()
{
    // get the data
    if (!m_netReply)
        return false;
    QByteArray replyData = m_netReply->readAll();
    dropNetworkConnection();

    // make the QImage from data
    QImage image = QImage::fromData(replyData);
    if (image.isNull())
        return false;

    // update contents
    m_netWidth = image.width();
    m_netHeight = image.height();
    resetContentsRatio();
    m_progress = 1.0;
    m_photo = new CPixmap(image);
    m_opaquePhoto = !m_photo->hasAlpha();
    applyPostLoadEffects();
    setAcceptHoverEvents(true);

    // notify image change
    emit contentChanged();
    return true;
}

void PictureContent::slotNetworkError()
{
    // clear state
    if (!m_netReply)
        return;
    dropNetworkConnection();
    m_progress = 0.0;
    setAcceptHoverEvents(true);
}

void PictureContent::slotNetworkProgress(qint64 a, qint64 b)
{
    m_progress = b > 0 ? (double)a / (double)b : 0.0;
    update();
}
