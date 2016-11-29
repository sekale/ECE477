package com.example.android.bluetoothlegatt;

import android.net.Uri;
import android.os.AsyncTask;
import android.text.format.Time;
import android.util.Log;
import android.util.Pair;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;

public class FetchNewsTask extends AsyncTask<Void, Void, String[]>
{

    private BluetoothLeService mBluetoothLeService;
    private Pair<String,String> newsList[] = new Pair[3];

    String[] cachedStrs = null;
    void passCacheString(String[] newsStringCache)
    {
        Log.v(LOG_TAG, "newsStringCache: " + newsStringCache[0] + " | " + newsStringCache[1] + " | " + newsStringCache[2]);
        cachedStrs = newsStringCache;
        Log.v(LOG_TAG, "cacheStr: " + cachedStrs[0] + " | " + cachedStrs[1] + " | " + cachedStrs[2]);
    }

    void passBLEService(BluetoothLeService mBLEService)
    {
        mBluetoothLeService = mBLEService;
    }

    private final String LOG_TAG = com.example.android.bluetoothlegatt.FetchNewsTask.class.getSimpleName();

    void parseAndSendToMicro(String msg, int newsNo)
    {
        int startIndx = 0;
        String s = "N,";
        while(msg.length() > 0)
        {
            int subStrSize = 16;
            if(msg.length() <= subStrSize) subStrSize = msg.length();
            s += String.valueOf(newsNo) + msg.substring( startIndx , startIndx + subStrSize ) + "!";
            msg = msg.substring(subStrSize, msg.length());

            sendToMicro(s);
            s = "n,";   // reset s after sending to micro
        }
    }

    void sendToMicro(String msg)
    {
        //noinspection StatementWithEmptyBody
        while(mBluetoothLeService.isWriteOpsLockFree()){}
        mBluetoothLeService.lockWriteOps();
        mBluetoothLeService.writeStringCharacteristic(msg);
        //noinspection StatementWithEmptyBody
        while(mBluetoothLeService.isWriteOpsLockFree())
        {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Log.v(LOG_TAG, "News Sent: " + msg);
    }

    @Override
    protected String[] doInBackground(Void... params)
    {
        // Will contain the raw JSON response as a string.
        Log.v(LOG_TAG, "Starting News Fetch");

        try
        {
            final String FORECAST_BASE_URL = "http://feeds.reuters.com/reuters/topNews";

            Document doc = null;
            try
            {
                doc = Jsoup.connect(FORECAST_BASE_URL).get();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
            Elements newsHeadlines = doc.select("item > title");
            Elements linkHeadlines = doc.select("item > link");
            //Log.v(LOG_TAG, newsHeadlines.toString());

            String[] finalStr = new String[newsHeadlines.size()];

            int loopStart = newsHeadlines.size() - 1;
            if(loopStart > 2) loopStart = 2;
            for(int i = loopStart; i >= 0; --i)
            {
                String key = newsHeadlines.get(i).html();
                String val = linkHeadlines.get(i).html();
                finalStr[i] = key;
                Log.v(LOG_TAG, key + ":" + val);
                newsList[i] = Pair.create(key,val);
            }

            Log.v(LOG_TAG, "CacheStrs before sending article: " +
                    cachedStrs[0] + " | " + cachedStrs[1] + " | " + cachedStrs[2]);
            Log.v(LOG_TAG, "newsList before sending article: \n* " +
                    newsList[0].first + " \n* " + newsList[1].first + " \n* " + newsList[2].first);
            for(int i = 0; i < 3; ++i)
            {
                if(cachedStrs[i] == null || !cachedStrs[i].equals(newsList[i].first))
                {
                    parseAndSendToMicro(newsList[i].first, i+1);
                    cachedStrs[i] = new String(newsList[i].first);
                }
            }
            Log.v(LOG_TAG, "CacheStrs after sending article: \n* " +
                    cachedStrs[0] + " \n* " + cachedStrs[1] + " \n* " + cachedStrs[2]);
            return finalStr;
        }
        catch (Exception e)
        {
            Log.e(LOG_TAG, "Error ", e);
            // If the code didn't successfully get the weather data, there's no point in attemping
            // to parse it.
            return null;
        }
    }

    @Override
    protected void onPostExecute(String result[]) {
        if (result != null)
        {
            //Log.v(LOG_TAG, result[0]);
//            for(String article : result)
//            {
//                Log.v(LOG_TAG, "adding news article: " + article);
//            }

//            mBluetoothLeService.lockWriteOps();
//            mBluetoothLeService.writeStringCharacteristic(result[0]);
//            //noinspection StatementWithEmptyBody
//            while(mBluetoothLeService.isWriteOpsLockFree()){}
        }
    }
}