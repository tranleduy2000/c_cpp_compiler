package com.pdaxrom.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

public class XMLParser {
	private static final String TAG = "XMLParser";

	// constructor
	public XMLParser() {

	}

	/**
	 * Getting XML from URL making HTTP request
	 * @param url string
	 * */
	public String getXmlFromUrl(String url) {
		String xml = null;

		try {
			// defaultHttpClient
			DefaultHttpClient httpClient = new DefaultHttpClient();
			HttpPost httpPost = new HttpPost(url);

			HttpResponse httpResponse = httpClient.execute(httpPost);
			int status = httpResponse.getStatusLine().getStatusCode();
			if (status != HttpStatus.SC_OK) {
				System.err.println(TAG + " Response: " + status);
				return null;
			}
			System.err.println(TAG + " Response: " + status);
			HttpEntity httpEntity = httpResponse.getEntity();
			xml = EntityUtils.toString(httpEntity);

		} catch (UnsupportedEncodingException e) {
			System.err.println(TAG + " error: " + e);
		} catch (ClientProtocolException e) {
			System.err.println(TAG + " error: " + e);
		} catch (IOException e) {
			System.err.println(TAG + " error: " + e);
		}
		// return XML
		return xml;
	}
	
	/**
	 * Getting XML from file
	 * @param file path string
	 */
	public String getXmlFromFile(String filePath) {
		File f = new File(filePath);
		if (!f.exists()) {
			return null;
		}
		StringBuilder sb = new StringBuilder();
		try {
			FileInputStream fin = new FileInputStream(f);
			BufferedReader reader = new BufferedReader(new InputStreamReader(fin));
			String line = null;
			while((line = reader.readLine()) != null) {
				sb.append(line).append("\n");
			}
			reader.close();
		} catch (IOException e) {
			System.err.println(TAG + " getRepoXmlFromFile() IO error " + e);
			return null;
		}
		return sb.toString();
	}
	
	/**
	 * Getting XML DOM element
	 * @param XML string
	 * */
	public Document getDomElement(String xml){
		Document doc = null;
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		try {

			DocumentBuilder db = dbf.newDocumentBuilder();

			InputSource is = new InputSource();
		        is.setCharacterStream(new StringReader(xml));
		        doc = db.parse(is); 

			} catch (ParserConfigurationException e) {
				System.err.println(TAG + " Error: " + e.getMessage());
				return null;
			} catch (SAXException e) {
				System.err.println(TAG + " Error: " + e.getMessage());
	            return null;
			} catch (IOException e) {
				System.err.println(TAG + " Error: " + e.getMessage());
				return null;
			}

	        return doc;
	}
	
	/** Getting node value
	  * @param elem element
	  */
	 public final String getElementValue( Node elem ) {
	     Node child;
	     if( elem != null){
	         if (elem.hasChildNodes()){
	             for( child = elem.getFirstChild(); child != null; child = child.getNextSibling() ){
	                 if( child.getNodeType() == Node.TEXT_NODE  ){
	                     return child.getNodeValue();
	                 }
	             }
	         }
	     }
	     return "";
	 }
	 
	 /**
	  * Getting node value
	  * @param Element node
	  * @param key string
	  * */
	 public String getValue(Element item, String str) {		
			NodeList n = item.getElementsByTagName(str);		
			return this.getElementValue(n.item(0));
		}
}
