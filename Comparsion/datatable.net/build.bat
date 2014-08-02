@ECHO OFF

SET CSC=c:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe

%CSC% -target:exe -reference:System.Data.dll -reference:System.Xml.dll -out:DataTable.exe DataTable.cs
%CSC% -target:exe -out:Poco.exe Poco.cs