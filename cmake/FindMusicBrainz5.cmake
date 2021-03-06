# Module to find the musicbrainz-4 library
#
# It defines
#  MUSICBRAINZ5_INCLUDE_DIRS - the include dirs
#  MUSICBRAINZ5_LIBRARIES - the required libraries
#  MUSICBRAINZ5_FOUND - true if both of the above have been found

# SPDX-FileCopyrightText: 2006, 2007 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

if(MUSICBRAINZ5_INCLUDE_DIR AND MUSICBRAINZ5_LIBRARIES)
    set(MUSICBRAINZ5_FIND_QUIETLY TRUE)
endif()

find_path(MUSICBRAINZ5_INCLUDE_DIR musicbrainz5/Disc.h)

find_library(MUSICBRAINZ5_LIBRARIES NAMES musicbrainz5cc)
if (NOT MUSICBRAINZ5_LIBRARIES)
    find_library(MUSICBRAINZ5_LIBRARIES NAMES musicbrainz5)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MusicBrainz5 DEFAULT_MSG MUSICBRAINZ5_INCLUDE_DIR MUSICBRAINZ5_LIBRARIES)

add_library(musicbrainz SHARED IMPORTED)
set_target_properties(musicbrainz PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MUSICBRAINZ5_INCLUDE_DIR}"
    IMPORTED_LOCATION "${MUSICBRAINZ5_LIBRARIES}"
)

mark_as_advanced(MUSICBRAINZ5_INCLUDE_DIR MUSICBRAINZ5_LIBRARIES)
