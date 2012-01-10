#  Copyright 2005 Eduardo Sztokbant <du@du.eti.br>
#
#  This file is part of stegotools.
#
#  stegotools is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  stegotools is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with stegotools; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

##########
# test.sh
#     stegotools testing shell-script
######################################

#!/bin/sh

if [ "`uname`" = "FreeBSD" ]; then
  md5app=gmd5sum
else
  md5app=md5sum
fi

# TEST 1a: message integrity test (n_last_bits = 1)
echo -n "  input message md5 = output message md5 (n_last_bits = 1): "

md5tux="`$md5app samples/tux.xpm | cut -d ' ' -f 1`"

cat samples/tux.xpm | ./stegwrite samples/Maddog-Du.bmp ./tmp.bmp 1 2>/dev/null
./stegread ./tmp.bmp 1 2>/dev/null > ./tmp.xpm
md5tmpxpm="`$md5app ./tmp.xpm | cut -d ' ' -f 1`"

if [ "$md5tmpxpm" = "$md5tux" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      md5in : $md5tux"
    echo "      md5out: $md5tmpxpm"
fi


# TEST 1b: message integrity test (n_last_bits = 2)
echo -n "  input message md5 = output message md5 (n_last_bits = 2): "

md5tux="`$md5app samples/tux.xpm | cut -d ' ' -f 1`"

cat samples/tux.xpm | ./stegwrite samples/Maddog-Du.bmp ./tmp.bmp 2 2>/dev/null
./stegread ./tmp.bmp 2 2>/dev/null > ./tmp.xpm
md5tmpxpm="`$md5app ./tmp.xpm | cut -d ' ' -f 1`"

if [ "$md5tmpxpm" = "$md5tux" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      md5in : $md5tux"
    echo "      md5out: $md5tmpxpm"
fi


# TEST 1c: message integrity test (n_last_bits = 4)
echo -n "  input message md5 = output message md5 (n_last_bits = 4): "

md5tux="`$md5app samples/tux.xpm | cut -d ' ' -f 1`"

cat samples/tux.xpm | ./stegwrite samples/Maddog-Du.bmp ./tmp.bmp 4 2>/dev/null
./stegread ./tmp.bmp 4 2>/dev/null > ./tmp.xpm
md5tmpxpm="`$md5app ./tmp.xpm | cut -d ' ' -f 1`"

if [ "$md5tmpxpm" = "$md5tux" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      md5in : $md5tux"
    echo "      md5out: $md5tmpxpm"
fi


# TEST 2: bitmap size test
echo -n "  input bmp size = output bmp size: "

inputbmpsize="`ls -la samples/Maddog-Du.bmp | awk '{ print $5 }'`"
outputbmpsize="`ls -la ./tmp.bmp | awk '{ print $5 }'`"

if [ "$inputbmpsize" = "$outputbmpsize" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      input bmp size : $inputbmpsize"
    echo "      output bmp size: $outputbmpsize"
fi


# TEST 3: input bmp md5 != output bmp md5
echo -n "  input bmp md5 != output bmp md5: "

inputbmpmd5="`$md5app samples/Maddog-Du.bmp | cut -d ' ' -f 1`"
outputbmpmd5="`$md5app tmp.bmp | cut -d ' ' -f 1`"

if [ "$inputbmpmd5" = "$outputbmpmd5" ]; then
    echo "FAILED"
    echo "      input bmp identical to output bmp"
    echo "      md5: $inputbmpmd5"
else
    echo "OK"
fi

# TEST 4a: truncate test (n_last_bits = 1)
echo -n "  truncate test (n_last_bits = 1): "

truncsize="`cat samples/Linux-ChangeLog-2.6.5 | ./stegwrite samples/Maddog-Du.bmp tmp.bmp 1 2>/dev/null | awk '{ print $6 }'`"

./stegread tmp.bmp 1 2>/dev/null > tmp.txt
restoresize="`ls -la ./tmp.txt | awk '{ print $5 }'`"

if [ "$truncsize" = "$restoresize" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      truncated size : $truncsize"
    echo "      restored size  : $restoresize"
fi

# TEST 4b: truncate test (n_last_bits = 2)
echo -n "  truncate test (n_last_bits = 2): "

truncsize="`cat samples/Linux-ChangeLog-2.6.5 | ./stegwrite samples/Maddog-Du.bmp tmp.bmp 2 2>/dev/null | awk '{ print $6 }'`"

./stegread tmp.bmp 2 2>/dev/null > tmp.txt
restoresize="`ls -la ./tmp.txt | awk '{ print $5 }'`"

if [ "$truncsize" = "$restoresize" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      truncated size : $truncsize"
    echo "      restored size  : $restoresize"
fi


# TEST 4c: truncate test (n_last_bits = 4)
echo -n "  truncate test (n_last_bits = 4): "

truncsize="`cat samples/Maddog-Du.bmp | ./stegwrite samples/Maddog-Du.bmp tmp.bmp 4 2>/dev/null | awk '{ print $6 }'`"

./stegread tmp.bmp 4 2>/dev/null > tmp.txt
restoresize="`ls -la ./tmp.txt | awk '{ print $5 }'`"

if [ "$truncsize" = "$restoresize" ]; then
    echo "OK"
else
    echo "FAILED"
    echo "      truncated size : $truncsize"
    echo "      restored size  : $restoresize"
fi


# CLEANUP
rm ./tmp.bmp ./tmp.xpm ./tmp.txt
