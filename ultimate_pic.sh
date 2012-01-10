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

##################
# ultimate_pic.sh:
#     simple shell script to put encrypted data hidden in an image file
########################################################################

#!/bin/sh

if [ $# != 3 ]; then
    echo "usage: $0 <image file> <message file> <dest key>"
fi

convert $1 tmp.bmp

gpg -r $3 -o tmp.gpg -e $2

cat tmp.gpg | ./stegwrite tmp.bmp ultimate.bmp

bzip2 -9 ultimate.bmp

rm tmp.gpg tmp.bmp
