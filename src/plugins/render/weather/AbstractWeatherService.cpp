//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "AbstractWeatherService.h"

namespace Marble {

AbstractWeatherService::AbstractWeatherService( QObject *parent )
    : QObject( parent )
{
}

AbstractWeatherService::~AbstractWeatherService() {
    
}

} // namespace Marble

#include "AbstractWeatherService.moc"
