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
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.UUID;

public class ControlActivity extends AppCompatActivity {

	Button btnD, btnR, btnLeft, btnRight, btnP, btnDisconnect, btnSpeed_1, btnSpeed_2, btnSpeed_3;
	String address = null;
	TextView textViewStatus;
	private ProgressDialog progressDialog;
	BluetoothAdapter myBluetoothAdapter = null;
	BluetoothSocket btSocket = null;
	InputStream inputStream = null;
	InputStreamReader inputStreamReader = null;
	private boolean isBtConnected = false;
	static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	Handler handler;
	private ReceiveMessageThread mReceiveMessageThread;
	// Status for handler.
	final int WHAT = 1;
	// private StringBuilder stringBuilder = new StringBuilder();

	@SuppressLint("HandlerLeak")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Intent newint = getIntent();
		address = newint.getStringExtra(MainActivity.EXTRA_ADDRESS);

		setContentView(R.layout.activity_control);

		btnD = findViewById(R.id.buttonForward);
		btnR = findViewById(R.id.buttonBack);
		btnLeft = findViewById(R.id.buttonLeft);
		btnRight = findViewById(R.id.buttonRight);
		btnP = findViewById(R.id.buttonStop);
		btnDisconnect = findViewById(R.id.buttonDisconnect);
		btnSpeed_1 = findViewById(R.id.buttonSpeed_1);
		btnSpeed_2 = findViewById(R.id.buttonSpeed_2);
		btnSpeed_3 = findViewById(R.id.buttonSpeed_3);
		textViewStatus = (TextView)findViewById(R.id.textViewStatus);

		handler = new Handler() {
			public void handleMessage(Message msg) {
				if (msg.what == 1) {
					Log.d("ControlActivity: ", "HC-0x said: " + new String((char[])msg.obj, 0, msg.arg1));
				} else {
				   // If msg.what != 1, something must be wrong...
				}
			}
		};

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
				mReceiveMessageThread.setStopFlag();
				inputStreamReader.close();
				inputStream.close();
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
					BluetoothDevice HC_0x = myBluetoothAdapter.getRemoteDevice(address);
					btSocket = HC_0x.createInsecureRfcommSocketToServiceRecord(myUUID);
					BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
					btSocket.connect();
					// Start a new Thread, reading from HC-0x.
					mReceiveMessageThread = new ReceiveMessageThread(btSocket);
					mReceiveMessageThread.start();
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

	class ReceiveMessageThread extends Thread {
		public ReceiveMessageThread(BluetoothSocket btSocket) {
			try {
				inputStream = btSocket.getInputStream();
				inputStreamReader = new InputStreamReader(inputStream);
			} catch (IOException e) {
				e.printStackTrace();
				Log.d("ControlActivity: ","Something's wrong when encapsulating InputStream.");
			}
		}

		boolean stopFlag = true;
		public void setStopFlag() {
			this.stopFlag = false;
		}

		@Override
		public void run() {
			Log.d("ControlActivity: ", "run() executed!");
			// buffer store for the stream
			char buffer[] = new char[256];
			// chars returned from read();
			int length;

			// Keep listening to the InputStream until an exception occurs
			while (stopFlag) {
				try {
					// Read from the InputStream
					// Get number of bytes and message in "buffer"
					length = inputStreamReader.read(buffer);
					if (length > 0) {
						handler.obtainMessage(WHAT, length, 0, buffer).sendToTarget();
					}
				} catch (IOException e) {
					Log.d("ControlActivity: ", "run() encountered error!");
					break;
				}
			}
		}
	}

}
