# SEOS logserver - demo application

The purposes of this demo application is to present all common features
offered by the log server, and help the user to configure it in the desired way.

The following components are used in the demo:

* app_filesystem - File system for storing the log into the files.
* app0x - Log file reader used at the end of the demo to dump logs from the
  files to the console.
* log_client - Several clients that are logging their activity. Pleas note that
  the name of the client corresponds to its configuration e.g. client
  "app_lvl_warning" has a filter on the server side set to the "WARNING" level,
  and the client "app_cl_filter_error" has a filter set to the "ERROR" level on
  the client side.
* log_server - The log server that is in charge of logging functionality.

The following features are presented in the demo:

* Several clients logging concurrently.
* Logging to the console.
* Logging to the file.
* Different log level filters on the client side.
* Different log level filters on the server side.
* Filters are disabled.
* Every client has its own unique ID.
* Log server can log as well.
* Log entry can be formatted in the custom way.
