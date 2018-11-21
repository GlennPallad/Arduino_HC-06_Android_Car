package com.bitwitcher.bwbogie;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

public class AvoidanceActivity extends Activity {

	static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

	private ProgressDialog progressDialog;
	private boolean isBtConnected = false;
	private ReceiveMessageThread mReceiveMessageThread;

	Button btnAvoidanceStart, btnAvoidanceStop, btnAvoidanceDisconnect;
	String address = null;
	TextView textViewStatus;
	BluetoothAdapter myBluetoothAdapter = null;
	BluetoothSocket btSocket = null;
	InputStream inputStream = null;

	private class ConnectBT extends AsyncTask<Void, Void, Void> {
		private boolean ConnectSuccess = true;

		@Override
		protected  void onPreExecute () {
			progressDialog = ProgressDialog.show(AvoidanceActivity.this, "Connecting...", "Please Wait!!!");
		}

		@Override
		protected Void doInBackground (Void... devices) {
			try {
				if ( btSocket == null || !isBtConnected ) {
					myBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
					BluetoothDevice HC_0x = myBluetoothAdapter.getRemoteDevice(address);
					btSocket = HC_0x.createInsecureRfcommSocketToServiceRecord(myUUID);
					BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
					btSocket.connect();
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
				/* Initializing to stop. */
				sendSignal(Protocol.A_STOP);
				/* Start a new Thread, reading from HC-0x. */
				mReceiveMessageThread = new ReceiveMessageThread(btSocket);
				mReceiveMessageThread.start();
			}
			progressDialog.dismiss();
		}
	}

	private class ReceiveMessageThread extends Thread {
		private final boolean RUN = true;
		private final boolean KILL = false;
		boolean threadStatus = RUN;

		public void killThread() {
			this.threadStatus = KILL;
		}

		public ReceiveMessageThread(BluetoothSocket btSocket) {
			try {
				inputStream = btSocket.getInputStream();
				// inputStreamReader = new InputStreamReader(inputStream);
			} catch (IOException e) {
				Log.d("AvoidanceActivity: ","Something's wrong when encapsulating InputStream.");
				e.printStackTrace();
			}
		}

		@Override
		public void run() {
			Log.d("AvoidanceActivity: ", "mReceiveMessageThread started!");
			/* buffer for inputStream */
			byte buffer[] = new byte[256];
			int buffLength;

			/* Keep listening to inputStream unless an exception occurs */
			while (this.threadStatus == RUN) {
				try {
					if (inputStream.available() > 0) {
						/* Wait until all bytes are received from HC-0x, you can modify 50 to larger number */
						/* to receive a longer bytes array. */
						Thread.sleep(50);
						/* Read from the InputStream */
						/* Get number of bytes and message in "buffer" */
						buffLength = inputStream.read(buffer);
						Message message = new Message();
						message.arg1 = buffLength;
						message.obj = buffer;
						if (buffLength > 0) {
							handler.sendMessage(message);
						}
					}
				} catch (IOException e) {
					Log.d("AvoidanceActivity: ", "mReceiveMessageThread encountered IOException!");
					e.printStackTrace();
					threadStatus = KILL;
					/* break is not necessary here but for better syntax accordance it was wrote. */
					break;
				} catch (InterruptedException e) {
					Log.d("AvoidanceActivity: ", "mReceiveMessageThread encountered InterruptedException!");
					e.printStackTrace();
					threadStatus = KILL;
					break;
				}
			}
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			int buffLength = msg.arg1;
			byte[] b_buffer = (byte[])msg.obj;
			char[] c_buffer = new char[buffLength];
			for (int i = 0; i < buffLength; i++) {
				c_buffer[i] = (char)(b_buffer[i] & 0xFF);
			}
			Log.d("AvoidanceActivity: ", "HC-0x said: " + new String(c_buffer, 0, msg.arg1));
		}
	};

	private void msg (String s) {
		Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
	}

	private void sendSignal ( String cmd ) {
		if ( btSocket != null ) {
			try {
				btSocket.getOutputStream().write((cmd + "\n").getBytes());
				/* Wait 50 millis ensure all bytes have been sent.
						BluetoothSocket can be closed before all bytes were sent
						if do not wait. */
				Thread.sleep(50);
			} catch (IOException e) {
				e.printStackTrace();
				msg("sendSignal() Error");
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	private void Disconnect () {
		if ( btSocket != null ) {
			try {
				mReceiveMessageThread.killThread();
				Log.d("AvoidanceActivity: ", "Thread killed.");
				/* Stop before disconnect. */
				sendSignal(Protocol.A_STOP);
				/* inputStreamReader automatically close when inputStream closed. */
				inputStream.close();
				Log.d("AvoidanceActivity: ", "inputStream closed.");
				btSocket.close();
				Log.d("AvoidanceActivity: ", "btSocket closed.");
			} catch(IOException e) {
				Log.d("AvoidanceActivity: ", "Close Error.");
				msg("Disconnect() Error");
			}
		}
		finish();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_avoidance);

		Intent intent = getIntent();
		address = intent.getStringExtra(ModeActivity.EXTRA_ADDRESS);

		new ConnectBT().execute();

		btnAvoidanceStart = findViewById(R.id.buttonAvoidanceStart);
		btnAvoidanceStop = findViewById(R.id.buttonAvoidanceStop);
		btnAvoidanceDisconnect = findViewById(R.id.buttonAvoidanceDisconnect);
		btnAvoidanceStart.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				sendSignal(Protocol.A_STAR);
			}
		});
		btnAvoidanceStop.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				sendSignal(Protocol.A_STOP);
			}
		});
		btnAvoidanceDisconnect.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Disconnect();
			}
		});
	}

	@Override
	protected void onDestroy() {
		Disconnect();
		super.onDestroy();
	}
}
