#!/usr/bin/sh
echo "taking image sequnce from $1.mp4"
ffmpeg -i "$1".mp4 -r 4 -vf scale=240:-1 imageSequences/"$1"-%04d.jpg
exit 0