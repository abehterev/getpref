getpref
=======

Search by phonenumber destination prefix and cost in MySQL DB

Usage: getpref -n `<phone_number>`

It's example how to search and to get the data which based on the phone
prefix. Used by me for small billing queries for getting cost of a destinations.
  
(C) 2014 by Andrey I. Bekhterev (info@behterev.su)
Released under the GPL (see license file)

make notes
==========

You can compile this application in two ways:
 * Production mode
 * Debug mode

For first case just do `make`.

For second case you can do `make debug`.

For clan "shit" files do `make clean`.

For init state do `make distclean`.
