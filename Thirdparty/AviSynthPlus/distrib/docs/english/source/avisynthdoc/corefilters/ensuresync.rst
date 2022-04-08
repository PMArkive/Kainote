
EnsureVBRMP3Sync
================

``EnsureVBRMP3Sync`` (clip)

``EnsureVBRMP3Sync`` will ensure synchronization of video with variable
bitrate audio (MP3-AVI's for example) during seeking or trimming. It does so
by buffering the audio. The name of the filter is a bit misleading, since it
is useful for every stream with variable bitrate audio and not just for MP3.

It will slow seeking down considerably, but is very useful when using
``Trim`` for instance. Always use it before trimming.
::

    # Ensures that soundtrack is in sync after trimming
    AviSource("movie.avi")
    EnsureVBRMP3Sync()
    Trim(250,2500)

$Date: 2007/05/05 09:39:34 $
