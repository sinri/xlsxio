XLSX I/O
========
Cross-platform C library for reading values from and writing values to .xlsx files.

Description
-----------
XLSX I/O aims to provide a C library for reading and writing .xlsx files.
The .xlsx file format is the native format used by Microsoft(R) Excel(TM) since version 2007.

Goal
----
The library was written with the following goals in mind:
- written in standard C, but allows being used by C++
- simple interface
- small footprint
- portable across different platforms (Windows, *nix)
- minimal dependancies: only depends on expat (only for reading) and libzip (which in turn depends on zlib)
- separate library for reading and writing .xlsx files

Reading .xlsx files:
- intended to process .xlsx files as a data table, which assumes the following:
  + assumes the first row contains header names
  + assumes the next rows contain values in the same columns as where the header names are supplied
  + only values are processed, anything else is ignored (formulas, layout, graphics, charts, ...)
- the entire shared string table is loaded in memory (warning: could be large for big spreadsheets with a lot of different values)
- supports .xlsx files without shared string table
- worksheet data itself is read on the fly without the need to buffer data in memory
- 2 methods are provided
  + a simple method that allows the application to iterate trough rows and cells
  + an advanced method (with less overhead) which calls callback functions for each cell and after each row

Writing .xlsx files:
- intended for writing data tables as .xlsx files, which assumes the following:
  + only support for writing data (no support for formulas, layout, graphics, charts, ...)
  + no support for multiple worksheets (only one worksheet per file)
- on the fly file generation without the need to buffer data in memory
- no support for shared strings (all values are written as inline strings)

Command Line Utilities
----------------------
Some command line utilties are included:
- xlsxio_xlsx2csv: converts all sheets in all specified .xlsx files to individual CSV (Comma Separated Values) files.
- xlsxio_csv2xlsx: converts all specified CSV (Comma Separated Values) files to .xlsx files.

Dependancies
------------
This project has the following depencancies:
- [libzip](http://www.nih.at/libzip/) (libxlsxio_read and libxlsxio_write)
- [expat](http://www.libexpat.org/) (only for libxlsxio_read)

License
-------
XLSX I/O is released under the terms of the MIT License (MIT), see LICENSE.txt.

SINRI NOTE
-------

Run at base:

	cmake .
	make
	make install

Then the installations are:

	# Debian 7

	-- Install configuration: ""
	-- Installing: /usr/local/lib/libxlsxio_read.a
	-- Installing: /usr/local/lib/libxlsxio_write.a
	-- Installing: /usr/local/lib/libxlsxio_read.so
	-- Removed runtime path from "/usr/local/lib/libxlsxio_read.so"
	-- Installing: /usr/local/lib/libxlsxio_write.so
	-- Removed runtime path from "/usr/local/lib/libxlsxio_write.so"
	-- Installing: /usr/local/bin/xlsxio_xlsx2csv
	-- Removed runtime path from "/usr/local/bin/xlsxio_xlsx2csv"
	-- Installing: /usr/local/bin/xlsxio_csv2xlsx
	-- Removed runtime path from "/usr/local/bin/xlsxio_csv2xlsx"
	-- Installing: /usr/local/bin/example_xlsxio_write_getversion
	-- Removed runtime path from "/usr/local/bin/example_xlsxio_write_getversion"
	-- Installing: /usr/local/bin/example_xlsxio_write
	-- Removed runtime path from "/usr/local/bin/example_xlsxio_write"
	-- Installing: /usr/local/bin/example_xlsxio_read
	-- Removed runtime path from "/usr/local/bin/example_xlsxio_read"
	-- Installing: /usr/local/bin/example_xlsxio_read_advanced
	-- Removed runtime path from "/usr/local/bin/example_xlsxio_read_advanced"
	-- Installing: /usr/local/include
	-- Installing: /usr/local/include/xlsxio_read.h
	-- Installing: /usr/local/include/xlsxio_write.h
	-- Installing: /usr/local/include/xlsxio_version.h

	# Mac OS

	-- Install configuration: ""
	-- Installing: /usr/local/lib/libxlsxio_read.a
	-- Installing: /usr/local/lib/libxlsxio_write.a
	-- Installing: /usr/local/lib/libxlsxio_read.dylib
	-- Installing: /usr/local/lib/libxlsxio_write.dylib
	-- Installing: /usr/local/bin/xlsxio_xlsx2csv
	-- Installing: /usr/local/bin/xlsxio_csv2xlsx
	-- Installing: /usr/local/bin/example_xlsxio_write_getversion
	-- Installing: /usr/local/bin/example_xlsxio_write
	-- Installing: /usr/local/bin/example_xlsxio_read
	-- Installing: /usr/local/bin/example_xlsxio_read_advanced
	-- Installing: /usr/local/include
	-- Installing: /usr/local/include/xlsxio_read.h
	-- Installing: /usr/local/include/xlsxio_version.h
	-- Installing: /usr/local/include/xlsxio_write.h

Compile sth in folder sinri:

	gcc ../libxlsxio_read.dylib ../libxlsxio_write.dylib a.c ../libxlsxio_read.a ../libxlsxio_write.a
	# On LeqeeAliyunTest Debian Wheezy
	gcc /usr/local/lib/libxlsxio_write.so /usr/local/lib/libxlsxio_read.so SinriExcel2CSV.c /usr/local/lib/libxlsxio_read.a /usr/local/lib/libxlsxio_write.a -o SinriExcel2CSV
