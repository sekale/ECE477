package com.example.android.bluetoothlegatt;


import android.text.format.Time;

/**
 * Created by Kanishk Yadav on 11/26/2016.
 */

public class NewsArticle
{
    private String headlines;
    private String link;
    Time time;

    public NewsArticle(String heading, String page)
    {
        headlines = heading;
        link = page;
        time = new Time();
        time.setToNow();
    }

    public String getHeadlines()
    {
        return headlines;
    }

    public String getLink()
    {
        return link;
    }
}
