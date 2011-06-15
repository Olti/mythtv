#ifndef MYTHVERBOSE_H_
#define MYTHVERBOSE_H_

#ifdef __cplusplus
#   include <cerrno>
#   include <QDateTime>
#   include <QString>
#   include <QTextStream>
#   include <QMutex>
#   include <iostream>
#else
#   include <errno.h>
# if HAVE_GETTIMEOFDAY
#   include <sys/time.h>
#   include <time.h> // for localtime()
# endif
#endif

#include "mythbaseexp.h"  //  MBASE_PUBLIC , et c.
#include "mythlogging.h"

/// This MAP is for the various VERBOSITY flags, used to select which
/// messages we want printed to the console.
///
/// The 5 fields are:
///     enum
///     enum value
///     "-v" arg string
///     additive flag (explicit = 0, additive = 1)
///     help text for "-v help"
///
/// To create a new VB_* flag, this is the only piece of code you need to
/// modify, then you can start using the new flag and it will automatically be
/// processed by the parse_verbose_arg() function and help info printed when
/// "-v help" is used.

#define VERBOSE_MAP(F) \
    F(VB_ALL,       0xffffffff, "all",                                        \
                             0, "ALL available debug output")                 \
    F(VB_MOST,      0x3ffeffff, "most",                                       \
                             0, "Most debug (nodatabase,notimestamp,noextra)")\
    F(VB_IMPORTANT, 0x00000001, "important",                                  \
                             0, "Errors or other very important messages")    \
    F(VB_GENERAL,   0x00000002, "general",                                    \
                             1, "General info")                               \
    F(VB_RECORD,    0x00000004, "record",                                     \
                             1, "Recording related messages")                 \
    F(VB_PLAYBACK,  0x00000008, "playback",                                   \
                             1, "Playback related messages")                  \
    F(VB_CHANNEL,   0x00000010, "channel",                                    \
                             1, "Channel related messages")                   \
    F(VB_OSD,       0x00000020, "osd",                                        \
                             1, "On-Screen Display related messages")         \
    F(VB_FILE,      0x00000040, "file",                                       \
                             1, "File and AutoExpire related messages")       \
    F(VB_SCHEDULE,  0x00000080, "schedule",                                   \
                             1, "Scheduling related messages")                \
    F(VB_NETWORK,   0x00000100, "network",                                    \
                             1, "Network protocol related messages")          \
    F(VB_COMMFLAG,  0x00000200, "commflag",                                   \
                             1, "Commercial detection related messages")      \
    F(VB_AUDIO,     0x00000400, "audio",                                      \
                             1, "Audio related messages")                     \
    F(VB_LIBAV,     0x00000800, "libav",                                      \
                             1, "Enables libav debugging")                    \
    F(VB_JOBQUEUE,  0x00001000, "jobqueue",                                   \
                             1, "JobQueue related messages")                  \
    F(VB_SIPARSER,  0x00002000, "siparser",                                   \
                             1, "Siparser related messages")                  \
    F(VB_EIT,       0x00004000, "eit",                                        \
                             1, "EIT related messages")                       \
    F(VB_VBI,       0x00008000, "vbi",                                        \
                             1, "VBI related messages")                       \
    F(VB_DATABASE,  0x00010000, "database",                                   \
                             1, "Display all SQL commands executed")          \
    F(VB_DSMCC,     0x00020000, "dsmcc",                                      \
                             1, "DSMCC carousel related messages")            \
    F(VB_MHEG,      0x00040000, "mheg",                                       \
                             1, "MHEG debugging messages")                    \
    F(VB_UPNP,      0x00080000, "upnp",                                       \
                             1, "upnp debugging messages")                    \
    F(VB_SOCKET,    0x00100000, "socket",                                     \
                             1, "socket debugging messages")                  \
    F(VB_XMLTV,     0x00200000, "xmltv",                                      \
                             1, "xmltv output and related messages")          \
    F(VB_DVBCAM,    0x00400000, "dvbcam",                                     \
                             1, "DVB CAM debugging messages")                 \
    F(VB_MEDIA,     0x00800000, "media",                                      \
                             1, "Media Manager debugging messages")           \
    F(VB_IDLE,      0x01000000, "idle",                                       \
                             1, "System idle messages")                       \
    F(VB_CHANSCAN,  0x02000000, "channelscan",                                \
                             1, "Channel Scanning messages")                  \
    F(VB_GUI,       0x04000000, "gui",                                        \
                             1, "GUI related messages")                       \
    F(VB_SYSTEM,    0x08000000, "system",                                     \
                             1, "External executable related messages")       \
    /* space for more flags */                                                \
    /* space for more flags */                                                \
    F(VB_EXTRA,     0x40000000, "extra",                                      \
                             1, "More detailed messages in selected levels")  \
    F(VB_TIMESTAMP, 0x80000000, "timestamp",                                  \
                             1, "Conditional data driven messages")           \
    F(VB_NONE,      0x00000000, "none",                                       \
                             0, "NO debug output")

#define VERBOSE_ENUM(ARG_ENUM, ARG_VALUE, ARG_STRING, ARG_ADDITIVE, ARG_HELP)\
    ARG_ENUM = ARG_VALUE ,

enum VerboseMask
{
    VERBOSE_MAP(VERBOSE_ENUM)
    VB_UNUSED_END // keep at end
};

/// This global variable is set at startup with the flags
/// of the verbose messages we want to see.
extern MBASE_PUBLIC unsigned int print_verbose_messages;
#ifdef __cplusplus
  extern MBASE_PUBLIC QMutex verbose_mutex;
#endif

// Helper for checking verbose flags outside of VERBOSE macro
#define VERBOSE_LEVEL_NONE        (print_verbose_messages == 0)
#define VERBOSE_LEVEL_CHECK(mask) ((print_verbose_messages & (mask)) == (mask))

// There are two VERBOSE macros now.  One for use with Qt/C++, one for use
// without Qt.
//
// Neither of them will lock the calling thread, but rather put the log message
// onto a queue.

#ifdef __cplusplus
#define VERBOSE(mask, ...) \
    LogPrintQString(mask, LOG_INFO, QString(__VA_ARGS__))
#else
#define VERBOSE(mask, ...) \
    LogPrint(mask, LOG_INFO, __VA_ARGS__)
#endif


#ifdef  __cplusplus
    /// Verbose helper function for ENO macro
    extern MBASE_PUBLIC QString safe_eno_to_string(int errnum);

    extern MBASE_PUBLIC QString verboseString;

    MBASE_PUBLIC int parse_verbose_arg(QString arg);

    /// This can be appended to the VERBOSE args with 
    /// "+".  Please do not use "<<".  It uses
    /// a thread safe version of strerror to produce the
    /// string representation of errno and puts it on the
    /// next line in the verbose output.
    #define ENO QString("\n\t\t\teno: ") + safe_eno_to_string(errno)
#endif


#endif

/* vim: set expandtab tabstop=4 shiftwidth=4: */
