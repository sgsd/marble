/*
    Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>

    This file is part of the KDE project

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QDebug>

#include "GeoParser.h"
#include "GeoTagHandler.h"

// Set to a value greather than 0, to dump parent node chain while parsing
#define DUMP_PARENT_STACK 1

GeoParser::GeoParser(GeoDataGenericSourceType source)
    : QXmlStreamReader()
    , m_document(0)
    , m_source(source)
{
}

GeoParser::~GeoParser()
{
}

#if DUMP_PARENT_STACK > 0
static void dumpParentStack(const QString& name, int size, bool close)
{
    static int depth = 0;

    if (!close)
        depth++;

    QString result;
    for (int i = 0; i < depth; ++i)
        result += " ";

    if (close) {
        depth--;
        result += "</";
    } else
        result += "<";

    result += name + "> stack size " + QString::number(size);
    fprintf(stderr, "%s\n", qPrintable(result));
}
#endif

bool GeoParser::read(QIODevice* device)
{
    // Assert previous document got released.
    Q_ASSERT(!m_document);
    m_document = createDocument();
    Q_ASSERT(m_document);

    // Set data source
    setDevice(device);

    // Start parsing
    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (isValidDocumentElement()) {
#if DUMP_PARENT_STACK > 0
                dumpParentStack(name().toString(), m_nodeStack.size(), false);
#endif
 
                parseDocument();
                break;
            } else
                raiseDocumentElementError();
        }
    }

    if (error())
        qDebug() << "[GeoParser::read] -> Error occurred:" << errorString();

    return !error();
}

bool GeoParser::isValidElement(const QString& tagName) const
{
    return name() == tagName;
}

GeoStackItem GeoParser::parentElement(unsigned int depth)
{
    QStack<GeoStackItem>::const_iterator it = m_nodeStack.constEnd() - 1;

    if (it - depth < m_nodeStack.constBegin())
        return GeoStackItem();

    return *(it - depth);
}

void GeoParser::parseDocument()
{
    Q_ASSERT(isStartElement());

    while (!atEnd()) {
        readNext();
        GeoTagHandler::QualifiedName qName(name().toString(), namespaceUri().toString());

        if (isEndElement()) {
            if (!isValidDocumentElement())
                m_nodeStack.pop();

#if DUMP_PARENT_STACK > 0
            dumpParentStack(name().toString(), m_nodeStack.size(), true);
#endif
            break;
        }

        if (isStartElement()) {
            GeoStackItem stackItem(qName, 0);

            if (const GeoTagHandler* handler = GeoTagHandler::recognizes(qName))
                stackItem.assignNode(handler->parse(*this));

            m_nodeStack.push(stackItem);

#if DUMP_PARENT_STACK > 0
            dumpParentStack(name().toString(), m_nodeStack.size(), false);
#endif
 
            parseDocument();
        }
    }
}

void GeoParser::raiseDocumentElementError()
{
    raiseError(QObject::tr("File format unrecognized"));
}

GeoDocument* GeoParser::releaseDocument()
{
    GeoDocument* document = m_document;
    m_document = 0;
    return document;
}
