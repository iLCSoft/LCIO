@SETLOCAL
@SET ANT_HOME=%LCIO%\tools
@java -cp "%ANT_HOME%\freehep-buildtools.jar;%ANT_HOME%\ant.jar;%ANT_HOME%\ant-nodeps.jar;%ANT_HOME%\junit.jar;%ANT_HOME%\ant-junit.jar;%ANT_HOME%\jaxp.jar;%ANT_HOME%\parser.jar;%JDK_HOME%\lib\tools.jar;%AIDA%" -Dant.home="%ANT_HOME%" %ANT_OPTS% org.apache.tools.ant.Main %1 %2 %3 %4 %5 %6 %7 %8 %9
