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
    // DON'T i18n them!
    m_category->insertItem("blues");
    m_category->insertItem("classical");
    m_category->insertItem("country");
    m_category->insertItem("data");
    m_category->insertItem("folk");
    m_category->insertItem("jazz");
    m_category->insertItem("newage");
    m_category->insertItem("reggae");
    m_category->insertItem("rock");
    m_category->insertItem("soundtrack");
    m_category->insertItem("misc");

    // On the other hand, the Genre is completely arbitrary. But we follow
    // kaudiocreator's cue and make life easy for people.
    //
    // To cope with preexisting records which don't match an entry, we will
    // add one later if needed.
    m_genre->insertItem("Unknown");
    m_genre->insertItem("A Cappella");
    m_genre->insertItem("Acid Jazz");
    m_genre->insertItem("Acid Punk");
    m_genre->insertItem("Acid");
    m_genre->insertItem("Acoustic");
    m_genre->insertItem("Alternative");
    m_genre->insertItem("Alt. Rock");
    m_genre->insertItem("Ambient");
    m_genre->insertItem("Anime");
    m_genre->insertItem("Avantgarde");
    m_genre->insertItem("Ballad");
    m_genre->insertItem("Bass");
    m_genre->insertItem("Beat");
    m_genre->insertItem("Bebop");
    m_genre->insertItem("Big Band");
    m_genre->insertItem("Black Metal");
    m_genre->insertItem("Bluegrass");
    m_genre->insertItem("Blues");
    m_genre->insertItem("Booty Bass");
    m_genre->insertItem("BritPop");
    m_genre->insertItem("Cabaret");
    m_genre->insertItem("Celtic");
    m_genre->insertItem("Chamber Music");
    m_genre->insertItem("Chanson");
    m_genre->insertItem("Chorus");
    m_genre->insertItem("Christian Gangsta Rap");
    m_genre->insertItem("Christian Rap");
    m_genre->insertItem("Christian Rock");
    m_genre->insertItem("Classical");
    m_genre->insertItem("Classic Rock");
    m_genre->insertItem("Club-house");
    m_genre->insertItem("Club");
    m_genre->insertItem("Comedy");
    m_genre->insertItem("Contemporary Christian");
    m_genre->insertItem("Country");
    m_genre->insertItem("Crossover");
    m_genre->insertItem("Cult");
    m_genre->insertItem("Dance Hall");
    m_genre->insertItem("Dance");
    m_genre->insertItem("Darkwave");
    m_genre->insertItem("Death Metal");
    m_genre->insertItem("Disco");
    m_genre->insertItem("Dream");
    m_genre->insertItem("Drum & Bass");
    m_genre->insertItem("Drum Solo");
    m_genre->insertItem("Duet");
    m_genre->insertItem("Easy Listening");
    m_genre->insertItem("Electronic");
    m_genre->insertItem("Ethnic");
    m_genre->insertItem("Eurodance");
    m_genre->insertItem("Euro-House");
    m_genre->insertItem("Euro-Techno");
    m_genre->insertItem("Fast-Fusion");
    m_genre->insertItem("Folklore");
    m_genre->insertItem("Folk/Rock");
    m_genre->insertItem("Folk");
    m_genre->insertItem("Freestyle");
    m_genre->insertItem("Funk");
    m_genre->insertItem("Fusion");
    m_genre->insertItem("Game");
    m_genre->insertItem("Gangsta Rap");
    m_genre->insertItem("Goa");
    m_genre->insertItem("Gospel");
    m_genre->insertItem("Gothic Rock");
    m_genre->insertItem("Gothic");
    m_genre->insertItem("Grunge");
    m_genre->insertItem("Hardcore");
    m_genre->insertItem("Hard Rock");
    m_genre->insertItem("Heavy Metal");
    m_genre->insertItem("Hip-Hop");
    m_genre->insertItem("House");
    m_genre->insertItem("Humor");
    m_genre->insertItem("Indie");
    m_genre->insertItem("Industrial");
    m_genre->insertItem("Instrumental Pop");
    m_genre->insertItem("Instrumental Rock");
    m_genre->insertItem("Instrumental");
    m_genre->insertItem("Jazz+Funk");
    m_genre->insertItem("Jazz");
    m_genre->insertItem("JPop");
    m_genre->insertItem("Jungle");
    m_genre->insertItem("Latin");
    m_genre->insertItem("Lo-Fi");
    m_genre->insertItem("Meditative");
    m_genre->insertItem("Merengue");
    m_genre->insertItem("Metal");
    m_genre->insertItem("Musical");
    m_genre->insertItem("National Folk");
    m_genre->insertItem("Native American");
    m_genre->insertItem("Negerpunk");
    m_genre->insertItem("New Age");
    m_genre->insertItem("New Wave");
    m_genre->insertItem("Noise");
    m_genre->insertItem("Oldies");
    m_genre->insertItem("Opera");
    m_genre->insertItem("Other");
    m_genre->insertItem("Polka");
    m_genre->insertItem("Polsk Punk");
    m_genre->insertItem("Pop-Funk");
    m_genre->insertItem("Pop/Funk");
    m_genre->insertItem("Pop");
    m_genre->insertItem("Porn Groove");
    m_genre->insertItem("Power Ballad");
    m_genre->insertItem("Pranks");
    m_genre->insertItem("Primus");
    m_genre->insertItem("Progressive Rock");
    m_genre->insertItem("Psychedelic Rock");
    m_genre->insertItem("Psychedelic");
    m_genre->insertItem("Punk Rock");
    m_genre->insertItem("Punk");
    m_genre->insertItem("R&B");
    m_genre->insertItem("Rap");
    m_genre->insertItem("Rave");
    m_genre->insertItem("Reggae");
    m_genre->insertItem("Retro");
    m_genre->insertItem("Revival");
    m_genre->insertItem("Rhythmic Soul");
    m_genre->insertItem("Rock & Roll");
    m_genre->insertItem("Rock");
    m_genre->insertItem("Salsa");
    m_genre->insertItem("Samba");
    m_genre->insertItem("Satire");
    m_genre->insertItem("Showtunes");
    m_genre->insertItem("Ska");
    m_genre->insertItem("Slow Jam");
    m_genre->insertItem("Slow Rock");
    m_genre->insertItem("Sonata");
    m_genre->insertItem("Soul");
    m_genre->insertItem("Sound Clip");
    m_genre->insertItem("Soundtrack");
    m_genre->insertItem("Southern Rock");
    m_genre->insertItem("Space");
    m_genre->insertItem("Speech");
    m_genre->insertItem("Swing");
    m_genre->insertItem("Symphonic Rock");
    m_genre->insertItem("Symphony");
    m_genre->insertItem("Synthpop");
    m_genre->insertItem("Tango");
    m_genre->insertItem("Techno-Industrial");
    m_genre->insertItem("Techno");
    m_genre->insertItem("Terror");
    m_genre->insertItem("Thrash Metal");
    m_genre->insertItem("Top 40");
    m_genre->insertItem("Trailer");
    m_genre->insertItem("Trance");
    m_genre->insertItem("Tribal");
    m_genre->insertItem("Trip-Hop");
    m_genre->insertItem("Vocal");

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


