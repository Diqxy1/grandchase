## console
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target=System.out
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%d %-5p %l - %m%n

## text
log4j.appender.file=org.apache.log4j.RollingFileAppender 
log4j.appender.file.File=.\\log\\log.log
log4j.appender.file.encoding=UTF-8
log4j.appender.file.MaxFileSize=100KB
log4j.appender.file.MaxBackupIndex = 100
log4j.appender.file.Apppend=true
log4j.appender.file.layout=org.apache.log4j.PatternLayout
log4j.appender.file.layout.ConversionPattern=%d %-5p %l - %m%n

## HTML
log4j.appender.html=org.apache.log4j.RollingFileAppender 
log4j.appender.html.File=.\\log\\log.htm
log4j.appender.html.encoding=UTF-8
log4j.appender.html.MaxFileSize=100KB
log4j.appender.html.MaxBackupIndex = 100
log4j.appender.html.Apppend=true
log4j.appender.html.layout=org.apache.log4j.HTMLLayout 
log4j.appender.html.layout.ConversionPattern=%d %-5p %l - %m%n

## XML
## log4j.appender.xml=org.apache.log4j.RollingFileAppender 
## log4j.appender.xml.File=.\\log\\log.xml
## log4j.appender.xml.encoding=UTF-8
## log4j.appender.xml.MaxFileSize=100KB
## log4j.appender.xml.MaxBackupIndex = 100
## log4j.appender.xml.Apppend=true
## log4j.appender.xml.layout=org.apache.log4j.XMLLayout 
## log4j.appender.xml.file.layout.ConversionPattern=%d %-5p %l - %m%n

##, xml
log4j.rootLogger=debug, stdout, file, html
log4j.debug=true
# log4j.disable = INFO
# log4j.additivity.your.category.name=false