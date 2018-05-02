/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.preprocessor;

import junit.framework.TestCase;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;


/**
 * Created by Duy on 02-May-18.
 */

public class ExampleTest extends TestCase {

    public void testGetExampleC() throws IOException, InterruptedException, ParserConfigurationException, TransformerException {

        DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
        org.w3c.dom.Document doc = docBuilder.newDocument();
        final org.w3c.dom.Element root = doc.createElement("examples");

        String baseLocation = "https://www.programiz.com/c-programming/examples";
        Document document = Jsoup.connect(baseLocation).get();
        Element table = document.getElementsByTag("table").first();
        Elements links = table.getElementsByTag("a");
        String language = "C";
        for (Element child : links) {
            String link = child.attr("href");
            System.out.println(link);
            link = "https://www.programiz.com" + link;
            try {
                parseExample(link, doc, root, language);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        Transformer transformer = transformerFactory.newTransformer();
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");
        transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
        DOMSource source = new DOMSource(doc);
        File f = new File("./app/src/main/assets/example/" + language + "/" + "index.xml");
        f.getParentFile().mkdirs();
        f.createNewFile();
        StreamResult result = new StreamResult(f);
        transformer.transform(source, result);
        System.out.println("File saved!");
    }

    private void parseExample(String link, org.w3c.dom.Document creator, org.w3c.dom.Element parent, String language) throws IOException {
        Document document = Jsoup.connect(link)
                .userAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/25.0")
                .referrer("http://www.google.com")
                .get();
        String title = document.getElementsByTag("h1").text();
        Element code = document.body()
                .select("pre > code").first()
                .getElementsByTag("code").first();
        String codeStr = code.text();
        System.out.println(codeStr);

        File file = new File("./app/src/main/assets/example/" + language + "/" + link.substring(link.lastIndexOf("/")) + ".c");
        file.getParentFile().mkdirs();
        file.createNewFile();
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(codeStr.getBytes());


        org.w3c.dom.Element exampleNode = creator.createElement("example");
        org.w3c.dom.Element languageNode = creator.createElement("language");
        languageNode.appendChild(creator.createTextNode(language));
        exampleNode.appendChild(languageNode);
        org.w3c.dom.Element titleNode = creator.createElement("title");
        titleNode.appendChild(creator.createTextNode(title));
        exampleNode.appendChild(titleNode);
        org.w3c.dom.Element relativePath = creator.createElement("relative_path");
        relativePath.appendChild(creator.createTextNode("/example/" + language + "/" + file.getName()));
        exampleNode.appendChild(relativePath);

        parent.appendChild(exampleNode);
    }

    public void testGetExampleCPlusPLus() {

    }
}
