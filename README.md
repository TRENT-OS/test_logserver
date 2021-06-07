# OS LogServer - Test Application

The purpose of this test application is to present all common features offered
by the LogServer, and help the user to configure it in the desired way.

The following components are used in the test:

* FileReaderWriter
* LogFileReader
* LogsAllLevels
* LogServer

The following features are presented in the test:

* Several clients logging concurrently.
* Logging to the console.
* Logging to the file.
* Different log level filters on the client-side.
* Different log level filters on the server-side.
* Filters are disabled.
* Every client has its unique ID.
* LogServer can log as well.
* Log entry can be formatted in the custom way.
