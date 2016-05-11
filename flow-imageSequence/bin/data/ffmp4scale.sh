
#!/usr/bin/sh
echo "taking image sequnce from $1.mp4"
ffmpeg -i "$1".mp4 -vf scale=320:-1 scaled/"$1".mp4
exit 0