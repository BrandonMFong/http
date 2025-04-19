/**
 * author: brando
 * date: 4/9/25
 */

#include "log.hpp"
#include <stdarg.h>
#include <bflibcpp/bflibcpp.hpp>
#include <sys/types.h>
#include <unistd.h>

extern "C" {
#include <bflibc/bflibc.h>
}

void _LogWriteEntry(BFFileWriter * filewriter, int mode, ...) {
#ifndef TESTING
	if (!filewriter) return;

	va_list arg0, arg1;
	va_start(arg0, mode);
	va_start(arg1, mode);

	const char * format = va_arg(arg0, const char *);
	if (!format) return;

	char * logstr = BFStringCreateFormatArgListString(format, arg0);
	if (!logstr) return;

	BFDateTime dt = {0};
	if (BFTimeGetCurrentDateTime(&dt)) return;

	format = "[%02d/%02d/%04d, %02d:%02d:%02d] - %s";

	BFFileWriterQueueFormatLine(
		filewriter,
		format,
		dt.month,
		dt.day,
		dt.year,
		dt.hour,
		dt.minute,
		dt.second,
		logstr
	);

#ifdef ENABLE_LOG_CONSOLE_PRINT
	printf(
		format,
		dt.month,
		dt.day,
		dt.year,
		dt.hour,
		dt.minute,
		dt.second,
		logstr
	);
	printf("\n");
	fflush(stdout);
#endif // ENABLE_LOG_CONSOLE_PRINT

	va_end(arg0);
	va_end(arg1);

	BFFree(logstr);
#endif
}

