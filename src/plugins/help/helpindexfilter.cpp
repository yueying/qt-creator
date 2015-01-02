/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "helpindexfilter.h"

#include "centralwidget.h"

#include <topicchooser.h>

#include <extensionsystem/pluginmanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/helpmanager.h>

#include <QIcon>

using namespace Core;
using namespace Help;
using namespace Help::Internal;

Q_DECLARE_METATYPE(ILocatorFilter*)

HelpIndexFilter::HelpIndexFilter()
{
    setId("HelpIndexFilter");
    setDisplayName(tr("Help Index"));
    setIncludedByDefault(false);
    setShortcutString(QString(QLatin1Char('?')));

    m_icon = QIcon(QLatin1String(":/help/images/bookmark.png"));
}

HelpIndexFilter::~HelpIndexFilter()
{
}

void HelpIndexFilter::prepareSearch(const QString &entry)
{
    if (entry.length() < 2)
        m_keywords = Core::HelpManager::findKeywords(entry, caseSensitivity(entry), 200);
    else
        m_keywords = Core::HelpManager::findKeywords(entry, caseSensitivity(entry));
}

QList<LocatorFilterEntry> HelpIndexFilter::matchesFor(QFutureInterface<LocatorFilterEntry> &future, const QString &entry)
{
    Q_UNUSED(entry) // search is already done in the GUI thread in prepareSearch
    QList<LocatorFilterEntry> entries;
    foreach (const QString &keyword, m_keywords) {
        if (future.isCanceled())
            break;
        entries.append(LocatorFilterEntry(this, keyword, QVariant(), m_icon));
    }

    return entries;
}

void HelpIndexFilter::accept(LocatorFilterEntry selection) const
{
    const QString &key = selection.displayName;
    const QMap<QString, QUrl> &links = Core::HelpManager::linksForKeyword(key);

    if (links.size() == 1)
        emit linkActivated(links.begin().value());
    else
        emit linksActivated(links, key);
}

void HelpIndexFilter::refresh(QFutureInterface<void> &future)
{
    Q_UNUSED(future)
    // Nothing to refresh
}