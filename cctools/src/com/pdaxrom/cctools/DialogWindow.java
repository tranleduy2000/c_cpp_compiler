package com.pdaxrom.cctools;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;

public class DialogWindow extends Activity {
	private final static String TAG = "cctools-DialogWindow";
	private final Context context = this;
	private final Handler handler = new Handler();

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Thread t = new Thread() {
        	public void run() {
        		try {
        	        Socket s = CCToolsActivity.dialogServiceSocket;
        			BufferedReader inFromClient = new BufferedReader(new InputStreamReader(s.getInputStream()));
        			//DataOutputStream outToClient = new DataOutputStream(s.getOutputStream());
        			String clientSentence;
        				clientSentence = inFromClient.readLine();
        		    Log.i(TAG, "Received: " + clientSentence);
        		    if (clientSentence.contentEquals("yesno")) {
        				String title = inFromClient.readLine();
        				String message = inFromClient.readLine();
        				dialogYesNo(title, message, s);
        		    } else if (clientSentence.contentEquals("editbox")) {
        				String title = inFromClient.readLine();
        				String message = inFromClient.readLine();
        				dialogEditBox(title, message, s);
        		    } else if (clientSentence.contentEquals("textview")) {
        				String title = inFromClient.readLine();
        				String message = inFromClient.readLine();
        				String text = inFromClient.readLine();
        				dialogTextView(title, message, text, s);
        		    } else {
        		    	s.close();
        		    	finish();
        		    }
        		} catch (IOException e) {
        			Log.i(TAG, "IOException " + e);
        		}
        	}
        };
        t.start();
    }
        
    private void dialogYesNo(final String title, final String message, final Socket socket) {
    	Runnable proc = new Runnable() {
    		public void run() {
    	    	new AlertDialog.Builder(context)
    	    	.setTitle(title)
    	    	.setMessage(message)
    	    	.setOnCancelListener(new DialogInterface.OnCancelListener() {					
					public void onCancel(DialogInterface dialog) {
						returnStrings("cancel", null, socket);
					}
				})
    	    	.setPositiveButton(getString(R.string.button_yes), new DialogInterface.OnClickListener() {
    				public void onClick(DialogInterface dialog, int which) {
    					returnStrings("yes", null, socket);
    				}
    			})
    			.setNeutralButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
    				public void onClick(DialogInterface dialog, int which) {
						returnStrings("cancel", null, socket);
    				}
    			})
    	    	.setNegativeButton(getString(R.string.button_no), new DialogInterface.OnClickListener() {
    				public void onClick(DialogInterface dialog, int which) {
						returnStrings("no", null, socket);
    				}
    			})
    			.show();
    		}
    	};
    	handler.post(proc);
    }

    private void dialogEditBox(final String title, final String message, final Socket socket) {
    	Runnable proc = new Runnable() {
    		public void run() {
    			final EditText input = new EditText(context);
    			new AlertDialog.Builder(context)
    		    .setTitle(title)
    		    .setMessage(message)
    		    .setView(input)
    	    	.setOnCancelListener(new DialogInterface.OnCancelListener() {					
					public void onCancel(DialogInterface dialog) {
						returnStrings("cancel", null, socket);
					}
				})
    		    .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
    		        public void onClick(DialogInterface dialog, int whichButton) {
    		            String value = input.getText().toString(); 
    		            returnStrings("ok", value, socket);
    		        }
    		    }).setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
    		        public void onClick(DialogInterface dialog, int whichButton) {
    		        	returnStrings("cancel", null, socket);
    		        }
    		    }).show();
    		}
    	};
    	handler.post(proc);
    }
    
    private void dialogTextView(final String title, final String message, final String text, final Socket socket) {
    	Runnable proc = new Runnable() {
    		public void run() {
    			final TextView output = new TextView(context);
    			output.setText(text);
    			new AlertDialog.Builder(context)
    		    .setTitle(title)
    		    .setMessage(message)
    		    .setView(output)
    	    	.setOnCancelListener(new DialogInterface.OnCancelListener() {					
					public void onCancel(DialogInterface dialog) {
						returnStrings("cancel", null, socket);
					}
				})
    		    .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
    		        public void onClick(DialogInterface dialog, int whichButton) {
    		            returnStrings("ok", null, socket);
    		        }
    		    }).show();
    		}
    	};
    	handler.post(proc);
    }
        
    private void returnStrings(String status, String message, Socket socket) {
		DataOutputStream outToClient;
		try {
			outToClient = new DataOutputStream(socket.getOutputStream());
			if (message != null) {
				outToClient.writeBytes(status + " " + message);
			} else {
				outToClient.writeBytes(status);
			}
			socket.close();
			finish();
		} catch (IOException e) {
			Log.e(TAG, "DialogYesNo-" + status + " " + e);
		}						    	
    }
}
