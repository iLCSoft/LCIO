package hep.lcio.util;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Jeremy McCormick <jeremym@slac.stanford.edu>
 * @version $Id: RunCommandHandler.java,v 1.2 2010-06-30 20:56:12 jeremy Exp $
 */
public class RunCommandHandler extends CommandHandler 
{
    String className;
    List<String> args;
    
    public RunCommandHandler()
    {
        super("run", "Run a class in the LCIO jar.");
    }
    
    public void execute() throws Exception 
    {
        // Get the class and make a new instance.
        Class klass = Class.forName(className);
        Object object = klass.newInstance();
        
        // Find the main method to be called.
        Method mainMethod = null;       
        for (Method method : klass.getMethods())
        {
            // Check if this looks like the main method.
            if (method.getName().equals("main"))
            {
                int modifiers = method.getModifiers();
                if (Modifier.isStatic(modifiers) && Modifier.isPublic(modifiers))
                {
                    if (method.getParameterTypes().length == 1)
                    {
                        Class paramType = method.getParameterTypes()[0];                    
                        if (paramType.equals((new String[0]).getClass()))                        
                        {
                            mainMethod = method;
                            break;
                        }
                    }
                }
            }
        }
        
        // The main method was not found.
        if (mainMethod == null)
            throw new RuntimeException("The class " + klass.toString() + " doesn't have a main method.");
        
        // Setup the arguments and invoke the main routine.
        Object pargs[] = new Object[1];
        pargs[0] = args.toArray(new String[0]);
        mainMethod.invoke(object, pargs);
    }

    public void parse(String[] argv) throws Exception 
    {
        // Need at least name of class to run.
        if (argv.length < 1)
            throw new RuntimeException("Not enough arguments.  Missing class name to run.");
        
        // Get class name to run.
        className = argv[0]; 

        // New argument list.
        args = new ArrayList<String>();
        
        // Add extra command line arguments.
        if (argv.length > 1)
        {            
            for (int i=1; i<argv.length; i++)
            {
                args.add(argv[i]);
            }
        }        
    }
}