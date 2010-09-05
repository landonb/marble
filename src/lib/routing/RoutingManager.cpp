//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#include "RoutingManager.h"

#include "AlternativeRoutesModel.h"
#include "MarbleWidget.h"
#include "MarbleModel.h"
#include "RouteRequest.h"
#include "RoutingModel.h"
#include "MarbleRunnerManager.h"
#include "AdjustNavigation.h"

#include <QtGui/QMessageBox>

namespace Marble
{

class RoutingManagerPrivate
{
public:
    RoutingManager* q;

    RoutingModel *m_routingModel;

    MarbleModel *m_marbleModel;

    AlternativeRoutesModel* m_alternativeRoutesModel;

    RouteRequest *m_routeRequest;

    bool m_workOffline;

    RoutingManagerPrivate( MarbleModel *marbleModel, RoutingManager* manager, QObject *parent );

    MarbleRunnerManager* m_runnerManager;

    bool m_haveRoute;

    AdjustNavigation *m_adjustNavigation;
};

RoutingManagerPrivate::RoutingManagerPrivate( MarbleModel *model, RoutingManager* manager, QObject *parent ) :
        q( manager ),
        m_routingModel( new RoutingModel( model ) ),
        m_marbleModel( model ),
        m_alternativeRoutesModel(new AlternativeRoutesModel( model, parent ) ),
        m_routeRequest( new RouteRequest( manager ) ),
        m_workOffline( false ),
        m_runnerManager( new MarbleRunnerManager( model->pluginManager(), q ) ),
        m_haveRoute( false ), m_adjustNavigation( 0 )
{
    // nothing to do
}

RoutingManager::RoutingManager( MarbleModel *marbleModel, QObject *parent ) : QObject( parent ),
        d( new RoutingManagerPrivate( marbleModel, this, this ) )
{
    connect( d->m_runnerManager, SIGNAL( routeRetrieved( GeoDataDocument* ) ),
             this, SLOT( retrieveRoute( GeoDataDocument* ) ) );
}

RoutingManager::~RoutingManager()
{
    delete d;
}

RoutingModel *RoutingManager::routingModel()
{
    return d->m_routingModel;
}

void RoutingManager::retrieveRoute( RouteRequest *route )
{
    d->m_routeRequest = route;
    updateRoute();
}

void RoutingManager::updateRoute()
{
    if ( !d->m_routeRequest ) {
        return;
    }

    d->m_haveRoute = false;

    int realSize = 0;
    for ( int i = 0; i < d->m_routeRequest->size(); ++i ) {
        // Sort out dummy targets
        if ( d->m_routeRequest->at( i ).longitude() != 0.0 && d->m_routeRequest->at( i ).latitude() != 0.0 ) {
            ++realSize;
        }
    }

    d->m_alternativeRoutesModel->newRequest( d->m_routeRequest );
    if ( realSize > 1 ) {
        emit stateChanged( RoutingManager::Downloading, d->m_routeRequest );
        d->m_runnerManager->setWorkOffline( d->m_workOffline );
        d->m_runnerManager->retrieveRoute( d->m_routeRequest );
    } else {
        d->m_routingModel->clear();
        emit stateChanged( RoutingManager::Retrieved, d->m_routeRequest );
    }
}

void RoutingManager::setWorkOffline( bool offline )
{
    d->m_workOffline = offline;
}

void RoutingManager::retrieveRoute( GeoDataDocument* route )
{
    d->m_alternativeRoutesModel->addRoute( route );

    if ( !d->m_haveRoute ) {
        d->m_haveRoute = true;
        emit stateChanged( Retrieved, d->m_routeRequest );
    }

    emit routeRetrieved( route );
}

AlternativeRoutesModel* RoutingManager::alternativeRoutesModel()
{
    return d->m_alternativeRoutesModel;
}

void RoutingManager::setAdjustNavigation( AdjustNavigation* adjustNavigation )
{
    d->m_adjustNavigation = adjustNavigation;
}

AdjustNavigation* RoutingManager::adjustNavigation()
{
    return d->m_adjustNavigation;
}

RouteRequest* RoutingManager::routeRequest()
{
    return d->m_routeRequest;
}

} // namespace Marble

#include "RoutingManager.moc"
