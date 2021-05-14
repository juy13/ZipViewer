//
// Created by julian on 5/13/21.
//

#include "inc/mainwindow.h"
#include <QApplication>


#include "mz.h"
#ifdef HAVE_COMPAT
#include "mz_compat.h"
#endif
#include "mz_crypt.h"
#include "mz_os.h"
#include "mz_strm.h"
#ifdef HAVE_BZIP2
#include "mz_strm_bzip.h"
#endif
#ifdef HAVE_PKCRYPT
#include "mz_strm_pkcrypt.h"
#endif
#include "mz_strm_mem.h"
#include "mz_strm_os.h"
#ifdef HAVE_WZAES
#include "mz_strm_wzaes.h"
#endif
#ifdef HAVE_ZLIB
#include "mz_strm_zlib.h"
#endif
#include "mz_zip.h"

#include <cstdio> /* printf, snprintf */

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#  define snprintf _snprintf
#endif





int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    ZipViewerWin w;
    w.show();

    return a.exec();
}