#include "lastfm.h"

LastFm::LastFm()
{
}

QString LastFm::GetLastFmLatestTrack(QString aXmlData) {

    QString workingPayload = aXmlData.remove("</track>").remove("</recenttracks>").simplified();
    QString artistElement;
    QString nameElement;

    QString artistHyphenTitle;

    int artistOffset = aXmlData.indexOf("<artist");
    int nameOffset = workingPayload.indexOf("<name>");

    if (aXmlData.length() !=0) {

        /* Strip away everything before <artist> */
        artistElement = workingPayload.remove(0, artistOffset + 40);
        artistOffset = workingPayload.indexOf("<artist");

        /* Toss artistElement's content back upstream... */
        workingPayload = artistElement;

        /* Extract the artist metadata */
        artistElement.truncate(artistElement.indexOf("</artist>"));

        /* Quick-and-dirty name fetching */
        nameOffset = workingPayload.indexOf("<name>");
        nameElement = workingPayload;
        nameElement.remove(0, artistOffset + artistElement.size() + 1);
        nameElement.remove("</artist> <name>");
        nameElement.truncate(nameElement.indexOf("</name>"));

        artistHyphenTitle.append(artistElement).append(" - ").append(nameElement);

        return artistHyphenTitle;
    }

    else {
        return "No data";
    }
}