void CDInfoDialogBase::setInfo( const KCDDB::CDInfo &info )
{
    m_artist->setText(info.artist.stripWhiteSpace());
    m_title->setText(info.title.stripWhiteSpace());
    m_category->setCurrentText(info.category.stripWhiteSpace());
    // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
    m_revision->setText(QString::number(info.revision));
    m_genre->setCurrentText(info.genre.stripWhiteSpace());
    m_year->setValue(info.year);
    m_comment->setText(info.extd.stripWhiteSpace());
    m_id->setText(info.id.stripWhiteSpace());

    m_trackList->clear();
    for (unsigned i = 1; i < info.trackInfoList.count(); i++)
    {
        QListViewItem *item = new QListViewItem(m_trackList, 0);

        // FIXME KDE4: handle track and disc lengths here too, once KCDDBInfo::CDInfo is updated.
        item->setText(TRACK_NUMBER, QString().sprintf("%02d", i + 1));
        item->setText(TRACK_TITLE, info.trackInfoList[i].title);

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

    // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
}


KCDDB::CDInfo CDInfoDialogBase::info() const
{
    KCDDB::CDInfo info;
    KCDDB::TrackInfo track;

    info.artist = m_artist->text().stripWhiteSpace();
    info.title = m_title->text().stripWhiteSpace();
    info.category = m_category->currentText();
    info.genre = m_genre->currentText().stripWhiteSpace();
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
