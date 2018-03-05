package xyz.pallad.glenn.blueserial;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class ControlActivity extends AppCompatActivity {

    Button btnD, btnR, btnLeft, btnRight, btnP, btnDisconnect, btnSpeed_1, btnSpeed_2, btnSpeed_3;
    String address = null;
    TextView textViewStatus;
    private ProgressDialog progressDialog;
    BluetoothAdapter myBluetoothAdapter = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
//    Handler handler;
//    private ReceiveMessage mReceiveMessage;
    // Status for handler.
    final int WHAT = 1;
    private StringBuilder stringBuilder = new StringBuilder();

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent newint = getIntent();
        address = newint.getStringExtra(MainActivity.EXTRA_ADDRESS);

        setContentView(R.layout.activity_control);

        btnD = (Button)findViewById(R.id.buttonForward);
        btnR = (Button)findViewById(R.id.buttonBack);
        btnLeft = (Button)findViewById(R.id.buttonLeft);
        btnRight = (Button)findViewById(R.id.buttonRight);
        btnP = (Button)findViewById(R.id.buttonStop);
        btnDisconnect = (Button)findViewById(R.id.buttonDisconnect);
        btnSpeed_1 = (Button)findViewById(R.id.buttonSpeed_1);
        btnSpeed_2 = (Button)findViewById(R.id.buttonSpeed_2);
        btnSpeed_3 = (Button)findViewById(R.id.buttonSpeed_3);
//        textViewStatus = (TextView)findViewById(R.id.textViewStatus);

//        handler = new Handler() {
//            public void handleMessage(Message msg) {
//                if (msg.what == 1) {
//                    byte[] readBuffer = (byte[]) msg.obj;
//                    String strReceived = new String(readBuffer, 0, msg.arg1);
//                    stringBuilder.append(strReceived);
//                    int endOfLineIndex = stringBuilder.indexOf("\r\n");
//                    if (endOfLineIndex > 0) {
//                        String stringBuilderPrint = stringBuilder.substring(0, endOfLineIndex);
//                        stringBuilder.delete(0, stringBuilder.length());
//                        textViewStatus.setText(stringBuilderPrint);
//                    } else {
//                        // Nothing received...
//                    }
//                } else {
//                    // If msg.what != 1, something must be wrong...
//                }
//            }
//        };

        new ConnectBT().execute();

        btnD.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("D");
            }
        });

        btnR.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("R");
            }
        });

        btnLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("Left");
            }
        });

        btnRight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("Right");
            }
        });

        btnSpeed_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("Speed_1");
            }
        });

        btnSpeed_2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("Speed_2");
            }
        });

        btnSpeed_3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("Speed_3");
            }
        });

        btnP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("P");
            }
        });

        btnDisconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                Disconnect();
            }
        });
    }

    private void sendSignal ( String cmd ) {
        if ( btSocket != null ) {
            try {
                btSocket.getOutputStream().write(cmd.getBytes());
            } catch (IOException e) {
                msg("Error");
            }
        }
    }

    private void Disconnect () {
        if ( btSocket!=null ) {
            try {
                btSocket.close();
            } catch(IOException e) {
                msg("Error");
            }
        }
        finish();
    }

    private void msg (String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true;

        @Override
        protected  void onPreExecute () {
            progressDialog = ProgressDialog.show(ControlActivity.this, "Connecting...", "Please Wait!!!");
        }

        @Override
        protected Void doInBackground (Void... devices) {
            try {
                if ( btSocket==null || !isBtConnected ) {
                    myBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice HC_06 = myBluetoothAdapter.getRemoteDevice(address);
                    btSocket = HC_06.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
//                    mReceiveMessage = new ReceiveMessage(btSocket);
//                    mReceiveMessage.start();
                }
            } catch (IOException e) {
                ConnectSuccess = false;
            }

            return null;
        }

        @Override
        protected void onPostExecute (Void result) {
            super.onPostExecute(result);

            if (!ConnectSuccess) {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            } else {
                msg("Connected");
                isBtConnected = true;
            }

            progressDialog.dismiss();
        }
    }

//    private class ReceiveMessage extends Thread {
//        private final InputStream inputStream;
//
//        public ReceiveMessage(BluetoothSocket socket) {
//            InputStream tmpIn = null;
//            // OutputStream tmpOut = null;
//
//            try {
//               tmpIn = socket.getInputStream();
//            } catch (IOException e) {
//                // Do nothing...
//            }
//            inputStream = tmpIn;
//        }
//
//        @Override
//        public void run() {
//            textViewStatus.setText("run()");
//            // buffer store for the stream
//            byte[] buffer = new byte[256];
//            // bytes returned from read();
//            int bytes;
//
//            // Keep listening to the InputStream until an exception occurs
//            while (true) {
//                try {
//                    // Read from the InputStream
//                    // Get number of bytes and message in "buffer"
//                    bytes = inputStream.read(buffer);
//                    handler.obtainMessage(WHAT, bytes, -1, buffer).sendToTarget();
//                } catch (IOException e) {
//                    break;
//                }
//            }
//        }
//    }

}
