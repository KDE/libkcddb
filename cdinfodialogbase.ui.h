/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
const char *CDInfoDialogBase::SEPARATOR = " / ";

void CDInfoDialogBase::init()
{
    // These are only 11 Category values defined by CDDB. See
    //
    // http://www.freedb.org/modules.php?name=Sections&sop=viewarticle&artid=26
    //
    m_categoryCDDB << "blues" << "classical" << "country" <<
        "data" << "folk" << "jazz" << "misc" <<
        "newage" << "reggae" << "rock" << "soundtrack";
    m_categoryI18n << i18n("Blues") << i18n("Classical") << i18n("Country") <<
        i18n("Data") << i18n("Folk") << i18n("Jazz") << i18n("Miscellaneous") <<
        i18n("New Age") << i18n("Reggae") << i18n("Rock") << i18n("Soundtrack");
    m_category->insertStringList(m_categoryI18n);

    // On the other hand, the Genre is completely arbitrary. But we follow
    // kaudiocreator's cue and make life easy for people.
    //
    // To cope with preexisting records which don't match an entry, we will
    // add one later if needed.
    m_genreCDDB << "Unknown" << "A Cappella" << "Acid Jazz" <<
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
    m_genreI18n << i18n("Unknown") << i18n("A Cappella") << i18n("Acid Jazz") <<
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
    m_genre->insertStringList(m_genreI18n);

    // We want control over the visibility of this column. See artistChanged().
    m_trackList->setColumnWidthMode(TRACK_ARTIST, QListView::Manual);

    // Make the user-definable values in-place editable.
    m_trackList->setRenameable(TRACK_NUMBER, false);
    m_trackList->setRenameable(TRACK_TIME, false);
    m_trackList->setRenameable(TRACK_TITLE, true);
    m_trackList->setRenameable(TRACK_COMMENT, true);
    m_trackList->setRenameable(TRACK_ARTIST, true);
}

void CDInfoDialogBase::destroy()
{
}

void CDInfoDialogBase::slotTrackSelected( QListViewItem *item )
{
    emit play(item->text(0).toUInt()-1);
}

void CDInfoDialogBase::slotNextTrack()
{
    if (m_trackList->currentItem())
    {
        QListViewItem *item = m_trackList->currentItem()->nextSibling();
        m_trackList->setSelected(item, true);
        m_trackList->ensureItemVisible(item);
    }
}

void CDInfoDialogBase::slotTrackDoubleClicked( QListViewItem *item, const QPoint &, int column)
{
    m_trackList->rename(item, column);
}

#include <kdebug.h>
void CDInfoDialogBase::setInfo( const KCDDB::CDInfo &info, KCDDB::TrackOffsetList &trackStartFrames )
{
    m_artist->setText(info.artist.stripWhiteSpace());
    m_title->setText(info.title.stripWhiteSpace());

    // Lookup the CDDB category, and display the i18n'd version.
    int index = m_categoryCDDB.findIndex(info.category.stripWhiteSpace());
    m_category->setCurrentItem(index);

    // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
    m_revision->setText(QString::number(info.revision));

    // Lookup the CDDB genre, and display the i18n'd version.
    QString userDefinedGenre = info.genre.stripWhiteSpace();
    index = m_genreCDDB.findIndex(userDefinedGenre);
    if (index != -1)
    {
        m_genre->setCurrentItem(index);
    }
    else
    {
        m_genre->setCurrentText(userDefinedGenre);
    }
    m_year->setValue(info.year);
    m_comment->setText(info.extd.stripWhiteSpace());
    m_id->setText(info.id.stripWhiteSpace());

    // Now do the individual tracks.
    unsigned tracks = info.trackInfoList.count();
    m_length->setText(framesTime(trackStartFrames[tracks + 1] - trackStartFrames[0]));
    m_trackList->clear();
    for (unsigned i = 0; i < tracks; i++)
    {
        QListViewItem *item = new QListViewItem(m_trackList, 0);

        item->setText(TRACK_NUMBER, QString().sprintf("%02d", i + 1));
        item->setText(TRACK_TIME, framesTime(trackStartFrames[i + ((i + 1 < tracks) ? 1 : 2)] - trackStartFrames[i]));
        QString title = info.trackInfoList[i].title;
        int separator = title.find(SEPARATOR);
        if (separator == -1)
        {
            item->setText(TRACK_ARTIST, "");
            item->setText(TRACK_TITLE, title);
        }
        else
        {
            // We seem to have a compilation.
            item->setText(TRACK_ARTIST, title.left(separator));
            item->setText(TRACK_TITLE, title.right(separator + 3));
        }
        item->setText(TRACK_COMMENT, info.trackInfoList[i].extt);
    }
	kdWarning() <<trackStartFrames<<endl;
    // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
}

QString CDInfoDialogBase::framesTime(unsigned frames)
{
    QTime time;
    double ms;

    ms = frames * 1000 / 75.0;
    time = time.addMSecs((int)ms);

    // Use ".zzz" for milliseconds...
    QString temp2;
    if (time.hour() > 0)
        temp2 = time.toString("hh:mm:ss");
    else
        temp2 = time.toString("mm:ss");
    return temp2;
} // framesTime

KCDDB::CDInfo CDInfoDialogBase::info() const
{
    KCDDB::CDInfo info;
    KCDDB::TrackInfo track;

    info.artist = m_artist->text().stripWhiteSpace();
    info.title = m_title->text().stripWhiteSpace();
    
    // Lookup the i18n category, and return the CDDB version.
    info.category = m_categoryCDDB[m_category->currentItem()];

    // Lookup the i18n genre, and return the CDDB version if we can.
    QString userDefinedGenre = m_genre->currentText().stripWhiteSpace();
    int index = m_genreI18n.findIndex(userDefinedGenre);
    if (index != -1)
    {
        info.genre = m_genreCDDB[index];
    }
    else
    {
        info.genre = userDefinedGenre;
    }
    info.year = m_year->value();
    info.extd = m_comment->text().stripWhiteSpace();
    info.revision = m_revision->text().stripWhiteSpace().toUInt();
    info.id = m_id->text().stripWhiteSpace();
    for (QListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
    {
        // Combine the track artist if present with the title.
        QString trackArtist = item->text(TRACK_ARTIST).stripWhiteSpace();
        track.title = trackArtist;
        if (!trackArtist.isEmpty())
        {
            track.title.append(SEPARATOR);
        }
        track.title.append(item->text(TRACK_TITLE).stripWhiteSpace());
        info.trackInfoList.append(track);
        track.extt = item->text(TRACK_COMMENT).stripWhiteSpace();
        // FIXME KDE4: handle track lengths here too, once KCDDBInfo::CDInfo is updated.
    }
    // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
    return info;
}


void CDInfoDialogBase::artistChanged( const QString &newArtist )
{
    // Enable special handling of compilations.
    if (newArtist.stripWhiteSpace().compare("Various") == 0)
        m_trackList->adjustColumn(TRACK_ARTIST);
    else
        m_trackList->hideColumn(TRACK_ARTIST);
}

void CDInfoDialogBase::genreChanged( const QString &newGenre )
{
    // Disable changes to category if the version number indicates that a record
    // is already in the database, or if the genre is poorly set. The latter
    // condition also provides a "back-door" override.
    m_category->setEnabled((m_revision->text().stripWhiteSpace().toUInt() < 1) ||
                            (newGenre.compare("Unknown") == 0));
}
