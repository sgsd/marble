//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.

#include "jsonparser.h"

jsonParser::jsonParser()
{
}

jsonParser::~jsonParser()
{
}

twitterDataStructure jsonParser::parseObjectOnPosition ( const QString &content , int requiredObjectPosition )
{
        QString temp="var myJSONObject =" + content;
        myEngine.evaluate ( temp );

        dataStorage.user = myEngine.evaluate ( QString ( "return myJSONObject.photos[" )
                                               + QString::number ( requiredObjectPosition )
                                               + QString ( "].user;" ) ).toString();
        dataStorage.location = myEngine.evaluate ( QString ( "return myJSONObject.photos[" )
                               + QString::number ( requiredObjectPosition )
                               + QString ( "].location;" ) ).toString();
        dataStorage.text = myEngine.evaluate ( QString ( "return myJSONObject.photos[" )
                                               + QString::number ( requiredObjectPosition )
                                               + QString ( "].text;" ) ).toString();


        return dataStorage;
}

QList <twitterDataStructure> jsonParser::parseAllObjects ( const QString &content ,int numberOfObjects )
{
        QString temp = "var myJSONObject = " + content;
        int iterator = 0;//the count starts fom one

        myEngine.evaluate ( temp );
        while ( ( iterator ) < numberOfObjects ) {
                dataStorage.user = myEngine.evaluate ( QString ( "return myJSONObject[" )
                                                       + QString::number ( iterator )
                                                       + QString ( "].user.name;" ) ).toString();
                dataStorage.location = myEngine.evaluate ( QString ( "return myJSONObject[" )
                                       + QString::number ( iterator )
                                       + QString ( "].location;" ) ).toString();
                dataStorage.text = myEngine.evaluate ( QString ( "return myJSONObject[" )
                                                       + QString::number ( iterator )
                                                       + QString ( "].text;" ) ).toString();
                parsedJsonOutput.insert ( iterator , dataStorage );
                ++iterator;
        }

        return parsedJsonOutput;
}

googleMapDataStructure jsonParser::parseObject(QString &content)
{
QString temp = "var myJSONObject = " + content;
int iterator = 0;//the count starts fom one
googleMapDataStructure returnStructure;

myEngine.evaluate ( temp );
returnStructure.lat=myEngine.evaluate ( QString ( "return myJSONObject[" )
+ QString::number ( iterator )
+ QString ( "].user.name;" ) ).toString();;
returnStructure.lon=;
// qDebug
return returnStructure;
}