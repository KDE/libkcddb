// Copyright (C) 2005 by Shaheed Haque (srhaque@iee.org). All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//

#include <genres.h>
#include <klocale.h>

KCDDB::Genres::Genres()
{
    // The Genre is completely arbitrary. But we follow kaudiocreator's cue
    // and make life easy for people.
    //
    // To cope with preexisting records which don't match an entry, we will
    // add one later if needed.
    m_cddb << "Unknown" << "A Cappella" << "Acid Jazz" <<
            "Acid Punk" << "Acid" << "Acoustic" << "Alternative" <<
            "Alt. Rock" << "Ambient" << "Anime" << "Avantgarde" <<
            "Ballad" << "Bass" << "Beat" << "Bebop" <<
            "Big Band" << "Black Metal" << "Bluegrass" << "Blues" <<
            "Booty Bass" << "BritPop" << "Cabaret" << "Celtic" <<
            "Chamber Music" << "Chanson" << "Chorus" << "Christian Gangsta Rap" <<
            "Christian Rap" << "Christian Rock" << "Classical" << "Classic Rock" <<
            "Club-house" << "Club" << "Comedy" << "Contemporary Christian" <<
            "Country" << "Crossover" << "Cult" << "Dance Hall" <<
            "Dance" << "Darkwave" << "Death Metal" << "Disco" <<
            "Dream" << "Drum & Bass" << "Drum Solo" << "Duet" <<
            "Easy Listening" << "Electronic" << "Ethnic" << "Eurodance" <<
            "Euro-House" << "Euro-Techno" << "Fast-Fusion" << "Folklore" <<
            "Folk/Rock" << "Folk" << "Freestyle" << "Funk" <<
            "Fusion" << "Game" << "Gangsta Rap" << "Goa" <<
            "Gospel" << "Gothic Rock" << "Gothic" << "Grunge" <<
            "Hardcore" << "Hard Rock" << "Heavy Metal" << "Hip-Hop" <<
            "House" << "Humor" << "Indie" << "Industrial" <<
            "Instrumental Pop" << "Instrumental Rock" << "Instrumental" << "Jazz+Funk" <<
            "Jazz" << "JPop" << "Jungle" << "Latin" << "Lo-Fi" <<
            "Meditative" << "Merengue" << "Metal" << "Musical" <<
            "National Folk" << "Native American" << "Negerpunk" << "New Age" <<
            "New Wave" << "Noise" << "Oldies" << "Opera" <<
            "Other" << "Polka" << "Polsk Punk" << "Pop-Funk" <<
            "Pop/Funk" << "Pop" << "Porn Groove" << "Power Ballad" <<
            "Pranks" << "Primus" << "Progressive Rock" << "Psychedelic Rock" <<
            "Psychedelic" << "Punk Rock" << "Punk" << "R&B" <<
            "Rap" << "Rave" << "Reggae" << "Retro" <<
            "Revival" << "Rhythmic Soul" << "Rock & Roll" << "Rock" <<
            "Salsa" << "Samba" << "Satire" << "Showtunes" <<
            "Ska" << "Slow Jam" << "Slow Rock" << "Sonata" <<
            "Soul" << "Sound Clip" << "Soundtrack" << "Southern Rock" <<
            "Space" << "Speech" << "Swing" << "Symphonic Rock" <<
            "Symphony" << "Synthpop" << "Tango" << "Techno-Industrial" <<
            "Techno" << "Terror" << "Thrash Metal" << "Top 40" <<
            "Trailer" << "Trance" << "Tribal" << "Trip-Hop" <<
            "Vocal";
    m_i18n << i18n("Unknown") << i18n("A Cappella") << i18n("Acid Jazz") <<
            i18n("Acid Punk") << i18n("Acid") << i18n("Acoustic") << i18n("Alternative") <<
            i18n("Alt. Rock") << i18n("Ambient") << i18n("Anime") << i18n("Avantgarde") <<
            i18n("Ballad") << i18n("Bass") << i18n("Beat") << i18n("Bebop") <<
            i18n("Big Band") << i18n("Black Metal") << i18n("Bluegrass") << i18n("Blues") <<
            i18n("Booty Bass") << i18n("BritPop") << i18n("Cabaret") << i18n("Celtic") <<
            i18n("Chamber Music") << i18n("Chanson") << i18n("Chorus") << i18n("Christian Gangsta Rap") <<
            i18n("Christian Rap") << i18n("Christian Rock") << i18n("Classical") << i18n("Classic Rock") <<
            i18n("Club-house") << i18n("Club") << i18n("Comedy") << i18n("Contemporary Christian") <<
            i18n("Country") << i18n("Crossover") << i18n("Cult") << i18n("Dance Hall") <<
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

const QString KCDDB::Genres::cddb2i18n(const QString &genre) const
{
    QString userDefinedGenre = genre.stripWhiteSpace();
    int index = m_cddb.findIndex(userDefinedGenre);
    if (index != -1)
    {
        return m_i18n[index];
    }
    else
    {
        return userDefinedGenre;
    }
}

const QString KCDDB::Genres::i18n2cddb(const QString &genre) const
{
    QString userDefinedGenre = genre.stripWhiteSpace();
    int index = m_i18n.findIndex(userDefinedGenre);
    if (index != -1)
    {
        return m_cddb[index];
    }
    else
    {
        return userDefinedGenre;
    }
}
