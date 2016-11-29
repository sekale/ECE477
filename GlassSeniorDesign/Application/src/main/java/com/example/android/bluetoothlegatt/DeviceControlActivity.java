package com.example.android.bluetoothlegatt;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.IBinder;
import android.text.format.Time;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private TextView mConnectionState;
    private TextView mDataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";


    // ********************************************
    private OneMinuteCountDownTimer countDownTimer;
    private final long startTime = 60 * 1000;
    private final long interval = 1 * 1000;

    String[] weatherStringCache = new String[3];
    String[] newsStringCache = new String[3];

    Queue<String> messageQueue = new LinkedList<String>();
    // ********************************************

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    void sendTimeData()
    {
        Time thisTime = new Time();
        thisTime.setToNow();
        String timeSent = "T,";
        int hour = thisTime.hour;
        int min = thisTime.minute;

        if(hour < 10) timeSent += "0";
        timeSent += String.valueOf(hour);

        if(min < 10) timeSent += "0";
        timeSent += String.valueOf(min);

        timeSent += "!";

        if(mBluetoothLeService != null)
        {
            while(mBluetoothLeService.isWriteOpsLockFree()) {
            }
            mBluetoothLeService.lockWriteOps();
            mBluetoothLeService.writeStringCharacteristic(timeSent);
            //noinspection StatementWithEmptyBody
            while (mBluetoothLeService.isWriteOpsLockFree())
            {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Log.v("Time Sent: ", timeSent);
        }
        else
        {
            Log.v("Time is: ", timeSent);
        }

    }

    void gatherWeatherandNewsData()
    {
        //mBluetoothLeService.writeCustomCharacteristic(65);
        //mBluetoothLeService.writeStringCharacteristic("hello");
        FetchWeatherTask fetchWeatherTask = new FetchWeatherTask();
        fetchWeatherTask.passCacheString(weatherStringCache);
        fetchWeatherTask.passMessageQueue(messageQueue);
        fetchWeatherTask.execute("47906");
        //fetchWeatherTask.execute("94043");

        FetchNewsTask fetchNewsTask = new FetchNewsTask();
        fetchNewsTask.passMessageQueue(messageQueue);
        fetchNewsTask.passCacheString(newsStringCache);
        fetchNewsTask.execute();
    }

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    // If a given GATT characteristic is selected, check for supported features.  This sample
    // demonstrates 'Read' and 'Notify' features.  See
    // http://d.android.com/reference/android/bluetooth/BluetoothGatt.html for the complete
    // list of supported characteristic features.
    private final ExpandableListView.OnChildClickListener servicesListClickListner =
            new ExpandableListView.OnChildClickListener()
            {
                @Override
                public boolean onChildClick(ExpandableListView parent, View v, int groupPosition,
                                            int childPosition, long id)
                {
                    if (mGattCharacteristics != null)
                    {
                        final BluetoothGattCharacteristic characteristic =
                                mGattCharacteristics.get(groupPosition).get(childPosition);
                        final int charaProp = characteristic.getProperties();
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_READ) > 0)
                        {
                            // If there is an active notification on a characteristic, clear
                            // it first so it doesn't update the data field on the user interface.
                            if (mNotifyCharacteristic != null)
                            {
                                mBluetoothLeService.setCharacteristicNotification(
                                        mNotifyCharacteristic, false);
                                mNotifyCharacteristic = null;
                            }
                            mBluetoothLeService.readCharacteristic(characteristic);
                        }
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0)
                        {
                            mNotifyCharacteristic = characteristic;
                            mBluetoothLeService.setCharacteristicNotification(
                                    characteristic, true);
                        }
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_WRITE) > 0)
                        {
                            //mNotifyCharacteristic = characteristic;
                            //mBluetoothLeService.setCharacteristicNotification(
                            //       characteristic, true);
                        }
                        return true;
                    }
                    return false;
                }
    };

    private void clearUI() {
        mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        mDataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.gatt_services_characteristics);

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
        mGattServicesList = (ExpandableListView) findViewById(R.id.gatt_services_list);
        mGattServicesList.setOnChildClickListener(servicesListClickListner);
        mConnectionState = (TextView) findViewById(R.id.connection_state);
        mDataField = (TextView) findViewById(R.id.data_value);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);

        //************************************
        countDownTimer = new OneMinuteCountDownTimer(startTime,interval);
        countDownTimer.start();
    }

    @Override
    protected void onResume() {
        super.onResume();
        gatherWeatherandNewsData();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(String data) {
        if (data != null) {
            mDataField.setText(data);
        }
    }

    // Demonstrates how to iterate through the supported GATT Services/Characteristics.
    // In this sample, we populate the data structure that is bound to the ExpandableListView
    // on the UI.
    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        String unknownServiceString = getResources().getString(R.string.unknown_service);
        String unknownCharaString = getResources().getString(R.string.unknown_characteristic);
        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();
        ArrayList<ArrayList<HashMap<String, String>>> gattCharacteristicData = new ArrayList<>();
        mGattCharacteristics = new ArrayList<>();

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            HashMap<String, String> currentServiceData = new HashMap<String, String>();
            uuid = gattService.getUuid().toString();
            currentServiceData.put(
                    LIST_NAME, SampleGattAttributes.lookup(uuid, unknownServiceString));
            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);

            ArrayList<HashMap<String, String>> gattCharacteristicGroupData =
                    new ArrayList<>();
            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();
            ArrayList<BluetoothGattCharacteristic> charas = new ArrayList<>();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                charas.add(gattCharacteristic);
                HashMap<String, String> currentCharaData = new HashMap<String, String>();
                uuid = gattCharacteristic.getUuid().toString();
                currentCharaData.put(
                        LIST_NAME, SampleGattAttributes.lookup(uuid, unknownCharaString));
                currentCharaData.put(LIST_UUID, uuid);
                gattCharacteristicGroupData.add(currentCharaData);
            }
            mGattCharacteristics.add(charas);
            gattCharacteristicData.add(gattCharacteristicGroupData);
        }

        SimpleExpandableListAdapter gattServiceAdapter = new SimpleExpandableListAdapter(
                this,
                gattServiceData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 },
                gattCharacteristicData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 }
        );
        mGattServicesList.setAdapter(gattServiceAdapter);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }

    public void sendQueuedData()
    {
        if(mBluetoothLeService == null)
        {
            Log.v("SendQueuedData", "BluetoothLeService is NULL");
            return;
        }
        while(messageQueue.size() > 0)
        {
            String msg = messageQueue.remove();
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
            Log.v("Sent:", msg);
        }
    }

    public void refreshData(View view)
    {
        gatherWeatherandNewsData();
        sendQueuedData();
    }

    public void gestureLeft(View view)
    {
        if(mBluetoothLeService != null)
        {
            while(mBluetoothLeService.isWriteOpsLockFree()) {
            }
            mBluetoothLeService.lockWriteOps();
            String s = "G,L!";
            mBluetoothLeService.writeStringCharacteristic(s);
            //noinspection StatementWithEmptyBody
            while (mBluetoothLeService.isWriteOpsLockFree())
            {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Log.v("Gesture Sent:", s);
        }
        else
        {
            Log.v("Gesture Sent:", "Error: bluetooth not connected for gesture LEFT");
        }
    }

    public void gestureRight(View view)
    {
        if(mBluetoothLeService != null)
        {
            while(mBluetoothLeService.isWriteOpsLockFree()) {
            }
            mBluetoothLeService.lockWriteOps();
            String s = "G,R!";
            mBluetoothLeService.writeStringCharacteristic(s);
            //noinspection StatementWithEmptyBody
            while (mBluetoothLeService.isWriteOpsLockFree())
            {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Log.v("Gesture Sent:", s);
        }
        else
        {
            Log.v("Gesture Sent:", "Error: bluetooth not connected for gesture RIGHT");
        }
    }

    public void gestureToggle(View view)
    {
        if(mBluetoothLeService != null)
        {
            while(mBluetoothLeService.isWriteOpsLockFree()) {
            }
            mBluetoothLeService.lockWriteOps();
            String s = "G,T!";
            mBluetoothLeService.writeStringCharacteristic(s);
            //noinspection StatementWithEmptyBody
            while (mBluetoothLeService.isWriteOpsLockFree())
            {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Log.v("Gesture Sent:", s);
        }
        else
        {
            Log.v("Gesture Sent:", "Error: bluetooth not connected for gesture TOGGLE");
        }
    }

    class OneMinuteCountDownTimer extends CountDownTimer
    {

        public OneMinuteCountDownTimer (long startTime, long interval)
        {
            super(startTime, interval);
            DeviceControlActivity.this.sendTimeData();
        }

        @Override
        public void onFinish()
        {
            DeviceControlActivity.this.sendTimeData();
            DeviceControlActivity.this.sendQueuedData();
            // gather new data to send for next time slot
            DeviceControlActivity.this.gatherWeatherandNewsData();

            this.start();   // restart countDownTimer for next minute
        }

        @Override
        public void onTick(long millisUntilFinished)
        {
            //Log.v("TimerValue: ","timer="+Time);
        }
    }
}

