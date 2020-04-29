#include "LibDebug/Debug.h"
#include <camkes.h>

static OS_LoggerFilter_Handle_t filter;

static void testLogging();
static void tearDownLogging();

// TODO What is the max possible entry, as what's below seems to be the limit,
//  however nothing gets displayed when used.
//
// Max possible entry is:
// (DATABUFFER_SIZE - (OS_Logger_LOG_LEVEL_LENGTH + OS_Logger_LOG_LEVEL_LENGTH))
// == 4096 - (2 + 2)
const char maxPossibleLogEntry[];

const char format_string[] =
                    "%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%";

#define NO_FILTER -1

void post_init()
{
    if(NO_FILTER == log_lvl)
    {
        OS_LoggerEmitter_getInstance(logServer_buf, NULL, API_LOG_SERVER_EMIT);
    }
    else
    {
        OS_LoggerFilter_ctor(&filter, log_lvl);
        OS_LoggerEmitter_getInstance(logServer_buf, &filter, API_LOG_SERVER_EMIT);
    }
}

int run()
{
    testLogging();
    tearDownLogging();

    return 0;
}

void testLogging()
{
    Debug_LOG(Debug_LOG_LEVEL_NONE, "   NONE", "Debug_LOG_NONE");
    Debug_LOG(Debug_LOG_LEVEL_MAX,  "    MAX", "Debug_LOG_MAX");

    Debug_LOG_ASSERT("");
    Debug_LOG_ASSERT("%s", maxPossibleLogEntry);
    Debug_LOG_ASSERT("%s", format_string);

    Debug_LOG_ASSERT ("Debug_LOG_ASSERT");
    Debug_LOG_FATAL  ("Debug_LOG_FATAL");
    Debug_LOG_ERROR  ("Debug_LOG_ERROR");
    Debug_LOG_WARNING("Debug_LOG_WARNING");
    Debug_LOG_INFO   ("Debug_LOG_INFO");
    Debug_LOG_DEBUG  ("Debug_LOG_DEBUG");
    Debug_LOG_TRACE  ("Debug_LOG_TRACE");

    Debug_LOG(Debug_LOG_LEVEL_CUSTOM, " CUSTOM", "Debug_LOG_CUSTOM");
}

void tearDownLogging()
{
    OS_LoggerEmitter_dtor();
    OS_LoggerFilter_dtor(&filter);

    done_emit();
}

const char maxPossibleLogEntry[] =
    "Lorem ipsum dolor sit amet consectetur adipiscing elit nulla imperdiet "
    "tempus, class litora sagittis per vitae varius risus potenti platea, "
    "magnis pellentesque tristique nec volutpat lacinia ultrices conubia "
    "praesent. Sociis justo tempus turpis in bibendum volutpat sodales "
    "malesuada nisl magnis primis etiam vitae pellentesque potenti, aliquam "
    "torquent eu ante dignissim duis tincidunt congue tellus laoreet morbi "
    "feugiat class. Cum sem taciti mattis iaculis fringilla accumsan nec, quam "
    "purus cubilia nisl dictumst ad dis nulla, tempor egestas eu habitant "
    "massa justo. Sem fringilla iaculis rutrum malesuada litora auctor congue "
    "faucibus, ligula platea hendrerit tempor est aliquet integer, laoreet "
    "potenti nisl augue semper lacus lobortis. Nisl iaculis magna lectus odio "
    "facilisis est nullam, feugiat varius nunc nibh mi primis scelerisque "
    "conubia, cras nam malesuada tellus gravida diam. Ut habitasse quis "
    "rutrum venenatis sagittis dignissim orci blandit pretium ad magna, "
    "eleifend malesuada purus condimentum posuere sollicitudin eget "
    "ullamcorper tempor. Nam nostra nulla suscipit pharetra purus sociis "
    "molestie fames primis interdum dapibus, tempus ante justo tellus orci "
    "pretium sed dis ac urna id, accumsan habitasse lacinia tincidunt sociosqu "
    "sagittis netus euismod varius venenatis. Metus urna tempor primis massa "
    "porta libero auctor sem in tellus, nunc velit orci viverra elementum "
    "sociis ridiculus eget iaculis litora convallis, fusce quam vitae lacinia "
    "est taciti hac luctus tempus. Rhoncus litora mattis mus torquent fusce "
    "justo elementum fermentum, per tempor imperdiet facilisi conubia lacinia "
    "sociosqu, eros in mollis enim dis neque blandit. Posuere purus odio "
    "bibendum sagittis nisl metus, lobortis commodo mi ante urna. Congue "
    "aliquet felis hac euismod id, class viverra sociis massa in egestas, "
    "litora magna commodo ornare. Euismod in metus sociis pellentesque aenean "
    "blandit, cubilia nibh eu pharetra hendrerit parturient purus, nisl "
    "viverra pra.";

