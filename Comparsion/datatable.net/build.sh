#!/bin/sh
mono-csc -target:exe -reference:System.Data.dll -reference:System.Xml.dll -out:DataTable.exe DataTable.cs

mono-csc -target:exe -out:Poco.exe Poco.cs
