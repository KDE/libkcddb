/*
    SPDX-FileCopyrightText: 2005 Shaheed Haque <srhaque@iee.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "genres.h"

#include "kcddbi18n.h"

class KCDDB::Genres::Private
{
public:
    QStringList cddb;
    QStringList i18n;
};

KCDDB::Genres::Genres()
    : d(new Private)
{
    // The Genre is completely arbitrary. But we follow kaudiocreator's cue
    // and make life easy for people.
    //
    // To cope with preexisting records which don't match an entry, we will
    // add one later if needed.
    d->cddb << QLatin1String( "Unknown" ) << QLatin1String( "A Cappella" ) << QLatin1String( "Acid Jazz" ) <<
            QLatin1String( "Acid Punk" ) << QLatin1String( "Acid" ) << QLatin1String( "Acoustic" ) << QLatin1String( "Alternative" ) <<
            QLatin1String( "Alt. Rock" ) << QLatin1String( "Ambient" ) << QLatin1String( "Anime" ) << QLatin1String( "Avantgarde" ) <<
            QLatin1String( "Ballad" ) << QLatin1String( "Bass" ) << QLatin1String( "Beat" ) << QLatin1String( "Bebop" ) <<
            QLatin1String( "Big Band" ) << QLatin1String( "Black Metal" ) << QLatin1String( "Bluegrass" ) << QLatin1String( "Blues" ) <<
            QLatin1String( "Booty Bass" ) << QLatin1String( "BritPop" ) << QLatin1String( "Cabaret" ) << QLatin1String( "Celtic" ) <<
            QLatin1String( "Chamber Music" ) << QLatin1String( "Chanson" ) << QLatin1String( "Chorus" ) << QLatin1String( "Christian Gangsta Rap" ) <<
            QLatin1String( "Christian Rap" ) << QLatin1String( "Christian Rock" ) << QLatin1String( "Classical" ) << QLatin1String( "Classic Rock" ) <<
            QLatin1String( "Club-house" ) << QLatin1String( "Club" ) << QLatin1String( "Comedy" ) << QLatin1String( "Contemporary Christian" ) <<
            QLatin1String( "Country" ) << QLatin1String( "Crossover" ) << QLatin1String( "Cult" ) << QLatin1String( "Dance Hall" ) <<
            QLatin1String( "Dance" ) << QLatin1String( "Darkwave" ) << QLatin1String( "Death Metal" ) << QLatin1String( "Disco" ) <<
            QLatin1String( "Dream" ) << QLatin1String( "Drum & Bass" ) << QLatin1String( "Drum Solo" ) << QLatin1String( "Duet" ) <<
            QLatin1String( "Easy Listening" ) << QLatin1String( "Electronic" ) << QLatin1String( "Ethnic" ) << QLatin1String( "Eurodance" ) <<
            QLatin1String( "Euro-House" ) << QLatin1String( "Euro-Techno" ) << QLatin1String( "Fast-Fusion" ) << QLatin1String( "Folklore" ) <<
            QLatin1String( "Folk/Rock" ) << QLatin1String( "Folk" ) << QLatin1String( "Freestyle" ) << QLatin1String( "Funk" ) <<
            QLatin1String( "Fusion" ) << QLatin1String( "Game" ) << QLatin1String( "Gangsta Rap" ) << QLatin1String( "Goa" ) <<
            QLatin1String( "Gospel" ) << QLatin1String( "Gothic Rock" ) << QLatin1String( "Gothic" ) << QLatin1String( "Grunge" ) <<
            QLatin1String( "Hardcore" ) << QLatin1String( "Hard Rock" ) << QLatin1String( "Heavy Metal" ) << QLatin1String( "Hip-Hop" ) <<
            QLatin1String( "House" ) << QLatin1String( "Humor" ) << QLatin1String( "Indie" ) << QLatin1String( "Industrial" ) <<
            QLatin1String( "Instrumental Pop" ) << QLatin1String( "Instrumental Rock" ) << QLatin1String( "Instrumental" ) << QLatin1String( "Jazz+Funk" ) <<
            QLatin1String( "Jazz" ) << QLatin1String( "JPop" ) << QLatin1String( "Jungle" ) << QLatin1String( "Latin" ) << QLatin1String( "Lo-Fi" ) <<
            QLatin1String( "Meditative" ) << QLatin1String( "Merengue" ) << QLatin1String( "Metal" ) << QLatin1String( "Musical" ) <<
            QLatin1String( "National Folk" ) << QLatin1String( "Native American" ) << QLatin1String( "Negerpunk" ) << QLatin1String( "New Age" ) <<
            QLatin1String( "New Wave" ) << QLatin1String( "Noise" ) << QLatin1String( "Oldies" ) << QLatin1String( "Opera" ) <<
            QLatin1String( "Other" ) << QLatin1String( "Polka" ) << QLatin1String( "Polsk Punk" ) << QLatin1String( "Pop-Funk" ) <<
            QLatin1String( "Pop/Funk" ) << QLatin1String( "Pop" ) << QLatin1String( "Porn Groove" ) << QLatin1String( "Power Ballad" ) <<
            QLatin1String( "Pranks" ) << QLatin1String( "Primus" ) << QLatin1String( "Progressive Rock" ) << QLatin1String( "Psychedelic Rock" ) <<
            QLatin1String( "Psychedelic" )<< QLatin1String( "Punk Rock" ) << QLatin1String( "Punk" ) << QLatin1String( "R&B" ) <<
            QLatin1String( "Rap" ) << QLatin1String( "Rave" ) << QLatin1String( "Reggae" ) << QLatin1String( "Retro" ) <<
            QLatin1String( "Revival" ) << QLatin1String( "Rhythmic Soul" ) << QLatin1String( "Rock & Roll" ) << QLatin1String( "Rock" ) <<
            QLatin1String( "Salsa" ) << QLatin1String( "Samba" ) << QLatin1String( "Satire" ) << QLatin1String( "Showtunes" ) <<
            QLatin1String( "Ska" ) << QLatin1String( "Slow Jam" ) << QLatin1String( "Slow Rock" ) << QLatin1String( "Sonata" ) <<
            QLatin1String( "Soul" ) << QLatin1String( "Sound Clip" ) << QLatin1String( "Soundtrack" ) << QLatin1String( "Southern Rock" ) <<
            QLatin1String( "Space" ) << QLatin1String( "Speech" ) << QLatin1String( "Swing" ) << QLatin1String( "Symphonic Rock" ) <<
            QLatin1String( "Symphony" ) << QLatin1String( "Synthpop" ) << QLatin1String( "Tango" ) << QLatin1String( "Techno-Industrial" ) <<
            QLatin1String( "Techno" ) << QLatin1String( "Terror" ) << QLatin1String( "Thrash Metal" ) << QLatin1String( "Top 40" ) <<
            QLatin1String( "Trailer" ) << QLatin1String( "Trance" ) << QLatin1String( "Tribal" ) << QLatin1String( "Trip-Hop" ) <<
            QLatin1String( "Vocal" );
    d->i18n << i18n("Unknown") << i18n("A Cappella") << i18n("Acid Jazz") <<
            i18n("Acid Punk") << i18n("Acid") << i18n("Acoustic") << i18n("Alternative") <<
            i18n("Alt. Rock") << i18n("Ambient") << i18n("Anime") << i18n("Avantgarde") <<
            i18n("Ballad") << i18n("Bass") << i18n("Beat") << i18n("Bebop") <<
            i18n("Big Band") << i18n("Black Metal") << i18n("Bluegrass") << i18n("Blues") <<
            i18n("Booty Bass") << i18n("BritPop") << i18n("Cabaret") << i18n("Celtic") <<
            i18n("Chamber Music") << i18n("Chanson") << i18n("Chorus") << i18n("Christian Gangsta Rap") <<
            i18n("Christian Rap") << i18n("Christian Rock") << i18n("Classical") << i18n("Classic Rock") <<
            i18n("Club-house") << i18n("Club") << i18n("Comedy") << i18n("Contemporary Christian") <<
            i18nc("music genre", "Country") << i18n("Crossover") << i18n("Cult") << i18n("Dance Hall") <<
            i18n("Dance") << i18n("Darkwave") << i18n("Death Metal") << i18n("Disco") <<
            i18n("Dream") << i18n("Drum & Bass") << i18n("Drum Solo") << i18n("Duet") <<
            i18n("Easy Listening") << i18n("Electronic") << i18n("Ethnic") << i18n("Eurodance") <<
            i18n("Euro-House") << i18n("Euro-Techno") << i18n("Fast-Fusion") << i18n("Folklore") <<
            i18n("Folk/Rock") << i18n("Folk") << i18n("Freestyle") << i18n("Funk") <<
            i18n("Fusion") << i18n("Game") << i18n("Gangsta Rap") << i18n("Goa") <<
            i18n("Gospel") << i18n("Gothic Rock") << i18n("Gothic") << i18n("Grunge") <<
            i18n("Hardcore") << i18n("Hard Rock") << i18n("Heavy Metal") << i18n("Hip-Hop") <<
            i18n("House") << i18n("Humor") << i18n("Indie") << i18n("Industrial") <<
            i18n("Instrumental Pop") << i18n("Instrumental Rock") << i18n("Instrumental") << i18n("Jazz+Funk") <<
            i18n("Jazz") << i18n("JPop") << i18n("Jungle") << i18n("Latin") << i18n("Lo-Fi") <<
            i18n("Meditative") << i18n("Merengue") << i18n("Metal") << i18n("Musical") <<
            i18n("National Folk") << i18n("Native American") << i18n("Negerpunk") << i18n("New Age") <<
            i18n("New Wave") << i18n("Noise") << i18n("Oldies") << i18n("Opera") <<
            i18n("Other") << i18n("Polka") << i18n("Polsk Punk") << i18n("Pop-Funk") <<
            i18n("Pop/Funk") << i18n("Pop") << i18n("Porn Groove") << i18n("Power Ballad") <<
            i18n("Pranks") << i18n("Primus") << i18n("Progressive Rock") << i18n("Psychedelic Rock") <<
            i18n("Psychedelic") << i18n("Punk Rock") << i18n("Punk") << i18n("R&B") <<
            i18n("Rap") << i18n("Rave") << i18n("Reggae") << i18n("Retro") <<
            i18n("Revival") << i18n("Rhythmic Soul") << i18n("Rock & Roll") << i18n("Rock") <<
            i18n("Salsa") << i18n("Samba") << i18n("Satire") << i18n("Showtunes") <<
            i18n("Ska") << i18n("Slow Jam") << i18n("Slow Rock") << i18n("Sonata") <<
            i18n("Soul") << i18n("Sound Clip") << i18n("Soundtrack") << i18n("Southern Rock") <<
            i18n("Space") << i18n("Speech") << i18n("Swing") << i18n("Symphonic Rock") <<
            i18n("Symphony") << i18n("Synthpop") << i18n("Tango") << i18n("Techno-Industrial") <<
            i18n("Techno") << i18n("Terror") << i18n("Thrash Metal") << i18n("Top 40") <<
            i18n("Trailer") << i18n("Trance") << i18n("Tribal") << i18n("Trip-Hop") <<
            i18n("Vocal");
}

KCDDB::Genres::~Genres()
{
    delete d;
}

KCDDB::Genres::Genres(const Genres& other)
    :d(new Private)
{
    *d = *other.d;
}

KCDDB::Genres& KCDDB::Genres::operator=(const Genres& other)
{
    *d = *other.d;
    return *this;
}

const QStringList &KCDDB::Genres::cddbList() const
{
    return d->cddb;
}

const QStringList &KCDDB::Genres::i18nList() const
{
    return d->i18n;
}

const QString KCDDB::Genres::cddb2i18n(const QString &genre) const
{
    QString userDefinedGenre = genre.trimmed();
    int index = d->cddb.indexOf(userDefinedGenre);
    if (index != -1)
    {
        return d->i18n[index];
    }
    else
    {
        return userDefinedGenre;
    }
}

const QString KCDDB::Genres::i18n2cddb(const QString &genre) const
{
    QString userDefinedGenre = genre.trimmed();
    int index = d->i18n.indexOf(userDefinedGenre);
    if (index != -1)
    {
        return d->cddb[index];
    }
    else
    {
        return userDefinedGenre;
    }
}
