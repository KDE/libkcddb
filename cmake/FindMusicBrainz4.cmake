# Module to find the musicbrainz-4 library
#
# It defines
#  MUSICBRAINZ4_INCLUDE_DIR - the include dir
#  MUSICBRAINZ4_LIBRARIES - the required libraries
#  MUSICBRAINZ4_FOUND - true if both of the above have been found

# Copyright (c) 2006,2007 Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(MUSICBRAINZ4_INCLUDE_DIR AND MUSICBRAINZ4_LIBRARIES)
   set(MUSICBRAINZ4_FIND_QUIETLY TRUE)
endif(MUSICBRAINZ4_INCLUDE_DIR AND MUSICBRAINZ4_LIBRARIES)

FIND_PATH(MUSICBRAINZ4_INCLUDE_DIR musicbrainz4/Disc.h)

FIND_LIBRARY( MUSICBRAINZ4_LIBRARIES NAMES musicbrainz4)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( MusicBrainz4 DEFAULT_MSG
                                   MUSICBRAINZ4_INCLUDE_DIR MUSICBRAINZ4_LIBRARIES)

MARK_AS_ADVANCED(MUSICBRAINZ4_INCLUDE_DIR MUSICBRAINZ4_LIBRARIES)

