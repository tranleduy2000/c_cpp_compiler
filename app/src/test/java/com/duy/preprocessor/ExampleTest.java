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

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;


/**
 * Created by Duy on 02-May-18.
 */

public class ExampleTest extends TestCase {

    public void testGetExampleC() throws IOException, InterruptedException, ParserConfigurationException, TransformerException {
        String baseLocation = "https://www.programiz.com/c-programming/examples";
        String language = "C";
//        getExamples(baseLocation, language, "c");
    }

    public void testGetExampleCPlusPLus() throws IOException, TransformerException, ParserConfigurationException {
        String baseLocation = "https://www.programiz.com/cpp-programming/examples";
        String language = "cpp";
//        getExamples(baseLocation, language, "cpp");

    }

    private void getExamples(String baseLocation, String language, String ext) throws ParserConfigurationException, IOException, TransformerException {
        StringBuilder xmlBuilder = new StringBuilder();
        xmlBuilder.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n").append("\n")
                .append("<examples>").append("\n");
        Document document = Jsoup.connect(baseLocation).get();
        Element table = document.getElementsByTag("table").first();
        Elements links = table.getElementsByTag("a");

        for (Element child : links) {
            String link = child.attr("href");
            System.out.println(link);
            link = "https://www.programiz.com" + link;
            try {
                parseExample(link, xmlBuilder, language, ext);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        xmlBuilder.append("</examples>");

        File f = new File("./app/src/main/assets/example/" + language + "/" + "index.xml");
        f.getParentFile().mkdirs();
        f.createNewFile();
        FileOutputStream fileOutputStream = new FileOutputStream(f);
        fileOutputStream.write(xmlBuilder.toString().getBytes());
    }

    private void parseExample(String link, StringBuilder xmlBuilder, String language, String ext) throws IOException {
        Document document = Jsoup.connect(link)
                .userAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/25.0")
                .referrer("http://www.google.com")
                .get();
        String title = document.getElementsByTag("h1").text();
        String desc = document.getElementsByAttributeValue("name", "description").attr("content");
        Element code = document.body()
                .select("pre > code").first()
                .getElementsByTag("code").first();
        StringBuilder codeBuilder = new StringBuilder();
        codeBuilder.append("// ").append(title).append("\n\n");
        codeBuilder.append("// ").append(desc.replace("\n", "\n// "));
        codeBuilder.append("\n\n");
        codeBuilder.append(code.text());
        System.out.println(codeBuilder);

        File file = new File("./app/src/main/assets/example/" + language + "/" + link.substring(link.lastIndexOf("/")) + "." + ext);
        file.getParentFile().mkdirs();
        file.createNewFile();
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(codeBuilder.toString().getBytes());

        xmlBuilder.append("<example>").append("\n");
        xmlBuilder.append("<language>").append(language).append("</language>").append("\n");
        xmlBuilder.append("<title>").append(title).append("</title>").append("\n");
        xmlBuilder.append("<desc>").append(desc).append("</desc>").append("\n");
        xmlBuilder.append("<relative_path>").append(file.getName()).append("</relative_path>").append("\n");
        xmlBuilder.append("</example>").append("\n");
    }

}
