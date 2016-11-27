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

    //public ArrayAdapter<String> mForecastAdapter;
    private BluetoothLeService mBluetoothLeService;
    private HashMap<String,String> newsMap = new HashMap<String,String>();


    void passBLEService(BluetoothLeService mBLEService)
    {
        mBluetoothLeService = mBLEService;
    }

    private final String LOG_TAG = com.example.android.bluetoothlegatt.FetchNewsTask.class.getSimpleName();

    void parseAndSendToMicro(String msg)
    {
        int startIndx = 0;
        String s = "N,";
        while(msg.length() > 0)
        {
            int subStrSize = 18;
            if(msg.length() <= subStrSize) subStrSize = msg.length();
            s += msg.substring( startIndx , startIndx + subStrSize );
            msg = msg.substring(subStrSize, msg.length());

            Log.v(LOG_TAG, "Sending: " + s);// + "\nMsg length: " + msg.length());
            sendToMicro(s);
            s = "n,";   // reset s after sending to micro
        }
    }

    void sendToMicro(String msg)
    {
        mBluetoothLeService.lockWriteOps();
        mBluetoothLeService.writeStringCharacteristic(msg);
        //noinspection StatementWithEmptyBody
        while(mBluetoothLeService.isWriteOpsLockFree()){}

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected String[] doInBackground(Void... params)
    {
        // Will contain the raw JSON response as a string.
        String forecastJsonStr = null;
        Log.v(LOG_TAG, "Starting News Fetch");

        try
        {
            // Construct the URL for the OpenWeatherMap query
            // Possible parameters are avaiable at OWM's forecast API page, at
            // http://openweathermap.org/API#forecast
            final String FORECAST_BASE_URL =
                    "http://feeds.reuters.com/reuters/topNews";

            Document doc = null;
            try {
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
            for(int i = newsHeadlines.size() - 1; i >= 0; --i)
            {
                String key = newsHeadlines.get(i).html();
                String val = linkHeadlines.get(i).html();
                finalStr[i] = key;
                Log.v(LOG_TAG, key + ":" + val);
                //NewsArticle article = new NewsArticle(key, val);
                if(newsMap.containsKey(key) == false)
                {
                    newsMap.put(key, val);
                    parseAndSendToMicro(key);
                }
                //Log.v(LOG_TAG, newsHeadlines.get(i).html() + "->" + linkHeadlines.get(i).html());
            }
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