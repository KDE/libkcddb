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
    m_categories = KCDDB::Categories();
    m_category->insertStringList(m_categories.i18nList());
    m_genres = KCDDB::Genres();
    m_genre->insertStringList(m_genres.i18nList());

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
    m_category->setCurrentText(m_categories.cddb2i18n(info.category));

    // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
    m_revision->setText(QString::number(info.revision));
    m_genre->setCurrentText(m_genres.cddb2i18n(info.genre));
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
        if (info.artist != "Various" || separator == -1)
        {
            item->setText(TRACK_ARTIST, "");
            item->setText(TRACK_TITLE, title);
        }
        else
        {
            // We seem to have a compilation.
            item->setText(TRACK_ARTIST, title.left(separator));
            item->setText(TRACK_TITLE, title.mid(separator + 3));
        }
        item->setText(TRACK_COMMENT, info.trackInfoList[i].extt);
    }
    // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.

    if (info.artist == "Various")
        m_trackList->adjustColumn(TRACK_ARTIST);
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
    info.category = m_categories.i18n2cddb(m_category->currentText());
    info.genre = m_genres.i18n2cddb(m_genre->currentText());
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
        track.extt = item->text(TRACK_COMMENT).stripWhiteSpace();
        info.trackInfoList.append(track);
        // FIXME KDE4: handle track lengths here too, once KCDDBInfo::CDInfo is updated.
    }
    // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
    return info;
}


void CDInfoDialogBase::artistChanged( const QString &newArtist )
{
    // Enable special handling of compilations.
    if (newArtist.stripWhiteSpace().compare("Various") == 0)
    {
        for (QListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
        {
            QString title = item->text(TRACK_TITLE);
            int separator = title.find(SEPARATOR);
            if (separator != -1)
            {
                // Artists probably entered already
                item->setText(TRACK_ARTIST, title.left(separator));
                item->setText(TRACK_TITLE, title.mid(separator + 3));
            }
        }
        m_trackList->adjustColumn(TRACK_ARTIST);
        m_trackList->adjustColumn(TRACK_TITLE);
    }
    else
    {
        for (QListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
        {
            QString artist = item->text(TRACK_ARTIST);
            if (!artist.isEmpty())
            {
                item->setText(TRACK_ARTIST, QString::null);
                item->setText(TRACK_TITLE, artist + SEPARATOR + item->text(TRACK_TITLE));
            }
        }
        m_trackList->hideColumn(TRACK_ARTIST);
        m_trackList->adjustColumn(TRACK_TITLE);
    }
}

void CDInfoDialogBase::genreChanged( const QString &newGenre )
{
    // Disable changes to category if the version number indicates that a record
    // is already in the database, or if the genre is poorly set. The latter
    // condition also provides a "back-door" override.
    m_category->setEnabled((m_revision->text().stripWhiteSpace().toUInt() < 1) ||
                            (newGenre.compare("Unknown") == 0));
}
