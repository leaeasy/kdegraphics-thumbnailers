/**
 Copyright (C) 2008  Unai Garro <ugarro@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
**/

#include "pdfcreator.h"

#include <poppler-qt5.h>
#include <kdcraw/kdcraw.h>
#include <KPluginFactory>


K_PLUGIN_CLASS_WITH_JSON(PDFCreator, "pdfthumbnail.json")

PDFCreator::PDFCreator(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args)
{
}

PDFCreator::~PDFCreator()
{
}

KIO::ThumbnailResult PDFCreator::create(const KIO::ThumbnailRequest &request)
{
    //load the image into the QByteArray
    QByteArray data;
    const QString path = request.url().toLocalFile();

    bool loaded=KDcrawIface::KDcraw::loadEmbeddedPreview(data, path);

    if (!loaded) {
        return KIO::ThumbnailResult::fail();
    }

    //Load the image into a QImage
    QImage preview;
    if (!preview.loadFromData(data) || preview.isNull()) {
        return KIO::ThumbnailResult::fail();
    }

    Poppler::Document* document = Poppler::Document::load(path);
    if (!document || document->isLocked()) {
        delete document;
        return KIO::ThumbnailResult::fail();
    }
    
    // Access page of the PDF file
    Poppler::Page* pdfPage = document->page(0);  // Document starts at page 0

    // Generate a QImage of the rendered page
    QImage image = pdfPage->renderToImage();
    if (image.isNull()) {
        delete pdfPage;
        delete document;
        return KIO::ThumbnailResult::fail();
    }

    //Scale the image as requested by the thumbnailer
    QImage img=preview.scaled(request.targetSize(),Qt::KeepAspectRatio);
    delete pdfPage;
    delete document;
    return KIO::ThumbnailResult::pass(img);
}

#include "pdfcreator.moc"
