function name(volume, muted) {
    console.debug(volume)
    var split_base = 65536/4.0;
    var icon = null;
    if ((volume / split_base <= 0) || muted) {
        icon = "audio-volume-muted";
    } else if (volume / split_base <= 1) {
        icon = "audio-volume-low";
    } else if (volume / split_base <= 2) {
        icon = "audio-volume-medium";
    } else {
        icon = "audio-volume-high";
    }
    return icon;
}