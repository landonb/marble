//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010 Utku Aydın <utkuaydin34@gmail.com>
//

#ifndef OPENDESKTOPITEM_H
#define OPENDESKTOPITEM_H
 
#include <QAction>
#include "AbstractDataPluginItem.h"
#include "TinyWebBrowser.h"
#include "MarbleWidget.h"

namespace Marble
{

class TinyWebBrowser;
 
class OpenDesktopItem : public AbstractDataPluginItem
{
    Q_OBJECT

    public:
        explicit OpenDesktopItem(QObject *parent);

        ~OpenDesktopItem();

        bool initialized() const;

        void addDownloadedFile( const QString& url, const QString& type );
        
        void paint( QPainter *painter );

        bool operator<( const AbstractDataPluginItem *other ) const;

        void updateToolTip();
        
        QAction *action();
        
        QUrl profileUrl() const;

        QUrl avatarUrl() const;

        void setAvatarUrl( const QUrl& url );

        QString fullName() const;

        void setFullName(const QString& fullName );

        QString location() const;

        void setLocation(const QString& location );

        QString role() const;

        void setRole(const QString& role );

        void setMarbleWidget(MarbleWidget *widget);
    
    public Q_SLOTS:
        void openBrowser();

    private:
        MarbleWidget *m_marbleWidget;
        QString m_fullName;
        QString m_location;
        QString m_role;
        QPixmap m_pixmap;
        QUrl m_avatarUrl;
        QAction* m_action;
};
 
}
#endif // OPENDESKTOPITEM_H
